#include "UserContactForm.hpp"

/*!
 * \brief Initialization of the class UserContactForm
 *
 * Create new instance of the inner widget, align the widgets vertically
 * and connect the signal from ContactData with appropriate slot (notify
 * about changed status of one of the string-like input data).
 *
 * \param title - title of the window
 * \param parent - pointer to the parent widget
 */
UserContactForm::UserContactForm(const char *title, QWidget *parent) : FramedWidget(title, parent)
{
    user_info = new ContactData(this);

    setVerticalLayout();

    connect(user_info, SIGNAL(contactStateChanged(bool)), this, SLOT(userDataEditing(bool)));
}
/*!
 * \brief Align the title and inner widget vertically
 *
 * Add additional spacer item between the title and ContactData object.
 */
void UserContactForm::setVerticalLayout()
{
    v_layout->addWidget(title);
    v_layout->addItem(new QSpacerItem(1, 10, QSizePolicy::Minimum, QSizePolicy::Minimum));
    v_layout->addWidget(user_info);
}
/*!
 * \brief Called when the data inside ContactData changed
 *
 * Pass the information about the changes further, by emitting the
 * appropriate signal depending on the status of the data.
 *
 * \param[in] data_valid - true if data is correct and false otherwise
 */
void UserContactForm::userDataEditing(bool data_valid)
{
    if(data_valid)
        emit userDataCorrect();
    else
        emit userDataWrong();
}
