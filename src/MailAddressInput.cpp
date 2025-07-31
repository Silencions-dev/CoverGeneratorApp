#include "MailAddressInput.hpp"

/*!
 * \brief Initialization of the class MailAddressInput
 *
 * Call the constructor of the parent in the initialization list.
 * Set the input data description and the placeholder text inside
 * the QLineEdit.
 *
 * \param[in] parent - pointer to the parent widget
 */
MailAddressInput::MailAddressInput(QWidget *parent) : UserInput(parent)
{
    data_label->setText("Adres email*:");
    data_input->setPlaceholderText("example@domain.com");
}
/*!
 * \brief Check the correctness of the provided email address
 *
 * Get the data from the QLineEdit field and check it with
 * different criteria. First, check if the input field is empty.
 * Then, check if the length of the new string does not exceed
 * the limit. Check if the email address has exactly one "at sign"
 * and if the domains are recognizable (.pl and .com). Overwrite
 * status and currently stored data.
 */
void MailAddressInput::checkData()
{
    QString new_data = data_input->text();

    if(new_data.isEmpty())
    {
        status = UserDataStatus::USER_NEUTRAL;
        return;
    }

    if(new_data.length() > MailAddrLengthLimit)
    {
        status = UserDataStatus::USER_WRONG;
        return;
    }

    if(!checkSpecialChars(new_data))
        return;

    if(!checkDomains(new_data))
        return;

    status = UserDataStatus::USER_CORRECT;
    current_data = new_data;
}
/*!
 * \brief Check if in the given string there is exactly one "at sign"
 *
 * Count all "at signs" in a given string and check if there is
 * exactly one. If there is 0 or more than 1, change the status
 * to wrong and return false.
 *
 * \param[in] new_data - typed string
 * \return True if there is one "at sign" and false otherwise
 */
bool MailAddressInput::checkSpecialChars(const QString & new_data)
{
    uint8_t monkey_num = new_data.count("@", Qt::CaseInsensitive);

    if(monkey_num != 1)
    {
        status = UserDataStatus::USER_WRONG;
        return false;
    }

    return true;
}
/*!
 * \brief Check the domain of the provided email address
 *
 * First split the new string using dots as a separator.
 * If there is less than two parts (no dots), return false.
 * Then check the last element on the list, which should be
 * a domain. Compare it with the defined ones (.pl and .com)
 * and determine correctness.
 *
 * \param[in] new_data - typed string
 * \return True if domain is recognized and false otherwise
 */
bool MailAddressInput::checkDomains(const QString & new_data)
{
    QStringList dot_fragments = new_data.split(".");

    if(dot_fragments.size() < 2)
        return false;

    QString domain = dot_fragments.last();

    if((domain != QString(Domain_PL)) && (domain != QString(Domain_COM)))
    {
        status = UserDataStatus::USER_WRONG;
        return false;
    }

    return true;
}
