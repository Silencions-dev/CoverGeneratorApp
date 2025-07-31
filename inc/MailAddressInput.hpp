#ifndef MAILADDRESSINPUT_HPP
#define MAILADDRESSINPUT_HPP

/*!
 * \file
 * \brief Definition of the class MailAddressInput
 *
 * This file contains definition of the class MailAddressInput,
 * which is responsible for checking the structure of the
 * email address provided by the user.
 */

#include "UserInput.hpp"

/*!
 * \brief Class that implements verification of the email address
 *
 * MailAddressInput inherits from the more general UserInput class,
 * which provides the visual interface for the user string-like input.
 * This class overrides the checkData() method and verifies the
 * specifically email address string.
 */
class MailAddressInput : public UserInput
{
    private:

        // Const attributes =========================================

        /*!
         * \brief Polish accepted domain
         */
        static constexpr const char *Domain_PL = "pl";
        /*!
         * \brief General accepted domain
         */
        static constexpr const char *Domain_COM = "com";
        /*!
         * \brief Maximum length of the email address
         */
        static constexpr uint8_t MailAddrLengthLimit = 50;

        // Private methods ==========================================

        /*!
         * \brief Check the domain of the provided email address
         */
        bool checkDomains(const QString & new_data);
        /*!
         * \brief Check if in the given string there is exactly one "at sign"
         */
        bool checkSpecialChars(const QString & new_data);
        /*!
         * \brief Check the correctness of the provided email address
         */
        void checkData() override;

    public:
        /*!
         * \brief Initialization of the class MailAddressInput
         */
        MailAddressInput(QWidget *parent = nullptr);
};

#endif // MAILADDRESSINPUT_HPP
