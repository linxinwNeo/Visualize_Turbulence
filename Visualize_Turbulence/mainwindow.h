#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

#include "FileLoader/ReadFile.h"
#include "Geometry/Mesh.h"
#include "Lines/PathLine.h"

extern Mesh* mesh;
extern vector<PathLine*> pathlines;
extern ReadFile* file;
extern const QString meshFilePath, dataFilePath;
extern const unsigned int NUM_SEEDS;
extern const unsigned int max_num_steps;
extern const double time_step_size;
extern const double dist_scale;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
