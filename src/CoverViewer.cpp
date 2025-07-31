#include "CoverViewer.hpp"

CoverViewer::CoverViewer(const char *widget_title, QWidget *parent) : FramedWidget(widget_title, parent)
{
    this->viewer = new Viewer(this);
    this->next = new QPushButton(tr("     Następny     "), this);
    this->prev = new QPushButton(tr("     Poprzedni     "), this);
    this->part_name = new QLabel(" ", this);
    this->buttons_layout = new QHBoxLayout();

    this->number_of_parts = 0;
    this->index = 0;

    this->setWidgetGraphics();
}

void CoverViewer::setWidgetGraphics()
{
    // Zachowanie geometrii obrazów
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->setStoredWidgets();
    this->setButtonsLayout();
    this->setVerticalLayout();
}

void CoverViewer::setStoredWidgets()
{
    QPalette buttons_palette;
    buttons_palette.setColor(QPalette::Button, QColor(QRgb(SIL_GRAY)));
    buttons_palette.setColor(QPalette::ButtonText, QColor(QRgb(SIL_NAVY)));

    this->prev->setPalette(buttons_palette);
    this->next->setPalette(buttons_palette);

    changeStyle(this->next, ":/button_stylesheet/ButtonUnlocked.qss");
    changeStyle(this->prev, ":/button_stylesheet/ButtonUnlocked.qss");

    this->next->setMinimumHeight(20);
    this->prev->setMinimumHeight(20);
    this->next->setFlat(false);

    this->part_name->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    this->part_name->setStyleSheet("background-color: rgba(0,0,0,0%)");
    this->part_name->setAlignment(Qt::AlignCenter);

    this->viewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(this->next, SIGNAL(pressed()), this, SLOT(viewNextPart()));
    connect(this->prev, SIGNAL(pressed()), this, SLOT(viewPrevPart()));
}

void CoverViewer::setButtonsLayout()
{
    this->buttons_layout->addWidget(this->prev);
    this->buttons_layout->addWidget(this->part_name);
    this->buttons_layout->addWidget(this->next);

    this->buttons_layout->setAlignment(Qt::AlignBottom);
}

void CoverViewer::setVerticalLayout()
{
    this->v_layout->addWidget(this->title);
    this->v_layout->addWidget(this->viewer);
    this->v_layout->addLayout(this->buttons_layout);
}

void CoverViewer::getPathsAndQuantity(DicModelsPcs & part_paths)
{
    this->parts = part_paths;
    this->number_of_parts = part_paths.size();

    std::vector<std::string> paths;
    for(uint8_t i = 0; i < this->parts.size(); ++i)
        paths.push_back(this->parts[i].first);

    this->viewer->getModelsPaths(paths);
    this->viewer->uploadModels();

    this->viewer->drawPart(this->index);
    std::string part_info = this->getPartInfo(this->index);
    this->part_name->setText(QString::fromStdString(part_info));
}

void CoverViewer::changePartName()
{
    if(this->parts.size() == 0)
        return;

    this->viewer->drawPart(this->index);
    std::string part_info = this->getPartInfo(this->index);
    this->part_name->setText(QString::fromStdString(part_info));
}

void CoverViewer::viewNextPart()
{
    ++this->index;
    if(this->index >= this->number_of_parts)
        this->index = 0;

    this->changePartName();
}

void CoverViewer::viewPrevPart()
{
    --this->index;
    if(this->index < 0)
        this->index = this->number_of_parts - 1;

    this->changePartName();
}

std::string CoverViewer::getPartInfo(uint8_t index)
{
    std::stringstream path_stream(this->parts[index].first);
    std::vector<std::string> path_elements;

    uint8_t i = 0;
    while(!path_stream.eof())
    {
        std::string element;
        std::getline(path_stream, element, '/');
        path_elements.push_back(element);
        ++i;
    }

    std::string part_info = path_elements[i - 1] + " x" + std::to_string(this->parts[index].second);

    return part_info;
}
