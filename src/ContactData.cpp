#include "ContactData.hpp"

/*!
 * \brief Initialization of the class ContactData
 *
 * Create the new instances of the inner widgets. Adjust
 * the settings of the widgets and the main layout.
 *
 * \param[in] parent - pointer to the parent widget
 */
ContactData::ContactData(QWidget *parent) : QWidget(parent)
{
    user_data[0] = new MailAddressInput(this);
    user_data[1] = new PhoneNumberInput(this);

    columns_layout = new QHBoxLayout(this);
    left_column = new QVBoxLayout();
    right_column = new QVBoxLayout();

    setWidgets();
    setLayout();
}
/*!
 * \brief Set the graphics and behavior of the inner widgets
 *
 * Connect signals about changed status from every data input
 * to the slot, which triggers the verification of the data.
 * Define the geometrical behavior of the widget.
 */
void ContactData::setWidgets()
{
    for(uint8_t i = 0; i < INFO_SIZE; ++i)
        connect(user_data[i], SIGNAL(dataChanged()), this, SLOT(inputStateChanged()));

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}
/*!
 * \brief Set the layouts of the widget
 *
 * Define two column (vertical layouts): left for the descriptions
 * and right for the data inputs. Align left column to the right and
 * right to the left. Finally align both columns horizontally and
 * move them to the center of the window. Set additional spacing between
 * columns.
 */
void ContactData::setLayout()
{
    for(uint8_t i = 0; i < INFO_SIZE; ++i)
    {
        QLabel *data_label = user_data[i]->dataLabel();
        QLineEdit *data_input = user_data[i]->dataInput();

        data_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        left_column->addWidget(data_label);
        right_column->addWidget(data_input);
    }

    left_column->setAlignment(Qt::AlignRight);
    right_column->setAlignment(Qt::AlignLeft);

    columns_layout->addLayout(left_column);
    columns_layout->addLayout(right_column);
    columns_layout->setAlignment(Qt::AlignCenter);
    columns_layout->setSpacing(10);
}
/*!
 * \brief Check if the data in the inputs is correct
 *
 * Get the status of each contact data (email address and
 * phone number) and check the correctness. Return false if
 * the email status is different than correct or the phone number
 * is wrong (phone number is optional, thus the status can be
 * neutral). Otherwise return true.
 *
 * \return True if correct (email especially) and false if wrong
 */
bool ContactData::isDataCorrect()
{
    UserDataStatus mail_status = user_data[0]->returnStatus();
    UserDataStatus phone_status = user_data[1]->returnStatus();

    if(mail_status != UserDataStatus::USER_CORRECT)
        return false;

    // Phone number is an optional step
    if(phone_status == UserDataStatus::USER_WRONG)
        return false;

    return true;
}
/*!
 * \brief Notify about the changed state of the inputs
 *
 * Get the overall status of the contact data and emit
 * signal with the boolean (correct or wrong / true or false)
 * to notify the ContactWindow, which will enable or disable
 * sending email.
 */
void ContactData::inputStateChanged()
{
    bool data_state = isDataCorrect();

    emit contactStateChanged(data_state);
}
