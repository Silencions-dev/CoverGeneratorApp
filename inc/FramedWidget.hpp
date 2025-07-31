#ifndef FRAMEDWIDGET_HPP
#define FRAMEDWIDGET_HPP

/*!
 * \file
 * \brief Definition of the class FramedWidget
 *
 * This file contains definition of the class FramedWidget,
 * which is the base class for all the widgets displayed
 * directly in the MainWindow.
 */

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QFile>

/*!
 * \def SIL_PURPLE
 * \brief Color of the frames
 */
#define SIL_PURPLE 0xC324EA
/*!
 * \def SIL_NAVY
 * \brief Color of the letters in the widgets
 */
#define SIL_NAVY 0x020242
/*!
 * \def SIL_GRAY
 * \brief Color of the widgets background
 */
#define SIL_GRAY 0xECECEC
/*!
 * \def STEPS
 * \brief Number of the generated steps
 */
#define STEPS 3
/*!
 * \def MAX_DIM_INFO
 * \brief Max number of the dimension inputs (first two steps)
 */
#define MAX_DIM_INFO 3
/*!
 * \def MAX_SPEC_INFO
 * \brief Max number of the dimension inputs for the step 3
 */
#define MAX_SPEC_INFO 4
/*!
 * \brief Steps identifiers
 */
enum Steps
{
    /*! Extreme dimensions of the device */
    STEP_1,
    /*! Distances to the nearest obstacles */
    STEP_2,
    /*! Minimum distances between device and inner surfaces of the cover */
    STEP_3
};

/*!
 * \brief Base class of the widgets visible in the main window
 *
 * FramedWidgets inherits from the QFrame class, so every child
 * widget has the outer frame to separate it from the main window.
 * Moreover, it adds title to each widget and vertical layout to
 * align title with the rest of the elements inside.
 */
class FramedWidget : public QFrame
{
    public:
        /*!
         * \brief Title of the widget
         */
        QLabel *title;
        /*!
         * \brief Main vertical layout for the title and inner elements
         */
        QVBoxLayout *v_layout;
        /*!
         * \brief Default constructor
         */
        FramedWidget() { }
        /*!
         * \brief Initialization of the class FramedWidget
         */
        FramedWidget(const char *widget_title, QWidget *parent);
        /*!
         * \brief Change the title of the window
         */
        void changeTitle(QString new_title) { title->setText(new_title); }
        /*!
         * \brief Virtual destructor
         */
        virtual ~FramedWidget() { }
};

/*!
 * \brief Change the appearance of the given widget
 */
void changeStyle(QWidget *widget, const char *path);

#endif // FRAMEDWIDGET_HPP

