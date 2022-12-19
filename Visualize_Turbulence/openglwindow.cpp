#define GL_SILENCE_DEPRECATION
#include <cfloat>
#include <QMouseEvent>
#include <QSurfaceFormat>
#include <GLUT/glut.h>
#include <QTimer>

#include "openglwindow.h"
#include "Others/Utilities.h"
#include "Others/Draw.h"
#include "Geometry/Mesh.h"

openGLWindow::openGLWindow(QWidget *parent) : QOpenGLWidget(parent)
{
    this->s_old = 0.;
    this->t_old = 0.;
    this->last_x = 0.;
    this->last_y = 0.;
    this->zoom_factor = 1.;
    this->trans_x = 0.;
    this->trans_y = 0.;
    this->cur_mesh = NULL;
    this->animation_time = this->model_time = 0.;

    // init matrices
    Utility::mat_ident( this->rotmat );

    Utility::mat_ident( this->ObjXmat );
    this->cur_mesh = meshes[0];

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

    for(Mesh* mesh : meshes){
        if(mesh != NULL)
            delete mesh;
    }
    meshes.clear();
    this->cur_mesh = NULL;

    return;
}


// time control
void openGLWindow::increment_time( ){
    this->animation_time += time_step_size;
    this->model_time += time_step_size;

    if(this->animation_time >= cur_mesh->num_time_steps - 1.){
        this->animation_time = this->model_time = 0;
    }

    // loop from beginning
//    if(this->animation_time >= 60){
//        this->animation_time = this->model_time = 0;
//    }

//    // iteration through each model, last 10 secs for each
//    if( animation_time < 30 && this->cur_mesh != meshes[1]){
//        this->model_time = 0.;
//        switch_cur_mesh(meshes[1]);
//    }
//    else if(animation_time >= 30 && animation_time < 60 && this->cur_mesh != meshes[4]){
//        this->model_time = 0.;
//        switch_cur_mesh(meshes[4]);
//    }

    this->update();
}


void openGLWindow::initializeGL()
{
    this->initializeOpenGLFunctions();
    if( meshes.size() == 0 ) return;

    this->rot_center = cur_mesh->rot_center; //set the rotation center!

    glEnable(GL_MULTISAMPLE_ARB);
    glEnable(GL_DEPTH_TEST);

    // set timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(increment_time()));
    timer->start(time_step_size * MSECS_PER_SEC);

    glShadeModel (GL_SMOOTH);

    float White[ ] = { 1.,1.,1.,1. };
    float color[ ] = { 1.,0.,0. };
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, Utility::MulArray3( .3f, White )  ) ;
    glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, Utility::Array3( 0., 0., 0. ) );

    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, color );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, color );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, Utility::Array3( 0.7, 0.7, 0.7 ) );
    glMaterialf ( GL_FRONT_AND_BACK, GL_SHININESS, 2.f );


    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    float LightColor[] = {1, 1, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, Utility::MulArray3( .2, White ) );
    glLightModeli ( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
    glLightfv( GL_LIGHT0, GL_AMBIENT, Utility::Array3( 0., 0., 0. ) );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, LightColor );
    glLightfv( GL_LIGHT0, GL_SPECULAR, LightColor );
    glLightf ( GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1. );
    glLightf ( GL_LIGHT0, GL_LINEAR_ATTENUATION, 0. );
    glLightf ( GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0. );
}


