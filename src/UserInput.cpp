#include "UserInput.hpp"

/*!
 * \brief Initialization of the class UserInput
 *
 * Create new instances of the inner widgets, set their settings and
 * connect the signals from the input field with the appropriate slots.
 *
 * \param[in] parent - pointer to the parent widget
 */
UserInput::UserInput(QWidget *parent) : QWidget(parent)
{
    data_label = new QLabel(this);
    data_input = new QLineEdit(this);

    setWidgets();

    connect(data_input, SIGNAL(editingFinished()), this, SLOT(newData()));
    connect(data_input, SIGNAL(textEdited(QString)), this, SLOT(dataEditing()));
}
/*!
 * \brief Set the settings and graphics of the inner widgets
 *
 * Set the geometrical behavior of the data description and
 * call the function that sets the settings of the input field.
 */
void UserInput::setWidgets()
{
    data_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    setLineEdit(data_input);
}
/*!
 * \brief Set the settings and graphics of the input field
 *
 * Set the appearance of the text inside the field, frame and graphics
 * (default color of the frame).
 *
 * \param[in] input - pointer to the QLineEdit object
 */
void UserInput::setLineEdit(QLineEdit *input)
{
    input->setReadOnly(false);
    input->setFrame(true);
    input->setTextMargins(1, 1, 1, 1);
    input->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    input->setMaxLength(TextLimit);
    input->setMinimumWidth(LineEditWidth);

    changeStyle(input, styles::EDIT_NEUTRAL);
}
/*!
 * \brief Change the frame color of the input field according to the current status
 *
 * Change the color of the frame (with appropriate style sheet file) based
 * on the current status of the data inside the QLineEdit. Notify about changes
 * with the signal emission.
 */
void UserInput::adjustFrameColor()
{
    switch(this->status)
    {
        case UserDataStatus::USER_NEUTRAL:
            changeStyle(data_input, styles::EDIT_NEUTRAL);
            break;
        case UserDataStatus::USER_WRONG:
            changeStyle(data_input, styles::EDIT_WRONG);
            break;
        case UserDataStatus::USER_CORRECT:
            changeStyle(data_input, styles::EDIT_CORRECT);
            break;
        default:
            break;
    }

    emit dataChanged();
}
/*!
 * \brief Trigger the verification of the new data when appeared
 *
 * First check the current data, change the inner status and adjust
 * the frame color of input field (QLineEdit) based on the changed status.
 */
void UserInput::newData()
{
    checkData();
    adjustFrameColor();
}
void newData();
/*!
 * \brief Change the status to neutral when data is edited
 *
 * Change the status to NEUTRAL and adjust the frame color of the
 * input field (QLineEdit).
 */
void UserInput::dataEditing()
{
    status = UserDataStatus::USER_NEUTRAL;
    adjustFrameColor();
}
