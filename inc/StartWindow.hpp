#ifndef STARTWINDOW_HPP
#define STARTWINDOW_HPP

/*!
 * \file
 * \brief Definition of the class StartWindow
 *
 * This file contains definition of the class StartWindow,
 * which implements the start screen - first window after
 * opening the applciation.
 */

#include <QPushButton>
#include <QPicture>
#include "FramedWidget.hpp"
#include "StylesPaths.hpp"

/*!
 * \brief Class that implements start screen of the application
 *
 * StartWindow is the first widget after launching the app. It
 * contains logo and two buttons, which makes it in fact a very
 * simple menu. User can go straight to the main program (generator)
 * or see the portfolio with the special covers first.
 */
class StartWindow : public QWidget
{
    Q_OBJECT
    private:

        // Functional members =======================================

        /*!
         * \brief Container for the logo of the company visible at the center of the screen
         */
        QLabel *logo_image;
        /*!
         * \brief Button that directs to the main program (generator)
         */
        QPushButton *generate_button;
        /*!
         * \brief Button that directs to the basic portfolio window
         */
        QPushButton *portfolio_button;
        /*!
         * \brief Main vertical layout
         */
        QVBoxLayout *v_layout;

        // Const attributes =========================================

        /*!
         * \brief Width of the logo
         */
        static constexpr int LogoWidth = 300;
        /*!
         * \brief Height of the logo
         */
        static constexpr int LogoHeight = 300;
        /*!
         * \brief Minimum width of the buttons
         */
        static constexpr int MinButtonWidth = 400;
        /*!
         * \brief Minimum height of the buttons
         */
        static constexpr int MinButtonHeight = 50;
        /*!
         * \brief Path to the image with the logo
         */
        static constexpr const char *LogoPath = ":/Logo/images/silencions_logo.png";

        // Private methods ==========================================

        /*!
         * \brief Set the settings and graphics of the inner widgets
         */
        void setStoredWidgets();
        /*!
         * \brief Set the layout of the inner widgets (vertical alignment)
         */
        void setWidgetsLayout();
        /*!
         * \brief Set the graphics of the given button
         */
        void setButtonGraphics(QPushButton *button);

    public:
        /*!
         * \brief Initialization of the class StartWindow
         */
        StartWindow(QWidget *parent = nullptr);

    public slots:
        /*!
         * \brief Called when the generator button has been clicked
         */
        void generatorButtonClicked();
        /*!
         * \brief Called when the portfolio button has been clicked
         */
        void portfolioButtonClicked();

    signals:
        /*!
         * \brief Notify the main window about switching to the generator
         */
        void goToGenerator();
        /*!
         * \brief Notify the main window about switching to the portfolio
         */
        void goToPortfolio();
};

#endif // STARTWINDOW_HPP
