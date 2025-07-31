#include "GeneratorError.hpp"

/*!
 * \brief Initialization of the class GeneratorError
 *
 * Read the possible generator errors and possible numeric input
 * data errors from the text files to the given buffers.
 */
GeneratorError::GeneratorError()
{
    readMessages(GenErrorsPath, gen_error_msgs, GenErrorsNum);
    readMessages(UserInputErrorsPath, input_error_msgs, UserInputErrorsNum);
}
/*!
 * \brief Set the current error of the generator
 *
 * \param[in] error - generator error (enumeration)
 */
void GeneratorError::raise(GenErrors error)
{
    error_msg = gen_error_msgs[error];
}
/*!
 * \brief Set the current error of the input data
 *
 * \param[in] error - input data error (enumeration)
 */
void GeneratorError::raise(InputErrors error)
{
    error_msg = input_error_msgs[error];
}
/*!
 * \brief Read and load the error messages form the given file
 *
 * Open the file located at the given path. If failed to open,
 * write the appropriate message on the standard error output
 * and return false. Read the file line by line and assign each
 * of them as an element of the target array. Add additional
 * new line characters at the beginning and at the end of each
 * error message.
 *
 * \param[in] path - path to the file
 * \param[in] messages - reference to the buffer for the messages
 * \param[in] msg_num - total number of messages
 * \return True if loaded and false otherwise
 */
bool GeneratorError::readMessages(const char *path, std::vector<QString> & messages, int msg_num)
{
    QFile msg_file(path);

    if(!msg_file.open(QIODevice::ReadOnly))
    {
        std::cerr << "\nUnable to open messages file. Exit...\n";
        return false;
    }

    QTextStream text_str(&msg_file);
    uint8_t i = 0;

    while(!text_str.atEnd())
    {
        QString qt_line = text_str.readLine();

        if(!qt_line.isEmpty() && i < msg_num)
        {
            messages.push_back(QString("\n") + qt_line + QString("\n"));
            ++i;
        }
    }

    msg_file.close();

    return true;
}
