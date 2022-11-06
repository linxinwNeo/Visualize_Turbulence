#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Geometry/Mesh.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Mesh* m = new Mesh();

}

MainWindow::~MainWindow()
{
    delete ui;
}

