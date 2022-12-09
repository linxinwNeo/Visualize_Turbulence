#ifndef UTILITIES_H
#define UTILITIES_H


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
#include "Lines/PathLine.h"
#include "Lines/StreamLine.h"
#include "Surfaces/Isosurface.h"
#include "Others/TraceBall.h"
#include "Others/ColorTable.h"

extern Mesh* mesh;
extern bool LeftButtonDown, MiddleButtonDown, RightButtonDown;
extern const double cone_base_radius;
extern const double cone_height;
extern const double cylinder_height;
extern const double cylinder_radius;
extern const int slices;
extern const double arrow_color[];
extern vector<PathLine*> pathlines;
extern unordered_map< double, vector<StreamLine*> > streamlines_for_all_t;
extern unordered_map< double, Isosurface*> isosurfaces_for_all_t;
extern ColorTable CT;
extern const unsigned int NUM_SEEDS;
extern const unsigned int max_num_steps;
extern const UI frames_per_sec;
extern const double time_step_size;
extern const double surface_level_ratio;
extern unordered_map<double, double> surface_level_vals;
extern const double dist_step_size;

extern bool show_streamlines;
extern bool show_pathlines;
extern bool show_isosurfaces;
extern bool show_boundary_wireframe;
extern bool show_opage_boundary_tris;
extern bool show_axis;

extern const double boundary_tri_alpha;

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


inline void throwErrorMessage( const QString message )
{
    QMessageBox messageBox;
    messageBox.critical(0, "Error", message);
    messageBox.setFixedSize(500,200);
    exit(-1);
}


inline void ScreenToSecondWin(
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


inline bool is_in_set(set<Tet*> s, Tet* tet)
{
    if(s.find(tet) == s.end()) return false;
    return true;
}


inline void Quanternion_to_Matrix4x4(const QQuaternion& q, Matrix& mat4x4)
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


inline void multmatrix(const Matrix m)
{
    int i, j, index = 0;

    GLfloat mat[16];

    for ( i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            mat[index++] = m[i][j];

    glMultMatrixf( mat );
}


inline void mat_ident(Matrix m)
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


// output is saved in min_idx and min_val
inline void array_min(const double ds[], const unsigned int& size, unsigned int& min_idx, double& min_val )
{
    if(size <= 0) {
        qDebug() << "min_val: pasing an array of incorrect size!";
        return;
    }
    min_idx = 0;
    min_val = ds[0];
    for(unsigned int i = 1; i < size; i++ ){
        if(ds[i] < min_val){
            min_idx = i;
            min_val = ds[i];
        }
    }
}


// utility to create an array from 3 separate values:
inline float * Array3( float a, float b, float c )
{
    static float array[4];
    array[0] = a;
    array[1] = b;
    array[2] = c;
    array[3] = 1.;
    return array;
}
// utility to create an array from a multiplier and an array:
inline float * MulArray3( float factor, float array0[3] )
{
    static float array[4];
    array[0] = factor * array0[0];
    array[1] = factor * array0[1];
    array[2] = factor * array0[2];
    array[3] = 1.;
    return array;
}
#endif // UTILITIES_H
