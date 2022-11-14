QT       += core gui openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    FileLoader/ReadFile.cpp \
    Geometry/Edge.cpp \
    Geometry/Mesh.cpp \
    Geometry/Tet.cpp \
    Geometry/Triangle.cpp \
    Geometry/Vertex.cpp \
    TraceBall.cpp \
    main.cpp \
    mainwindow.cpp \
    openglwindow.cpp \
    point.cpp \
    pointVector.cpp

HEADERS += \
    FileLoader/ReadFile.h \
    Geometry/Edge.h \
    Geometry/Mesh.h \
    Geometry/Tet.h \
    Geometry/Triangle.h \
    Geometry/Vector3d.h \
    Geometry/Vertex.h \
    TraceBall.h \
    globalVars.h \
    mainwindow.h \
    openglwindow.h \
    point.h \
    pointVector.h \
    predefined.h \
    utility_functions.h

FORMS += \
    mainwindow.ui

mac: LIBS += -framework GLUT

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
