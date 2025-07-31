#include "CoverPortfolioWindow.hpp"

/*!
 * \brief Initialization of the class CoverPortfolioWindow
 *
 * Create new instances of the inner widgets. Adjust
 * the options and graphics.
 *
 * \param[in] parent - pointer to the parent widget
 */
CoverPortfolioWindow::CoverPortfolioWindow(QWidget *parent) : SummaryWindow(parent)
{
    contact_button = new QPushButton(tr("Kontakt"), this);
    back_button = new QPushButton(tr("Cofnij"), this);
    results = new ResultTerminal("Podsumowanie", this);
    images = new SlidesViewer("Obudowy specjalne", this);

    setStoredWidgets();
    setWidgetsLayout();
}
/*!
 * \brief Sets the settings of the inner widgets
 *
 * Connect the signals from the buttons with the appropriate
 * slots. Set the appearance of each button. Load the images
 * prepared for the portfolio and start the automatic slide
 * show.
 */
void CoverPortfolioWindow::setStoredWidgets()
{
    connect(back_button, SIGNAL(pressed()), this, SLOT(backButtonPressed()));
    connect(contact_button, SIGNAL(pressed()), this, SLOT(contactButtonPressed()));

    setButtonGraphics(contact_button);
    setButtonGraphics(back_button);

    images->loadPortfolioImages();
    images->startSlideShow();
}
/*!
 * \brief Sets the layout of the window
 *
 * First, align the right column of the widgets (result
 * terminal and two buttons). Then, align the image viewer
 * with the right column horizontally with additional separation
 * (spacer item). Set the proportion of the left widget and
 * right column to 3:2.
 */
void CoverPortfolioWindow::setWidgetsLayout()
{
    main_ver_layout->addWidget(results);
    main_ver_layout->addItem(new QSpacerItem(10, 15, QSizePolicy::Minimum, QSizePolicy::Preferred));
    main_ver_layout->addWidget(contact_button);
    main_ver_layout->addWidget(back_button);

    main_hor_layout->addWidget(images);
    main_hor_layout->addItem(new QSpacerItem(10, 10, QSizePolicy::Preferred, QSizePolicy::Minimum));
    main_hor_layout->addLayout(main_ver_layout);

    main_hor_layout->setStretch(0, 3);
    main_hor_layout->setStretch(2, 2);
}
/*!
 * \brief Display info connected to the failed generation
 *
 * This method sets the window in the case of the failure
 * during generation of the cover. It prints proper message
 * along with the input dimensions given by the user. It
 * also shows the button to the contact window.
 *
 * \param[in] inputs - 2D array with dimensions given by the user
 */
void CoverPortfolioWindow::switchToFailed(std::vector<std::vector<int>> & inputs, GeneratorError & err_handler)
{
    results->typeNoCoverGenerated(err_handler.returnErrorMessage());
    results->typeInputs(inputs);
    results->scrollUp();
    contact_button->setVisible(true);

    state = PortfolioState::FAILED;
}
/*!
 * \brief Display info when casually watching portfolio
 *
 * Display the information correlated with the basic portfolio.
 * Hide the contact button, since basic portfolio does not
 * have the results or summary, it is not a consequence of
 * the generator.
 */
void CoverPortfolioWindow::switchToBasic()
{
    results->typePortfolioInfo();
    results->scrollUp();
    contact_button->setVisible(false);

    state = PortfolioState::BASIC;
}
/*!
 * \brief Slot called when "back" button is pressed
 *
 * Since portfolio can be viewed in two states and in
 * two different parts of the program, back button triggers
 * different action. If the portfolio is in a basic mode, button
 * should switch to the start screen. If the portfolio is a part
 * of the results, button should switch to the generator.
 */
void CoverPortfolioWindow::backButtonPressed()
{
    if(state == PortfolioState::BASIC)
        emit goBackToMenu();
    else if(state == PortfolioState::FAILED)
        emit goBackToGenerator();
}
