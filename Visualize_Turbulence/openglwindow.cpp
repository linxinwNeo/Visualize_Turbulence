#define GL_SILENCE_DEPRECATION
#include <QMouseEvent>
#include <QSurfaceFormat>
#include <GLUT/glut.h>

#include "openglwindow.h"
#include "Others/Utilities.h"
#include "Others/Draw.h"

openGLWindow::openGLWindow(QWidget *parent) : QOpenGLWidget(parent)
{
    this->s_old = 0.;
    this->t_old = 0.;
    this->last_x = 0.;
    this->last_y = 0.;
    this->zoom_factor = 1.;
    this->trans_x = 0.;
    this->trans_y = 0.;

    // init matrices
    mat_ident( this->rotmat );

    for(int i = 0; i < 16; i++)
        this->ObjXmat[i]=0.;
    this->ObjXmat[0] = this->ObjXmat[5] = this->ObjXmat[10] = this->ObjXmat[15] = 1;

    // set up format
    QSurfaceFormat fmt;
    fmt.setRenderableType(QSurfaceFormat::OpenGL);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    fmt.setSamples(32);
    QSurfaceFormat::setDefaultFormat(fmt);
    this->setFormat(fmt);

}


openGLWindow::~openGLWindow()
{
    if(this->timer){
        delete this->timer;
    }
    this->timer = NULL;

    return;
}


void openGLWindow::initializeGL()
{
    this->initializeOpenGLFunctions();
    if( mesh == NULL ) return;
    this->rot_center = mesh->rot_center; //set the rotation center!

    glEnable(GL_MULTISAMPLE_ARB);

}


void openGLWindow::paintGL()
{
    if( mesh == NULL ) return;
    int time_idx = 0;
    if(!this->timer){ // first time running paintGL
        this->timer = new QTime();
        this->start_t = this->timer->currentTime();
    }
    else{
        QTime cur_t = this->timer->currentTime();
        unsigned int msecs = this->start_t.msecsTo(cur_t);
        if(msecs > mesh->num_time_steps * MSECS_PER_SEC){
            this->start_t = cur_t;
            msecs = 0;
        }
        time_idx = floor(msecs/MSECS_PER_SEC);
    }

    glMatrixMode(GL_PROJECTION);
    glEnable(GL_DEPTH_TEST);

    glPushMatrix(); // push 1st projection matrix
    glLoadIdentity();
    glOrtho(-1.5, 1.5, -1, 1., -1000.0, 4000.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); // push 1st modelView matrix
    glLoadIdentity();
    this->set_scene();

    glClearColor (0.7, 0.7, 0.7, 1.0);  // grey background for rendering color coding and lighting
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    draw_axis();

    draw_wireframe(mesh->boundary_tris);
    double max, min;
    mesh->max_vel_mag(0, min, max);
    for(const auto& pl: pathlines){
            draw_pathlines(pl, min, max);
    }


    glPopMatrix(); // pop 1st modelView matrix

    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); // pop 1st projection matrix
}


void openGLWindow::set_scene() const
{
    // translate the scene
    glTranslatef(trans_x, trans_y, 0);


    glTranslatef(this->rot_center.entry[0] ,
                 this->rot_center.entry[1],
                 this->rot_center.entry[2]);
    //qDebug() << this->rot_center.entry[0] << " "<< this->rot_center.entry[1] << " " << this->rot_center.entry[2] ;

    multmatrix( this->rotmat );

    glScalef(this->zoom_factor, this->zoom_factor, this->zoom_factor);

    glTranslatef(   -this->rot_center.entry[0] ,
                        -this->rot_center.entry[1] ,
                        -this->rot_center.entry[2]  );
}


// reset all the modelView transformation variables
void openGLWindow::reset_scene()
{
    this->trans_x = this->trans_y = 0.;
    this->zoom_factor = 1.;

    // init matrices
    mat_ident( this->rotmat );

    for(int i = 0; i < 16; i++)
        this->ObjXmat[i]=0.;
    this->ObjXmat[0] = this->ObjXmat[5] = this->ObjXmat[10] = this->ObjXmat[15] = 1;

    this->update();
}

void openGLWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        this->leftButtonDown(event);
    }
    else if(event->button() == Qt::MiddleButton){
        this->middleButtonDown(event);
    }
    else if( event->button() == Qt::RightButton ){
        this->rightButtonDown(event);
    }
}

