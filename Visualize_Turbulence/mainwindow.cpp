#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}


MainWindow::~MainWindow()
{
    delete ui;


}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
     switch( event->key() )
     {
        case Qt::Key_Escape:
            // close the application
            this->close(); break;
        case Qt::Key_R:
            // reset the scene
            this->ui->openGLWidget->reset_scene();
        default:
            return;
     }
}

