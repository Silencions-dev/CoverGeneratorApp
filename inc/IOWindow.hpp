#ifndef IOWINDOW_HPP
#define IOWINDOW_HPP

/*!
 * \file
 * \brief Definition of the class IOWindow
 *
 * This file contains definition of the class IOWindow,
 * which implements the basic widget for providing the
 * numeric data. It is used by the DataWindow in the
 * stack widget as an instance designed for each step of
 * the generator.
 */

#include "FramedWidget.hpp"
#include "BoolDimInput.hpp"

/*!
 * \brief Class that implements widget for the input/output data
 *
 * IOWindow collects multiple pairs of the description + input box
 * in a single window. It can be designed specifically for each
 * step of the generator - descriptions, number of inputs or type
 * of inputs (required or optional with check boxes).
 */
class IOWindow : public QWidget
{
    Q_OBJECT
    private:

        // Functional members =======================================

        /*!
         * \brief List of descriptions for each input data
         */
        std::vector<QLabel*> data_name;
        /*!
         * \brief Fields for the input data
         */
        std::vector<DimInput*> data_input;
        /*!
         * \brief Number of inputs in the window
         */
        uint8_t inputs_number = 0;
        /*!
         * \brief Message with the status of the data window
         */
        QString status_message;
        /*!
         * \brief Left column of the window for the descriptions
         */
        QVBoxLayout *left_column_layout;
        /*!
         * \brief Right column of the window for the data fields
         */
        QVBoxLayout *right_column_layout;
        /*!
         * \brief Horizontal alignment of the columns
         */
        QHBoxLayout *columns_layout;

        // Private methods ==========================================

        /*!
         * \brief Create the objects of DimInput or BoolDimInput
         */
        void createInputs(bool check_boxes);
        /*!
         * \brief Updates the status message with the current error from the given input
         */
        void updateMessageStatus(uint8_t index);
        /*!
         * \brief Set the layout of the inner widgets
         */
        void setWidgetsLayout();
        /*!
         * \brief Align the descriptions vertically (first column)
         */
        void setLabelsLayout();
        /*!
         * \brief Align the input data fields vertically (second column)
         */
        void setDataLayout();
        /*!
         * \brief Align first and second column horizontally
         */
        void alignBothColumns();

    public:
        /*!
         * \brief Initiazliation of the class IOWindow
         */
        IOWindow(uint8_t inputs, bool add_check_boxes, QWidget *parent = nullptr);
        /*!
         * \brief Set the description of the input fields
         */
        void getInputsTitles(const QString *titles);
        /*!
         * \brief Return the numeric data from the given input
         *
         * \param[in] index - index of the data field on the list
         * \return Numerical value stored in the data field (-1 in case of the status different than correct)
         */
        int returnDataFrom(int index) { return this->data_input[index]->returnData(); }
        /*!
         * \brief Check if all data inputs contain correct values
         */
        bool allInputsCorrect();
        /*!
         * \brief Check if at least one data input contains the wrong value
         */
        bool oneInputWrong();
        /*!
         * \brief Check if at least one data input is still edited by the usere
         */
        bool oneInputEditing();
        /*!
         * \brief Check if all data inputs are disabled by the check boxes
         */
        bool allInputsDisabled();
        /*!
         * \brief Return the number of stored data inputs
         *
         * \return Number of data inputs (size of the list)
         */
        uint8_t retInputsNumber() { return inputs_number; }
        /*!
         * \brief Return the current status message (possible error from one of the inputs)
         *
         * \return Constant reference to the error message
         */
        const QString & retStatusMsg() { return status_message; }

    public slots:
        /*!
         * \brief Notify higher widget about changes in the data fields
         */
        void verifyInputsHigher() { emit dataInsideChanged(); }

    signals:
        /*!
         * \brief Emitted, when status of one of the data inputs changed
         */
        void dataInsideChanged();
};

#endif // IOWINDOW_HPP
