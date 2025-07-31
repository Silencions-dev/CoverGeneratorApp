#ifndef GENERATORERROR_HPP
#define GENERATORERROR_HPP

/*!
 * \file
 * \brief Definition of the class GeneratorError
 *
 * This file contains definition of the class GeneratorError,
 * which contains the predefined error messages possible to
 * occur during collecting the input data from the user and
 * generating the cover.
 */

#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <vector>
#include <iostream>

/*!
 * \brief Errors of the generator
 */
enum GenErrors
{
    LEFT_COLLISION,  /*! Collision with the obstacle on the left */
    RIGHT_COLLISION, /*! Collision with the obstacle on the right */
    BACK_COLLISION,  /*! Collision with hthe obstacle on the back */
    TOO_LONG,        /*! Desired cover is too wide */
    TOO_WIDE,        /*! Desired cover is too deep */
    TOO_HIGH,        /*! Desired cover is too high */
    NO_PART_WIDTH,   /*! No part for the desired depth */
    NO_PART_LENGTH   /*! No part for the desired width */
};
/*!
 * \brief Errors of the input data
 */
enum InputErrors
{
    WRONG_FORMAT,   /*! Wrong format of the data (not a number) */
    TOO_HIGH_VALUE, /*! Too high value of the data */
    WRONG_VALUE     /*! Wrong value of the data (negative number) */
};

/*!
 * \brief Class that implements the basic error handling
 *
 * GeneratorError contains the predefined errors that can
 * happen in the DataWindow and HPCover classes (input data
 * collection and generator). One may raise the given error
 * and eaisly pass the GeneratorError object across the app
 * architecture, for example to display the error message in
 * the appropriate window.
 */
class GeneratorError
{
    private:

        // Functional members =======================================

        /*!
         * \brief Message of the current error
         */
        QString error_msg;
        /*!
         * \brief Array with the generator error messages
         */
        std::vector<QString> gen_error_msgs;
        /*!
         * \brief Array with the input data error messages
         */
        std::vector<QString> input_error_msgs;

        // Const attributes =========================================

        /*!
         * \brief Number of possible errors of the generator
         */
        static constexpr int GenErrorsNum = 8;
        /*!
         * \brief Number of possible errors of the input data
         */
        static constexpr int UserInputErrorsNum = 3;
        /*!
         * \brief Path to the error messages of the generator
         */
        static constexpr char const *GenErrorsPath = ":/errors/txt/GenErrorMsgs.txt";
        /*!
         * \brief Path to the error messages of the input data
         */
        static constexpr char const *UserInputErrorsPath = ":/errors/txt/InputErrorMsgs.txt";

        // Private methods ==========================================

        /*!
         * \brief Read and load the error messages form the given file
         */
        bool readMessages(const char *path, std::vector<QString> & messages, int msg_num);

    public:
        /*!
         * \brief Initialization of the class GeneratorError
         */
        GeneratorError();
        /*!
         * \brief Set the current error of the generator
         */
        void raise(GenErrors error);
        /*!
         * \brief Set the current error of the input data
         */
        void raise(InputErrors error);
        /*!
         * \brief Return the message of the current error
         *
         * \return Contant reference to the error message
         */
        const QString & returnErrorMessage() { return this->error_msg; }
};

#endif // GENERATORERROR_HPP
