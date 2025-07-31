#ifndef PHONENUMBERINPUT_HPP
#define PHONENUMBERINPUT_HPP

/*!
 * \file
 * \brief Definition of the class PhoneNumberInput
 *
 * This file contains definition of the class PhoneNumberInput,
 * which is responsible for checking the structure of the
 * phone number provided by the user.
 */

#include "UserInput.hpp"

/*!
 * \brief Class that implements verification of the phone number
 *
 * PhoneNumberInput inherits from the more general UserInput class,
 * which provides the visual interface for the user string-like input.
 * This class overrides the checkData() method and verifies the
 * specifically phone number string.
 */
class PhoneNumberInput : public UserInput
{
    private:

        // Const attributes =========================================

        /*!
         * \brief Correct length of the polish phone number
         */
        static constexpr uint8_t MaxPhoneNumberLength = 9;

        // Private methods ==========================================

        /*!
         * \brief Check if the provided string can be converted to number
         */
        bool checkNumberNumeric(const QString & new_data);
        /*!
         * \brief Remove from the string all not numeric characters
         */
        bool filterNumberFromChars(QString & new_data);
        /*!
         * \brief Check the correctness of the provided phone number
         */
        void checkData() override;

    public:
        /*!
         * \brief Initialization of the class PhoneNumberInput
         */
        PhoneNumberInput(QWidget *parent = nullptr);
};

#endif // PHONENUMBERINPUT_HPP
