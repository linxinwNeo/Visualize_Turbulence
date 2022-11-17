#include "mainwindow.h"

#include <QApplication>

#include "Others/GlobalVars.h"

extern ReadFile* file;
extern Mesh* mesh;
extern const QString meshFilePath;
extern const QString dataFilePath;


void read_files();


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    read_files();

    MainWindow w;
    w.show();

    return a.exec();
}


void read_files(){
    file = new ReadFile( meshFilePath, dataFilePath );
    mesh = file->mesh;
}
