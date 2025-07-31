#ifndef COVERVIEWER_HPP
#define COVERVIEWER_HPP

#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include "Viewer.hpp"
#include "FramedWidget.hpp"

typedef std::vector<std::pair<std::string, uint8_t>> DicModelsPcs;

class CoverViewer : public FramedWidget
{
    Q_OBJECT

    private:
        Viewer *viewer;
        QPushButton *next;
        QPushButton *prev;
        QLabel *part_name;
        QHBoxLayout *buttons_layout;

        DicModelsPcs parts;

        uint8_t number_of_parts;
        int index;

        void setButtonsLayout();

        void setWidgetGraphics() override;
        void setVerticalLayout() override;
        void setStoredWidgets() override;

        std::string getPartInfo(uint8_t index);
        void changePartName();

    public:
        CoverViewer(const char *widget_title, QWidget *parent = nullptr);
        void getPathsAndQuantity(DicModelsPcs & part_paths);

    public slots:
        void viewNextPart();
        void viewPrevPart();

};

#endif // COVERVIEWER_HPP
