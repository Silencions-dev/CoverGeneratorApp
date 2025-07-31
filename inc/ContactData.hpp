#ifndef CONTACTDATA_HPP
#define CONTACTDATA_HPP

/*!
 * \file
 * \brief Definition of the class ContactData
 *
 * This file contains definition of the class ContactData,
 * which graphically arranges two data inputs: email
 * address and phone number.
 */

#include "PhoneNumberInput.hpp"
#include "MailAddressInput.hpp"

/*!
 * \def INFO_SIZE
 * \brief Number of data inputs to store in the widget
 */
#define INFO_SIZE 2

/*!
 * \brief Class that stores the inputs for the email and phone number
 *
 * ContactData class arranges two UserInput objects in a column
 * layout. It controls the state of the data (correct or not) and
 * eventually notify the parent widget (UserContactForm) about the
 * change.
 */
class ContactData : public QWidget
{
    Q_OBJECT
    private:

        // Functional members =======================================

        /*!
         * \brief Array with the inputs for the contact forms of the user
         */
        UserInput *user_data[INFO_SIZE];
        /*!
         * \brief Column layout for the descriptions of the inputs (QLabel)
         */
        QVBoxLayout *left_column;
        /*!
         * \brief Column layout for the input fields (QLineEdit)
         */
        QVBoxLayout *right_column;
        /*!
         * \brief Horizontal layout to align both columns
         */
        QHBoxLayout *columns_layout;

         // Private methods ==========================================

        /*!
         * \brief Set the graphics and behavior of the inner widgets
         */
        void setWidgets();
        /*!
         * \brief Set the layouts of the widget
         */
        void setLayout();

    public:
        /*!
         * \brief Initialization of the class ContactData
         */
        ContactData(QWidget *parent = nullptr);
        /*!
         * \brief Return the text stored in the given contact input (email or phone number)
         *
         * \param[in] index - number (ID) of the desired input
         * \return Const reference to the text stored in the QLineEdit of the given input
         */
        const QString & returnContactData(uint8_t index) { return user_data[index]->returnCurrentData(); }
        /*!
         * \brief Check if the data in the inputs is correct
         */
        bool isDataCorrect();

    public slots:
        /*!
         * \brief Notify about the changed state of the inputs
         */
        void inputStateChanged();

    signals:
        /*!
         * \brief Emit when one of the inputs has changed the state
         *
         * \param[in] data_valid - boolean with the info about correctness
         *                         of the data
         */
        void contactStateChanged(bool data_valid);
};

#endif // CONTACTDATA_HPP
