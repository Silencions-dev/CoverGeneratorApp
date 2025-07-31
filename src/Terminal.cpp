#include "Terminal.hpp"

/*!
 * \brief Initialization of the class Terminal
 *
 * Create new instance of the inner widget (QLabel), set the geometrical
 * behavior of the own window and trigger the setting of the inner widgets
 * along with the layout.
 *
 * \param[in] widget_title - title of the window
 * \param[in] parent - pointer to the parent widget
 */
Terminal::Terminal(const char *terminal_title, QWidget *parent) : FramedWidget(terminal_title, parent)
{
    editor = new QLabel(this);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    setStoredWidgets();
    setVerticalLayout();
}
/*!
 * \brief Set the settings and graphics of the inner widgets
 *
 * Change the appearance of the text (font, size, color) and behavior
 * of the text inside the QLabel. Change the graphics and size policy
 * of the QLabel.
 */
void Terminal::setStoredWidgets()
{
    QFont big_letters;
    big_letters.setPointSize(16);
    QPalette line_edit_palette;
    line_edit_palette.setColor(QPalette::Window, QColor(QRgb(SIL_GRAY)));
    line_edit_palette.setColor(QPalette::WindowText, QColor(QRgb(SIL_NAVY)));

    editor->setWordWrap(true);
    editor->setAlignment(Qt::AlignCenter);
    editor->setAutoFillBackground(true);
    editor->setFont(big_letters);
    editor->setPalette(line_edit_palette);
    editor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}
/*!
 * \brief Align the title of the window with the stored widget vertically
 */
void Terminal::setVerticalLayout()
{
    v_layout->addWidget(title);
    v_layout->addWidget(editor);
}
/*!
 * \brief Set the background color of the widget according to the given palette
 *
 * \param[in] palette - palette with defined color
 */
void Terminal::setBackgroundColor(const QPalette & palette)
{
    editor->setAutoFillBackground(true);
    editor->setPalette(palette);
}
/*!
 * \brief Print the given string in the QLabel
 *
 * \param[in] message - constant reference to the string
 */
void Terminal::typeFormatMsg(const QString & message)
{
    editor->setText(message);
}
/*!
 * \brief Overloaded << operator for displaing messages
 *
 * Call the appropriate method of the given Terminal object in
 * order to display the text in QLabel.
 *
 * \param[in] term - reference to the Terminal object
 * \param[in] message - constant reference to the string
 * \return Reference to the object of the class Terminal
 */
Terminal & operator<< (Terminal & term, const QString & message)
{
    term.typeFormatMsg(message);
    return term;
}
