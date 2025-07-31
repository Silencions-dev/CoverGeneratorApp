#ifndef HPCOVER_HPP
#define HPCOVER_HPP

/*!
 * \file
 * \brief Definition of the class HPCover
 *
 * This file contains definition of the class HPCover,
 * which has all tools necessary to generate and describe
 * the cover in detail (inner / outer dimensions, number of
 * modules, collisions, errors).
 */

#include <cmath>
#include <sstream>
#include <QDebug>
#include "GeneratorError.hpp"

/*!
 * \brief Number of considered sides of the heat pump
 */
static constexpr uint8_t DevSides = 4;
/*!
 * \brief Number of considered dimensions
 */
static constexpr uint8_t DimNum = 3;
/*!
 * \brief Structure with the names of all parameters
 *
 * Names of the parameters corresponds to the configuration
 * file. Thus, those strings are used to parse the custom
 * config and correctly distribute the values inside the
 * HPCover object.
 */
struct VariableNames
{
    /*! Possible widths of the cover */
    std::string BASE_LENGTH = "lengths";
    /*! Possible depths of the cover */
    std::string BASE_WIDTH = "widths";
    /*! Heights of the modules (base, standard and top) */
    std::string BASE_HEIGHT = "heights";
    /*! Width correction value, which converts inner width to outer width */
    std::string OUT_LENGTH = "out_length_param";
    /*! Depth correction value, which converts inner depth to outer depth */
    std::string OUT_WIDTH = "out_width_param";
    /*! Height correction value, which converts inner height to outer height */
    std::string OUT_HEIGHT = "out_height_param";
    /*! Width correction value, which converts approx inner width to the accurate one */
    std::string ACC_LENGTH = "acc_length_param";
    /*! Depth correction value, which converts approx inner depth to the accurate one */
    std::string ACC_WIDTH = "acc_width_param";
    /*! Height correction value, which converts approx inner height to the accurate one */
    std::string ACC_HEIGHT= "acc_height_param";
    /*! Minimum space between inner surface of the cover and front of the device */
    std::string FRONT_SPACE = "front_space";
    /*! Minimum space between inner surface of the cover and sides of the device */
    std::string SIDE_SPACE = "side_space";
    /*! Minimum space between inner surface of the cover and back of the device */
    std::string BACK_SPACE = "back_space";
    /*! Minimum space between inner surface of the cover and top of the device */
    std::string TOP_SPACE = "top_space";
    /*! Minimum distance to the wall behind the device, for which cover should be standard */
    std::string WALL_SPACE = "wall_space";
};
/*!
 * \brief Identification of the dimension in the arrays
 */
enum DIM_ID
{
    LENGTH, /*! Width */
    WIDTH,  /*! Depth */
    HEIGHT  /*! Height */
};

/*!
 * \brief Class that implements the virtual sHP cover
 *
 * HPCover class is responsible for generating the
 * cover based on the provided data from the user.
 * It decides about success or failure (standard or
 * special cover) using the parameters from the custom
 * configuration file (available parts) and checking the
 * collisions with the nearby obstacles. It is implemented
 * in a way, which allows for the recaluclation of the
 * cover.
 */
class HPCover
{
    private:

        // Functional members =======================================

        /*!
         * \brief Dynamic 2D array with the data from the user
         */
        std::vector<std::vector<int>> dim_matrix;
        /*!
         * \brief Inner dimensions of the generated cover
         */
        std::vector<uint16_t> inner_dimensions;
        /*!
         * \brief Outer dimensions of the generated cover
         */
        std::vector<uint16_t> outer_dimensions;
        /*!
         * \brief Minimal necessary inner dimensions of the cover
         */
        std::vector<uint16_t> approx_inner_dim;
        /*!
         * \brief Number of wall modules (size of the standard cover)
         */
        uint8_t modules = 0;
        /*!
         * \brief Handler of the generator errors
         */
        GeneratorError error_handler;
        /*!
         * \brief Base dimensions available in the parts library
         */
        std::vector<float> base_dimensions[DimNum];
        /*!
         * \brief Corrections used to caclulate the preceise inner dimensions
         */
        float acc_in_dimensions[DimNum];
        /*!
         * \brief Corrections used to caclulate the preceise outer dimensions
         */
        float acc_out_dimensions[DimNum];
        /*!
         * \brief Required minimal spaces inside the cover
         */
        float inner_offsets[DevSides];
        /*!
         * \brief Minimal distance to the wall behind for the standard cover
         */
        float wall_offset = 0;

        // Const attributes =========================================

        /*!
         * \brief Constant structure with the names of the config parameters
         */
        const VariableNames params;
        /*!
         * \brief Maximum number of standard modules (max height)
         */
        static constexpr uint8_t MaxWallModules = 3;
        /*!
         * \brief Path to the custom configuration file
         */
        static constexpr char const *GenConfigPath = ":/cover_params/txt/CoverParameters.txt";

        // Private methods ==========================================

        /*!
         * \brief Read and analyze the line of the file
         */
        bool readLine(std::stringstream & stream);
        /*!
         * \brief Calculate the required width
         */
        bool countInnerLength();
        /*!
         * \brief Calculate the required depth
         */
        bool countInnerWidth();
        /*!
         * \brief Calculate the required height
         */
        bool countInnerHeight();
        /*!
         * \brief Calculate the precise outer dimensions of the generated cover
         */
        void countOuterDimensions();
        /*!
         * \brief Check if there is a collision with the object on the left or right
         */
        bool checkForLengthCollision(int constraint);
        /*!
         * \brief Check if there is a collision with the object behind the device
         */
        bool checkForWidthCollision(int constraint);
        /*!
         * \brief Find the base dimension available for the approximated one
         */
        float pickRightDimension(DIM_ID dim_idx);
        /*!
         * \brief Choose the appropriate number of the wall modules to the required height
         */
        uint8_t pickRightModulesQuantity();
        /*!
         * \brief Check if the required dimensions can be provided by the available parts
         */
        bool searchPartsLibrary();

    public:
        /*!
         * \brief Initialization of the class HPCover
         */
        HPCover();
        /*!
         * \brief Generate the cover
         */
        bool generateCover(std::vector<std::vector<int>> dimensions);
        /*!
         * \brief Display the loaded configuration parameters (debug only)
         */
        void printResults();
        /*!
         * \brief Read parameters of the generator from the configuration file
         */
        bool readParameters();
        /*!
         * \brief Return inner dimensions of the generated cover
         *
         * \return Constant reference to the dynamic array with dimensions
         */
        const std::vector<uint16_t> & returnInnerDim() { return this->inner_dimensions; }
        /*!
         * \brief Return outer dimensions of the generated cover
         *
         * \return Constant reference to the dynamic array with dimensions
         */
        const std::vector<uint16_t> & returnOuterDim() { return this->outer_dimensions; }
        /*!
         * \brief Return total number of the wall modules of the generated cover
         *
         * \return Total number of the wall modules
         */
        uint8_t returnModulesQuantity() { return this->modules; }
        /*!
         * \brief Return currently stored user data
         */
        std::vector<std::vector<int>> & returnFinalDimensions();
        /*!
         * \brief Return the handler to the stored error object
         *
         * \return Reference to the object with possibly raised error
         */
        GeneratorError & returnErrorHandler() { return this->error_handler; }
};

#endif // HPCOVER_HPP
