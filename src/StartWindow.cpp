#include "StartWindow.hpp"

/*!
 * \brief Initialization of the class StartWindow
 *
 * Create new instances of the inner widgets, set their settings and
 * connect the buttons with appropriate slots.
 *
 * \param[in] parent - pointer to the parent widget
 */
StartWindow::StartWindow(QWidget *parent) : QWidget(parent)
{
    logo_image = new QLabel(this);
    generate_button = new QPushButton(tr("    Generowanie obudowy    "), this);
    portfolio_button = new QPushButton(tr("    Nasze realizacje    "), this);
    v_layout = new QVBoxLayout(this);

    setStoredWidgets();

    connect(generate_button, SIGNAL(pressed()), this, SLOT(generatorButtonClicked()));
    connect(portfolio_button, SIGNAL(pressed()), this, SLOT(portfolioButtonClicked()));
}
/*!
 * \brief Set the settings and graphics of the inner widgets
 *
 * Load the logo image from the file. Then, set the own background color
 * and visual appearance of the buttons. Finally align the widgets.
 */
void StartWindow::setStoredWidgets()
{
    QPixmap image;
    image.load(LogoPath);
    image = image.scaled(LogoWidth, LogoHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->logo_image->setPixmap(image);

    QPalette palette;
    palette.setColor(QPalette::Window, QColor(QRgb(SIL_GRAY)));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    this->setButtonGraphics(this->generate_button);
    this->setButtonGraphics(this->portfolio_button);

    this->setWidgetsLayout();
}
/*!
 * \brief Set the layout of the inner widgets (vertical alignment)
 *
 * Align all widgets in one column with additional spaces between them.
 * Place the layout at the center of the window.
 */
void StartWindow::setWidgetsLayout()
{
    this->v_layout->addWidget(this->logo_image, 0, Qt::AlignCenter);
    this->v_layout->addItem(new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Preferred));
    this->v_layout->addWidget(this->generate_button, 0, Qt::AlignCenter);
    this->v_layout->addItem(new QSpacerItem(10, 15, QSizePolicy::Minimum, QSizePolicy::Preferred));
    this->v_layout->addWidget(this->portfolio_button, 0, Qt::AlignCenter);

    this->v_layout->setAlignment(Qt::AlignCenter);
}
/*!
 * \brief Set the graphics of the given button
 *
 * Set the minimium size of the button, text color, geometrical
 * behavior and apply style sheet (basic enabled button).
 * Force the update method to refresh the appearance in the window.
 *
 * \param[in] button - pointer to the button
 */
void StartWindow::setButtonGraphics(QPushButton *button)
{
    button->setMinimumHeight(MinButtonHeight);
    button->setMinimumWidth(MinButtonWidth);

    QPalette buttons_palette;
    buttons_palette.setColor(QPalette::ButtonText, QColor(QRgb(SIL_NAVY)));
    button->setPalette(buttons_palette);

    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    button->setAutoFillBackground(true);

    button->setFlat(false);
    button->setEnabled(true);
    changeStyle(button, styles::BUTTON_UNLOCKED);

    button->update();
}
/*!
 * \brief Called when the generator button has been clicked
 *
 * Emit the signal, which notifies the MainWindow to switch to
 * the generator part of the application.
 */
void StartWindow::generatorButtonClicked()
{
    emit this->goToGenerator();
}
/*!
 * \brief Called when the portfolio button has been clicked
 *
 * Emit the signal, which notifies the MainWindow to switch to
 * the portoflio part of the applciation.
 */
void StartWindow::portfolioButtonClicked()
{
    emit this->goToPortfolio();
}
