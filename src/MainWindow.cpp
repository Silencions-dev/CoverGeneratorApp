#include "MainWindow.hpp"

/*!
 * \brief Initialization of the class MainWindow
 *
 * Create new instances of the stored inner widgets. Then, set the
 * parameters of the main window, like size and bottom message.
 * Add main parts of the application to the QStackedWidget, load all
 * messages for the generator part, set the graphics and settings of
 * the buttons, connect slots with signals and finally create alignment.
 *
 * \param[in] parent - pointer to the parent widget
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    pages = new QStackedWidget(this);
    main_widget = new QWidget();
    start_window = new StartWindow();
    contact = new ContactWindow();
    result_window = new CoverResultWindow();
    portfolio_window = new CoverPortfolioWindow();
    // Establish connection with the server
    app_statistics = new Statistics(this);
    // Create rest of the app elements
    cover = new HPCover();
    images = new ImagesViewer("Przykłady", main_widget);
    input_dim = new DataWindow("Wymiary wewnętrzne", main_widget);
    terminal = new Terminal("Komunikaty", this);
    next_button = new QPushButton("Dalej", main_widget);
    prev_button = new QPushButton("Cofnij", main_widget);
    right_column = new QVBoxLayout();
    columns = new QHBoxLayout(main_widget);

    setStatusBar(new QStatusBar());
    statusBar()->showMessage("Generator s|HP|");
    setFixedSize(MainWindowWidth, MainWindowHeight);

    if(!cover->readParameters())
        qDebug() << "Error during reading parameters. Probably wrong path or filename. Abort.\n";

    addWidgetsToPages();
    readMessages();
    setMainButtons();
    makeConnections();
    setLayout();

    // App ready to work
    *terminal << messages[Msg::TYPE_INNER_DIM];
}
/*!
 * \brief Add all main widgets to the stack widget in the correct order
 *
 * Add the widgets in the correct sequence. Finally set the QStackedWidget
 * as the main one and display the initial part of the application, which
 * is the start window (menu).
 */
void MainWindow::addWidgetsToPages()
{
    pages->addWidget(start_window);
    pages->addWidget(main_widget);
    pages->addWidget(result_window);
    pages->addWidget(portfolio_window);
    pages->addWidget(contact);

    setCentralWidget(pages);
    pages->setCurrentIndex(AppMainWidgets::START_WIN);
}
/*!
 * \brief Set the graphics of the given button
 *
 * Change the color of the text inside the button and set the
 * geometrical behavior of it. Set the maximum height of the
 * button.
 *
 * \param[in] button - pointer to the button
 */
void MainWindow::setButtonGraphics(QPushButton *button)
{
    QPalette buttons_palette;
    buttons_palette.setColor(QPalette::ButtonText, QColor(QRgb(SIL_NAVY)));
    button->setPalette(buttons_palette);

    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    button->setAutoFillBackground(true);
    button->setMaximumHeight(ButtonMaxHeight);
}
/*!
 * \brief Set the settings and graphics of the main buttons
 *
 * Apply the basic appearance setting on each button and then
 * lock the 'go next' button and unlock the 'go back' one (initial
 * state).
 */
void MainWindow::setMainButtons()
{
    setButtonGraphics(next_button);
    setButtonGraphics(prev_button);

    lockButton(next_button);
    unlockButton(prev_button);
}
/*!
 * \brief Connect all signals with the appropriate slots (event-based program flow)
 *
 * Those connections are the major logic of the application and determine the
 * visual responses to the user actions.
 */