void openGLWindow::paintGL()
{
    if( meshes.size() == 0 ) return;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); // push 1st projection matrix
    glLoadIdentity();


    glOrtho(-1.5, 1.5, -1, 1., -1000.0, 4000.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); // push 1st modelView matrix
    glLoadIdentity();
    this->set_scene(); // we draw everything after this call

    glClearColor (0.7, 0.7, 0.7, 1.0);  // grey background for rendering color coding and lighting
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->cur_mesh = meshes[0];
    //main_routine(meshes[0]);

    Tet* tet = meshes[0]->tets[0];
    vector<Vertex*> temp_verts; vector<Edge*> temp_edges; vector<Tet*> temp_tets;

    Vertex* v1 = tet->verts[0]; v1->cords = Vector3d(0.5, 0, 0);
    Vertex* v2 = tet->verts[1]; v2->cords = Vector3d(0, 0.5, 0);
    Vertex* v3 = tet->verts[2]; v3->cords = Vector3d(-0.5, 0, 0);
    Vertex* v4 = tet->verts[3]; v4->cords = Vector3d(0, 0, 0.5);

    glPushMatrix();
    glTranslatef(0, 0.6, 0);
    glColor3f(0, 0, 1);
    glBegin(GL_LINES);
        glVertex3d(v1->x(), v1->y(), v1->z());
        glVertex3d(v2->x(), v2->y(), v2->z());

        glVertex3d(v1->x(), v1->y(), v1->z());
        glVertex3d(v3->x(), v3->y(), v3->z());

        glVertex3d(v1->x(), v1->y(), v1->z());
        glVertex3d(v4->x(), v4->y(), v4->z());

        glVertex3d(v2->x(), v2->y(), v2->z());
        glVertex3d(v3->x(), v3->y(), v3->z());

        glVertex3d(v2->x(), v2->y(), v2->z());
        glVertex3d(v4->x(), v4->y(), v4->z());

        glVertex3d(v3->x(), v3->y(), v3->z());
        glVertex3d(v4->x(), v4->y(), v4->z());
    glEnd();
    glPopMatrix();

    tet->subdivide(0, temp_verts, temp_edges, temp_tets);
    for(UI i = 5; i < 6; i++){
        Tet* t = temp_tets[i];
        for(Edge* e : t->edges){
            Vertex* v1 = e->verts[0];
            Vertex* v2 = e->verts[1];

            glColor3f(0, 0, 1);
            glBegin(GL_LINES);
                glVertex3f(v1->x(), v1->y(), v1->z());
                glVertex3f(v2->x(), v2->y(), v2->z());
            glEnd();
        }
    }

//    if(cur_mesh == meshes[1]){
//        glPushMatrix();
//        glTranslatef(-1.3, 0, 0);
//        main_routine(meshes[0]);
//        glPopMatrix();

//        main_routine(meshes[1]);

//        glPushMatrix();
//        glTranslatef(1.3, 0, 0);
//        main_routine(meshes[2]);
//        glPopMatrix();
//    }
//    else{
//        glPushMatrix();
//        glTranslatef(-1.3, 0, 0);
//        main_routine(meshes[3]);
//        glPopMatrix();

//        main_routine(meshes[4]);

//        glPushMatrix();
//        glTranslatef(1.3, 0, 0);
//        main_routine(meshes[5]);
//        glPopMatrix();
//    }


    glPopMatrix(); // pop 1st modelView matrix

    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); // pop 1st projection matrix
}


void openGLWindow::main_routine(Mesh * mesh) const
{
    if(show_streamlines){
        double max = DBL_MIN, min = DBL_MAX;
        mesh->max_vel_mag(model_time, min, max);
        const auto& sls = mesh->streamlines_for_all_t.at(model_time);
        for(StreamLine* sl:sls){
            draw_streamlines(sl, min, max);
        }
    }

    if(show_isosurfaces){
        double max = DBL_MIN, min = DBL_MAX;
        const auto& isosurface = mesh->isosurfaces_for_all_t.at(model_time);
        draw_isosurfaces(isosurface, min, max);
    }

    //    if(show_pathlines){
    //    }

    if(show_boundary_wireframe)
        draw_wireframe(mesh->boundary_tris);

    if(show_opage_boundary_tris){
        draw_opague_boundary_tris(boundary_tri_alpha, mesh->boundary_tris);
    }
}


void openGLWindow::set_scene() const
{

    // translate the scene
    glTranslatef(trans_x, trans_y, 0);


    glTranslatef(this->rot_center.entry[0] ,
                 this->rot_center.entry[1],
                 this->rot_center.entry[2]);
    //qDebug() << this->rot_center.entry[0] << " "<< this->rot_center.entry[1] << " " << this->rot_center.entry[2] ;

    Utility::multmatrix( this->rotmat );

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
    Utility::mat_ident( this->rotmat );

    for(int i = 0; i < 16; i++)
        this->ObjXmat[i]=0.;
    this->ObjXmat[0] = this->ObjXmat[5] = this->ObjXmat[10] = this->ObjXmat[15] = 1;

    this->rot_center = cur_mesh->rot_center;
    this->update();
}


void openGLWindow::switch_cur_mesh(Mesh *mesh)
{
    this->cur_mesh = mesh;
    reset_scene();
    return;
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
    Utility::ScreenToSecondWin(event->pos().x(), event->pos().y(), firstwin_leftbottom_x, firstwin_leftbottom_y,
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

    Utility::ScreenToSecondWin(p.x(), p.y(), firstwin_leftbottom_x, firstwin_leftbottom_y,
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


