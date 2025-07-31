#ifndef CONTACTWINDOW_HPP
#define CONTACTWINDOW_HPP

/*!
 * \file
 * \brief Definition of the class ContactWindow
 *
 * This file contains definition of the class ContactWindow,
 * which includes necessary informations for the user in order
 * to contact with the company via email.
 */

#include <QTextEdit>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include "ResultTerminal.hpp"
#include "EmailEditor.hpp"
#include "UserContactForm.hpp"

/*!
 * \brief Class that implements layout of the contact window
 *
 * ContactWindow class consists of multiple widgets organized
 * in way to represent the window with contact information.
 * This is: automatically generated text of the email (which
 * can be copied by the user), necessary informations (company
 * email adress, instructions) and button to go back to the main app.
 */
class ContactWindow : public QWidget
{
    Q_OBJECT
    private:

        // Functional members =======================================

        /*!
         * \brief Manager of the network access (HTTP requests)
         */
        QNetworkAccessManager *net_access;
        /*!
         * \brief Button used to go back to the main window
         */
        QPushButton *back_button;
        /*!
         * \brief Button used to send an email from the client
         */
        QPushButton *send_button;
        /*!
         * \brief Widget with automatically generated mail content
         */
        EmailEditor *email;
        /*!
         * \brief Get contact info from the user (optionally)
         */
        UserContactForm *user_contact;
        /*!
         * \brief Widget with additional informations / instructions
         */
        ResultTerminal *info;
        /*!
         * \brief Main layout of the contact window
         */
        QVBoxLayout *main_layout;
        /*!
         * \brief Vertical layout of the right column
         */
        QVBoxLayout *right_column;
        /*!
         * \brief Horizontal layout (part of main layout)
         */
        QHBoxLayout *h_layout;
        /*!
         * \brief Counter of the sent emails
         */
        uint8_t send_counter = 0;
        /*!
         * \brief Limit of the mails sent per session (default is 2)
         */
        uint8_t send_limit = 2;
        /*!
         * \brief URL to the server, which sends the email
         */
        QString mail_url;
        /*!
         * \brief Receiver of the mail (static target)
         */
        QString mail_receiver;
        /*!
         * \brief API key obtained from the server
         */
        QString api_key;
        /*!
         * \brief Flag, which indicates if it is possible to send the mail
         */
        bool mail_enabled = false;

        // Const attributes =========================================

        /*!
         * \brief Path to the configuration file of the mail service
         */
        static constexpr const char *MailConnectConfig = ":/network_params/network/MailSrvConctConfig.json";
        /*!
         * \brief Text of the back button
         */
        static constexpr const char *BackBttnText = "Wróć do generatora";
        /*!
         * \brief Default text of the send button
         */
        static constexpr const char *SendBttnText = "Wyślij maila";
        /*!
         * \brief Text of the send button, when sending is in process
         */
        static constexpr const char *SendingBttnText = "Wysyłanie...";
        /*!
         * \brief Text of the send button, when message has been sent
         */
        static constexpr const char *SentBttnText = "Wiadomość wysłana";
        /*!
         * \brief Text of the send button, when the email limit has been reached
         */
        static constexpr const char *LimitReachedBttnText = "Osiągnięto limit wysyłania wiadomości";
        /*!
         * \brief Maximum height of the buttons in the window
         */
        static constexpr const uint8_t BttnMaxHeight = 45;

        // Private methods ==========================================

        /*!
         * \brief Sets the layout of the window
         */
        void setLayout();
        /*!
         * \brief Sets the graphics and geometry of the widgets
         */
        void setWidgetsGraphics();
        /*!
         * \brief Sets the graphics of the given button
         */
        void setButtonGraphics(QPushButton *button);
        /*!
         * \brief Read the parameteres for the mail service from the file
         */
        void getMailServerConfig();
        /*!
         * \brief Disable the given button
         */
        void lockButton(QPushButton *button);
        /*!
         * \brief Enable the given button
         */
        void unlockButton(QPushButton *button);
        /*!
         * \brief Send the email from the app
         */
        void sendEmail();
        /*!
         * \brief Check if the send limit has been reached
         */
        bool limitReached();
        /*!
         * \brief Change the name of the "send email" button
         */
        void changeSendButtonName();
        /*!
         * \brief Adjust the window if the email has been sent
         */
        void mailSentSuccessfully();
        /*!
         * \brief Adjust the window if the email failed to sent
         */
        void mailSentFailed(const QString error_msg);

    public:
        /*!
         * \brief Initialization of the class ContactWindow
         */
        ContactWindow(QWidget *parent = nullptr);
        /*!
         * \brief Displays info and invokes generation of the mail content
         */
        void generateMail(bool std_cover, const QString & summary);

    public slots:
        /*!
         * \brief Slot called when "back" button is pressed
         */
        void backButtonPressed() { emit this->goToGenerator(); }
        /*!
         * \brief Send an email with the address and/or phone number of the user
         */
        void sendButtonPressed();
        /*!
         * \brief Enable "send mail" button
         */
        void enableMailButton();
        /*!
         * \brief Disable "send mail" button
         */
        void disableMailButton();
        /*!
         * \brief Check the mail service response
         */
        void mailSendingFinished(QNetworkReply *reply);
        /*!
         * \brief Notify that the generated text has been selected
         */
        void passMailContentSelected() { emit this->userCopiedContent(); }
        /*!
         * \brief Get the API key from the Statistics class if obtained
         *
         * \param[in] received_key - copy of the key
         */
        void getServerAPIKey(const QString received_key) { api_key = received_key; }
        /*!
         * \brief Get the current session ID fomr the Statistics class if obtained
         *
         * \param[in] session_id - ID of the app session (used for the email title)
         */
        void getSessionID(const int session_id) { email->getOrderID(session_id); }

    signals:
        /*!
         * \brief Signal with request to go back to the main app
         *
         * This signal is emitted when "back" button is pressed (invoked
         * in the corresponding slot).
         */
        void goToGenerator();
        /*!
         * \brief Emitted when user has sent the mail
         */
        void userSendMail();
        /*!
         * \brief Emitted when user has seleceted the text (assumed copy)
         */
        void userCopiedContent();
};

#endif // CONTACTWINDOW_HPP