void MainWindow::makeConnections()
{
    connect(this->result_window, SIGNAL(goBackToGenerator()), this, SLOT(reEnterInputData()));
    connect(this->result_window, SIGNAL(goToContact()), this, SLOT(goToContactWindow()));

    connect(this->portfolio_window, SIGNAL(goToContact()), this, SLOT(goToContactWindow()));
    connect(this->portfolio_window, SIGNAL(goBackToMenu()), this, SLOT(goToStartScreen()));
    connect(this->portfolio_window, SIGNAL(goBackToGenerator()), this, SLOT(goToMainProgram()));

    connect(this->input_dim, SIGNAL(allDataCorrect()), this, SLOT(dataCorrect()));
    connect(this->input_dim, SIGNAL(oneDataWrong(const QString &)), this, SLOT(dataWrong(const QString &)));
    connect(this->input_dim, SIGNAL(oneDataEditing()), this, SLOT(dataStillEditing()));
    connect(this->input_dim, SIGNAL(noInputsGiven()), this, SLOT(dataNotGiven()));

    connect(this->next_button, SIGNAL(pressed()), this, SLOT(goToNextStep()));
    connect(this->prev_button, SIGNAL(pressed()), this, SLOT(goToPreviousStep()));

    connect(this->start_window, SIGNAL(goToGenerator()), this, SLOT(goToMainProgram()));
    connect(this->start_window, SIGNAL(goToPortfolio()), this, SLOT(goToPortfolio()));

    connect(this->contact, SIGNAL(goToGenerator()), this, SLOT(goToMainProgram()));
    connect(this->contact, SIGNAL(userSendMail()), this, SLOT(contactAutoUpdateDB()));
    connect(this->contact, SIGNAL(userCopiedContent()), this, SLOT(contactManualUpdateDB()));

    connect(this->app_statistics, SIGNAL(apiKeyReceived(const QString)), this->contact, SLOT(getServerAPIKey(const QString)));
    connect(this->app_statistics, SIGNAL(sessionIDAssigned(int)), this->contact, SLOT(getSessionID(int)));
}
/*!
 * \brief Set the main layout of the main window
 *
 * MainWindow aligns only the generator part of the application.
 * Rest of the elements, like contact window, results or portfolio,
 * are completed in the separate classes and only switched by
 * QStackedWidget. This method creates the right column of widgets
 * and aligns it horizontally with the image viewer. Proportions between
 * the image viewer and the right column is set to 3:2.
 */
void MainWindow::setLayout()
{
    this->right_column->addWidget(this->input_dim);
    this->right_column->addItem(new QSpacerItem(10, 15, QSizePolicy::Minimum, QSizePolicy::Preferred));
    this->right_column->addWidget(this->terminal);
    this->right_column->addItem(new QSpacerItem(10, 15, QSizePolicy::Minimum, QSizePolicy::Preferred));
    this->right_column->addWidget(this->next_button);
    this->right_column->addWidget(this->prev_button);

    this->columns->addWidget(this->images);
    this->columns->addItem(new QSpacerItem(10, 10, QSizePolicy::Preferred, QSizePolicy::Minimum));
    this->columns->addLayout(this->right_column);

    this->columns->setStretch(0, 3);
    this->columns->setStretch(2, 2);
}
/*!
 * \brief Set the background color of the given button
 *
 * Set the given color using QPalette object and call
 * update method, so the new appearance of the button
 * is immediately pushed to the screen.
 *
 * \param[in] button - pointer to the button
 * \param[in] color - color of the button
 */
void MainWindow::setButtonColor(QPushButton *button, QColor color)
{
    QPalette new_palette = button->palette();
    new_palette.setColor(QPalette::Button, color);
    button->setPalette(new_palette);

    button->update();
}
/*!
 * \brief Disable the button
 *
 * Set the button to be flat (no visual effect after clicking),
 * disable it (no signals from the button) and apply the appropriate
 * style.
 *
 * \param[in] button - pointer to the button
 */
void MainWindow::lockButton(QPushButton *button)
{
    button->setFlat(true);
    button->setEnabled(false);

    changeStyle(button, styles::BUTTON_LOCKED);
}
/*!
 * \brief Enable the button
 *
 * Turn off the flat option (button is visually clickable),
 * enable the signals and set the appropriate style.
 *
 * \param[in] button - pointer to the button
 */
void MainWindow::unlockButton(QPushButton *button)
{
    button->setFlat(false);
    button->setEnabled(true);

    changeStyle(button, styles::BUTTON_UNLOCKED);
}
/*!
 * \brief Called when all data inputs in the current step are correct
 *
 * Enable the 'go next' button and display the message that
 * data is correct.
 */
