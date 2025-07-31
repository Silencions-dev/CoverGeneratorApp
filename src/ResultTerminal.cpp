#include "ResultTerminal.hpp"

/*!
 * \brief Initialization of the class ResultTerminal
 *
 * Create new instance of the QTextEdit and then load all predefined
 * texts and descriptions from the files. At the end set the graphics
 * of the stored widgets.
 *
 * \param[in] widget_title - title of the window
 * \param[in] parent - pointer to the parent widget
 */
ResultTerminal::ResultTerminal(const char *widget_title, QWidget *parent) : FramedWidget(widget_title, parent)
{
    terminal = new QTextEdit(this);

    readResultDescriptions(ResultDescPath_1, first_step_data_desc, MAX_DIM_INFO);
    readResultDescriptions(ResultDescPath_2, second_step_data_desc, MAX_DIM_INFO);
    readResultDescriptions(ResultDescPath_3, third_step_data_desc, MAX_SPEC_INFO);
    readResultDescriptions(ResultUserInfoPath, input_data_desc, STEPS);

    setWidgetGraphics();
}
/*!
 * \brief Set the graphics of the inner widgets
 *
 * Set the geometrical behavior of the own window and then
 * the settings of the terminal and finally main layout.
 */
void ResultTerminal::setWidgetGraphics()
{
    // Zchowanie geometrii obrazów
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    setStoredWidgets();
    setVerticalLayout();
}
/*!
 * \brief Set the settings and graphics of the inner widgets
 *
 * Set the QTextEdit to be "read only" and its alignment to be
 * center in the layout. Apply the designed style sheet file and
 * define the geometrical behavior.
 */
void ResultTerminal::setStoredWidgets()
{
    terminal->setReadOnly(true);
    terminal->setAlignment(Qt::AlignCenter);

    changeStyle(terminal, styles::EDIT_WIN_BASIC);

    terminal->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}
/*!
 * \brief Align title with the rest of inner widgets
 */
void ResultTerminal::setVerticalLayout()
{
    v_layout->addWidget(title);
    v_layout->addWidget(terminal);
}
/*!
 * \brief Display the text in the highest form of distinction
 *
 * Define the format to be magenta, bold and size of 16. Set the center
 * alignment. Display raw text with in the given format.
 *
 * \param[in] message - constant reference to the string that should be displayed
 */
void ResultTerminal::typeResultsTitle(const QString & message)
{
    QTextCharFormat title_format = terminal->textCursor().charFormat();

    title_format.setForeground(QBrush(QColor(SIL_PURPLE)));
    title_format.setFontWeight(QFont::Bold);
    title_format.setFontPointSize(16.0f);

    terminal->setAlignment(Qt::AlignCenter);

    typeText(message, title_format);
}
/*!
 * \brief Display the text as the header (high level of distinction)
 *
 * Define the format to be dark blue, bold and size of 13. Set the left
 * alignment. Display raw text with in the given format.
 *
 * \param[in] message - constant reference to the string that should be displayed
 */
void ResultTerminal::typeHeaderText(const QString & message)
{
    QTextCharFormat header_format = terminal->textCursor().charFormat();

    header_format.setForeground(QBrush(QColor(SIL_NAVY)));
    header_format.setFontWeight(QFont::Bold);
    header_format.setFontPointSize(13.0f);

    terminal->setAlignment(Qt::AlignLeft);

    typeText(message, header_format);
}
/*!
 * \brief Display the plain text (basic format)
 *
 * Define the format to be dark blue, normal and size of 13. Set the left
 * alignment. Display raw text with in the given format.
 *
 * \param[in] message - constant reference to the string that should be displayed
 */
void ResultTerminal::typePlainText(const QString & message)
{
    QTextCharFormat plain_format = terminal->textCursor().charFormat();

    plain_format.setForeground(QBrush(QColor(SIL_NAVY)));
    plain_format.setFontPointSize(13.0f);
    plain_format.setFontWeight(QFont::Normal);

    terminal->setAlignment(Qt::AlignLeft);

    typeText(message, plain_format);
}
/*!
 * \brief Display the bold colored text
 *
 * Define the format to be magenta, bold and size of 13.
 * Display raw text with in the given format.
 *
 * \param[in] message - constant reference to the string that should be displayed
 */
