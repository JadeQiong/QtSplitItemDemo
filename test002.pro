QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


INCLUDEPATH += C:\Users\mech-mind_ljq\Downloads\opencv\build\include\
#               C:\Users\mech-mind_ljq\Downloads\opencv\build\include\opencv2\

# LIBS += F:/OpenCV/OpenCV3.4.3/opencv_3.4.3_build_qt/x86/mingw/lib/libopencv_*.dll.a
LIBS += D:\OpenCV4.5.1\opencv\opencv-build\lib\libopencv_*.a

CONFIG += c++11

CONFIG(debug,debug|release){
LIBS+= -L$$PWD/../../../../../opencv/build/x64/vc15/lib/ -lopencv_world453d
}
else{
LIBS += -L$$PWD/../../../../../opencv/build/x64/vc15/lib/ -lopencv_world453
}
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    commands.cpp \
    main.cpp \
    mainwindow.cpp \
    redoitems.cpp \
    redoscene.cpp \
    widget.cpp

HEADERS += \
    commands.h \
    mainwindow.h \
    redoitems.h \
    redoscene.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../opencv/build/x64/vc15/lib/ -lopencv_world453
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../opencv/build/x64/vc15/lib/ -lopencv_world453d

INCLUDEPATH += $$PWD/../../../../../opencv/build/x64/vc15
DEPENDPATH += $$PWD/../../../../../opencv/build/x64/vc15
