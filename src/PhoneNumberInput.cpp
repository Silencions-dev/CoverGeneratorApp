#include "PhoneNumberInput.hpp"

/*!
 * \brief Initialization of the class PhoneNumberInput
 *
 * Call the constructor of the parent in the initialization list.
 * Set the input data description and the placeholder text inside
 * the QLineEdit.
 *
 * \param[in] parent - pointer to the parent widget
 */
PhoneNumberInput::PhoneNumberInput(QWidget *parent) : UserInput(parent)
{
    data_label->setText("Numer telefonu:");
    data_input->setPlaceholderText("XXX-XXX-XXX");
}
/*!
 * \brief Check the correctness of the provided phone number
 *
 * Get the data from the QLineEdit field and check it with
 * different criteria. First, check if the input field is empty.
 * Then, remove from the string all special characters, like '+',
 * dashes '-' or white space, and check the length of the number.
 * Finally check if the given string can be converted to the numerical
 * value. Overwrite status and currently stored data.
 */
void PhoneNumberInput::checkData()
{
    QString new_data = data_input->text();

    if(new_data.isEmpty())
    {
        status = UserDataStatus::USER_NEUTRAL;
        return;
    }

    if(!filterNumberFromChars(new_data))
        return;

    if(!checkNumberNumeric(new_data))
        return;

    status = UserDataStatus::USER_CORRECT;
    current_data = new_data;
}
/*!
 * \brief Check if the provided string can be converted to number
 *
 * Use the QString method toInt to check if the given string data
 * can be transformed to the numerical value.
 *
 * \param[in] new_data - phone number in the string form from the input
 * \return True if correct numeric value and false otherwise
 */
bool PhoneNumberInput::checkNumberNumeric(const QString & new_data)
{
    bool convert_flag;

    int32_t numeric_dimension = new_data.toInt(&convert_flag);

    if(!convert_flag || numeric_dimension <= 0)
    {
        status = UserDataStatus::USER_WRONG;
        return false;
    }

    return true;
}
/*!
 * \brief Remove from the string all not numeric characters
 *
 * Remove or special characters from the given string, like '+',
 * dashes '-' or whitespaces. Check the length of the cleared
 * string.
 *
 * \param[in] new_data - phone number in the string form from the input
 * \return True if length of the filtered string is correct and false otherwise
 */
bool PhoneNumberInput::filterNumberFromChars(QString & new_data)
{
    new_data.remove(" ", Qt::CaseInsensitive);
    new_data.remove("-", Qt::CaseInsensitive);

    if(new_data[0] == "+")
        new_data.remove(0, 3);

    if(new_data.length() != MaxPhoneNumberLength)
    {
        status = UserDataStatus::USER_WRONG;
        return false;
    }

    return true;
}

