void ResultTerminal::typeBoldText(const QString & message)
{
    QTextCharFormat bold_format = terminal->textCursor().charFormat();

    bold_format.setForeground(QBrush(QColor(SIL_PURPLE)));
    bold_format.setFontPointSize(13.0f);
    bold_format.setFontWeight(QFont::Bold);

    typeText(message, bold_format);
}
/*!
 * \brief Display the bold red text
 *
 * Define the format to be red, bold and size of 13. Set the center
 * alignment. Display raw text with in the given format.
 *
 * \param[in] message - constant reference to the string that should be displayed
 */
void ResultTerminal::typeErrorText(const QString &message)
{
    QTextCharFormat error_format = terminal->textCursor().charFormat();

    error_format.setForeground(QBrush(QColor(255, 0, 0)));
    error_format.setFontPointSize(13.0f);
    error_format.setFontWeight(QFont::Bold);

    terminal->setAlignment(Qt::AlignCenter);

    typeText(message, error_format);
}
/*!
 * \brief Display the text in the given format
 *
 * Move the cursor of QTextEdit and the end and append the given text
 * (do not overwrite). Append new text to the appropriate string,
 * depending on the values of the boolean variables.
 *
 * \param[in] message - constant reference to the string that should be displayed
 * \param[in] format - constant reference to the format information
 */
void ResultTerminal::typeText(const QString & message, const QTextCharFormat & format)
{
    terminal->moveCursor(QTextCursor::End);
    terminal->textCursor().insertText(message, format);
    terminal->moveCursor(QTextCursor::End);

    if(save_results)
        results_info += message;
    if(save_inputs)
        input_info += message;
}
/*!
 * \brief Display the inner and outer dimensions of the generated cover
 *
 * Remove the currently visible text from the editor and also clear the
 * string with the old results. Set the "save_results" flag to be true and
 * print the inner and outer dimensions of the generated cover using
 * two for loops. At the end set the flag to false. In this way ResulTerminal
 * object always overwrite the old results and have new one ready for other
 * tasks, like generating the mail content.
 *
 * \param[in] inner_dim - constant reference to the array with inner dimensions
 * \param[in] outer_dim - constant reference to the array with outer dimensions
 */
void ResultTerminal::typeDimensions(const std::vector<uint16_t> & inner_dim,
                                    const std::vector<uint16_t> & outer_dim)
{
    terminal->clear();

    typeResultsTitle("Akustyczna Obudowa Modułowa\nWyniki\n\n");

    results_info.clear();
    save_results = true;

    typeHeaderText(QString("Wymiary zewnętrzne:\n"));

    QString outer_dim_labels[3] = {QString(" (X): "), QString(" (Y): "), QString(" (Z): ")};
    QString inner_dim_labels[3] = {QString(" (A): "), QString(" (B): "), QString(" (C): ")};

    for(uint8_t i = 0; i < outer_dim.size(); ++i)
    {
        QString text_dim = QString::number(outer_dim[i]);
        typePlainText(getDataDescription(0, i) + outer_dim_labels[i]);
        typeBoldText(text_dim);
        typePlainText(" mm\n");
    }

    typeHeaderText(QString("\nWymiary wewnętrzne:\n"));

    for(uint8_t i = 0; i < outer_dim.size(); ++i)
    {
        QString text_dim = QString::number(inner_dim[i]);
        typePlainText(getDataDescription(0, i) + inner_dim_labels[i]);
        typeBoldText(text_dim);
        typePlainText(" mm\n");
    }

    save_results = false;
}
/*!
 * \brief Display the input dimensions provided by the user
 *
 * Clear the string with the old input data from the user. Set the
 * "save_inputs" flag to true and print the input data using two
 * nested for loops. At the end set the flag to false. In this way ResulTerminal
 * object always overwrite the old inputs and have new one ready for other
 * tasks, like generating the mail content.
 *
 * \param[in] dimensions - constant reference to the 2D array with input data
 */
void ResultTerminal::typeInputs(const std::vector<std::vector<int>> & dimensions)
{

    input_info.clear();
    save_inputs = true;

    typeResultsTitle("\nPodane wymiary\n");

    for(uint8_t i = 0; i < dimensions.size(); ++i)
    {
        typeHeaderText(QString("\n") + input_data_desc[i] + QString("\n"));

        for(uint8_t j = 0; j < dimensions[i].size(); ++j)
        {
            typePlainText(getDataDescription(i, j) + QString(": "));

            if(dimensions[i][j] == -1)
                typeBoldText(QString("Nie podano\n"));
            else
            {
                typeBoldText(QString::number(dimensions[i][j]));
                typePlainText(" mm\n");
            }
        }
    }

    save_inputs = false;
}
/*!
 * \brief Display the information for the portfolio window
 *
 * Load the ready paragraphs from the file as a whole, split them
 * using new line characters and display each one in the appropriate
 * format.
 */
