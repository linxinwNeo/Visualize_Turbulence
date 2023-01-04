#ifndef ECGWINDOW_H
#define ECGWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTime>
#include <QObject>
#include "Analysis/ECG.h"

class ECGWindow : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

public:
    ECGWindow(QWidget *parent);
    ~ECGWindow();

protected:
    void initializeGL() override;
    void paintGL() override;
    void main_routine() const;

public:
    double s_old;
    double t_old;
    double last_x;
    double last_y;

    ECG* ecg;
    void update_scene();

};

#endif // ECGWINDOW_H
