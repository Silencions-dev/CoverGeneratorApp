#ifndef RESULTTERMINAL_HPP
#define RESULTTERMINAL_HPP

/*!
 * \file
 * \brief Definition of the class ResultTerminal
 *
 * This file contains definition of the class ResultTerminal,
 * which is an advanced widget for displaying multiple texts
 * in the same or different formats.
 */

#include <vector>
#include <QTextEdit>
#include <iostream>
#include "FramedWidget.hpp"
#include "StylesPaths.hpp"

/*!
 * \brief Class that implements text visualization methods
 *
 * ResultTerminal inherits the class FramedWidget, which makes it
 * the top level widget visible in the MainWindow (with the frame
 * and title). It implements multiple methods with predefined rules
 * of how to format and display texts according to the parent widget,
 * e.g. results of the generator, information in the contact window
 * or information in the portfolio.
 */
class ResultTerminal : public FramedWidget
{
    private:

        // Functional members =======================================

        /*!
         * \brief Widget to display read-only text
         */
        QTextEdit *terminal;
        /*!
         * \brief Generator results packed in the form of formatted string (for email)
         */
        QString results_info;
        /*!
         * \brief Generator input data packed in the form of formatted string (for email)
         */
        QString input_info;
        /*!
         * \brief Descriptions of the data inputs in step 1 of the generator
         */
        QString first_step_data_desc[MAX_DIM_INFO];
        /*!
         * \brief Descriptions of the data inputs in step 2 of the generator
         */
        QString second_step_data_desc[MAX_DIM_INFO];
        /*!
         * \brief Descriptions of the data inputs in step 3 of the generator
         */
        QString third_step_data_desc[MAX_SPEC_INFO];
        /*!
         * \brief Titles of each step
         */
        QString input_data_desc[STEPS];
        /*!
         * \brief Flag, which indicates whether to save the generator results string
         */
        bool save_results = false;
        /*!
         * \brief Flag, which indicates whether to save the generator inputs string
         */
        bool save_inputs = false;

        // Const attributes =========================================

        /*!
         * \brief Path to the descriptions of the data inputs in step 1
         */
        static constexpr const char *ResultDescPath_1 = ":/descriptions/txt/ResultStepDesc_1.txt";
        /*!
         * \brief Path to the descriptions of the data inputs in step 2
         */
        static constexpr const char *ResultDescPath_2 = ":/descriptions/txt/ResultStepDesc_2.txt";
        /*!
         * \brief Path to the descriptions of the data inputs in step 3
         */
        static constexpr const char *ResultDescPath_3 = ":/descriptions/txt/ResultStepDesc_3.txt";
        /*!
         * \brief Path to the titles of each step
         */
        static constexpr const char *ResultUserInfoPath = ":/descriptions/txt/ResultUserInfoDescriptions.txt";
        /*!
         * \brief Path to the text displayed in the portfolio window
         */
        static constexpr const char *InfoPortfolioPath = ":/info/txt/PortfolioInfo.txt";
        /*!
         * \brief Path to the text displayed when generator failed to create cover
         */
        static constexpr const char *InfoNotGeneratedPath = ":/info/txt/NotGeneratedInfo.txt";
        /*!
         * \brief Path to the text displayed in the contact form window
         */
        static constexpr const char *InfoContactFormPath = ":/info/txt/ContactFormInfo.txt";

        // Private methods ==========================================

        /*!
         * \brief Set the graphics of the inner widgets
         */
        void setWidgetGraphics();
        /*!
         * \brief Align title with the rest of inner widgets
         */
        void setVerticalLayout();
        /*!
         * \brief Set the settings and graphics of the inner widgets
         */
        void setStoredWidgets();
        /*!
         * \brief Display the text in the highest form of distinction
         */
        void typeResultsTitle(const QString & message);
        /*!
         * \brief Display the text as the header (high level of distinction)
         */
        void typeHeaderText(const QString & message);
        /*!
         * \brief Display the plain text (basic format)
         */
        void typePlainText(const QString & message);
        /*!
         * \brief Display the bold colored text
         */
        void typeBoldText(const QString & message);
        /*!
         * \brief Display the bold red text
         */
        void typeErrorText(const QString & message);
        /*!
         * \brief Display the text in the given format
         */
        void typeText(const QString & message, const QTextCharFormat & format);
        /*!
         * \brief Read the data descriptions/titles from the file
         */
        void readResultDescriptions(const char *path, QString *target_buffer, uint8_t data_quantity);
        /*!
         * \brief Read the text from the file as one block
         */
        void readTheWholeText(const char *path, QString *target_buffer);
        /*!
         * \brief Get the description of the data input at the given step and index
         */
        QString getDataDescription(int step, int index);

    public:
        /*!
         * \brief Initialization of the class ResultTerminal
         */
        ResultTerminal(const char *widget_title, QWidget *parent = nullptr);
        /*!
         * \brief Display the inner and outer dimensions of the generated cover
         */
        void typeDimensions(const std::vector<uint16_t> & inner_dim, const std::vector<uint16_t> & outer_dim);
        /*!
         * \brief Display the input dimensions provided by the user
         */
        void typeInputs(const std::vector<std::vector<int>> & dimensions);
        /*!
         * \brief Display the information for the portfolio window
         */
        void typePortfolioInfo();
        /*!
         * \brief Display the information about failure of the generator with the given error
         */
        void typeNoCoverGenerated(const QString & error_msg);
        /*!
         * \brief Display information for the contact form window
         */
        void typeContactFormInfo();
        /*!
         * \brief Display additional error message to the already visible text
         *
         * \param[in] message - constant reference to the string with error message
         */
        void addErrorMessage(const QString & message) { this->typeErrorText(message); }
        /*!
         * \brief Move the cursor of QTextEdit to the top
         */
        void scrollUp() { this->terminal->moveCursor(QTextCursor::Start); }
        /*!
         * \brief Return the results packed in the form of formatted string (for email)
         *
         * \return Constant reference to the string with the results in a text form
         */
        const QString & returnResultSummary() { return this->results_info; }
        /*!
         * \brief Return the input data packed in the form of formatted string (for email)
         *
         * \return Constant reference to the string with the input data in a text form
         */
        const QString & returnInputSummary() { return this->input_info; }
};

#endif // RESULTTERMINAL_HPP
