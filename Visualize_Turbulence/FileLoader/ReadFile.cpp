#include <QTextStream>
#include <QDebug>
#include <QTime>

#include "FileLoader/ReadFile.h"
#include "Others/Utilities.h"


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

    QTime t = t.currentTime();

    // assume both file has same ordering of the vertices
    this->ReadMeshFile(this->meshPath); // read Mesh file first
    this->ReadDataFile(this->dataPath); // then read data file

    // calculate addition things about mesh
    this->mesh->build_triangles();
    this->mesh->build_edges();
    this->mesh->build_tetNeighbors();

    this->mesh->calc_Bounding_Sphere();
    this->mesh->calc_normal_for_all_tris();
    this->mesh->calc_center_for_all_tet();

    // correctness check
    qDebug() << "Mesh: num of triangles:" << this->mesh->num_tris();
    qDebug() << "Mesh: num of boundary triangles: " <<  this->mesh->num_boundary_tris();
    qDebug() << "Mesh: num of tets: " <<  this->mesh->num_tets();
    qDebug() << "Mesh: num of verts: " <<  this->mesh->num_verts();
    qDebug() << "Mesh: num of edges: " <<  this->mesh->num_edges();


    for(Triangle* tri : this->mesh->tris){
        if(tri->num_tets() > 2 || tri->num_tets() == 0){
            qDebug() << "Triangle: num of tets not right!: " <<  tri->num_tets();
        }
        if(tri->num_verts() != 3){
            qDebug() << "Triangle: num of verts not right! " <<  tri->num_verts();
        }
        if(tri->num_edges() != 3){
            qDebug() << "Triangle: num of edges not right! " <<  tri->num_edges();
        }
    }

    for(Tet* tet : this->mesh->tets){
        if(tet->num_edges() != 6){
            qDebug() << "Tet: num_edges not right! " <<  tet->num_edges();
        }

        if(tet->num_tris() != 4){
            qDebug() << "Tet: num_tris not right! " <<  tet->num_tris();
        }

        if(tet->num_verts() != 4){
            qDebug() << "Tet: num_verts not right! " <<  tet->num_verts();
        }
    }

    for(Edge* e:this->mesh->edges){
        if(e->num_verts() != 2){
            qDebug() << "Edge: num of verts not right!" << e->num_verts();
        }
    }

    qDebug() << "Reading Files takes" <<  t.msecsTo(t.currentTime())/1000. << "secs\n\n";
}


ReadFile::~ReadFile()
{

}


void ReadFile::ReadMeshFile(const QString f){
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
    qDebug() << "Reading Mesh File" << f;

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
    if( flag == false ){
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
    // triangles: [0,1,2], [0,2,3], [0,1,2], [1,2,3] in order
    flag = false;
    while (!in.atEnd()) {
        line = in.readLine();
        if( line.contains( "% Elements (tetrahedra)" ) ){
            flag = true;
            break;
        }
    }

    // checking if we found the element part
    if( flag == false ){
        throwErrorMessage( "ReadFile::ReadMeshFile(QString f): Couldn't find elements" );
    }

    for( int i = 0; i < num_tets; i++ ){
        line = in.readLine();
        QStringList strings = line.split(" ", Qt::SkipEmptyParts);
        Tet* tet = new Tet();
        for( int j = 0; j < strings.size(); j++ ){
            int vertex_idx = strings[j].toInt() - 1; // -1 beacuase data's index is starting with 1
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
        1. read num of nodes, make sure the num of nodes are the same
        2. read num of expressions
        3. read descriptions, num of expression/this tells how many time steps we have in this file
        4. read data for individual nodes
    */

    QFile file(f);
    qDebug() << "Reading Data File" << f;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        throwErrorMessage( QString("ReadFile::ReadDataFile(QString f): couldn't open the file: ").append(f) ); return;
    }

    QTextStream in(&file);
    QString line;
    unsigned int num_expressions = 0;

    while (!in.atEnd()) {
        line = in.readLine();
        // step 1:
        // check if data File has the same number of nodes
        if( line.contains( "% Nodes:" ) ){
            QStringList strings = line.split(" ", Qt::SkipEmptyParts);
            unsigned long num_nodes = strings[ strings.size() - 1 ].toInt();
            if( num_nodes != this->mesh->num_verts() ){
                throwErrorMessage( "ReadFile::ReadDataFile(QString f): the # of nodes for the data file and the mesh file are different!" ); return;
            }
        }


        // step 2: read num of expressions
        if( line.contains( "% Expressions:" ) ){
            QStringList strings = line.split(" ", Qt::SkipEmptyParts);
            num_expressions = strings[ strings.size() - 1 ].toInt();
        }

        // step 3: read descprition
        if( line.contains( "% Description:" ) ){
            QStringList strings = line.split(",", Qt::SkipEmptyParts);
            if(strings.size() != 13){
                throwErrorMessage( "ReadFile::ReadDataFile(QString f): the size of description is not 13! This means the data has different format that we expect!" ); return;
            }
        }

        if( line.contains("% x") ) break;
    }

    // calculate how many time steps
    const int expected_num_expressions = 7;
    const unsigned int reminder = num_expressions % expected_num_expressions;
    if(reminder != 0){
        throwErrorMessage( "ReadFile::ReadDataFile(QString f): the num of expressions is not right!" ); return;
    }
    const unsigned int num_time_steps = num_expressions / expected_num_expressions;
    this->mesh->num_time_steps = num_time_steps;

    // step 4: read the data
    unsigned long vert_count = 0;
    unsigned int i;
    while ( !in.atEnd() && vert_count < this->mesh->num_verts() ) {
        line = in.readLine();
        QStringList strings = line.split(" ", Qt::SkipEmptyParts);
        if( strings.size() != num_expressions+3 ){
            throwErrorMessage( "ReadFile::ReadDataFile(QString f): vert does not have correct data format" ); return;
        }
        Vertex* cur_vert = this->mesh->verts[vert_count];

        for( i = 0; i < num_time_steps; i++ ){
            cur_vert->vors.reserve( num_time_steps ); // reserve enough space for many time steps data
            cur_vert->vels.reserve( num_time_steps ); // reserve enough space for many time steps data

            // calculate base index for each time step
            // first 3 indices are reserved for coordinates which we already have.
            const int idx = 3 + i * expected_num_expressions;
            // read velocity vector
            cur_vert->set_vel( (double) i, strings[idx].toDouble(), strings[idx+1].toDouble(), strings[idx+2].toDouble());
            // read vorticity vector
            cur_vert->set_vor( (double) i, strings[idx+3].toDouble(), strings[idx+4].toDouble(), strings[idx+5].toDouble());
            // read Turbulent dynamic viscosity
            cur_vert->set_mu( (double) i, strings[idx+6].toDouble() );
        }

        vert_count ++;
    }


    if( vert_count != this->mesh->num_verts() ) throwErrorMessage( "ReadFile::ReadDataFile(QString f): the num of verts is not right!" ); return;
}
