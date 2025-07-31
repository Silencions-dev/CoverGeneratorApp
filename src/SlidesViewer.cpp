#include "SlidesViewer.hpp"

/*!
 * \brief Initialization of the class SlidesViewer
 *
 * Create new instances of the inner widgets and connect the
 * timeout signal with switching the image.
 *
 * \param[in] img_title - title of the window
 * \param[in] parent - pointer to the parent widget
 * \param[in] fire_time - timeout of the timer (4 seconds by default)
 */
SlidesViewer::SlidesViewer(const char *img_title, QWidget *parent, int fire_time) : ImagesViewer(img_title, parent)
{
    slide_timer = new QTimer(this);
    slide_timer->setInterval(fire_time * 1000); // sec to msec

    connect(slide_timer, SIGNAL(timeout()), image, SLOT(setImageAccrdToDir()));
}
