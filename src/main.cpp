#include "MainWindow.hpp"
#include <QFontDatabase>
#include <QApplication>

constexpr const char *FONT_PATH = ":/custom_fonts/fonts/Barlow-Regular.ttf";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFontDatabase::addApplicationFont(FONT_PATH);
    QFont main_font("Barlow", 12, QFont::DemiBold);
    // Jeżeli nie będzie danej czcionki, to ustawi podobną.
    main_font.setStyleHint(QFont::Monospace);
    QApplication::setFont(main_font);

    MainWindow app_window;

    app_window.show();

    return a.exec();
}
