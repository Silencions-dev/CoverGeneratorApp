#include "FailedWindow.hpp"

FailedWindow::FailedWindow(QWidget *parent) : CoverPortfolioWindow(parent)
{
    this->contact_button = new QPushButton(tr("Kontakt"), this);

    //this->setStoredWidgets();
    //this->setWidgetsLayout();
}

void FailedWindow::setStoredWidgets()
{
    connect(this->back_button, SIGNAL(pressed()), this, SLOT(backButtonPressed()));
    connect(this->contact_button, SIGNAL(pressed()), this, SLOT(contactButtonPressed()));

    this->back_button->setEnabled(true);
    this->contact_button->setEnabled(true);

    QPalette buttons_palette;
    buttons_palette.setColor(QPalette::ButtonText, QColor(QRgb(SIL_NAVY)));
    this->back_button->setPalette(buttons_palette);
    this->contact_button->setPalette(buttons_palette);

    changeStyle(this->back_button, ":/button_stylesheet/ButtonUnlocked.qss");
    changeStyle(this->contact_button, ":/button_stylesheet/ButtonUnlocked.qss");

    this->images->loadPortfolioImages();

    this->images->startSlideShow();
}

void FailedWindow::setWidgetsLayout()
{
    this->back_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    this->back_button->setMaximumHeight(BUTTON_MAX_HEIGHT);
    this->contact_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    this->contact_button->setMaximumHeight(BUTTON_MAX_HEIGHT);

    this->main_ver_layout->addWidget(this->results);
    this->main_ver_layout->addItem(new QSpacerItem(10, 15, QSizePolicy::Minimum, QSizePolicy::Preferred));
    this->main_ver_layout->addWidget(this->contact_button);
    this->main_ver_layout->addWidget(this->back_button);

    this->main_hor_layout->addWidget(this->images);
    this->main_hor_layout->addItem(new QSpacerItem(10, 10, QSizePolicy::Preferred, QSizePolicy::Minimum));
    this->main_hor_layout->addLayout(this->main_ver_layout);

    this->main_hor_layout->setStretch(0, 3);
    this->main_hor_layout->setStretch(2, 2);
}

