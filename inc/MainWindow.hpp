#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

/*!
 * \file
 * \brief Definition of the class MainWindow
 *
 * This file contains definition of the class MainWindow,
 * which collects all top-level widgets and carries all
 * logical structure of the applciation.
 */

#include <QMainWindow>
#include <QStatusBar>
#include "DataWindow.hpp"
#include "ImagesViewer.hpp"
#include "Terminal.hpp"
#include "HPCover.hpp"
#include "CoverResultWindow.hpp"
#include "CoverPortfolioWindow.hpp"
#include "StartWindow.hpp"
#include "ContactWindow.hpp"
#include "Statistics.hpp"

/*!
 * \def MSG_NUM
 * \brief Number of the messages displayed in the main terminal
 */
#define MSG_NUM 5
/*!
 * \def MAX_STEPS
 * \brief Maximum number of the generator steps
 */
#define MAX_STEPS 3

/*!
 * \brief Main messages for the user in the generator part of the app
 */
enum Msg
{
    TYPE_INNER_DIM,    /*! Instruction for step 1 */
    TYPE_OBST_DIM,     /*! Instruction for step 2 */
    TYPE_MIN_SPACE,    /*! Instruction for step 3 */
    TYPE_DATA_CORRECT, /*! Information about correct input data */
    TYPE_DATA_WRONG    /*! Information about wrong input data */
};
/*!
 * \brief Sequence of the main parts of the application
 */
enum AppMainWidgets
{
    START_WIN,     /*! Main menu */
    GENERATOR,     /*! Cover generator */
    RESULTS_WIN,   /*! Window with the results */
    PORTFOLIO_WIN, /*! Window with the portfolio */
    CONTACT_WIN    /*! Contact window */
};

/*!
 * \brief Class that implements main window of the application
 *
 * MainWindow inherits from QMainWindow, which makes is the
 * custom place to store and manage visual side of the app.
 * It creates every top-level (framed) widget and so their
 * inner widgets. MainWindow manages the program flow and
 * apply corresponding graphical changes.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    private:

        // Functional members =======================================

        /*!
         * \brief Stack of main parts of the application
         *
         * It contains the parts presented in the enumeration
         * AppMainWidgets. Widgets can be easily switched and
         * so the whole appearance of the MainWindow.
         */
        QStackedWidget *pages;
        /*!
         * \brief Sends the statistics updates to the server
         */
        Statistics *app_statistics;
        /*!
         * \brief Container for the main widget (then set as central)
         */
        QWidget *main_widget;
        /*!
         * \brief Main menu of the app
         */
        StartWindow *start_window;
        /*!
         * \brief Contact form for the user
         */
        ContactWindow *contact;
        /*!
         * \brief Window with the results of successfully generated cover
         */
        CoverResultWindow *result_window;
        /*!
         * \brief Window with the portfolio of special covers
         */
        CoverPortfolioWindow *portfolio_window;
        /*!
         * \brief Displays and switch the images (like photo gallery)
         */
        ImagesViewer *images;
        /*!
         * \brief Displays formatted text in the main window
         */
        Terminal *terminal;
        /*!
         * \brief Widget with the inputs for the numeric data of the generator
         */
        DataWindow *input_dim;
        /*!
         * \brief Switches to the next part of the app
         */
        QPushButton *next_button;
        /*!
         * \brief Switches to the previous part of the app
         */
        QPushButton *prev_button;
        /*!
         * \brief Aligns two columns horizontally
         */
        QHBoxLayout *columns;
        /*!
         * \brief Right column for the smaller widgets
         *
         * Left column is just the ImagesViewer
         */
        QVBoxLayout *right_column;
        /*!
         * \brief Generates the cover based on the input data
         */
        HPCover *cover;
        /*!
         * \brief Current step of the generator (not page of the main window)
         */
        Steps current_step = Steps::STEP_1;
        /*!
         * \brief Numerical representative of the current step
         */
        uint8_t step_counter = 0;
        /*!
         * \brief Buffer for the messages displayed in the Terminal in the generator part of the app
         */
        QString messages[MSG_NUM];

        // Const attributes =========================================

        /*!
         * \brief Width of the main window
         */
        static constexpr int MainWindowWidth = 1100;
        /*!
         * \brief Height of the main window
         */
        static constexpr int MainWindowHeight = 700;
        /*!
         * \brief Maximum height of the buttons inside
         */
        static constexpr int ButtonMaxHeight = 45;
        /*!
         * \brief Path to the messages displayed during the generator part (info for each step)
         */
        static constexpr const char *GenStepsInfoPath = ":/descriptions/txt/Messages.txt";

        // Private methods ==========================================

        /*!
         * \brief Set the settings and graphics of the main buttons
         */
        void setMainButtons();
        /*!
         * \brief Set the graphics of the given button
         */
        void setButtonGraphics(QPushButton *button);
        /*!
         * \brief Set the background color of the given button
         */
        void setButtonColor(QPushButton *button, QColor color);
        /*!
         * \brief Disable the button
         */
        void lockButton(QPushButton *button);
        /*!
         * \brief Enable the button
         */
        void unlockButton(QPushButton *button);
        /*!
         * \brief Add all main widgets to the stack widget in the correct order
         */
        void addWidgetsToPages();
        /*!
         * \brief Set the main layout of the main window
         */
        void setLayout();
        /*!
         * \brief Connect all signals with the appropriate slots (event-based program flow)
         */
        void makeConnections();
        /*!
         * \brief Move the step counter according to the direction
         */
        void moveCounter(int move);
        /*!
         * \brief Generate cover and handle the results (success or fail)
         */
        void generateCover();
        /*!
         * \brief Read generator messages from the file
         */
        bool readMessages();
        /*!
         * \brief Display portfolio in the "fail" mode
         */
        void goToPortfolioFailed(std::vector<std::vector<int>> & inputs, GeneratorError & err_handler);
        /*!
         * \brief Display the message in the Terminal according to the current step
         */
        void matchMessageToStep(Steps step);

    public:
        /*!
         * \brief Initialization of the class MainWindow
         */
        MainWindow(QWidget *parent = nullptr);

    public slots:
        /*!
         * \brief Called when all data inputs in the current step are correct
         */
        void dataCorrect();
        /*!
         * \brief Called when at least one data input is wrong
         */
        void dataWrong(const QString & err_msg);
        /*!
         * \brief Called when at least one data input is still edited
         */
        void dataStillEditing();
        /*!
         * \brief Called when all inputs are disabled (by check boxes)
         */
        void dataNotGiven();
        /*!
         * \brief Change the appearance of the window for the next generator step
         */
        void goToNextStep();
        /*!
         * \brief Change the appearance of the window for the previous generator step
         */
        void goToPreviousStep();
        /*!
         * \brief Go back to the generator part of the application
         */
        void reEnterInputData();
        /*!
         * \brief Go to the generator part
         */
        void goToMainProgram();
        /*!
         * \brief Go to the portfolio window
         */
        void goToPortfolio();
        /*!
         * \brief Go to the main menu / start screen
         */
        void goToStartScreen();
        /*!
         * \brief Go to the contact form window
         */
        void goToContactWindow();
        /*!
         * \brief Update the server database, when user sent email through the app
         */
        void contactAutoUpdateDB();
        /*!
         * \brief Update the server database, when user sent email manually
         */
        void contactManualUpdateDB();
};

#endif // MAINWINDOW_HPP
