#include "CoverImage.hpp"

/*!
 * \brief Initialization of the class CoverImage
 *
 * Define the geometrical behavior of this window.
 * Prepare the images for the first step of the
 * generator.
 *
 * \param[in] parent - pointer to the parent widget
 */
CoverImage::CoverImage(QWidget *parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setStep(Steps::STEP_1);
}
/*!
 * \brief Draws the images inside the widget
 *
 * Load the image from the file to which path is located
 * in the array at the current index. If failed to load
 * image, simply return. Otherwise scale the image to the
 * current size of the widget. Draw the image in the center
 * of the widget.
 */
void CoverImage::paintEvent(QPaintEvent *event)
{
    QPainter drawer(this);

    QImage render(image_files[image_index]);

    if(render.isNull())
    {
        qDebug() << "\nERROR. Render inside 'paintEvent' of CoverImage could not be loaded.";
        return;
    }

    render = render.scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    int x = render.width();
    int y = render.height();

    drawer.drawImage((width() - x) / 2, (height() - y) / 2, render);
}
/*!
 * \brief Load the set of images according to the given step
 *
 * Input argument (step) determines the path to the directory
 * with the proper images. This method is called by the main
 * window, when user switch between the steps of the generator.
 *
 * \param[in] step - current step of the generator (enum type)
 */
void CoverImage::setStep(Steps step)
{
    switch(step)
    {
        case Steps::STEP_1:
            searchFiles(FirstStepImgsPath);
            break;
        case Steps::STEP_2:
            searchFiles(SecStepImgsPath);
            break;
        case Steps::STEP_3:
            searchFiles(ThirdStepImgsPath);
            break;
        default:
            break;
    }
}
/*!
 * \brief Collect all image files from the given directory
 *
 * Get list of all files loacted at the given path. Clear
 * the array with the old paths and push back new ones
 * (assuming that every file is an image, no verification).
 * Call the update method to trigger the paintEvent and draw
 * the image.
 *
 * \param[in] path_raw - path to the directory with considered images
 */
void CoverImage::searchFiles(const char *path_raw)
{
    QString path(path_raw);
    QDir directory(path);

    const QStringList files_list = directory.entryList(QDir::Files);

    image_files.clear();

    foreach(QString filename, files_list)
        image_files.push_back(path + filename);

    update();
}
/*!
 * \brief Sets the next image
 *
 * First, provide behavior of the circular buffer - if
 * image index reaches the position after the last one
 * (which does not exist) set it to the first element.
 * Change the direction of showing images to the right (next)
 * and force paintEvent.
 */
void CoverImage::setNextImage()
{
    if(image_index == image_files.size() - 1)
        image_index = 0;
    else
        ++image_index;

    showcase_dir = DispDirection::RIGHT;

    update();
}
/*!
 * \brief Sets the previous image
 *
 * First, provide behavior of the circular buffer - if
 * image index reaches the position before the first one
 * (which does not exist) set it to the last element.
 * Change the direction of showing images to the left (previous)
 * and force paintEvent.
 */
void CoverImage::setPrevImage()
{
    if(image_index == 0)
        image_index = image_files.size() - 1;
    else
        --image_index;

    showcase_dir = DispDirection::LEFT;

    update();
}
/*!
 * \brief Sets image according to the iterating direction
 *
 * Switch the image according to the last saved direction
 * of showing the images determined by the user. This method
 * is called by the timer in automatic slide show.
 */
void CoverImage::setImageAccrdToDir()
{
    if(showcase_dir == DispDirection::RIGHT)
        setNextImage();
    else
        setPrevImage();
}
/*!
 * \brief Load all portfolio images file names
 */
void CoverImage::loadPortfolioImages()
{
    searchFiles(PortfolioImgsPath);
}
/*!
 * \brief Load all images used in generator summary (results)
 *
 * Result images differ for every size of the standard covers, so
 * the input argument (size) determines the final path to the
 * directory (size is in fact the subdirectory).
 *
 * \param[in] size - symbol with the size of the cover (S, M, L or XL)
 */
void CoverImage::loadResultImages(const char *cover_size)
{
    std::string path = ResultsImgsPath;
    path += cover_size;

    searchFiles(path.c_str());
}
