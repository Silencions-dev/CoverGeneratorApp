#ifndef COVERPORTFOLIOWINDOW_HPP
#define COVERPORTFOLIOWINDOW_HPP

/*!
 * \file
 * \brief Definition of the class CoverPortfolioWindow
 *
 * This file contains definition of the class CoverPortfolioWindow,
 * which displays portfolio of the custom covers with some
 * additional information.
 */

#include "SummaryWindow.hpp"
#include "SlidesViewer.hpp"
#include "GeneratorError.hpp"

/*!
 * \brief Class that implements layout of the portoflio window
 *
 * CoverPortfolioWindow inherits the SummaryWindow features, especially
 * the main layout. It creates a slide show with the renders of
 * designed custom covers. Additionally, there are some information
 * that depend on state of the application: failed generation of the cover
 * or just viewing the portfolio.
 */
class CoverPortfolioWindow : public SummaryWindow
{
    Q_OBJECT
    private:

        // Functional members =======================================
        /*!
         * \brief Version (state) of the Portfolio window
         */
        enum PortfolioState
        {
            BASIC, /*! Basic portfolio window */
            FAILED /*! Portfolio window, when generator failed */
        };
        /*!
         * \brief Silde show of the portfolio images
         */
        SlidesViewer *images;
        /*!
         * \brief Current role of the portfolio in main window
         */
        PortfolioState state;

        // Private methods ==========================================

        /*!
         * \brief Sets the settings of the inner widgets
         */
        void setStoredWidgets();
        /*!
         * \brief Sets the layout of the window
         */
        void setWidgetsLayout();

    public:
        /*!
         * \brief Initialization of the class CoverPortfolioWindow
         */
        CoverPortfolioWindow(QWidget *parent = nullptr);
        /*!
         * \brief Display info connected to the failed generation
         */
        void switchToFailed(std::vector<std::vector<int>> & inputs, GeneratorError & err_handler);
        /*!
         * \brief Display info when casually watching portfolio
         */
        void switchToBasic();
        /*!
         * \brief Starts the timer of the slide show
         */
        void startSlideTimer() { images->startSlideShow(); }
        /*!
         * \brief Stops the timer of the slide show
         */
        void stopSlideTimer() { images->stopSlideShow(); }

    public slots:
        /*!
         * \brief Slot called when "back" button is pressed
         */
        void backButtonPressed() override;

    signals:
        /*!
         * \brief Signal sent to go back to the main menu of the app (from basic protfolio)
         */
        void goBackToMenu();
        /*!
         * \brief Signal sent to go back to the generator (from failed portfolio)
         */
        void goBackToGenerator();
};

#endif // COVERPORTFOLIOWINDOW_HPP