void MainWindow::dataCorrect()
{
    unlockButton(next_button);
    *terminal << messages[Msg::TYPE_DATA_CORRECT];
}
/*!
 * \brief Called when at least one data input is wrong
 *
 * Disable the 'go next' button and display the error message.
 *
 * \param[in] err_msg - error message returned by one of the inputs
 */
void MainWindow::dataWrong(const QString & err_msg)
{
    lockButton(next_button);
    *terminal << err_msg;
}
/*!
 * \brief Called when at least one data input is still edited
 *
 * Disable the 'go next' button and display the default message,
 * which corresponds to the current step of the generator.
 */
void MainWindow::dataStillEditing()
{
    lockButton(next_button);
    matchMessageToStep(static_cast<Steps>(step_counter));
}
/*!
 * \brief Called when all inputs are disabled (by check boxes)
 *
 * Enable the 'go next' button and display the default message,
 * which corresponds to the current step of the generator.
 */
void MainWindow::dataNotGiven()
{
    unlockButton(next_button);
    matchMessageToStep(static_cast<Steps>(step_counter));
}
/*!
 * \brief Change the appearance of the window for the next generator step
 *
 * Trigger the generation of the cover, if the step counter reached the
 * limit. If not, increment the counter and rearrange the main window.
 */
void MainWindow::goToNextStep()
{
    // Protection from counter overflow
    // Generate the cover
    if(step_counter >= MAX_STEPS - 1)
    {
        input_dim->saveData(static_cast<Steps>(step_counter));
        generateCover();
        return;
    }

    moveCounter(1);
}
/*!
 * \brief Change the appearance of the window for the previous generator step
 *
 * If the step counter reached the bottom limit, go to the start window.
 * If not, decrement the counter and rearrange the main window.
 */
void MainWindow::goToPreviousStep()
{
    // Protection from counter overflow
    if(step_counter == 0)
    {
        pages->setCurrentIndex(AppMainWidgets::START_WIN);
        return;
    }

    moveCounter(-1);
}
/*!
 * \brief Move the step counter according to the direction
 *
 * Save the data at the current step and update the counter.
 * Then rearrange the main window according to the new step
 * of the generator. Notify the outer database about going
 * into the next step.
 *
 * \param[in] move - determines the direction (-1 or +1)
 */
void MainWindow::moveCounter(int move)
{
    // Save current data
    input_dim->saveData(static_cast<Steps>(this->step_counter));
    // Update step and so the widgets
    step_counter += move;
    current_step = static_cast<Steps>(step_counter);
    input_dim->goToStep(current_step);
    images->goToStep(current_step);
    input_dim->checkAllInputs();
    // Send the current step to the database
    app_statistics->updateStepsHistory(step_counter + 1);
}
/*!
 * \brief Generate cover and handle the results (success or fail)
 *
 * Set the current main widget to the result window. Generate the cover
 * and check the results. If failed, switch to the portoflio window with
 * appropriate information about failure. If succeeded, pass the dimensions
 * to the result window. Send the created string with the results from result
 * window to the contact window. In both cases, fail or success, notify the
 * outer database about the result.
 */
void MainWindow::generateCover()
{
    pages->setCurrentIndex(AppMainWidgets::RESULTS_WIN);

    if(!cover->generateCover(input_dim->returnDimensions()))
    {
        goToPortfolioFailed(input_dim->returnDimensions(), cover->returnErrorHandler());
        contact->generateMail(false, portfolio_window->returnTerminalContents(false));
        app_statistics->updateResultsType(false);

        return;
    }

    result_window->getGeneratorResults(cover->returnInnerDim(),
                                       cover->returnOuterDim());

    result_window->getGeneratorInputs(cover->returnFinalDimensions());
    result_window->getModulesQuantity(cover->returnModulesQuantity());
    contact->generateMail(true, result_window->returnTerminalContents());
    // Update server statistics DB
    app_statistics->updateResultsType(true);
    app_statistics->updateGeneratorRerun();
}
/*!
 * \brief Display the message in the Terminal according to the current step
 *
 * Iterate over steps enumeration using 'switch-case' and display the
 * appropriate message.
 *
 * \param[in] step - step of the generator (enumeration)
 */
