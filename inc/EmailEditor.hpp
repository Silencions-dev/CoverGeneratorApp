#ifndef EMAILEDITOR_HPP
#define EMAILEDITOR_HPP

/*!
 * \file
 * \brief Definition of the class EmailEditor
 *
 * This file contains definition of the class EmailEditor,
 * which generates the main text of the email and display it
 * in the window.
 */

#include <QTextEdit>
#include <iostream>
#include <fstream>
#include "FramedWidget.hpp"

/*!
 * \brief Class that implements generator of the email
 *
 * EmailEditor inherits the class FramedWidget, which makes it the top level
 * widget visible in the MainWindow (with the frame and title). It creates
 * the full email message based on the output of the generator, user contact
 * data and ID of the app session. Plain generated text is displayed in the
 * read-only mode, so it can be easily copied.
 */
class EmailEditor : public FramedWidget
{
    Q_OBJECT
    private:

        // Functional members =======================================

        /*!
         * \brief Widget, which displays the generated content
         */
        QTextEdit *editor;
        /*!
         * \brief Main content of the email
         */
        QString email_content;
        /*!
         * \brief Title of the email
         */
        QString email_title;
        /*!
         * \brief Result from the generator (failed or successful)
         */
        QString generator_summary;
        /*!
         * \brief Number of the potential order - app session ID
         */
        int order_id = 0;

        // Const attributes =========================================

        /*!
         * \brief Path to the title of the mail in case of the standard cover
         */
        static constexpr char const * StdMailPath = ":/mail/txt/MailStdCover.txt";
        /*!
         * \brief Path to the title of the mail in case of the special cover
         */
        static constexpr char const * SpecialMailPath = ":/mail/txt/MailSpecialCover.txt";

        // Private methods ==========================================

        /*!
         * \brief Set the settings of the inner widgets
         */
        void setStoredWidgets();
        /*!
         * \brief Align the inner widgets vertically
         */
        void setVerticalLayout();
        /*!
         * \brief Set the graphics of the inner widgets
         */
        void setWidgetGraphics();
        /*!
         * \brief Read and return the text located in the given file
         */
        QString readContent(const char *path);

    public:
        /*!
         * \brief Initialization of the class EmailEditor
         */
        EmailEditor(const char *widget_title, QWidget *parent = nullptr);
        /*!
         * \brief Create the email title, header and attach results
         */
        void createGeneratorSummary(bool std_cover, const QString & results);
        /*!
         * \brief Generate the full email message
         */
        void generateMail(const QString & mail_address, const QString & phone_number);
        /*!
         * \brief Get the order ID from the outer widget
         *
         * \param[in] id - identification number of the current app session
         */
        void getOrderID(int id) { order_id = id; }
        /*!
         * \brief Return the main content of the generated email
         *
         * \return Constant reference to the generated content
         */
        const QString & emailContent() { return email_content; }
        /*!
         * \brief Return the title of the generated email
         *
         * \return Constant reference to the generated title
         */
        const QString & emailTitle() { return email_title; }

    public slots:
        /*!
         * \brief React to the change of the selection of the displayed text
         */
        void textSelectionChanged(bool selection);

    signals:
        /*!
         * \brief Signal emitted when text has been selected
         */
        void mailContentSelected();
};

#endif // EMAILEDITOR_HPP
