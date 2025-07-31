#include "DimInput.hpp"

/*!
 * \brief Initialization of the class DimInput
 *
 * Create new instances of the inner widget, adjust the
 * settings of them and connect the signals from QLineEdits
 * with appropriate slots.
 *
 * \param[in] parent - pointer to the parent widget
 */
DimInput::DimInput(QWidget *parent) : QWidget(parent)
{
    input_dim = new QLineEdit(this);
    unit = new QLabel("mm", this);
    h_layout = new QHBoxLayout(this);

    setStoredWidgets();

    connect(input_dim, SIGNAL(editingFinished()), this, SLOT(newData()));
    connect(input_dim, SIGNAL(textEdited(QString)), this, SLOT(dataEditing()));
}
/*!
 * \brief Set the settings and graphics of the inner widgets.
 *
 * Adjust the appearance of the QLineEdit field, like: margins of
 * the text, visible frame, placeholder text and default basic style
 * (from style sheet). Set alignment of the input field to the left
 * horizontally and center vertically. Define the geometrical behavior
 * of the QLineEdit.
 */
void DimInput::setStoredWidgets()
{
    input_dim->setReadOnly(false);
    input_dim->setFrame(true);
    input_dim->setTextMargins(1, 1, 1, 1);
    input_dim->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    input_dim->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    input_dim->setPlaceholderText("0");

    changeStyle(input_dim, styles::EDIT_NEUTRAL);
}
/*!
 * \brief Aligns the inner widgets horizontally
 *
 * Align the QLineEdit and the QLabel with the unit of the
 * given data (distance in mm).
 */
void DimInput::setHorizontalLayout()
{
    h_layout->addWidget(input_dim);
    h_layout->addWidget(unit);
}
/*!
 * \brief Check the correctness of the new data from the QLineEdit
 *
 * First, convert the data to the numeric value and check if
 * the conversion finished successfully. Then, check if the value
 * is negative or exceeds the limit and eventually raise the
 * appropriate error. If new data passed the verification, overwrite
 * it using the given pointer and return true.
 *
 * \param[in] new_data - new raw data from the edit field
 * \param[in] correct_data - pointer to the variable in which converted value should be stored
 * \return True if data is correct and false otherwise
 */
bool DimInput::checkNewData(QString new_data, uint16_t *correct_data)
{
    bool convert_flag;

    int numeric_dimension = new_data.toInt(&convert_flag);

    if(!convert_flag)
    {
        data_error.raise(InputErrors::WRONG_FORMAT);
        return false;
    }

    if(numeric_dimension < 0)
    {
        data_error.raise(InputErrors::WRONG_VALUE);
        return false;
    }

    if(numeric_dimension >= 2000)
    {
        data_error.raise(InputErrors::TOO_HIGH_VALUE);
        return false;
    }

    if(correct_data != nullptr)
        *correct_data = static_cast<uint16_t>(numeric_dimension);

    return true;
}
/*!
 * \brief Change the color of the QLineEdit frame
 *
 * It changes the color of the frame according to the current status.
 * Appropriate colors are defined in a spearate style sheet files.
 *
 * \param[in] status - current status of the input field (enumeration)
 */
void DimInput::setEditStatus(DimensionStatus status)
{
    switch(status)
    {
        case DimensionStatus::WRONG:
            changeStyle(input_dim, styles::EDIT_WRONG);
            break;
        case DimensionStatus::CORRECT:
            changeStyle(input_dim, styles::EDIT_CORRECT);
            break;
        case DimensionStatus::NEUTRAL:
            changeStyle(input_dim, styles::EDIT_NEUTRAL);
            break;
        default:
            break;
    }
    // Aktualizacja statusu pola tekstowego
    dim_status = status;
}
/*!
 * \brief Loads the new data when user finished editing
 *
 * Get the text from the QLineEdit and check if the string with the
 * data is empty. If so, set the basic style of the QLineEdit and return.
 * If not, verify the correctness of the data. In the case of verified
 * value set the status to correct. With every change of the status also
 * emit the signal about the changes to notify the higher widgets so they
 * can appropriately react.
 */
void DimInput::newData()
{
    QString new_data = input_dim->text();

    if(new_data.isEmpty())
    {
        setEditStatus(DimensionStatus::NEUTRAL);
        emit dataChanged();
        return;
    }

    if(!checkNewData(new_data, &dimension))
    {
        // Wprowadzone dane są nieprawidłowe
        setEditStatus(DimensionStatus::WRONG);
        emit dataChanged();
        return;
    }

    // Wprowadzone dane są prawidłowe
    setEditStatus(DimensionStatus::CORRECT);

    emit dataChanged();
}
/*!
 * \brief Display the last saved correct dimension
 *
 * Convert the stored numeric value back to the string
 * and display it in the input data field. Call the
 * setEditStatus method to adjust the style of the
 * input field to correct data (assuming that saved
 * data is always correct).
 *
 * \param[in] saved_data - dimension to display inside input field
 */
void DimInput::displaySavedData(int saved_data)
{
    QString data_string = QString::number(saved_data);
    dimension = saved_data;
    input_dim->setText(data_string);
    setEditStatus(DimensionStatus::CORRECT);
}
/*!
 * \brief Resets the input field
 *
 * Clear the content of the QLineEdit, set the stored
 * dimension to 0 and set the style to the basic one
 * (not wrong nor correct).
 */
void DimInput::clearEditField()
{
    input_dim->clear();
    dimension = 0;
    setEditStatus(DimensionStatus::NEUTRAL);
}
/*!
 * \brief Changes the status when user started to edit
 *
 * Change the style to neutral and notify higher widgets
 * about changed status.
 */
void DimInput::dataEditing()
{
    // Zmienia kolor pola na neutralny
    setEditStatus(DimensionStatus::NEUTRAL);

    emit dataChanged();
}
/*!
 * \brief Returns the stored dimension
 *
 * Return verified data only. If the status is other than
 * correct, return -1.
 *
 * \return Saved dimension (integer)
 */
int DimInput::returnData()
{
    if(dim_status == NEUTRAL || dim_status == WRONG || dim_status == DISABLED)
        return -1;

    return dimension;
}