void MainWindow::matchMessageToStep(Steps step)
{
    switch(step)
    {
        case Steps::STEP_1:
            *terminal << messages[Msg::TYPE_INNER_DIM];
            break;
        case Steps::STEP_2:
            *terminal << messages[Msg::TYPE_OBST_DIM];
            break;
        case Steps::STEP_3:
            *terminal << messages[Msg::TYPE_MIN_SPACE];
            break;
    }
}
/*!
 * \brief Go back to the generator part of the application
 */
void MainWindow::reEnterInputData()
{
    pages->setCurrentIndex(AppMainWidgets::GENERATOR);
}
/*!
 * \brief Go to the generator part
 *
 * Stop the timer of the portoflio after leaving and switch
 * to the generator part of the app. Notify the outer database
 * about reaching the new step.
 */
void MainWindow::goToMainProgram()
{
    portfolio_window->stopSlideTimer();
    pages->setCurrentIndex(AppMainWidgets::GENERATOR);
    // Update server statistics DB
    app_statistics->updateStepsHistory(step_counter + 1);
}
/*!
 * \brief Go to the portfolio window
 *
 * Display basic portfolio with special covers. Start the slide show
 * and notify outer database about reaching the portfolio.
 */
void MainWindow::goToPortfolio()
{
    portfolio_window->switchToBasic();
    portfolio_window->startSlideTimer();
    pages->setCurrentIndex(AppMainWidgets::PORTFOLIO_WIN);
    // Update server statistics DB
    app_statistics->updatePortfolioViewed();
}
/*!
 * \brief Display portfolio in the "fail" mode
 *
 * Pass user input data and generator error message to the portfolio
 * widget. Start the slide show.
 *
 * \param[in] inputs - generator inputs from the user
 * \param err_handler - error message of the generator (why cover failed to generate)
 */
void MainWindow::goToPortfolioFailed(std::vector<std::vector<int>> & inputs, GeneratorError & err_handler)
{
    portfolio_window->switchToFailed(inputs, err_handler);
    portfolio_window->startSlideTimer();
    pages->setCurrentIndex(AppMainWidgets::PORTFOLIO_WIN);
}
/*!
 * \brief Go to the contact form window
 */
void MainWindow::goToContactWindow()
{
    pages->setCurrentIndex(AppMainWidgets::CONTACT_WIN);
}
/*!
 * \brief Go to the main menu / start screen
 *
 * Before reaching start screen, stop the timer of the
 * previously displayed portfolio widget.
 */
void MainWindow::goToStartScreen()
{
    portfolio_window->stopSlideTimer();
    pages->setCurrentIndex(AppMainWidgets::START_WIN);
}
/*!
 * \brief Update the server database, when user sent email through the app
 */
void MainWindow::contactAutoUpdateDB()
{
    app_statistics->updateContactType(true);
}
/*!
 * \brief Update the server database, when user sent email manually
 */
void MainWindow::contactManualUpdateDB()
{
    app_statistics->updateContactType(false);
}
/*!
 * \brief Read generator messages from the file
 *
 * Open the file with the messages of the generator. If cannot open,
 * return false and print the message on the qDebug output. Read the
 * file line by line and assign each one as a separate string in the
 * target array.
 *
 * \return True if loaded and false otherwise
 */
bool MainWindow::readMessages()
{
    QFile msg_file(GenStepsInfoPath);

    if(msg_file.open(QIODevice::ReadOnly))
    {
        QTextStream text_str(&msg_file);
        uint8_t i = 0;
        while(!text_str.atEnd())
        {
            QString qt_line = text_str.readLine();

            if(!qt_line.isEmpty() && i < MSG_NUM)
            {
                messages[i] = qt_line;
                ++i;
            }
        }
    }
    else
    {
        qDebug() << "\nUnable to open steps info file. Exit...\n";
        return false;
    }

    msg_file.close();

    return true;
}

