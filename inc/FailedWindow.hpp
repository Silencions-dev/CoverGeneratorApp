#ifndef FAILEDWINDOW_HPP
#define FAILEDWINDOW_HPP

#include "CoverPortfolioWindow.hpp"

class FailedWindow : public CoverPortfolioWindow
{
    Q_OBJECT
    private:
        QPushButton *contact_button;

        void setStoredWidgets() override;
        void setWidgetsLayout() override;

    public:
        FailedWindow(QWidget *parent = nullptr);
        ~FailedWindow() { }

    public slots:
        void connectButtonPressed() { emit this->goToContact(); }

    signals:
        void goToContact();
};

#endif // FAILEDWINDOW_HPP
