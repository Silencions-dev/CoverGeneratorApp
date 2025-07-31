#include "ContactWindow.hpp"

/*!
 * \brief Initialization of the class ContactWindow
 *
 * Create new instances of the inner widgets. Adjust the graphics
 * of the widgets and main layout of the window. Connect signals
 * with appropriate slots. Initially lock the button, which sends
 * email. Read the parameters of the email service provided by the
 * server (parameters in the configuration file).
 *
 * \param[in] parent - pointer to the parent widget
 */
ContactWindow::ContactWindow(QWidget *parent) : QWidget(parent)
{
    net_access = new QNetworkAccessManager(this);
    back_button = new QPushButton(BackBttnText, this);
    send_button = new QPushButton(SendBttnText, this);
    email = new EmailEditor("Treść maila", this);
    user_contact = new UserContactForm("Dane kontaktowe", this);
    info = new ResultTerminal("Informacje", this);
    main_layout = new QVBoxLayout(this);
    right_column = new QVBoxLayout();
    h_layout = new QHBoxLayout();

    setWidgetsGraphics();
    setLayout();

    connect(back_button, SIGNAL(pressed()), this, SLOT(backButtonPressed()));
    connect(send_button, SIGNAL(pressed()), this, SLOT(sendButtonPressed()));
    connect(user_contact, SIGNAL(userDataCorrect()), this, SLOT(enableMailButton()));
    connect(user_contact, SIGNAL(userDataWrong()), this, SLOT(disableMailButton()));
    connect(net_access, SIGNAL(finished(QNetworkReply*)), this, SLOT(mailSendingFinished(QNetworkReply*)));
    connect(email, SIGNAL(mailContentSelected()), this, SLOT(passMailContentSelected()));

    // Wait for the email address to unlock the button
    lockButton(send_button);
    // Get mail server connection config
    getMailServerConfig();
}
/*!
 * \brief Sets the graphics and geometry of the widgets
 *
 * Adjust the graphics of each button. Define geometrical
 * behavior of each inner widget.
 */
void ContactWindow::setWidgetsGraphics()
{
    setButtonGraphics(back_button);
    setButtonGraphics(send_button);

    email->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    info->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    user_contact->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    back_button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    send_button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}
/*!
 * \brief Sets the layout of the window
 *
 * Collect the terminal and contact data in one column.
 * Then align QTextEditor with that column and set the
 * little space between them. Finally define the main
 * vertical layout, which contains horizontal layout,
 * spacer item and two buttons.
 */
void ContactWindow::setLayout()
{
    right_column->addWidget(info);
    right_column->addWidget(user_contact);
    h_layout->addWidget(email);
    h_layout->addItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Minimum));
    h_layout->addLayout(this->right_column);
    h_layout->setStretch(0, 1);
    h_layout->setStretch(2, 1);
    main_layout->addLayout(h_layout);
    main_layout->addItem(new QSpacerItem(10, 10, QSizePolicy::Preferred, QSizePolicy::Minimum));
    main_layout->addWidget(send_button);
    main_layout->addWidget(back_button);
}
/*!
 * \brief Sets the graphics of the given button
 *
 * Change the color of the text visible in the button. Set
 * maximum height of the button and initially enable it (change
 * the style).
 *
 * \param[in] button - pointer to the button
 */
void ContactWindow::setButtonGraphics(QPushButton *button)
{
    QPalette buttons_palette;
    buttons_palette.setColor(QPalette::ButtonText, QColor(QRgb(SIL_NAVY)));
    button->setPalette(buttons_palette);

    unlockButton(button);

    button->setMaximumHeight(BttnMaxHeight);
}
/*!
 * \brief Read the parameteres for the mail service from the file
 *
 * Open the file with the parameters. If failed, disable the
 * sending email function and simply return. Otherwise read the
 * whole content and treat it as a JSON data. If it is a JSON, get
 * the values under the known keys and assign them to the appropriate
 * variables. If JSON document is empty, return false. At the end
 * enable the sending email function.
 */
void ContactWindow::getMailServerConfig()
{
    QFile config(MailConnectConfig);

    if(!config.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        mail_enabled = false;
        return;
    }

    QByteArray data = config.readAll();
    config.close();
    QJsonDocument json_doc = QJsonDocument::fromJson(data);

    if(!json_doc.isObject())
    {
        mail_enabled = false;
        return;
    }

    QJsonObject json_obj = json_doc.object();
    mail_url = json_obj.value("mail_url").toString();
    mail_receiver = json_obj.value("mail_receiver").toString();
    send_limit = json_obj.value("limit").toInt();

    mail_enabled = true;
}
/*!
 * \brief Disable the given button
 *
 * Turn off the functionality of the button and change
 * the style so it looks like "disabled".
 *
 * \param[in] button - pointer to the button
 */
void ContactWindow::lockButton(QPushButton *button)
{
    button->setFlat(true);
    button->setEnabled(false);

    changeStyle(button, styles::BUTTON_LOCKED);
}
/*!
 * \brief Enable the given button
 *
 * Turn on the functionality of the button and change
 * the style so it looks like "enabled".
 *
 * \param[in] button - pointer to the button
 */
