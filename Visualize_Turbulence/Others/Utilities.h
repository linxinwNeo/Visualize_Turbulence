#ifndef UTILITIES_H
#define UTILITIES_H

#include <queue>
#define GL_SILENCE_DEPRECATION

#include <QString>
#include <QMessageBox>
#include <set>
#include <OpenGL/gl.h>
#include <OpenGL/gltypes.h>
#include <QtGui/qgenericmatrix.h>
#include <QtGui/qquaternion.h>

#include "Geometry/Mesh.h"
#include "Geometry/Tet.h"
#include "Others/TraceBall.h"
#include "Others/ColorTable.h"

extern vector<Mesh*> meshes;
extern bool LeftButtonDown, MiddleButtonDown, RightButtonDown;
extern const double cone_base_radius;
extern const double cone_height;
extern const double cylinder_height;
extern const double cylinder_radius;
extern const int slices;
extern const double arrow_color[];
extern ColorTable CT;
extern const UI NUM_SEEDS;
extern const UI max_num_steps;
extern const UI NUM_SEEDS_for_Limit;
extern const UI max_num_steps_for_Limit;
extern const UI frames_per_sec;
extern const double time_step_size;
extern const double surface_level_ratio;
extern unordered_map<double, double> surface_level_vals;
extern const double dist_step_size;
extern const unsigned int max_num_recursion;
extern const double zero_threshold;
extern const double h;

extern bool show_streamlines;
extern bool show_pathlines;
extern bool show_isosurfaces;
extern bool show_boundary_wireframe;
extern bool show_opage_boundary_tris;
extern bool show_axis;
extern bool build_ECG;
extern bool tracing_streamlines_from_seed;
extern bool tracing_streamlines_from_critical_pts;
extern bool show_ECG_connections;
extern bool show_ECG_edge_constructions;
extern bool show_fixedPts;
extern bool show_tets_with_fixedPts;
extern bool show_seeds;

extern const double boundary_tri_alpha;

namespace Utility
{
    // function prototypes
    inline void throwErrorMessage( const QString message );
    inline void ScreenToSecondWin(
            const int px, const int py,
            const int screen_leftx, const int screen_bottomy,
            const int win_screen_sizex, const int win_screen_sizey,
            const double world_leftx, const double world_bottomy,
            const double win_world_sizex, const double win_world_sizey,
            double &s, double &t
    );
    inline void Quanternion_to_Matrix4x4(const QQuaternion& q, Matrix& mat4x4);
    inline void multmatrix(const Matrix m);
    inline void mat_ident(Matrix m);
    inline void mat_ident(float m[16]);
    inline vector<UL> generate_unique_random_Tet_idx(Mesh* mesh);
    inline bool is_in_set(set<Tet*> s, Tet* tet);
    inline bool is_in_set(set<Edge*> s, Edge* e);
    inline void array_min(const double ds[], const unsigned int& size, unsigned int& min_idx, double& min_val );
    inline void array_max(const double ds[], const unsigned int& size, unsigned int& max_idx, double& max_val );
    inline float * Array3( float a, float b, float c );
    inline float * MulArray3( float factor, float array0[3] );
    inline void clear_mem(vector<Vertex*> verts);
    inline void clear_mem(vector<Edge*> edges);
    inline void clear_mem(vector<Triangle*> tris);
    inline void clear_mem(vector<Tet*> tets);
    inline void clear_mem(queue<Tet*> tets);
    inline vector<Edge*> make_edges(vector<Vertex*> verts, bool add_each_other);
    void swap(double& a, double& b);
    void swap(long int& a, long int& b);
    double SingedDistance(const Vector3d P, const Vector3d a, const Vector3d b, const Vector3d c);
    inline double random_value(const double min, const double max);
}


inline void Utility::throwErrorMessage( const QString message )
{
    QMessageBox messageBox;
    messageBox.critical(0, "Error", message);
    messageBox.setFixedSize(500,200);
    exit(-1);
}


