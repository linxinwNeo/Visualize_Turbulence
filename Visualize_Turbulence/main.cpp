#include "mainwindow.h"

#include <QApplication>

#include "predefined.h"
#include "globalVars.h"

extern ReadFile* file;
extern Mesh* mesh;
extern const QString meshFilePath;
extern const QString dataFilePath;

void read_files();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    read_files();

    return a.exec();
}


void read_files(){
    file = new ReadFile( meshFilePath, dataFilePath );
    mesh = file->mesh;
}
