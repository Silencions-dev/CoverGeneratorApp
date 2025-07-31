QT += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/BoolDimInput.cpp \
    src/ContactData.cpp \
    src/ContactWindow.cpp \
    src/CoverImage.cpp \
    src/CoverPortfolioWindow.cpp \
    src/CoverResultWindow.cpp \
    src/DataWindow.cpp \
    src/DimInput.cpp \
    src/EmailEditor.cpp \
    src/FramedWidget.cpp \
    src/GeneratorError.cpp \
    src/HPCover.cpp \
    src/HttpDataframe.cpp \
    src/HttpManager.cpp \
    src/IOWindow.cpp \
    src/ImagesViewer.cpp \
    src/MailAddressInput.cpp \
    src/PhoneNumberInput.cpp \
    src/ResultTerminal.cpp \
    src/SlidesViewer.cpp \
    src/StartWindow.cpp \
    src/Statistics.cpp \
    src/SummaryWindow.cpp \
    src/Terminal.cpp \
    src/UserContactForm.cpp \
    src/UserInput.cpp \
    src/main.cpp \
    src/MainWindow.cpp

HEADERS += \
    inc/StylesPaths.hpp \
    inc/BoolDimInput.hpp \
    inc/ContactData.hpp \
    inc/ContactWindow.hpp \
    inc/CoverImage.hpp \
    inc/CoverPortfolioWindow.hpp \
    inc/CoverResultWindow.hpp \
    inc/DataWindow.hpp \
    inc/DimInput.hpp \
    inc/EmailEditor.hpp \
    inc/FramedWidget.hpp \
    inc/GeneratorError.hpp \
    inc/HPCover.hpp \
    inc/HttpDataframe.hpp \
    inc/HttpManager.hpp \
    inc/IOWindow.hpp \
    inc/ImagesViewer.hpp \
    inc/MailAddressInput.hpp \
    inc/MainWindow.hpp \
    inc/PhoneNumberInput.hpp \
    inc/ResultTerminal.hpp \
    inc/SlidesViewer.hpp \
    inc/StartWindow.hpp \
    inc/Statistics.hpp \
    inc/SummaryWindow.hpp \
    inc/Terminal.hpp \
    inc/UserContactForm.hpp \
    inc/UserInput.hpp

TRANSLATIONS += \
    HP_CoverDesigner_pl_PL.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH+=inc

RESOURCES += \
    resources/Images.qrc \
    resources/fonts.qrc \
    resources/parameters.qrc \
    resources/styles.qrc \
    resources/texts.qrc
