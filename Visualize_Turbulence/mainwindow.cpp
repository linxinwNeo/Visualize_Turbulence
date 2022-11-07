#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "predefined.h"
#include "globalVars.h"

extern ReadFile* file;
extern Mesh* mesh;
extern const QString meshFilePath;
extern const QString dataFilePath;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    file = new ReadFile( meshFilePath, dataFilePath );
    mesh = file->mesh;
}

MainWindow::~MainWindow()
{
    delete ui;
}

