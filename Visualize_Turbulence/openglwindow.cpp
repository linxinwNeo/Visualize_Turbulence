#include "openglwindow.h"

openGLWindow::openGLWindow(QWidget *parent) : QOpenGLWidget(parent)
{
    qInfo() << "openGLWindow Default Constructor has finished";
}

openGLWindow::~openGLWindow() {
    qInfo() << "openGLWindow Destructor called";
}


void openGLWindow::initializeGL(){
    this->initializeOpenGLFunctions();
}

void openGLWindow::paintGL()
{
    glClearColor (1.0, 1.0, 1.0, 1.0);  // background for rendering color coding and lighting
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
        glVertex3f(0, 0.5, 0);
        glVertex3f(0.5, 0, 0);


    glEnd();
    this->update();
}

void openGLWindow::mousePressEvent(QMouseEvent *event)
{

}

void openGLWindow::mouseReleaseEvent(QMouseEvent *event)
{

}

void openGLWindow::mouseMoveEvent(QMouseEvent *event)
{

}

void openGLWindow::rightButtonUp(QMouseEvent *event)
{

}

void openGLWindow::leftButtonMoved(QMouseEvent *event)
{

}

void openGLWindow::middleButtonDown(QMouseEvent *event)
{

}

void openGLWindow::middleButtonMoved(QMouseEvent *event)
{

}

void openGLWindow::wheelEvent(QWheelEvent *event)
{

}

