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

    UL i, j;
    // clear memoery for streamlines
    for( i = 0; i < streamlines_for_all_t.size(); i++ ){
        vector<StreamLine*> sls = streamlines_for_all_t[i];
        for( j = 0; j < sls.size(); j++ ){
            StreamLine* sl = sls[j];
            if(sl != NULL){
                delete sl;
            }
            sls[j] = NULL;
        }
        sls.clear();
    }
    streamlines_for_all_t.clear();

    // clear memoery for pathlines
    for( i = 0; i < pathlines.size(); i++ ){
        if(pathlines[i] != NULL){
            delete pathlines[i];
            pathlines[i] = NULL;
        }
    }
    pathlines.clear();
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

