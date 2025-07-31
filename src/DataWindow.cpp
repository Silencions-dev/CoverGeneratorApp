#include "DataWindow.hpp"

/*!
 * \brief Initialization of the class DataWindow
 *
 * Create new instances of the inner widgets. Assign the
 * IOWindow objects to the list in the correct sequence.
 * Load all data and steps descriptions from the given file
 * to the given buffers. Add the IOWindow objects in the
 * QStackedWidget and set the first one as an active (visible).
 * Initialize the 2D array for user dimensions with the -1 values
 * that can be interpreted as "not given". Set the graphics of the
 * window and connect signals from each IOWindow with the slot that
 * triggers the verification of all data inputs in the active IOWindow.
 *
 * \param[in] dw_title - initial title of the window
 * \param[in] parent - pointer to the parent widget
 */
DataWindow::DataWindow(const char *dw_title, QWidget *parent) : FramedWidget(dw_title, parent)
{
    steps_switcher = new QStackedWidget(this);

    data_windows[0] = new IOWindow(MAX_DIM_INFO, false, this);
    data_windows[1] = new IOWindow(MAX_DIM_INFO, true, this);
    data_windows[2] = new IOWindow(MAX_SPEC_INFO, true, this);

    // Read data descriptions and step titles
    readGeneratorDescriptions(GenDescPath_1, inner_dim_text, MAX_DIM_INFO);
    readGeneratorDescriptions(GenDescPath_2, min_dist_text, MAX_DIM_INFO);
    readGeneratorDescriptions(GenDescPath_3, min_spacing_text, MAX_SPEC_INFO);
    readGeneratorDescriptions(GenStepsTitlesPath, steps_title, STEPS);

    for(uint8_t i = 0; i < STEPS; ++i)
        steps_switcher->addWidget(data_windows[i]);

    steps_switcher->setCurrentIndex(0);

    for(uint8_t x = 0; x < STEPS; ++x)
    {
        std::vector<int> new_row;
        for(uint8_t y = 0; y < data_windows[x]->retInputsNumber(); ++y)
            new_row.push_back(-1);

        dimensions.push_back(new_row);
    }

    setWidgetGraphics();

    for(uint8_t i = 0; i < STEPS; ++i)
        connect(data_windows[i], SIGNAL(dataInsideChanged()), this, SLOT(checkAllInputs()));
}
/*!
 * \brief Sets the graphics of the inner widgets
 *
 * Define the geometrical behavior and maximum height
 * of this window. Adjust the options of the inner widgets
 * and set the vertical layout.
 */
void DataWindow::setWidgetGraphics()
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setMaximumHeight(250);
    setStoredWidgets();
    setVerticalLayout();
}
/*!
 * \brief Sets the settings of the inner widgets
 *
 * Pass the descriptions of each data input to the
 * stored IOWindows.
 */
void DataWindow::setStoredWidgets()
{   
    data_windows[0]->getInputsTitles(inner_dim_text);
    data_windows[1]->getInputsTitles(min_dist_text);
    data_windows[2]->getInputsTitles(min_spacing_text);
}
/*!
 * \brief Aligns the inner widget vertically
 *
 * Align the title of the window and stack widget with
 * IOWindow objects vertically. Separate them with the
 * spacer item.
 */
void DataWindow::setVerticalLayout()
{
    v_layout->addWidget(title);
    v_layout->addItem(new QSpacerItem(1, 10, QSizePolicy::Minimum, QSizePolicy::Preferred));
    v_layout->addWidget(steps_switcher);
}
/*!
 * \brief Check the state of all inputs in the current IOWindow
 *
 * Start with the verification if there is an active widget or
 * the QStackedWidget is not empty. If so, call the appropriate
 * methods of the active IOWindow and check the overall status
 * of the whole widget based on the separate states of the data
 * inputs. Notify the higher widget about the state of the data
 * window using appropriate signal.
 */
void DataWindow::checkAllInputs()
{
    int curr_id = steps_switcher->currentIndex();
    if(curr_id == -1)
        return;

    if(data_windows[curr_id]->allInputsDisabled())
        emit noInputsGiven();
    else if(data_windows[curr_id]->allInputsCorrect())
        emit allDataCorrect();
    else if(data_windows[curr_id]->oneInputWrong())
        emit oneDataWrong(data_windows[curr_id]->retStatusMsg());
    else if(data_windows[curr_id]->oneInputEditing())
        emit oneDataEditing();
}
/*!
 * \brief Change the appearance of the window according to the current step
 *
 * Convert the enumeration to the integer and use it to change
 * the active IOWindow along with the window title accordingly.
 * Force the changes on the screen calling update method.
 *
 * \param[in] step - step of the generator (enumeration)
 */
void DataWindow::goToStep(Steps step)
{
    int step_index = static_cast<int>(step);
    changeTitle(steps_title[step_index]);
    steps_switcher->setCurrentIndex(step_index);
    update();
}
/*!
 * \brief Save the data collected in the current step
 *
 * Start with the verification if there is an active widget or
 * the QStackedWidget is not empty. If so, get the number of
 * the inputs from the active IOWindow and collect all values into
 * the 2D array.
 *
 * \param[in] step - step of the generator (enumeration)
 */
void DataWindow::saveData(Steps step)
{
    int curr_id = steps_switcher->currentIndex();

    if(curr_id == -1)
        return;

    uint8_t inputs_num = data_windows[curr_id]->retInputsNumber();

    for(uint8_t i = 0; i < inputs_num; ++i)
        dimensions[step][i] = data_windows[curr_id]->returnDataFrom(i);
}
/*!
 * \brief Read the labels / titles from the given file to the desired buffer
 *
 * Open the file located at the given path. If failed to open,
 * display the appropriate message on the debug output and
 * simply return. If opened correctly, read the file line by
 * line and assign each of them as an element of the target array.
 * Close the file at the end.
 *
 * \param[in] path - path to the file with strings
 * \param[in] target_buffer - pointer to the array
 * \param[in] data_quantity - number of expected texts
 */
void DataWindow::readGeneratorDescriptions(const char *path, QString *target_buffer, uint8_t data_quantity)
{
    QFile info_file(path);

    if(!info_file.open(QIODevice::ReadOnly))
    {
        qDebug() << "\nUnable to open generator data descriptions file. Exit.\n";
        return;
    }

    QTextStream text_str(&info_file);
    uint8_t i = 0;

    while(!text_str.atEnd())
    {
        QString line = text_str.readLine();

        if(!line.isEmpty() && i < data_quantity)
        {
            target_buffer[i] = line;
            ++i;
        }
    }

    info_file.close();
}
















