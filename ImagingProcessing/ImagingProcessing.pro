QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

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
DESTDIR = $$PWD/../Bin
SOURCES += \
    DialogText.cpp \
    ModelGraph.cpp \
    ModelTreeWidget.cpp \
    MyQwtZoomer.cpp \
    MyWidget.cpp \
    PublicUse.cpp \
    RightDownWidget.cpp \
    RightUpWidget.cpp \
    SpectrogramPlotChild.cpp \
    TabWidget.cpp \
    VTKGeometry_1.cpp \
    WaterfallWidget.cpp \
    XYZWidget.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp

HEADERS += \
    DialogText.h \
    ModelGraph.h \
    ModelTreeWidget.h \
    MyQwtZoomer.h \
    MyWidget.h \
    PaintDrawBase.h \
    PaintDrawElipse.h \
    PaintDrawPicture.h \
    PaintDrawRect.h \
    PaintDrawText.h \
    PublicUse.h \
    RightDownWidget.h \
    RightUpWidget.h \
    SpectrogramPlotChild.h \
    TabWidget.h \
    VTKGeometry_1.h \
    WaterfallWidget.h \
    XYZWidget.h \
    mainwindow.h \
    qcustomplot.h

FORMS += \
    DialogText.ui \
    ModelTreeWidget.ui \
    MyWidget.ui \
    RightDownWidget.ui \
    RightUpWidget.ui \
    TabWidget.ui \
    XYZWidget.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
#VTK
LIBS += ../vtk8.0-vc14/VTK8.0_release/lib/*.lib
INCLUDEPATH += ../vtk8.0-vc14/VTK8.0_release/include/vtk-8.0/
#OpenCV
LIBS += ../opencv4.5/build/x64/vc15/lib/opencv_world450.lib
INCLUDEPATH += ../opencv4.5/build/include/opencv2
INCLUDEPATH += ../opencv4.5/build/include
#qwt
CONFIG += qwt
DEFINES += QT_DLL QWT_DLL
LIBS += -L"C:\Qt\Qt5.14.1\5.14.1\msvc2017_64\lib" -lqwt
INCLUDEPATH += C:\Qt\Qt5.14.1\5.14.1\msvc2017_64\include\QWT
#View3D
LIBS += ../QK_View/release/lib/View2D.lib
LIBS += ../QK_View/release/lib/View3D.lib
LIBS += ../QK_View/release/lib/CADModule.lib
INCLUDEPATH +=../QK_View/release/include

RESOURCES += \
    QSS/SetQssStyle.qrc \
    QSS/SetQssStyle.qrc
