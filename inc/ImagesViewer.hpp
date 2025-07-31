#ifndef IMAGESVIEWER_HPP
#define IMAGESVIEWER_HPP

/*!
 * \file
 * \brief Definition of the class ImagesViewer
 *
 * This file contains definition of the class ImagesViewer,
 * which displays the images and provides the interface to
 * switch between them.
 */

#include <QPushButton>
#include "CoverImage.hpp"
#include "StylesPaths.hpp"

/*!
 * \brief Class that implements window for the images
 *
 * ImagesViewer inherits the class FramedWidget, which makes it the top level
 * widget visible in the MainWindow (with the frame and title). It displays
 * the images from the CoverImage in the window with additional title at
 * the top and arrow buttons to switch between multiple images.
 */
class ImagesViewer : public FramedWidget
{
    Q_OBJECT
    protected:
        /*!
         * \brief Images manager (directly loads and paints the images)
         */
        CoverImage *image;

    private:

        // Functional members =======================================

        /*!
         * \brief Button used to show the next image
         */
        QPushButton *next;
        /*!
         * \brief Button used to show the previous image
         */
        QPushButton *prev;
        /*!
         * \brief Horizontal layout for the buttons
         */
        QHBoxLayout *buttons_layout;

        // Const attributes =========================================

        /*!
         * \brief Path to the icon with the right arrow
         */
        static constexpr const char *RightArrowPath = ":/Arrows/images/Arrows/arrow_right.png";
        /*!
         * \brief Path to the icon with the left arrow
         */
        static constexpr const char *LeftArrowPath = ":/Arrows/images/Arrows/arrow_left.png";
        /*!
         * \brief Minimum width of the switching button
         */
        static constexpr uint8_t SwitchBttnWidth = 80;
        /*!
         * \brief Minimum height of the switching button
         */
        static constexpr uint8_t SwitchBttnHeight = 25;

        // Private methods ==========================================

        /*!
         * \brief Align the arrow buttons horizontally
         */
        void setButtonsLayout();
        /*!
         * \brief Set the graphics of the given button
         */
        void setButtonGraphics(QPushButton *button);
        /*!
         * \brief Set the icon of the given button (image inside)
         */
        bool setButtonIcon(QPushButton *button, const char *icon_path);
        /*!
         * \brief Set the graphics of the inner widgets
         */
        void setWidgetGraphics();
        /*!
         * \brief Align inner widgets vertically
         */
        void setVerticalLayout();
        /*!
         * \brief Set the settings and graphics of the inner widgets
         */
        void setStoredWidgets();

    public:
        /*!
         * \brief Initiazliation of the class ImagesViewer
         */
        ImagesViewer(const char *img_title, QWidget *parent = nullptr);
        /*!
         * \brief Load the images of the portfolio
         */
        void loadPortfolioImages();
        /*!
         * \brief Load the images of the results for the given size
         */
        void loadResultImages(const char *cover_size);
        /*!
         * \brief Load the images for the given step of the generator
         */
        void goToStep(Steps step);
};

#endif // IMAGESVIEWER_HPP
