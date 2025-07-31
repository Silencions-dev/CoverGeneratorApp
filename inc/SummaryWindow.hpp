#ifndef SUMMARYWINDOW_HPP
#define SUMMARYWINDOW_HPP

/*!
 * \file
 * \brief Definition of the class SummaryWindow
 *
 * This file contains definition of the class SummaryWindow,
 * which is the base class for the CoverPortfolioWindow and
 * CoverResultWindow.
 */

#include <QPushButton>
#include <QHBoxLayout>
#include "ImagesViewer.hpp"
#include "ResultTerminal.hpp"
#include "StylesPaths.hpp"

/*!
 * \brief Class that implements the universal appearance of the summary
 *
 * SummaryWindow provides universal elements for the portfolio and
 * result windows, like buttons and ResultTerminal, since both
 * classes differ only in the type of the used image viewers.
 * From the summary window user can go to the contact form or
 * back to the generator.
 */
class SummaryWindow : public QWidget
{
    Q_OBJECT
    protected:

        // Functional members =======================================

        /*!
         * \brief Button used to go back (usually to the generator)
         */
        QPushButton *back_button;
        /*!
         * \brief Button used to go to the contact window
         */
        QPushButton *contact_button;
        /*!
         * \brief Widget used to display the formatted information (summary)
         */
        ResultTerminal *results;
        /*!
         * \brief Aligns columns horizontally (image viewer and right column)
         */
        QHBoxLayout *main_hor_layout;
        /*!
         * \brief Aligns inner widgets vertically
         */
        QVBoxLayout *main_ver_layout;

        // Const attributes =========================================

        /*!
         * \brief Maximum height of the buttons
         */
        static constexpr int ButtonMaxHeight = 45;

        // Protected methods ========================================

        /*!
         * \brief Set the graphics of the given button
         */
        void setButtonGraphics(QPushButton *button);

    public:
        /*!
         * \brief Initialization of the class SummaryWindow
         */
        SummaryWindow(QWidget *parent = nullptr);
        /*!
         * \brief Return the text displayed by the result terminal
         */
        const QString returnTerminalContents(bool all_data = true);
        /*!
         * \brief Virtual destructor
         */
        virtual ~SummaryWindow() { }

    public slots:
        /*!
         * \brief Pure virtual slot defined in the child classes
         */
        virtual void backButtonPressed() = 0;
        /*!
         * \brief Go to the contant window when contact button clicked
         */
        void contactButtonPressed() { emit this->goToContact(); }

    signals:
        /*!
         * \brief Notify the main window to switch to the contact form
         */
        void goToContact();
};

#endif // SUMMARYWINDOW_HPP
