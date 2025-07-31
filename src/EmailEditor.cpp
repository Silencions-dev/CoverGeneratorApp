#include "EmailEditor.hpp"

/*!
 * \brief Initialization of the class EmailEditor
 *
 * Create new instance of the QTextEdit and set the
 * graphics and settings of the widgets.
 *
 * \param[in] widget_title - title of the window
 * \param[in] parent - pointer to the parent widget
 */
EmailEditor::EmailEditor(const char *widget_title, QWidget *parent) : FramedWidget(widget_title, parent)
{
    editor = new QTextEdit(this);

    setWidgetGraphics();
}
/*!
 * \brief Set the graphics of the inner widgets
 *
 * Set the options of the QTextEdit and define the
 * vertical layout.
 */
void EmailEditor::setWidgetGraphics()
{
    setStoredWidgets();
    setVerticalLayout();
}
/*!
 * \brief Set the settings of the inner widgets
 *
 * Set the terminal (QTextEdit) to be read only and
 * add temporary text in case of some failure during
 * mail generation. Define geometrical behavior of the
 * QTextEdit and connect the signal about possible copy
 * of the content by the user (text selection) with the
 * appropriate slot.
 */
void EmailEditor::setStoredWidgets()
{
    editor->setReadOnly(true);
    editor->setText("No content generated.\n");

    editor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(editor, SIGNAL(copyAvailable(bool)), this, SLOT(textSelectionChanged(bool)));
}
/*!
 * \brief Align the inner widgets vertically
 *
 * Align the title of the window and the QTextEdit terminal
 * vertically.
 */
void EmailEditor::setVerticalLayout()
{
    v_layout->addWidget(title);
    v_layout->addWidget(editor);
}
/*!
 * \brief Create the email title, header and attach results
 *
 * Read the title of the email from the appropriate file,
 * depending on the type of the required cover (standard
 * or special) marked by the boolean input argument 'std_cover'.
 * Display the title in the terminal (QTextEdit). Then, add
 * to the title ID number assigned by the server (not visible
 * for the user). Display the formatted text from the ResultTerminal
 * class and save the whole content (title + results) in the string.
 *
 * \param[in] std_cover - true if generated cover is standard and false otherwise
 * \param[in] results - results from the generator in the text format
 */
void EmailEditor::createGeneratorSummary(bool std_cover, const QString & results)
{
    if(std_cover)
        email_title = readContent(StdMailPath);
    else
        email_title = readContent(SpecialMailPath);

    editor->setPlainText(email_title + "\n");
    email_title += QString(" (ID: %1)").arg(order_id);

    editor->moveCursor(QTextCursor::End);
    editor->textCursor().insertText(results);
    editor->moveCursor(QTextCursor::Start);

    generator_summary = editor->toPlainText();
}
/*!
 * \brief Generate the full email message
 *
 * Add the contact data of the user at the beginning of
 * the email with already prepared content.
 *
 * \param[in] mail_address - email address of the user
 * \param[in] phone_number - phone number of the user (optional)
 */
void EmailEditor::generateMail(const QString & mail_address, const QString & phone_number)
{
    QString mail_info = "Adres email: ";
    QString phone_info = "Numer telefonu: ";

    mail_info += mail_address;
    phone_info += phone_number;

    QString contact_header = "Dane kontaktowe klienta\n";

    email_content = contact_header + mail_info + "\n" +
                    phone_info + "\n" + generator_summary;
}
/*!
 * \brief React to the change of the selection of the displayed text
 *
 * Notify about the text selection inside QTextEdit if appeared.
 * If the argument 'selection' is false, it means that the text
 * has been deselected and so signal is not emitted.
 *
 * \param[in] selection - true if selected and false otherwise
 */
void EmailEditor::textSelectionChanged(bool selection)
{
    if(selection)
        emit mailContentSelected();
}
/*!
 * \brief Read and return the text located in the given file
 *
 * Open the file with the text and simply read the whole
 * content of it, since it has only the title of the email
 * (one line). Return the copy of the loaded text or empty
 * string if there was an error during opening of the file.
 *
 * \param[in] path - path to the text file
 * \return Copy of the text read from the file
 */
QString EmailEditor::readContent(const char *path)
{
    QString content;

    QFile mail_file(path);

    if(mail_file.open(QIODevice::ReadOnly))
    {
        QTextStream text_str(&mail_file);
        content = text_str.readAll();
    }
    else
        std::cerr << "\nUnable to open messages file. Exit...\n";

    return content;
}













