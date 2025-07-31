#ifndef TERMINAL_HPP
#define TERMINAL_HPP

/*!
 * \file
 * \brief Definition of the class Terminal
 *
 * This file contains definition of the class Terminal,
 * which displays the simple information.
 */

#include <QLineEdit>
#include <QDateTime>
#include "FramedWidget.hpp"

/*!
 * \brief Class that implements redirection of the text to the widget
 *
 * Terminal class is a simple widget with overloaded << operator,
 * so it can be treated as the alternative text output. Printed
 * string is displayed directly in the main window of the application.
 * It prints the text without advanced formatting, one style only.
 * It inherits from the FramedWidget, so it makes it the top-level
 * widget visible in the main window (with title and frame).
 */
class Terminal : public FramedWidget
{
    Q_OBJECT
    private:

        // Functional members =======================================

        /*!
         * \brief Widget to display the text
         */
        QLabel *editor;

        // Private methods ==========================================

        /*!
         * \brief Print the given string in the QLabel
         */
        void typeFormatMsg(const QString & message);
        /*!
         * \brief Align the title of the window with the stored widget vertically
         */
        void setVerticalLayout();
        /*!
         * \brief Set the settings and graphics of the inner widgets
         */
        void setStoredWidgets();

    public:
        /*!
         * \brief Initialization of the class Terminal
         */
        Terminal(const char *widget_title, QWidget *parent = nullptr);
        /*!
         * \brief Set the background color of the widget according to the given palette
         */
        void setBackgroundColor(const QPalette & palette);

    /*!
     * \brief Declaration of the friend function - overloaded << operator
     */
    friend Terminal & operator<< (Terminal & term, const QString & message);
};

/*!
 * \brief Overloaded << operator for displaing messages
 */
Terminal & operator<< (Terminal & term, const QString & message);

#endif // TERMINAL_HPP

