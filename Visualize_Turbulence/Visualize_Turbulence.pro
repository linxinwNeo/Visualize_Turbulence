QT       += core gui openglwidgets opengl

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
    Others/PointVector.cpp \
    Others/TraceBall.cpp \
    main.cpp \
    mainwindow.cpp \
    openglwindow.cpp

HEADERS += \
    FileLoader/ReadFile.h \
    Geometry/Edge.h \
    Geometry/Mesh.h \
    Geometry/Tet.h \
    Geometry/Triangle.h \
    Geometry/Vertex.h \
    Others/GlobalVars.h \
    Others/Matrix2x2.h \
    Others/Matrix3x3.h \
    Others/Point.h \
    Others/PointVector.h \
    Others/Predefined.h \
    Others/TraceBall.h \
    Others/Utility_functions.h \
    Others/Vector2d.h \
    Others/Vector3d.h \
    mainwindow.h \
    openglwindow.h

FORMS += \
    mainwindow.ui

mac: LIBS += -framework GLUT

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
