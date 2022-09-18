QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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
    main.cpp \
    mainwindow.cpp \
    object.cpp \
    objectschoose.cpp \
    pointset2rect.cpp \
    qlabel_tztek.cpp \
    qlistwidget_tztek.cpp \
    qtablewidget_tztek.cpp \
    readandwritexml.cpp \
    setting.cpp

HEADERS += \
    mainwindow.h \
    object.h \
    objectschoose.h \
    pointset2rect.h \
    qlabel_tztek.h \
    qlistwidget_tztek.h \
    qtablewidget_tztek.h \
    readandwritexml.h \
    setting.h

FORMS += \
    mainwindow.ui \
    objectschoose.ui \
    setting.ui

INCLUDEPATH += D:\opencv4.5.2\OpenCV411\opencv_build\install\include
INCLUDEPATH += D:\eigen

QT+=xml

LIBS += D:\opencv4.5.2\OpenCV411\opencv_build\lib\libopencv_*.a

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icon.qrc


