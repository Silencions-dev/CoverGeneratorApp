#include "IOWindow.hpp"

/*!
 * \brief Initiazliation of the class IOWindow
 *
 * Create the new instances of the inner widgets. Number of stored
 * data inputs is determined by the 'input' argument and type by the
 * 'add_check_boxes' argument. It initially creates list of the
 * data descriptions for each input field (using "?" sign). Set the
 * layout of the widget and connect signals from each data input
 * with one slot, which passes the information to the higher widgets.
 *
 * \param[in] inputs - number of data input
 * \param[in] add_check_boxes - inputs with or without check boxes
 * \param[in] parent - pointer to the parent widget
 */
IOWindow::IOWindow(uint8_t inputs, bool add_check_boxes, QWidget *parent) : QWidget(parent)
{
    inputs_number = inputs;

    createInputs(add_check_boxes);

    // Inicjalizacja etykiet
    for(uint8_t i = 0; i < inputs_number; ++i)
        data_name.push_back(new QLabel("?", this));

    // Inicjalizacja zarządców geometrii
    left_column_layout = new QVBoxLayout();
    right_column_layout = new QVBoxLayout();
    columns_layout = new QHBoxLayout(this);

    setWidgetsLayout();

    for(uint8_t i = 0; i < inputs_number; ++i)
        connect(data_input[i], SIGNAL(dataChanged()), this, SLOT(verifyInputsHigher()));
}
/*!
 * \brief Set the layout of the inner widgets
 *
 * Set the layout of the data descriptions (left column), then
 * layout of the data inputs (right column) and align both of
 * them horizontally.
 */
void IOWindow::setWidgetsLayout()
{
    setLabelsLayout();
    setDataLayout();
    alignBothColumns();
}
/*!
 * \brief Align the descriptions vertically (first column)
 *
 * Add all data input to the vertical layout with additional
 * alignment to the right (horizontal) and center (vertical).
 */
void IOWindow::setLabelsLayout()
{
    for(uint8_t i = 0; i < this->inputs_number; ++i)
    {
        data_name[i]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        left_column_layout->addWidget(data_name[i]);
    }

    left_column_layout->setAlignment(Qt::AlignRight);
}
/*!
 * \brief Align the input data fields vertically (second column)
 *
 * Add all data input to the vertical layout with additional
 * alignment to the left (horizontal).
 */
void IOWindow::setDataLayout()
{
    for(uint8_t i = 0; i < inputs_number; ++i)
        right_column_layout->addWidget(data_input[i]);

    right_column_layout->setAlignment(Qt::AlignLeft);
}
/*!
 * \brief Align first and second column horizontally
 *
 * Additionally set the alignment of the main layout itself
 * to be in the center of the widget.
 */
void IOWindow::alignBothColumns()
{
    columns_layout->addLayout(left_column_layout);
    columns_layout->addLayout(right_column_layout);
    columns_layout->setAlignment(Qt::AlignCenter);
}
/*!
 * \brief Create the objects of DimInput or BoolDimInput
 *
 * Create the desired number of the data inputs with the given type
 * (with or without check boxes). Trigger horizontal alignment of
 * the inner widgets of each data input.
 *
 * \param[in] check_boxes - true for the inputs with check boxes (BoolDimInput)
 */
void IOWindow::createInputs(bool check_boxes)
{
    for(uint8_t i = 0; i < inputs_number; ++i)
    {
        if(check_boxes)
            data_input.push_back(new BoolDimInput(this));
        else
            data_input.push_back(new DimInput(this));

        data_input[i]->setHorizontalLayout();
    }
}
/*!
 * \brief Set the description of the input fields
 *
 * Set the descriptions of each data input from the given
 * array.
 *
 * \param[in] titles - pointer to the buffer with the descriptions / titles
 */
void IOWindow::getInputsTitles(const QString *titles)
{
    for(uint8_t i = 0; i < inputs_number; ++i)
        data_name[i]->setText(titles[i]);
}
/*!
 * \brief Check if all data inputs contain correct values
 *
 * Iterate over list of stored data inputs and check the status.
 * If the status of all inputs is correct or disabled, then data
 * in the window is considered correct.
 *
 * \return True if all are correct and false otherwise
 */
bool IOWindow::allInputsCorrect()
{
    bool all_correct = true;

    for(uint8_t i = 0; i < inputs_number; ++i)
    {
        DimensionStatus status = data_input[i]->returnStatus();
        if((status != DimensionStatus::CORRECT) & (status != DimensionStatus::DISABLED))
        {
            all_correct = false;
            break;
        }
    }

    return all_correct;
}
/*!
 * \brief Check if at least one data input contains the wrong value
 *
 * Iterate over list of stored data inputs and check the status.
 * If the status of one of the inputs is wrong, break the loop and
 * update the message with the error (pointing to which data input
 * has wrong data and why).
 *
 * \return True if at least one wrong and false otherwise
 */
bool IOWindow::oneInputWrong()
{
    bool one_wrong = false;

    for(uint8_t i = 0; i < inputs_number; ++i)
    {
        if(this->data_input[i]->returnStatus() == DimensionStatus::WRONG)
        {
            one_wrong = true;
            updateMessageStatus(i);
            break;
        }
    }

    return one_wrong;
}
/*!
 * \brief Check if at least one data input is still edited by the user
 *
 * Iterate over list of stored data inputs and check the status.
 * If the status of one of the inputs is basic (neutral), then break
 * the loop and return true.
 *
 * \return True if at least one is edited and false otherwise
 */
bool IOWindow::oneInputEditing()
{
    bool one_editing = false;

    for(uint8_t i = 0; i < inputs_number; ++i)
    {
        if(data_input[i]->returnStatus() == DimensionStatus::NEUTRAL)
        {
            one_editing = true;
            break;
        }
    }

    return one_editing;
}
/*!
 * \brief Check if all data inputs are disabled by the check boxes
 *
 * Iterate over list of stored data inputs and check the status.
 * If the status of all inputs is disabled, then there is nothing
 * to be wrong or edited, so data in the window can be considered
 * correct.
 *
 * \return True if all disabled and false otherwise
 */
bool IOWindow::allInputsDisabled()
{
    bool all_disabled = true;

    for(uint8_t i = 0; i < inputs_number; ++i)
    {
        DimensionStatus status = data_input[i]->returnStatus();
        if((status != DimensionStatus::DISABLED))
        {
            all_disabled = false;
            break;
        }
    }

    return all_disabled;
}
/*!
 * \brief Updates the status message with the current error from the given input
 *
 * Get the error message from the given data input and add information
 * to the string, which field has wrong data (using the description).
 *
 * \param[in] index - index of the input data in the list
 */
void IOWindow::updateMessageStatus(uint8_t index)
{
    QString data_input_err = data_input[index]->returnError().returnErrorMessage();

    status_message = QString("W polu \"") + data_name[index]->text() + QString("\" ") +
                           data_input_err;
}
