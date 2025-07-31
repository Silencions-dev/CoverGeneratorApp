#ifndef DATAWINDOW_HPP
#define DATAWINDOW_HPP

/*!
 * \file
 * \brief Definition of the class DataWindow
 *
 * This file contains definition of the class DataWindow,
 * which is the widget with the fields for the input data
 * of the generator (dimensions).
 */

#include <QStackedWidget>
#include "IOWindow.hpp"

/*!
 * \brief Class that implements the widget for gathering input data
 *
 * DataWindow inherits the class FramedWidget, which makes it the top level
 * widget visible in the MainWindow (with the frame and title). It contains
 * multiple objects of IOWindow, one designed for each step of the generator
 * (different labels, types and number of data fields).
 */
class DataWindow : public FramedWidget
{
    Q_OBJECT
    private:

        // Functional members =======================================

        /*!
         * \brief Container for the IOWindow objects
         *
         * It enables switching (displaying) between different IOWindow
         * objects, which makes the DataWindow flexible. It can adjust to
         * different steps of the generator.
         */
        QStackedWidget *steps_switcher;
        /*!
         * \brief Array with pointers to IOWindows
         *
         * Each IOWindow is designed for the specific step (labels, types and
         * number of inputs).
         */
        IOWindow *data_windows[STEPS];
        /*!
         * \brief Dynamic 2D array to store the dimensions
         *
         * Each row column corresponds to different dimensions and each row to
         * different step.
         */       
        std::vector<std::vector<int>> dimensions;
        /*!
         * \brief Array with names of each step
         */
        QString steps_title[STEPS];
        /*!
         * \brief Array with labels for each QLineEdit input (step 1)
         *
         * Dimensions of the heat pump
         */
        QString inner_dim_text[MAX_DIM_INFO];
        /*!
         * \brief Array with labels for each QLineEdit input (step 2)
         *
         * Distances to the obstacles near the device
         */
        QString min_dist_text[MAX_DIM_INFO];
        /*!
         * \brief Array with labels for each QLineEdit input (step 3)
         *
         * Distances between the heat pump and inner surfaces of the cover
         */
        QString min_spacing_text[MAX_SPEC_INFO];

        // Const attributes =========================================

        /*!
         * \brief Path to the labels of the data for step 1
         */
        static constexpr char const *GenDescPath_1 = ":/descriptions/txt/GeneratorStepDesc_1.txt";
        /*!
         * \brief Path to the labels of the data for step 2
         */
        static constexpr char const *GenDescPath_2 = ":/descriptions/txt/GeneratorStepDesc_2.txt";
        /*!
         * \brief Path to the labels of the data for step 3
         */
        static constexpr char const *GenDescPath_3 = ":/descriptions/txt/GeneratorStepDesc_3.txt";
        /*!
         * \brief Path to the titles of each step (titles of the DataWindow in fact)
         */
        static constexpr char const *GenStepsTitlesPath = ":/descriptions/txt/GeneratorStepsTitle.txt";

        // Private methods ==========================================

        /*!
         * \brief Read the labels / titles from the given file to the desired buffer
         */
        void readGeneratorDescriptions(const char *path, QString *target_buffer, uint8_t data_quantity);
        /*!
         * \brief Sets the graphics of the inner widgets
         */
        void setWidgetGraphics();
        /*!
         * \brief Aligns the inner widget vertically
         */
        void setVerticalLayout();
        /*!
         * \brief Sets the settings of the inner widgets
         */
        void setStoredWidgets();

    public:
        /*!
         * \brief Initialization of the class DataWindow
         */
        DataWindow(const char *dw_title, QWidget *parent = nullptr);
        /*!
         * \brief Change the appearance of the window according to the current step
         */
        void goToStep(Steps step);
        /*!
         * \brief Save the data collected in the current step
         */
        void saveData(Steps step);
        /*!
         * \brief Return the gathered dimensions (for the calulcations)
         *
         * \return Reference to the dynamic 2D array with the user data
         */
        std::vector<std::vector<int>> & returnDimensions() { return dimensions; }

    public slots:
        /*!
         * \brief Check the state of all inputs in the current IOWindow
         */
        void checkAllInputs();

    signals:
        /*!
         * \brief Signal sent, when all inputs are correct or disabled
         */
        void allDataCorrect();
        /*!
         * \brief Signal sent, when at least one input is wrong
         */
        void oneDataWrong(const QString &);
        /*!
         * \brief Signal sent, when at least one input is working
         */
        void oneDataEditing();
        /*!
         * \brief Signal sent, when all inputs are disabled
         */
        void noInputsGiven();
};

#endif // DATAWINDOW_HPP
