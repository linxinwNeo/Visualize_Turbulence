#ifndef READFILE_H
#define READFILE_H

#include <QString>
#include <QFile>
#include "Geometry/Mesh.h"

class ReadFile {
public:
    // member variables
    QString meshPath;
    QString dataPath;

    Mesh* mesh;

    // member functions
    ReadFile();
    ReadFile(const QString, const QString);
    ~ReadFile();

    void ReadMeshFile(QString);
    void ReadDataFile(QString);
};

#endif // READFILE_H
