#include "Others/Draw.h"
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

void ECGWindow::main_routine()
{
    if(this->ecg == nullptr) return;
    qDebug() << "this ecg has " << ecg->num_nodes() << "nodes and " << ecg->num_edges() << "edges";

    glPushMatrix();
    glTranslatef(-0.6, -3.2, 0);

    glPointSize(10);
    glBegin(GL_POINTS);

    UL num_starting_pts = 0, num_ending_pts = 0, num_of_middle_col_pts = 0;
    UL num_rows = 0;

    // counting number of starting pts and num of ending pts
    for(ECG_NODE* node : ecg->get_nodes()){
        if(node->num_nodes() == 0) continue;

        if(node->num_inNodes() == 0 ){
            num_starting_pts ++;
            continue;
        }

        if(node->num_outNodes() == 0 ){
            num_ending_pts ++;
            continue;
        }

        num_of_middle_col_pts ++;
    }

    // decide how many rows
    if(num_starting_pts > num_ending_pts) num_rows = num_starting_pts;
    else num_rows = num_ending_pts;

    // calculate dx, dy
    double dy = 7. / num_rows;
    double middle_dy = 7. / num_of_middle_col_pts;
    double dx = 1.8 / 3.;

    double x = 0; double y = 0;  double z = 0;
    // draw starting nodes
    for(ECG_NODE* node : ecg->get_nodes()){
        if(node->num_nodes() == 0 || node->num_inNodes() != 0) continue;
        node->cords.set(x, y, z);
        Singularity* sing = node->sing;
        decide_color(sing->type);
        glVertex3f(x, y, z);
        y += dy;
    }

    x += dx;

    // draw middle col nodes
    y = 0.; // reset y
    for(ECG_NODE* node : ecg->get_nodes()){
        if(node->num_nodes() == 0 || node->num_inNodes() == 0 || node->num_outNodes() == 0) continue;
        node->cords.set(x, y, z);
        Singularity* sing = node->sing;
        decide_color(sing->type);
        glVertex3f(x, y, z);
        y += middle_dy;
    }

    x += dx;

    // draw starting nodes
    y = 0; // reset y
    for(ECG_NODE* node : ecg->get_nodes()){
        if(node->num_nodes() == 0 || node->num_outNodes() != 0) continue;
        node->cords.set(x, y, z);
        Singularity* sing = node->sing;
        decide_color(sing->type);
        glVertex3f(x, y, z);
        y += dy;
    }

    glEnd();


    glLineWidth(5);
    // draw edges
    glEnable(GL_DEPTH_TEST);
    glBegin(GL_LINES);
    for(const ECG_EDGE* edge : ecg->get_edges()){
        const ECG_NODE* n1 = edge->nodes[0];
        const ECG_NODE* n2 = edge->nodes[1];
        glColor3f(0,0,0);
        glVertex3f(n1->cords.x(), n1->cords.y(), n1->cords.z() - 1);
        glColor3f(1,1,1); // white on the other end
        glVertex3f(n2->cords.x(), n2->cords.y(), n2->cords.z() - 1);
    }
    glEnd();

    glPopMatrix();
}



void ECGWindow::redraw()
{
    this->update();
}