inline void Utility::ScreenToSecondWin(
        const int px, const int py,
        const int screen_leftx, const int screen_bottomy,
        const int win_screen_sizex, const int win_screen_sizey,
        const double world_leftx, const double world_bottomy,
        const double win_world_sizex, const double win_world_sizey,
        double &s, double &t
)
{
    double ratiox = (double)(px-screen_leftx)/ (double)win_screen_sizex;
    double ratioy = (double)(screen_bottomy - py)/(double)win_screen_sizey;

    s = (double) world_leftx + ratiox * win_world_sizex;
    t = (double) world_bottomy + ratioy * win_world_sizey;
}


inline bool Utility::is_in_set(set<Tet*> s, Tet* tet)
{
    if(s.find(tet) == s.end()) return false;
    return true;
}

inline bool Utility::is_in_set(set<Edge*> s, Edge* e)
{
    if(s.find(e) == s.end()) return false;
    return true;
}


inline void Utility::Quanternion_to_Matrix4x4(const QQuaternion& q, Matrix& mat4x4)
{
    QMatrix3x3 m3x3 = q.toRotationMatrix();
    const float* m3x3Data = m3x3.constData();
    mat4x4[0][0] = m3x3Data[0];
    mat4x4[0][1] = m3x3Data[1];
    mat4x4[0][2] = m3x3Data[2];
    mat4x4[0][3] = 0;
    mat4x4[1][0] = m3x3Data[3];
    mat4x4[1][1] = m3x3Data[4];
    mat4x4[1][2] = m3x3Data[5];
    mat4x4[1][3] = 0;
    mat4x4[2][0] = m3x3Data[6];
    mat4x4[2][1] = m3x3Data[7];
    mat4x4[2][2] = m3x3Data[8];
    mat4x4[2][3] = 0;
    mat4x4[3][0] = 0;
    mat4x4[3][1] = 0;
    mat4x4[3][2] = 0;
    mat4x4[3][3] = 1;
}


