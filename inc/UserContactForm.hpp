#ifndef USERCONTACTFORM_HPP
#define USERCONTACTFORM_HPP

/*!
 * \file
 * \brief Definition of the class UserContactForm
 *
 * This file contains definition of the class UserContactForm,
 * which collects the input fields for the contact data of
 * the user.
 */

#include "ContactData.hpp"

/*!
 * \brief Class that implements the visible widget for providing the contact data
 *
 * UserContactForm inherits from the FramedWidget, which makes
 * it the top-level widget visible in the main window (with
 * title and frame). It encapsulates already prepared ContactData
 * class with the title and frame, so UserContactForm has more
 * estetical purpose.
 */
class UserContactForm : public FramedWidget
{
    Q_OBJECT
    private:

        // Functional members =======================================

        /*!
         * \brief Input fields for the contact data
         */
        ContactData *user_info;

        // Private methods ==========================================

        /*!
         * \brief Align the title and inner widget vertically
         */
        void setVerticalLayout();

    public:
        /*!
         * \brief Initialization of the class UserContactForm
         */
        UserContactForm(const char *title, QWidget *parent = nullptr);
        /*!
         * \brief Return the contact data given by the index
         *
         * \param[in] index - ID of the contact data (0 for an email and 1 for a phone number)
         * \return Constant reference to the string with a contact data
         */
        const QString & returnContactData(uint8_t index) { return this->user_info->returnContactData(index); }

    public slots:
        /*!
         * \brief Called when the data inside ContactData changed
         */
        void userDataEditing(bool data_valid);

    signals:
        /*!
         * \brief Notify ContactWindow that provided contact data is correct
         */
        void userDataCorrect();
        /*!
         * \brief Notify ContactWindow that provided contact data is wrong
         */
        void userDataWrong();
};

#endif // USERCONTACTFORM_HPP