void ContactWindow::unlockButton(QPushButton *button)
{
    button->setFlat(false);
    button->setEnabled(true);

    changeStyle(button, styles::BUTTON_UNLOCKED);
}
/*!
 * \brief Displays info and invokes generation of the mail content
 *
 * Scroll up the terminal at the end of the function
 * call in order to show the text from the beginning.
 *
 * \param[in] std_cover - text of the mail for the standard cover (True)
 *                        or for the custom one (False)
 * \param[in] summary - string with the results of the cover generator
 */
void ContactWindow::generateMail(bool std_cover, const QString & summary)
{
    info->typeContactFormInfo();
    email->createGeneratorSummary(std_cover, summary);
    info->scrollUp();
}
/*!
 * \brief Send an email with the address and/or phone number of the user
 *
 * Change the text of the sending button to show that
 * the request is currently processed. Lock both send
 * and back buttons to force the user to wait for the
 * response from the server. Force the graphical update
 * on the screen and send email request.
 */
void ContactWindow::sendButtonPressed()
{
    send_button->setText(SendingBttnText);
    lockButton(send_button);
    lockButton(back_button);

    repaint();

    sendEmail();
    // Wait for the results...
}
/*!
 * \brief Send the email from the app
 *
 * Verify first if the email functionality is enabled by both
 * application and server connection. If so, prepare the HTTP
 * request with proper header. Get the contact data and pass it
 * to the EmailEditor, so it can generate the complete email.
 * Pack the title, content and the receeiver of the email into a
 * JSON document and attach it to the request. Finally send the
 * request to the server.
 */
void ContactWindow::sendEmail()
{
    if(!mail_enabled)
    {
        mailSentFailed(QString("Błąd wewnętrzny aplikaji."));
        return;
    }

    if(api_key.isEmpty())
    {
        mailSentFailed(QString("Brak połączenia z serwerem."));
        return;
    }

    QUrl url(mail_url);
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", api_key.toUtf8());

    const QString user_mail_addr = user_contact->returnContactData(0);
    const QString user_phone_number = user_contact->returnContactData(1);
    const QString email_title = email->emailTitle();

    this->email->generateMail(user_mail_addr, user_phone_number);
    const QString content = email->emailContent();

    // Build the JSON payload
    QJsonObject json;
    json["To"] = mail_receiver;
    json["Subject"] = email_title;
    json["Content"] = content;

    QJsonDocument jsonDoc(json);

    this->net_access->post(request, jsonDoc.toJson());
}
/*!
 * \brief Enable "send mail" button
 *
 * Change the name of the send button and unlock it
 * if user has not reached the email limit yet.
 */
void ContactWindow::enableMailButton()
{
    changeSendButtonName();

    if(!limitReached())
        unlockButton(send_button);
}
/*!
 * \brief Disable "send mail" button
 *
 * Change the name of the send button and lock it.
 */
void ContactWindow::disableMailButton()
{
    changeSendButtonName();

    lockButton(send_button);
}
/*!
 * \brief Check if the send limit has been reached
 *
 * \return True if limit reached, false otherwise
 */
bool ContactWindow::limitReached()
{
    bool limit_reached = false;

    if(send_counter > send_limit)
        limit_reached = true;

    return limit_reached;
}
/*!
 * \brief Change the name of the "send email" button
 *
 * If the limit has not been reached yet, change the
 * name to the standard one. Otherwise, set text with
 * the message that the limit has been reached.
 */
void ContactWindow::changeSendButtonName()
{
    if(send_counter <= send_limit)
        send_button->setText(SendBttnText);
    else
        send_button->setText(LimitReachedBttnText);
}
/*!
 * \brief Check the mail service response
 *
 * If the response does not have the error, notify the
 * higher widgets about successful operation and adjust
 * the graphics of the contact window. If failed, display
 * the appropriate message on the terminal.
 *
 * \param[in] reply - pointer to the reponse from the external server
 */
void ContactWindow::mailSendingFinished(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        mailSentSuccessfully();
        emit userSendMail();
    }
    else
        mailSentFailed(reply->errorString());
}
/*!
 * \brief Adjust the window if the email has been sent
 *
 * Change the text of the send button to the standard one.
 * Unclock only the go back button, refresh the text inside
 * the terminal and increment the sending counter.
 */
void ContactWindow::mailSentSuccessfully()
{
    send_button->setText(SentBttnText);
    unlockButton(back_button);

    info->typeContactFormInfo();
    info->scrollUp();

    ++send_counter;
}
/*!
 * \brief Adjust the window if the email failed to sent
 *
 * Unlock all buttons and change the text of the send button
 * to the standard one. Add the message about the error to the
 * text visible in the terminal.
 *
 * \param[in] error_msg - constant string with the detailed description
 *                        of the network error (debug only)
 */
void ContactWindow::mailSentFailed(const QString error_msg)
{
    send_button->setText(SendBttnText);
    unlockButton(back_button);
    unlockButton(send_button);

    info->typeContactFormInfo();
    info->addErrorMessage(QString("\n\nBłąd podczas wysyłania maila. Proszę "
                                        "spróbować ponownie lub wysłać wiadomość "
                                        "ręcznie.\n\n"));

    // For debug only
    // this->info->addErrorMessage(QString("Error: ") + error_msg);
}