inline void Utility::multmatrix(const Matrix m)
{
    int i, j, index = 0;

    GLfloat mat[16];

    for ( i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            mat[index++] = m[i][j];

    glMultMatrixf( mat );
}


inline void Utility::mat_ident(Matrix m)
{
    int i;

    for (i = 0; i <= 3; i++) {
        m[i][0] = 0.0;
        m[i][1] = 0.0;
        m[i][2] = 0.0;
        m[i][3] = 0.0;
        m[i][i] = 1.0;
    }
}

inline void Utility::mat_ident(float m[16])
{
    unsigned int i;

    for(i = 0; i < 16; i++)
        m[i]=0.;
    m[0] = m[5] = m[10] = m[15] = 1;
}


// output is saved in min_idx and min_val
inline void Utility::array_min(const double arr[], const unsigned int& size, unsigned int& min_idx, double& min_val )
{
    if(size <= 0) {
        qDebug() << "min_val: pasing an array of incorrect size!";
        return;
    }
    min_idx = 0;
    min_val = arr[0];
    for( unsigned int i = 1; i < size; i++ ){
        if(arr[i] < min_val){
            min_idx = i;
            min_val = arr[i];
        }
    }
}


// output is saved in min_idx and min_val
inline void Utility::array_max(const double ds[], const unsigned int& size, unsigned int& max_idx, double& max_val )
{
    if(size <= 0) {
        qDebug() << "min_val: pasing an array of incorrect size!";
        return;
    }
    max_idx = 0;
    max_val = ds[0];
    for(unsigned int i = 1; i < size; i++ ){
        if(ds[i] > max_val){
            max_idx = i;
            max_val = ds[i];
        }
    }
}


// utility to create an array from 3 separate values:
inline float * Utility::Array3( float a, float b, float c )
{
    static float array[4];
    array[0] = a;
    array[1] = b;
    array[2] = c;
    array[3] = 1.;
    return array;
}


// utility to create an array from a multiplier and an array:
inline float * Utility::MulArray3( float factor, float array0[3] )
{
    static float array[4];
    array[0] = factor * array0[0];
    array[1] = factor * array0[1];
    array[2] = factor * array0[2];
    array[3] = 1.;
    return array;
}


inline vector<UL> Utility::generate_unique_random_Tet_idx(Mesh* mesh)
{
    srand((unsigned) time(NULL));
    set<UL> seeded_tets;
    unsigned int cur_num_seeds = 0;
    while(cur_num_seeds < NUM_SEEDS)
    {
        int random_idx = rand() % mesh->num_tets();
        if( seeded_tets.find(random_idx) != seeded_tets.end() ) continue;
        else{
            seeded_tets.insert(random_idx);
            cur_num_seeds ++;
        }
    }

    vector<UL> seeds;
    seeds.reserve(NUM_SEEDS);
    for(UL idx : seeded_tets){
        seeds.push_back(idx);
    }

    return seeds;
}


inline void Utility::clear_mem(vector<Vertex*> verts){
    for(UI i = 0; i < verts.size(); i++){
        if(verts[i] != nullptr){
            delete verts[i];
            verts[i] = nullptr;
        }
    }
    verts.clear();
}


inline void Utility::clear_mem(vector<Edge*> edges){
    for(UI i = 0; i < edges.size(); i++){
        if(edges[i] != nullptr){
            delete edges[i];
            edges[i] = nullptr;
        }
    }
    edges.clear();
}

inline void Utility::clear_mem(vector<Triangle*> tris){
    for(UI i = 0; i < tris.size(); i++){
        if(tris[i] != nullptr){
            delete tris[i];
            tris[i] = nullptr;
        }
    }
    tris.clear();
}

inline void Utility::clear_mem(vector<Tet*> tets){
    for(UI i = 0; i < tets.size(); i++){
        if(tets[i] != nullptr){
            delete tets[i];
            tets[i] = nullptr;
        }
    }
    tets.clear();
}


inline void Utility::clear_mem(queue<Tet*> tets){
    while(!tets.empty()){
        Tet* tet = tets.front();
        tets.pop();
        delete tet;
    }
}

inline vector<Edge*> Utility::make_edges(vector<Vertex*> verts, bool add_each_other){
    vector<Edge*> edges;
    for(UI i = 0; i < verts.size(); i++){
        Vertex* v1 = verts[i];
        for(UI j = i+1; j < verts.size(); j++){
            Vertex* v2 = verts[j];
            Edge* e = new Edge(v1, v2);
            if(add_each_other){
                v1->add_edge(e);
                v2->add_edge(e);
            }
            edges.push_back(e);
        }
    }
    return edges;
}

inline void Utility::swap(double& a, double& b)
{
    double temp = a;
    a = b;
    b = temp;
}


inline void Utility::swap(long int & a, long int& b)
{
    long int temp = a;
    a = b;
    b = temp;
}


// https://people.sc.fsu.edu/~jburkardt/presentations/cg_lab_barycentric_tetrahedrons.pdf
// if val > 0, the pt is on the normal side of the plane
// if val == 0, the pt is on the plane
// if val < 0, the pt is on the other side of the plane
inline double Utility::SingedDistance(const Vector3d P, const Vector3d a, const Vector3d b, const Vector3d c)
{
    Vertex v1 = Vertex(a);
    Vertex v2 = Vertex(b);
    Vertex v3 = Vertex(c);
    Triangle tri = Triangle(&v1, &v2, &v3);

    // compute the normal of the triangle
    Vector3d n = tri.cal_normal();
    const Vector3d& Q = a;

    return dot( (P-Q), n );
}

inline double Utility::random_value(const double min, const double max){
    if(max < min) return 0.;
    if(max == min) return min;

    srand((unsigned)time(NULL));

    const double new_max = max - min;

    return (double)rand() / (double)RAND_MAX * new_max + min;
}

#endif // UTILITIES_H
