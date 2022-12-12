#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTime>

#include "Geometry/Mesh.h"
#include "Others/TraceBall.h"
#include "Others/Vector3d.h"


class openGLWindow : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

public:
    // variables
    double s_old;
    double t_old;
    double last_x;
    double last_y;

    QTimer* timer;
    double animation_time;
    double model_time;

    CTraceBall traceball;
    Quaternion rvec;

    double  zoom_factor;
    float   rotmat[4][4]; //storing rotation parameters
    float   ObjXmat[16]; //Storing current modelview transformation
    double trans_x;
    double trans_y;
    Vector3d rot_center;
    Mesh* cur_mesh;


    // functions
    openGLWindow(QWidget *parent);
    ~openGLWindow();
protected:
    void initializeGL() override;
    void paintGL() override;
    void main_routine() const;
    //void	resizeGL(int w, int h) override;

public:
    void set_scene() const;
    void reset_scene();
    void switch_cur_mesh(Mesh* mesh);

private:
    // mouse events
    void mousePressEvent(QMouseEvent * event) override ;
    void mouseReleaseEvent(QMouseEvent * event) override ;
    void mouseMoveEvent(QMouseEvent * event) override ;
    void wheelEvent(QWheelEvent * event) override;

    void leftButtonDown(const QMouseEvent *event);
    void leftButtonMoved(const QMouseEvent *event);
    void leftButtonUp(const QMouseEvent * event);

    void middleButtonDown(const QMouseEvent *event);
    void middleButtonMoved(const QMouseEvent *event);
    void middleButtonUp(const QMouseEvent *event);

    void rightButtonDown(const QMouseEvent *event);
    void rightButtonMoved(const QMouseEvent * event);
    void rightButtonUp(const QMouseEvent * event);

private slots:
    void increment_time();
};


#endif // OPENGLWINDOW_H
