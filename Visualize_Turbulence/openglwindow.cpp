#include <QMouseEvent>
#include "openglwindow.h"
#include "Geometry/Mesh.h"
#include "predefined.h"

extern Mesh* mesh;
bool LeftButtonDown = false;
bool MiddleButtonDown = false;


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

    for( int i = 0 ; i < mesh->num_tets(); i++ ){
        Tet* tet = mesh->tets[i];
        Vertex* v1 = tet->verts[0];
        Vertex* v2 = tet->verts[1];
        Vertex* v3 = tet->verts[2];
        Vertex* v4 = tet->verts[3];

        glBegin(GL_TRIANGLES);
            // face 1
            glVertex3f(v1->x, v1->y, v1->z);
            glVertex3f(v2->x, v2->y, v2->z);
            glVertex3f(v3->x, v3->y, v3->z);

            // face 2
            glVertex3f(v1->x, v1->y, v1->z);
            glVertex3f(v2->x, v2->y, v2->z);
            glVertex3f(v4->x, v4->y, v4->z);

            // face 3
            glVertex3f(v1->x, v1->y, v1->z);
            glVertex3f(v3->x, v3->y, v3->z);
            glVertex3f(v4->x, v4->y, v4->z);

            // face 4
            glVertex3f(v2->x, v2->y, v2->z);
            glVertex3f(v3->x, v3->y, v3->z);
            glVertex3f(v4->x, v4->y, v4->z);
        glEnd();
    }



    this->update();
}


void openGLWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        LeftButtonDown = true;
        QPointF p = event->pos();
        double s = (2.0 * p.x() - WIN_WIDTH) / WIN_HEIGHT;
        double t = (2.0 * ( win_height - p.y() ) - win_height) / win_width;

        s_old = s;
        t_old = t;
        last_x = p.x();
        last_y = p.y();
    }
    else if(event->button() == Qt::MiddleButton){
        middleButtonDown(event);
    }
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

