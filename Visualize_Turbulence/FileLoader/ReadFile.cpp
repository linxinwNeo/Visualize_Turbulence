#include <QTextStream>
#include <QDebug>
#include "FileLoader/ReadFile.h"
#include "utility_functions.h"


ReadFile::ReadFile()
{
    this->meshPath = "";
    this->dataPath = "";

    if( this->mesh != NULL ){
        delete this->mesh;
    }

    this->mesh = new Mesh();
}


ReadFile::ReadFile(const QString meshPath, const QString dataPath)
{
    this->meshPath = meshPath;
    this->dataPath = dataPath;

    if( this->mesh != NULL ){
        delete this->mesh;
    }

    this->mesh = new Mesh();

    this->ReadMeshFile(this->meshPath); // read Mesh file first
    this->ReadDataFile(this->dataPath);
}


ReadFile::~ReadFile()
{
    if( this->mesh != NULL ){
        delete this->mesh;
    }
}


void ReadFile::ReadMeshFile(QString f){
    /*we read the mesh file following this pattern:
        1. reading # of nodes
            "% Nodes:              4238"
        2. reading # of elements
            "% Elements:           21982"
        3. reading nodes
            starting with "% Coordinates"
        4. reading elements
            starting with "% Elements (tetrahedra)"
    ----------------------------------------------------------*/
    QFile file(f);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        throwErrorMessage( QString("ReadFile::ReadMeshFile(QString f): couldn't open the file: ").append(f) );
        return;
    }

    QTextStream in(&file);
    QString line;
    int num_nodes = -1;
    int num_tets = -1;
    bool flag = false;
    while (!in.atEnd()) {
        line = in.readLine();

        // step 1: reading # of nodes
        if( line.contains( "% Nodes:"  ) ){
            QStringList strings = line.split(" ", Qt::SkipEmptyParts);
            num_nodes = strings[ strings.size() - 1 ].toInt();
            this->mesh->verts.reserve(num_nodes);
        }

        // step 2: reading # of elements/tets
        if( line.contains( "% Elements:" ) ){
            QStringList strings = line.split(" ", Qt::SkipEmptyParts);
            num_tets = strings[ strings.size() - 1 ].toInt();
            this->mesh->tets.reserve(num_tets);
        }

        // step3: reading individual coordinates for nodes
        if( line.contains( "% Coordinates" ) ){
            flag = true;
            break;
        }
    }

    // checking if anything goes wrong
    if( flag == false){
        throwErrorMessage( "ReadFile::ReadMeshFile(QString f): something is wrong while reading mesh file" );
    }

    for( int i = 0; i < num_nodes; i++ ){
        line = in.readLine();
        QStringList strings = line.split(" ", Qt::SkipEmptyParts);
        Vertex* v = new Vertex(strings[0].toDouble(), strings[1].toDouble(), strings[2].toDouble());
        this->mesh->add_vert(v);
    }

    // step4: reading elements
    // note that vertex indices that describe elements in the file are starting from 1
    // element vertex indices: [0,1,2,3] will produce
    // faces: [0,1,2], [0,2,3], [0,1,2], [1,2,3] in order
    flag = false;
    while (!in.atEnd()) {
        line = in.readLine();
        if( line.contains( "% Elements (tetrahedra)" ) ){
            flag = true;
            break;
        }
    }

    // checking if we found the element part
    if( flag == false){
        throwErrorMessage( "ReadFile::ReadMeshFile(QString f): Couldn't find elements" );
    }

    for( int i = 0; i < num_tets; i++ ){
        line = in.readLine();
        QStringList strings = line.split(" ", Qt::SkipEmptyParts);
        Tet* tet = new Tet();
        for( int j = 0; j < strings.size(); j++ ){
            int vertex_idx = strings[j].toInt() - 1; // -1 beacuase it is starting with 1
            Vertex* v = this->mesh->verts[vertex_idx];
            tet->add_vert(v);
            v->add_tet(tet);
        }
        this->mesh->add_tet(tet);
    }

    return;
}


// should only be called when ReadMeshFile() is called
void ReadFile::ReadDataFile(QString f){
    // checking if mesh has been built
    if( this->mesh->num_verts() == 0 ) return;
    if( this->mesh->num_tets() == 0 ) return;

    /*we read the data file following this pattern:
        1. reading descriptions, this tells which index we are interested in
        2. read data for each node
    */

    QFile file(f);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        throwErrorMessage( QString("ReadFile::ReadDataFile(QString f): couldn't open the file: ").append(f) ); return;
    }

    QTextStream in(&file);
    QString line;
    while (!in.atEnd()) {
        line = in.readLine();
        // check if data File has the same number of nodes
        if( line.contains( "% Nodes:" ) ){
            QStringList strings = line.split(" ", Qt::SkipEmptyParts);
            int num_nodes = strings[ strings.size() - 1 ].toInt();
            if( num_nodes != this->mesh->num_verts() ){
                throwErrorMessage( "ReadFile::ReadDataFile(QString f): the # of nodes for the data file and the mesh file are different" ); return;
            }
        }

        // step 1: read description
        if( line.contains( "% Description:" ) ){

        }
    }
}