void ResultTerminal::typePortfolioInfo()
{
    QString info;
    readTheWholeText(InfoPortfolioPath, &info);
    QStringList paragraphs = info.split("\n\n");

    terminal->clear();

    typeResultsTitle(paragraphs[0].replace("\\n", "\n"));
    typePlainText(paragraphs[1].replace("\\n", "\n"));
    typeResultsTitle(paragraphs[2].replace("\\n", "\n"));
}
/*!
 * \brief Display the information about failure of the generator with the given error
 *
 * Load the ready paragraphs from the file as a whole, split them
 * using new line characters and display each one in the appropriate
 * format.
 *
 * \param[in] error_msg - constant reference to the error message returned by the generator
 */
void ResultTerminal::typeNoCoverGenerated(const QString & error_msg)
{
    QString info;
    readTheWholeText(InfoNotGeneratedPath, &info);
    QStringList paragraphs = info.split("\n\n");

    terminal->clear();

    typeResultsTitle(paragraphs[0].replace("\\n", "\n"));
    typeErrorText(error_msg);
    typePlainText(paragraphs[1].replace("\\n", "\n"));
    typeResultsTitle(paragraphs[2].replace("\\n", "\n"));
}
/*!
 * \brief Display information for the contact form window
 *
 * Load the ready paragraphs from the file as a whole, split them
 * using new line characters and display each one in the appropriate
 * format.
 */
void ResultTerminal::typeContactFormInfo()
{
    QString info;
    readTheWholeText(InfoContactFormPath, &info);
    QStringList paragraphs = info.split("\n\n");

    terminal->clear();

    typeResultsTitle(paragraphs[0].replace("\\n", "\n"));
    typePlainText(paragraphs[1].replace("\\n", "\n"));
    typePlainText(paragraphs[2].replace("\\n", "\n"));
    typeResultsTitle(paragraphs[3].replace("\\n", "\n"));
    typePlainText(paragraphs[4].replace("\\n", "\n"));
}
/*!
 * \brief Get the description of the data input at the given step and index
 *
 * Find the buffer, which corresponds to the given step using
 * "switch-case" and then assign the string from that buffer
 * using given index. Return the found string by copy.
 *
 * \param[in] step - step of the generator
 * \param[in] index - index of the description in the array
 * \return Copy of the string with the description
 */
QString ResultTerminal::getDataDescription(int step, int index)
{
    QString description;

    switch(step)
    {
        case 0:
            description = first_step_data_desc[index];
            break;
        case 1:
            description = second_step_data_desc[index];
            break;
        case 2:
            description = third_step_data_desc[index];
            break;
        default:
            break;
    }

    return description;
}
/*!
 * \brief Read the data descriptions/titles from the file
 *
 * Open the file and eventually return when not able to finish
 * this operation. Read the text line by line in a loop and
 * assign each line as the element of the target array.
 *
 * \param[in] path - path to the file
 * \param[in] target_buffer - pointer to the buffer for the texts
 * \param[in] data_quantity - number of expected texts
 */
void ResultTerminal::readResultDescriptions(const char *path, QString *target_buffer, uint8_t data_quantity)
{
    QFile info_file(path);

    if(!info_file.open(QIODevice::ReadOnly))
    {
        qDebug() << "\nUnable to open terminal data descriptions file. Exit.\n";
        return;
    }

    QTextStream text_str(&info_file);
    uint8_t i = 0;

    while(!text_str.atEnd())
    {
        QString line = text_str.readLine();

        if(!line.isEmpty() && i < data_quantity)
        {
            target_buffer[i] = line;
            ++i;
        }
    }

    info_file.close();
}
/*!
 * \brief Read the text from the file as one block
 *
 * Open the file and eventually return when not able to finish
 * this operation. Read the text as a whole to the given string
 * variable.
 *
 * \param[in] path - path to the file
 * \param[in] target_buffer - pointer to the string that should store the text
 */
void ResultTerminal::readTheWholeText(const char *path, QString *target_buffer)
{
    QFile info_file(path);

    if(!info_file.open(QIODevice::ReadOnly))
    {
        qDebug() << "\nUnable to open terminal information file. Exit.\n";
        return;
    }

    *target_buffer = info_file.readAll();

    info_file.close();
}
