#include "SummaryWindow.hpp"

/*!
 * \brief Initialization of the class SummaryWindow
 *
 * Create new instances of the two layouts (vertical and horizontal).
 *
 * \param[in] parent - pointer to the parent widget
 */
SummaryWindow::SummaryWindow(QWidget *parent) : QWidget(parent)
{
    main_hor_layout = new QHBoxLayout(this);
    main_ver_layout = new QVBoxLayout();
}
/*!
 * \brief Return the text displayed by the result terminal
 *
 * Build the output string by assigning and eventually adding texts
 * retured by the ResultTerminal.
 *
 * \param[in] all_data - true for the cover dimensions and user inputs and false for the input data only
 * \return Constant text from the terminal
 */
const QString SummaryWindow::returnTerminalContents(bool all_data)
{
    QString output;

    if(all_data)
    {
        output = results->returnResultSummary();
        output += results->returnInputSummary();
    }
    else
        output = results->returnInputSummary();

    return output;
}
/*!
 * \brief Set the graphics of the given button
 *
 * Change the color of the text, style of the button (basic enabled button) and
 * set the geometrical behavior.
 *
 * \param button - pointer to the button
 */
void SummaryWindow::setButtonGraphics(QPushButton *button)
{
    button->setEnabled(true);

    QPalette buttons_palette;
    buttons_palette.setColor(QPalette::ButtonText, QColor(QRgb(SIL_NAVY)));
    button->setPalette(buttons_palette);

    changeStyle(button, styles::BUTTON_UNLOCKED);

    button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    button->setMaximumHeight(ButtonMaxHeight);
}
