#ifndef DIMINPUT_HPP
#define DIMINPUT_HPP

/*!
 * \file
 * \brief Definition of the class DimInput
 *
 * This file contains definition of the class DimInput,
 * which provides user a tool to type the input data for
 * the generator.
 */

#include <QLineEdit>
#include <QHBoxLayout>
#include "FramedWidget.hpp"
#include "GeneratorError.hpp"
#include "StylesPaths.hpp"

/*!
 * \brief Possible status of the dimension input
 */
enum DimensionStatus
{
    WRONG,   /*! Wrong data */
    CORRECT, /*! Correct data */
    NEUTRAL, /*! Data is still edited */
    DISABLED /*! Input is disabled (check BoolDimInput) */
};

/*!
 * \brief Class that implements a place for the input data
 *
 * DimInput class connects two features: QLabel and QLineEdit.
 * First one describes the unit of the numerical qunatity
 * (milimeters by default). Second one is the field to type and
 * manage the data.
 */
class DimInput : public QWidget
{
    Q_OBJECT
    protected:

        // Functional members =======================================

        /*!
         * \brief Label with the unit of the dimension (milimeters)
         */
        QLabel *unit;
        /*!
         * \brief Field for the input data
         */
        QLineEdit *input_dim;
        /*!
         * \brief Horizontal layout for the edit field and label
         */
        QHBoxLayout *h_layout;
        /*!
         * \brief Dimension typed by the user (already converted to integer)
         */
        uint16_t dimension = 0;
        /*!
         * \brief Status of the given data
         */
        DimensionStatus dim_status = NEUTRAL;
        /*!
         * \brief Class with the possible error of the input data
         *
         * It can store predefined error as the "current one", when raised
         * by the outside object, like this one (DimInput).
         */
        GeneratorError data_error;

        // Protected methods ========================================

        /*!
         * \brief Set the settings and graphics of the inner widgets.
         */
        void setStoredWidgets();
        /*!
         * \brief Check the correctness of the new data from the QLineEdit
         */
        bool checkNewData(QString new_data, uint16_t *correct_data=nullptr);

    public:
        /*!
         * \brief Initialization of the class DimInput
         */
        DimInput(QWidget *parent = nullptr);
        /*!
         * \brief Change the color of the QLineEdit frame
         */
        void setEditStatus(DimensionStatus status);
        /*!
         * \brief Display the last saved correct dimension
         */
        void displaySavedData(int saved_data);
        /*!
         * \brief Return the current status
         *
         * \return Current status of the data (enumeration)
         */
        DimensionStatus returnStatus() { return this->dim_status; }
        /*!
         * \brief Return the current error
         *
         * \return Reference to the stored GeneratorError object
         */
        GeneratorError & returnError() { return this->data_error; }
        /*!
         * \brief Returns the stored dimension
         */
        int returnData();
        /*!
         * \brief Resets the input field
         */
        void clearEditField();
        /*!
         * \brief Aligns the inner widgets horizontally
         */
        virtual void setHorizontalLayout();

    public slots:
        /*!
         * \brief Loads the new data when user finished editing
         */
        void newData();
        /*!
         * \brief Changes the status when user started to edit
         */
        void dataEditing();

    signals:
        /*!
         * \brief Signal emitted when the state of the input field changes
         */
        void dataChanged();
};

#endif // DIMINPUT_HPP
