#include "ImagesViewer.hpp"

/*!
 * \brief Initiazliation of the class ImagesViewer
 *
 * Call the constructor of the parent widget in the initialization list.
 * Create new instances of the inner widgets and assign layout of the
 * buttons to the image itself (so the buttons are places ON the image).
 * Then set the graphics and settings of the inner widgets.
 *
 * \param[in] img_title - title of the window
 * \param[in] parent - pointer to the parent widget
 */
ImagesViewer::ImagesViewer(const char *img_title, QWidget *parent) : FramedWidget(img_title, parent)
{
    image = new CoverImage(this);
    next = new QPushButton(this);
    prev = new QPushButton(this);
    // Efekt nałożenia się przycisków na obrazie
    buttons_layout = new QHBoxLayout(image);
    // Ustawia grafikę okna
    setWidgetGraphics();
}
/*!
 * \brief Set the graphics of the inner widgets
 *
 * Define geometrical behavior of this window and go to the
 * inner widgets. Then align buttons horizontally and all
 * elements in a column (vertically).
 */
void ImagesViewer::setWidgetGraphics()
{
    // Zchowanie geometrii obrazów
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    setStoredWidgets();
    setButtonsLayout();
    setVerticalLayout();
}
/*!
 * \brief Align the arrow buttons horizontally
 *
 * Add buttons to the layout and separate them using spacer
 * item. Place the layout at the bottom of the window.
 */
void ImagesViewer::setButtonsLayout()
{
    buttons_layout->addWidget(prev);
    buttons_layout->addItem(new QSpacerItem(30, 1, QSizePolicy::Expanding, QSizePolicy::Preferred));
    buttons_layout->addWidget(next);
    buttons_layout->setAlignment(Qt::AlignBottom);
}
/*!
 * \brief Set the graphics of the given button
 *
 * Set the style of the given button to be unlocked by default.
 * Both arrow buttons will be always enabled, since images are
 * stored in a circular buffer - there is no start nor end.
 * Set the minimum size of the button and disable the flat option
 * (is clickable).
 *
 * \param[in] button - pointer to the button
 */
void ImagesViewer::setButtonGraphics(QPushButton *button)
{
    changeStyle(button, styles::BUTTON_UNLOCKED);

    button->setMinimumHeight(SwitchBttnHeight);
    button->setMinimumWidth(SwitchBttnWidth);
    button->setFlat(false);
}
/*!
 * \brief Set the settings and graphics of the inner widgets
 *
 * First, set the graphics of each button. Then, load the appropriate
 * arrow image (left or right) and place it in each button. Finally,
 * connect the signals from the buttons with the slots that are
 * responsible for switching the images (next or previous).
 */
void ImagesViewer::setStoredWidgets()
{
    setButtonGraphics(next);
    setButtonGraphics(prev);

    // Ustawienie strzałek w przyciskach
    if(!setButtonIcon(prev, LeftArrowPath))
        qDebug() << "\nError during loading left arrow image (button icon).";

    if(!setButtonIcon(next, RightArrowPath))
        qDebug() << "\nError during loading right arrow image (button icon).";

    // Połączenie przycisków z przełączaniem obrazków
    connect(next, SIGNAL(pressed()), image, SLOT(setNextImage()));
    connect(prev, SIGNAL(pressed()), image, SLOT(setPrevImage()));
}
/*!
 * \brief Align inner widgets vertically
 *
 * Align title of the window and the widget, in which
 * the images are directly painted.
 */
void ImagesViewer::setVerticalLayout()
{
    v_layout->addWidget(title);
    v_layout->addWidget(image);
}
/*!
 * \brief Load the images for the given step of the generator
 *
 * All paths to the images and buffers for them are stored in
 * the CoverImage class.
 *
 * \param[in] step - current step of the generator (enumeration)
 */
void ImagesViewer::goToStep(Steps step)
{
    image->setStep(step);
}
/*!
 * \brief Set the icon of the given button (image inside)
 *
 * Load the image located at the given path. If the object
 * is empty, return false. Create QIcon object and assign it
 * to the given button. Resize the icon to match the button size.
 *
 * \param[in] button - pointer to the button
 * \param[in] icon_path - path to the arrow icon
 * \return True if icon correcly loaded from the file and false otherwise
 */
bool ImagesViewer::setButtonIcon(QPushButton *button, const char *icon_path)
{
    QPixmap arrow(icon_path);

    if(arrow.isNull())
        return false;

    QIcon arrow_icon(arrow);

    button->setIcon(arrow_icon);
    button->setIconSize(QSize(SwitchBttnWidth, SwitchBttnHeight));

    return true;
}
/*!
 * \brief Load the images of the portfolio
 */
void ImagesViewer::loadPortfolioImages()
{
    image->loadPortfolioImages();
}
/*!
 * \brief Load the images of the results for the given size
 *
 * Argument 'cover_size' is in fact the name of a subdirectory with
 * the appropriate images.
 *
 * \param[in] cover_size - size of the cover as a C-string (S, M, L or XL)
 */
void ImagesViewer::loadResultImages(const char *cover_size)
{
    image->loadResultImages(cover_size);
}
