#include "CoverResultWindow.hpp"

/*!
 * \brief Initialization of the class CoverResultWindow
 *
 * Create new instances of the inner widgets. Adjust
 * the options and graphics.
 *
 * \param[in] parent - pointer to the parent widget
 */
CoverResultWindow::CoverResultWindow(QWidget *parent) : SummaryWindow(parent)
{
    back_button = new QPushButton(tr("Cofnij"), this);
    contact_button = new QPushButton(tr("Kontakt"), this);
    results = new ResultTerminal("Podsumowanie", this);
    images = new ImagesViewer("Wygenerowana obudowa", this);

    setStoredWidgets();
    setWidgetsLayout();
}
/*!
 * \brief Sets the settings and graphics of the inner widgets
 *
 * Connect the signals from the buttons with the appropriate
 * slots. Set the appearance of each button.
 */
void CoverResultWindow::setStoredWidgets()
{
    connect(back_button, SIGNAL(pressed()), this, SLOT(backButtonPressed()));
    connect(contact_button, SIGNAL(pressed()), this, SLOT(contactButtonPressed()));

    setButtonGraphics(contact_button);
    setButtonGraphics(back_button);
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
void CoverResultWindow::setWidgetsLayout()
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
 * \brief Load the images for the appropriate size of the cover
 *
 * Load the images prepared for the result window according
 * to the number of the wall modules. Number of modules determine
 * the size of the cover (S, M, L or XL) and so the path to the
 * correct images.
 *
 * \param[in] modules_pcs - number of modules
 */
void CoverResultWindow::getModulesQuantity(uint8_t modules_pcs)
{
    if(modules_pcs == 0)
        images->loadResultImages(S_Size);
    else if(modules_pcs == 2)
        images->loadResultImages(M_Size);
    else if(modules_pcs == 4)
        images->loadResultImages(L_Size);
    else if(modules_pcs == 6)
        images->loadResultImages(XL_Size);
}
/*!
 * \brief Get the output dimensions from the generator (results)
 *
 * Pass the numeric results to the terminal, which will convert
 * them to the formatted string and display on the screen.
 *
 * \param[in] inner_dim - constant reference to the 1D array with inner cover dimensions
 * \param[in] outer_dim - constant reference to the 1D array with outer cover dimensions
 */
void CoverResultWindow::getGeneratorResults(const std::vector<uint16_t> & inner_dim,
                                            const std::vector<uint16_t> & outer_dim)
{
    results->typeDimensions(inner_dim, outer_dim);
}
/*!
 * \brief Get the input dimensions from the generator (typed by the user)
 *
 * Pass the numeric input data to the terminal, which will convert
 * them to the formatted string and display on the screen. Inputs are
 * added to the previously printed results, so at the end there is an
 * instruction to scroll up the QTextEdit.
 *
 * \param[in] dimensions - reference to the 2D array with input dimensions
 */
void CoverResultWindow::getGeneratorInputs(std::vector<std::vector<int>> & dimensions)
{
    results->typeInputs(dimensions);
    results->scrollUp();
}
