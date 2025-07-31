#include "FramedWidget.hpp"

/*!
 * \brief Initialization of the class FramedWidget
 *
 * Create the new instances of the inner widgets. Change the
 * text of the window title to be strongly bold and aligned
 * to the top left corner of the widget. Define geometrical
 * behavior of the title. Set the appearance of the frame:
 * both frame style and line width.
 *
 * \param[in] widget_title - title of the window
 * \param[in] parent - pointer to the parent widget
 */
FramedWidget::FramedWidget(const char *widget_title, QWidget *parent) : QFrame(parent)
{
    // Tworzy tytuł widgetu i zarządcę wertykalnego
    title = new QLabel(tr(widget_title));
    // Pogrubione tytuły obramowanych widżetów
    QFont bold_font;
    bold_font.setBold(true);
    bold_font.setWeight(QFont::ExtraBold);

    title->setFont(bold_font);
    title->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    title->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    v_layout = new QVBoxLayout(this);
    // Nadaje odpowiedni styl obramowaniu
    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    // Grubość linii obramowania (maksymalna)
    setLineWidth(3);
}
/*!
 * \brief Change the appearance of the given widget
 *
 * Function outside the FramedWidget class. It reads the style sheet
 * file located at the given path, apply the style to the given widget
 * and then force the update on the screen. Close the file at the end.
 *
 * \param[in] widget - element to change style
 * \param[in] path - path to the CSS-like style sheet file
 */
void changeStyle(QWidget *widget, const char *path)
{
    QFile style_file(path);
    style_file.open(QFile::ReadOnly);
    widget->setStyleSheet(QString::fromLatin1(style_file.readAll()));
    widget->update();
    style_file.close();
}
