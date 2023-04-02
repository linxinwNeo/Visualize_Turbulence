#include "mainwindow.h"
#include "Others/Utilities.h"
#include "QtCore/qtimer.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->total_time = this->model_time = 0.;

    // set timer
    if(meshes.size() != 0){
        this->cur_mesh = meshes[0];
    }

    this->timer = new QTimer(this);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(increment_time()));
    this->timer->start(time_step_size * MSECS_PER_SEC);
}


MainWindow::~MainWindow()
{
    delete ui;

    if(this->timer){
        delete this->timer;
    }
    this->timer = NULL;
}

void MainWindow::update_time(const double t) const
{
    this->ui->modelWindow->time = t;
//    this->ui->graphWindow->time = t;
}

void MainWindow::redraw() const
{
    this->ui->modelWindow->redraw();
    this->ui->graphWindow->redraw();
}

void MainWindow::update_ecg_for_graphWin(ECG * ecg) const
{
    this->ui->graphWindow->set_ecg(ecg);
}

void MainWindow::update_mesh_for_modelWin(Mesh * mesh) const
{
    this->ui->modelWindow->set_mesh(mesh);
}


// switch the cur_mesh with new mesh
void MainWindow::switch_cur_mesh(Mesh *mesh)
{
    this->cur_mesh = mesh;
    this->update_mesh_for_modelWin(cur_mesh);
    this->ui->modelWindow->reset_scene();
}


bool animation_on = true;
void MainWindow::keyPressEvent(QKeyEvent *event)
{
     switch( event->key() )
     {
        case Qt::Key_Escape:
            // close the application
            this->close(); break;
        case Qt::Key_R:
            // reset the scene
            this->ui->modelWindow->reset_scene(); break;
        case Qt::Key_Space:
            if(animation_on) this->timer->stop();
            else {
                this->timer->start(time_step_size * MSECS_PER_SEC);
            }
            animation_on = !animation_on;
            break;

        default:
            return;
     }
}


void MainWindow::increment_time( ) {
    this->total_time += time_step_size;
    this->model_time += time_step_size;

    if(this->total_time >= this->cur_mesh->num_time_steps - 1.){
        this->total_time = this->model_time = 0;
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

    // update time, then ecg
    this->update_time(this->model_time);

    if(this->cur_mesh->ECG_for_all_t.find(model_time) != this->cur_mesh->ECG_for_all_t.end())
        this->update_ecg_for_graphWin(this->cur_mesh->ECG_for_all_t.at(model_time));

    this->redraw();
}


void MainWindow::on_showSLs_triggered()
{
    show_streamlines = !show_streamlines;
    this->redraw();
}


void MainWindow::on_showFPs_triggered()
{
    show_critical_pts = !show_critical_pts;
    this->redraw();
}


void MainWindow::on_showSeeds_triggered()
{
    show_seeds = !show_seeds;
    this->redraw();
}


void MainWindow::on_showAxis_triggered()
{
    show_axis = !show_axis;
    this->redraw();
}


void MainWindow::on_showBoundaryTriangles_triggered()
{
    show_opage_boundary_tris = !show_opage_boundary_tris;
    this->redraw();
}

