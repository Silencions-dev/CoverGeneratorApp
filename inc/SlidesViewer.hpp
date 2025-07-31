#ifndef SLIDESVIEWER_HPP
#define SLIDESVIEWER_HPP

/*!
 * \file
 * \brief Definition of the class SlidesViewer
 *
 * This file contains definition of the class SlidesViewer,
 * which adds functionality of automatic slide show controled
 * by the timer.
 */

#include <QTimer>
#include "ImagesViewer.hpp"

/*!
 * \brief Class that implements automatic photo gallery
 *
 * SlidesViewer inherits from the ImagesViewer, which
 * makes it the same visible instance in the main window,
 * but with additional function, which is automatic switching
 * of the images. It sets next or previous images according to
 * the last direction clicked by the user and timeout signal
 * from QTimer.
 */
class SlidesViewer : public ImagesViewer
{
    Q_OBJECT
    private:

        // Functional members =======================================

        /*!
         * \brief Timer of the automatic slide show
         */
        QTimer *slide_timer;

    public:
        /*!
         * \brief Initialization of the class SlidesViewer
         */
        SlidesViewer(const char *img_title, QWidget *parent = nullptr, int fire_time = 4);
        /*!
         * \brief Start slide show (start timer)
         */
        void startSlideShow() { slide_timer->start(); }
        /*!
         * \brief Stop slide show (stop timer)
         */
        void stopSlideShow() { slide_timer->stop(); }
};

#endif // SLIDESVIEWER_HPP
