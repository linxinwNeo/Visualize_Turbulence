#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include "TraceBall.h"
#include "Geometry/Vector3d.h"

class openGLWindow : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

public:
    // variables
    double s_old;
    double t_old;
    double last_x;
    double last_y;

    CTraceBall traceball;
    Quaternion rvec;

    double  zoom_factor;
    float   rotmat[4][4]; //storing rotation parameters
    float   ObjXmat[16]; //Storing current modelview transformation
    double trans_x;
    double trans_y;
    Vector3d rot_center;


    // functions
    openGLWindow(QWidget *parent);
    ~openGLWindow();
    void mat_ident( Matrix m );
    void set_scene();
    void multmatrix( const Matrix );

protected:
    void  initializeGL() override;
    void	paintGL() override;
    //void	resizeGL(int w, int h) override;

private:
    void mousePressEvent(QMouseEvent * event) override ;
    void mouseReleaseEvent(QMouseEvent * event) override ;
    void mouseMoveEvent(QMouseEvent * event) override ;
    void wheelEvent(QWheelEvent * event) override;

    void leftButtonDown(QMouseEvent *event);
    void leftButtonMoved(QMouseEvent *event);
    void leftButtonUp(QMouseEvent * event);

    void middleButtonDown(QMouseEvent *event);
    void middleButtonMoved(QMouseEvent *event);
    void middleButtonUp(QMouseEvent *event);

    void rightButtonDown(QMouseEvent *event);
    void rightButtonMoved(QMouseEvent * event);
    void rightButtonUp(QMouseEvent * event);
};

#endif // OPENGLWINDOW_H
