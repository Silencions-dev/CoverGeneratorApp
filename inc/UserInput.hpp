#ifndef USERINPUT_HPP
#define USERINPUT_HPP

/*!
 * \file
 * \brief Definition of the class UserInput
 *
 * This file contains definition of the class UserInput,
 * which provides the basic functionality of verifying and
 * accepting the string-like input data from the user.
 */

#include <QLineEdit>
#include <QHBoxLayout>
#include "FramedWidget.hpp"
#include "StylesPaths.hpp"

/*!
 * \brief Status of the data inside
 */
enum UserDataStatus
{
    USER_WRONG,   /*! Data is wrong */
    USER_CORRECT, /*! Data is correct */
    USER_NEUTRAL, /*! Data is still edited or not given */
};

/*!
 * \brief Class that implements widget for the string-like input data
 *
 * UserInput is the base class for the more specific input data fields.
 * It provides necessary elements, like the description (QLabel) and
 * input field (QLineEdit). It also contains some universal attributes and
 * functions, for example status or cotainer for the currently stored data.
 */
class UserInput : public QWidget
{
    Q_OBJECT
    protected:

        // Functional members =======================================

        /*!
         * \brief Description of the data input
         */
        QLabel *data_label;
        /*!
         * \brief Visible field to type the input data
         */
        QLineEdit *data_input;
        /*!
         * \brief Current status of the data inside the QLineEdit
         */
        UserDataStatus status = UserDataStatus::USER_NEUTRAL;
        /*!
         * \brief Data currently stored in the input
         */
        QString current_data;

        // Const attributes =========================================

        /*!
         * \brief Maximum length of the input string
         */
        static constexpr int TextLimit = 80;
        /*!
         * \brief Minimum width of the input field
         */
        static constexpr int LineEditWidth = 200;

        // Protected methods ========================================

        /*!
         * \brief Set the settings and graphics of the inner widgets
         */
        void setWidgets();
        /*!
         * \brief Set the settings and graphics of the input field
         */
        void setLineEdit(QLineEdit *input);
        /*!
         * \brief Change the frame color of the input field according to the current status
         */
        void adjustFrameColor();
        /*!
         * \brief Check the correctness of the input data
         *
         * This is the pure virtual function. It is implemented inside the child classes.
         * In this way, they provide the verification designed specifically for the
         * target data, like phone number or email address.
         */
        virtual void checkData() = 0;

    public:
        /*!
         * \brief Initialization of the class UserInput
         */
        UserInput(QWidget *parent = nullptr);
        /*!
         * \brief Return the pointer to the stored data description
         *
         * \return Pointer to the QLabel object
         */
        QLabel *dataLabel() { return this->data_label; }
        /*!
         * \brief Return the pointer to the stored input field
         *
         * \return Pointer to the QLineEdit object
         */
        QLineEdit *dataInput() { return this->data_input; }
        /*!
         * \brief Return the current status of the input data
         *
         * \return Current status
         */
        UserDataStatus returnStatus() { return this->status; }
        /*!
         * \brief Return the reference to the current string-like data
         *
         * \return Contant reference to the input data
         */
        const QString & returnCurrentData() { return this->current_data; }

    public slots:
        /*!
         * \brief Trigger the verification of the new data when appeared
         */
        void newData();
        /*!
         * \brief Change the status to neutral when data is edited
         */
        void dataEditing();

    signals:
        /*!
         * \brief Sent, when the status of the input data changed
         */
        void dataChanged();
};

#endif // USERINPUT_HPP
