#define GL_SILENCE_DEPRECATION
#include "ecgwindow.h"
#include <QDebug>
#include <GLUT/glut.h>

ECGWindow::ECGWindow(QWidget *parent) : QOpenGLWidget(parent)
{
    this->s_old = 0.;
    this->t_old = 0.;
    this->last_x = 0.;
    this->last_y = 0.;
    this->ecg = nullptr;

    // set up format
    QSurfaceFormat fmt;
    fmt.setRenderableType(QSurfaceFormat::OpenGL);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    fmt.setSamples(32);
    QSurfaceFormat::setDefaultFormat(fmt);
    this->setFormat(fmt);
}

ECGWindow::~ECGWindow()
{
    return;
}

void ECGWindow::initializeGL()
{
    this->initializeOpenGLFunctions();

    glShadeModel (GL_SMOOTH);

}

void ECGWindow::paintGL()
{
    qDebug() << "ECG paints!";
    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); // push 1st projection matrix
    glLoadIdentity();
    // W200*H800
    glOrtho(-1, 1, -4, 4., -1000.0, 4000.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); // push 1st modelView matrix
    glLoadIdentity();

    glClearColor (0.7, 0.7, 0.7, 1.0);  // grey background for rendering color coding and lighting
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->main_routine();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix(); // pop 1st modelView matrix

    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); // pop 1st projection matrix
}

void ECGWindow::main_routine() const
{
    if(this->ecg == nullptr) return;
}

void ECGWindow::update_scene()
{
    this->update();
}
