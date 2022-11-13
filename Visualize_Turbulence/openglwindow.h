#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>


class openGLWindow : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

public:
    // variables
    double s_old;
    double t_old;
    double last_x;
    double last_y;

    // functions
    openGLWindow(QWidget *parent);
    ~openGLWindow();

protected:
    void    initializeGL() override;
    void	paintGL() override;
    //void	resizeGL(int w, int h) override;

private:
    void mousePressEvent(QMouseEvent * event) override ;
    void mouseReleaseEvent(QMouseEvent * event) override ;
    void mouseMoveEvent(QMouseEvent * event) override ;

    void rightButtonDown(QMouseEvent *event);
    void rightButtonUp(QMouseEvent * event);

    void leftButtonMoved(QMouseEvent *event);

    void middleButtonDown(QMouseEvent *event);
    void middleButtonMoved(QMouseEvent *event);

    void wheelEvent(QWheelEvent * event) override;
};

#endif // OPENGLWINDOW_H
