#ifndef COVERRESULTWINDOW_HPP
#define COVERRESULTWINDOW_HPP

/*!
 * \file
 * \brief Definition of the class CoverResultWindow
 *
 * This file contains definition of the class CoverResultWindow,
 * which displays the generated cover and summarizes the
 * dimensions.
 */

#include <vector>
#include "SummaryWindow.hpp"

namespace fs = std::filesystem;

/*!
 * \brief Class that implements layout of the result window
 *
 * CoverResultWindow inherits the SummaryWindow features, especially
 * the main layout. It presents the render of the generated cover and
 * technical drawings, along with the summary of the dimensions (output
 * and input of the generator).
 */
class CoverResultWindow : public SummaryWindow
{
    Q_OBJECT
    private:

        // Functional members =======================================
        /*!
         * \brief Widget, which visualizes the result images
         */
        ImagesViewer *images;

        // Const attributes =========================================

        /*!
         * \brief Subdirectory with images of the small cover
         */
        static constexpr char const *S_Size = "S/";
        /*!
         * \brief Subdirectory with images of the medium cover
         */
        static constexpr char const *M_Size = "M/";
        /*!
         * \brief Subdirectory with images of the large cover
         */
        static constexpr char const *L_Size = "L/";
        /*!
         * \brief Subdirectory with images of the extra large cover
         */
        static constexpr char const *XL_Size = "XL/";

        // Private methods ==========================================

        /*!
         * \brief Sets the settings and graphics of the inner widgets
         */
        void setStoredWidgets();
        /*!
         * \brief Sets the layout of the window
         */
        void setWidgetsLayout();

    public:
        /*!
         * \brief Initialization of the class CoverResultWindow
         */
        CoverResultWindow(QWidget *parent = nullptr);
        /*!
         * \brief Get the output dimensions from the generator (results)
         */
        void getGeneratorResults(const std::vector<uint16_t> & inner_dim,
                                 const std::vector<uint16_t> & outer_dim);
        /*!
         * \brief Get the input dimensions from the generator (typed by the user)
         */
        void getGeneratorInputs(std::vector<std::vector<int>> & dimensions);
        /*!
         * \brief Load the images for the appropriate size of the cover
         */
        void getModulesQuantity(uint8_t modules_pcs);

    public slots:
        /*!
         * \brief Slot called when "back" button is pressed
         *
         * Result window allows for going back only to the generator as it is
         * always the previous state of the application (in comparison to the
         * portfolio window).
         */
        void backButtonPressed() override { emit goBackToGenerator(); }

    signals:
        /*!
         * \brief Signal sent to go back to the generator
         */
        void goBackToGenerator();
};

#endif // COVERRESULTWINDOW_HPP
