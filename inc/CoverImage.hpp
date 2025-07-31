#ifndef COVERIMAGE_HPP
#define COVERIMAGE_HPP

/*!
 * \file
 * \brief Definition of the class CoverImage
 *
 * This file contains definition of the class CoverImage, which
 * displays (draws) images correlated with the the covers.
 */

#include <QDir>
#include <QStringList>
#include <QPainter>
#include <vector>
#include "FramedWidget.hpp"

/*!
 * \brief Class that displays images in a circular buffer
 *
 * CoverImage class displays different sets of renders according to
 * the current state of the application. First three sets contains
 * examples with required dimensions (depending on the three steps).
 * Next two sets consists of custom covers portfolio and technical
 * drawings displayed in a final result section.
 */
class CoverImage : public QWidget
{
    Q_OBJECT
    private:

        // Functional members =======================================

        /*!
         * \brief Direction of iterating over images
         */
        enum DispDirection
        {
            LEFT, /*! Circular buffer going left */
            RIGHT /*! Circular buffer going right */
        };
        /*!
         * \brief Index of the currently displayed image
         */
        unsigned short int image_index = 0;
        /*!
         * \brief Dynamic array with file names of the images
         */
        std::vector<QString> image_files;
        /*!
         * \brief Direction of the images presentation (next as default)
         */
        DispDirection showcase_dir = DispDirection::RIGHT;

        // Const attributes =========================================

        /*!
         * \brief Path to the example images for step 1
         */
        static constexpr char const *FirstStepImgsPath = ":/ExamplesImgs/images/Step1/";
        /*!
         * \brief Path to the example images for step 2
         */
        static constexpr char const *SecStepImgsPath = ":/ExamplesImgs/images/Step2/";
        /*!
         * \brief Path to the example images for step 2
         */
        static constexpr char const *ThirdStepImgsPath = ":/ExamplesImgs/images/Step3/";
        /*!
         * \brief Path to the custom covers portfolio images
         */
        static constexpr char const *PortfolioImgsPath = ":/CoversImgs/images/ObudowySpecjalne/";
        /*!
         * \brief Path to the result images of the generator
         */
        static constexpr char const *ResultsImgsPath = ":/ResultsImgs/images/ObudowyWyniki/";

        // Private methods ==========================================

        /*!
         * \brief Collect all image files from the given directory
         */
        void searchFiles(const char *path_raw);

    public:
        /*!
         * \brief Initialization of the class CoverImage
         */
        CoverImage(QWidget *parent = nullptr);
        /*!
         * \brief Load the set of images according to the given step
         */
        void setStep(Steps step);
        /*!
         * \brief Load all portfolio images file names
         */
        void loadPortfolioImages();
        /*!
         * \brief Load all images used in generator summary (results)
         */
        void loadResultImages(const char *size);
        /*!
         * \brief Draws the images inside the widget
         */
        virtual void paintEvent(QPaintEvent *event) override;

     public slots:
        /*!
         * \brief Sets the next image
         */
        void setNextImage();
        /*!
         * \brief Sets the previous image
         */
        void setPrevImage();
        /*!
         * \brief Sets image according to the iterating direction
         */
        void setImageAccrdToDir();
};

#endif // COVERIMAGE_HPP