void openGLWindow::mouseReleaseEvent(QMouseEvent *event)
{

    // if it is left button...
    if(event->button() == Qt::LeftButton){
        this->leftButtonUp(event);
    }
    // if it is middle button...
    else if(event->button() == Qt::MiddleButton){
        this->middleButtonUp(event);
    }
    // if it is right button
    else if(event->button() == Qt::RightButton){
        this->rightButtonUp(event);
    }

    this->update();
}


void openGLWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(LeftButtonDown){
        this->leftButtonMoved(event);
    }
    // if it is middle button...
    else if(MiddleButtonDown){
        this->middleButtonMoved(event);
    }
    // if it is right button
    else if(RightButtonDown){
        this->rightButtonMoved(event);
    }
    this->update();
}


void openGLWindow::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;
        if (numDegrees.y() > 0) // zoom out
        {
            if(this->zoom_factor > 0.05){
                this->zoom_factor -= 0.05;
            }
        }
        else // zoom in
        {
            this->zoom_factor += 0.05;
        }
    this->update();
}

void openGLWindow::leftButtonDown(const QMouseEvent *event)
{
    LeftButtonDown = true;
    QPointF p = event->pos();
    double s = (2.0 * p.x() - WIN_WIDTH) / WIN_WIDTH;
    double t = (2.0 * ( WIN_HEIGHT - p.y() ) - WIN_HEIGHT) / WIN_HEIGHT;

    this->s_old = s;
    this->t_old = t;
    this->last_x = p.x();
    this->last_y = p.y();
}


void openGLWindow::leftButtonMoved(const QMouseEvent *event)
{
    float r[4];
    QPointF p = event->pos();
    double s = ( 2.0 * p.x() - WIN_WIDTH ) / WIN_WIDTH;
    double t = ( 2.0 * ( WIN_HEIGHT - p.y() ) - WIN_HEIGHT ) / WIN_HEIGHT;
    if( (s==s_old) && (t==t_old) )
        return;

    traceball.mat_to_quat(this->rotmat, this->rvec);
    traceball.trackball(r, this->s_old, this->t_old, s, t);
    traceball.add_quats(r, this->rvec, this->rvec);
    traceball.quat_to_mat(this->rvec, this->rotmat);

    s_old = s;
    t_old = t;
}


void openGLWindow::leftButtonUp(const QMouseEvent *event)
{
    LeftButtonDown = false;
    return;
}


void openGLWindow::rightButtonDown(const QMouseEvent *event)
{
    RightButtonDown = true;
    double s, t;
    double position[3];

    int firstwin_leftbottom_x = 0;
    int firstwin_leftbottom_y = WIN_HEIGHT;
    int firstwin_rightx = WIN_WIDTH;
    int firstwin_bottomy = WIN_HEIGHT;
    ScreenToSecondWin(event->pos().x(), event->pos().y(), firstwin_leftbottom_x, firstwin_leftbottom_y,
                      firstwin_rightx, firstwin_bottomy, 0, 0, 1, 1, position[0], position[1]);
    s = position[0];
    t = position[1];
    this->s_old = s;
    this->t_old = t;
}


void openGLWindow::rightButtonMoved(const QMouseEvent *event)
{
    double s, t;
    double position [3];


    int firstwin_leftbottom_x = 0;
    int firstwin_leftbottom_y = WIN_HEIGHT;
    int firstwin_rightx = WIN_WIDTH;
    int firstwin_bottomy = WIN_HEIGHT;

    QPointF p = event->pos();

    ScreenToSecondWin(p.x(), p.y(), firstwin_leftbottom_x, firstwin_leftbottom_y,
                    firstwin_rightx, firstwin_bottomy, 0, 0, 1, 1, position[0], position[1]);
    s = position[0];
    t = position[1];

    if (fabs(s - s_old)<1.e-7 && fabs(t - t_old)<1.e-7)
        return;

    double dx = s-s_old;
    double dy = t-t_old;

    trans_x += dx;
    trans_y += dy;

    s_old = s;
    t_old = t;
}


void openGLWindow::rightButtonUp(const QMouseEvent *event)
{
    RightButtonDown = false;
}


void openGLWindow::middleButtonDown(const QMouseEvent *event)
{
    MiddleButtonDown = true;
    return;
}


void openGLWindow::middleButtonMoved(const QMouseEvent *event)
{
    return;
}


void openGLWindow::middleButtonUp(const QMouseEvent *event)
{
    MiddleButtonDown = false;
    return;
}


