#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Analysis/ECG.h"
#include <QMainWindow>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QTimer* timer;
    double total_time;
    double model_time;
    Mesh* cur_mesh;

    void update_time(const double time) const;
    void redraw() const;
    void update_ecg_for_graphWin(ECG* ) const;
    void update_mesh_for_modelWin(Mesh*) const;
    void switch_cur_mesh(Mesh *mesh);


    void keyPressEvent(QKeyEvent *event) override;


private:
    Ui::MainWindow *ui;

private slots:
    void increment_time();
};
#endif // MAINWINDOW_H
