#ifndef DRAW_H
#define DRAW_H

#define GL_SILENCE_DEPRECATION

#include <cmath>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/gltypes.h>

#include "Others/Utilities.h"
#include "Others/Predefined.h"
#include "Geometry/Triangle.h"
#include "Geometry/Vertex.h"
#include "Lines/PathLine.h"
#include "Lines/StreamLine.h"
#include "Analysis/FixedPtDetect.h"

// function prototypes
inline void draw_cylinder(
            const double radius, const double height, const double slices
);

inline void draw_axis();
inline void draw_arrow();
inline void draw_arrow(PathLine* pl);
inline void draw_pathlines(PathLine* pl, const double min_vel_mag, const double max_vel_mag);
inline void draw_triangles(vector<Triangle*>& tris);
inline void draw_isosurfaces(const Isosurface* isosurface, const double min, const double max);

/*---------------------------------------------------------------------*/

inline void draw_axis()
{
    // x axis
    glMatrixMode(GL_MODELVIEW);

    glLineWidth(5);

    glBegin(GL_LINES);
        glColor3f(1, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(10, 0, 0);
    glEnd();

    // y axis
    glBegin(GL_LINES);
        glColor3f(0, 1, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 10, 0);
    glEnd();

    // z axis
    glBegin(GL_LINES);
        glColor3f(0, 0, 1);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 10);
    glEnd();
}


//https://gist.github.com/nikAizuddin/5ea402e9073f1ef76ba6
// z axis is the height
// the cylinder is pointing +z direction
inline void draw_cylinder(
            const double radius, const double height, const double slices
)
{
    GLfloat x              = 0.0;
    GLfloat y              = 0.0;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 2*PI / slices;

    /** Draw the tube */
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            glVertex3f(x, y , 0.0);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
        glVertex3f(radius, 0.0, 0.0);
    glEnd();

    /** Draw the circle on top of cylinder */
    glBegin(GL_POLYGON);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
    glEnd();
}


// draw an arrow at origin
inline void draw_arrow()
{
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    glTranslatef(0, 0, cylinder_height);
    glutSolidCone(cone_base_radius, cone_height, slices, 1);
    glPopMatrix();

    draw_cylinder(cylinder_radius, cylinder_height, slices);
}


inline void draw_triangles(vector<Triangle*>& tris)
{
    glBegin(GL_TRIANGLES);
        for( const Triangle* tri : tris ){
            vector<Vertex*> verts = tri->verts;
            Vertex* v1 = verts[0];
            Vertex* v2 = verts[1];
            Vertex* v3 = verts[2];

           glVertex3f(v1->x(), v1->y(), v1->z());
           glVertex3f(v2->x(), v2->y(), v2->z());
           glVertex3f(v3->x(), v3->y(), v3->z());
        }
    glEnd();
}


inline void draw_wireframe(vector<Triangle*>& tris)
{
    glLineWidth(2);
    glColor3f(0, 0, 1);
    for( const Triangle* tri : tris ){
        glBegin(GL_LINE_STRIP);
            vector<Vertex*> verts = tri->verts;
            Vertex* v1 = verts[0];
            Vertex* v2 = verts[1];
            Vertex* v3 = verts[2];

           glVertex3f(v1->x(), v1->y(), v1->z());
           glVertex3f(v2->x(), v2->y(), v2->z());
           glVertex3f(v3->x(), v3->y(), v3->z());
           glVertex3f(v1->x(), v1->y(), v1->z());
        glEnd();
    }

}


// draw an arrow at specificed location and pointing at vel.
inline void draw_arrow(const Vector3d& pos, const Vector3d& vel)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        QQuaternion q;
        QVector3D from = QVector3D(0, 0, 1);
        QVector3D to;
        to.setX(vel.x());
        to.setY(vel.y());
        to.setZ(vel.z());
        to.normalize();
        q = q.rotationTo(from, to);
        Matrix mat4x4;
        Utility::Quanternion_to_Matrix4x4(q, mat4x4);
        glTranslatef(pos.x(), pos.y(), pos.z()); // then translate
        Utility::multmatrix(mat4x4);  // rotate first
        draw_arrow();
    glPopMatrix();
}


inline void draw_pathlines(PathLine* pl, const double min_vel_mag, const double max_vel_mag)
{
    glLineWidth(4);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glBegin(GL_LINE_STRIP);
    for(const Vertex* v : pl->verts){
        const Vector3d& p = v->cords;
        const Vector3d& vel = v->vels.begin()->second;
        double vel_mag = length(vel);
        //const RGB color = CT.lookUp((vel_mag - min_vel_mag) / (max_vel_mag-min_vel_mag));
        glColor3f(1, 0, 0);
        glVertex3f(p.x(), p.y(), p.z());
    }
    glEnd();
    glPopMatrix();
}


inline void draw_streamline(const StreamLine* sl, const double min, const double max)
{
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);

    glLineWidth(4);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glBegin(GL_LINE_STRIP);

    const double dmag = max - min;

    long int i;
    // draw back_ward vertices in reverse order
    for(i = sl->num_bw_verts() - 1; i >= 0; i--){
        Vertex* vert = sl->bw_verts[i];
        const Vector3d& p = vert->cords;
        const Vector3d& vel = vert->vels.begin()->second;
        const double vel_mag = length(vel);
        const Vector3d color = CT.lookUp((vel_mag - min) / dmag);
        glColor3f(color.x(), color.y(), color.z());
//        glColor3f(0, 0, 1);
        glVertex3f(p.x(), p.y(), p.z());
    }

    {
        // draw seed
        const Vertex* seed = sl->seed;
        const Vector3d seed_cord = seed->cords;
        const double vel_mag = length(seed->vels.begin()->second);
        const Vector3d color = CT.lookUp((vel_mag - min) / dmag);
        glColor3f(color.x(), color.y(), color.z());
//         glColor3f(0, 0, 1);
        glVertex3f(seed_cord.x(), seed_cord.y(), seed_cord.z());
    }

    // draw fw vertices in order
    for(i = 0; i < sl->num_fw_verts() ; i++){
        Vertex* vert = sl->fw_verts[i];
        const Vector3d& p = vert->cords;
        const Vector3d& vel = vert->vels.begin()->second;
        const double vel_mag = length(vel);
        const Vector3d color = CT.lookUp((vel_mag - min) / dmag);
        glColor3f(color.x(), color.y(), color.z());
//        glColor3f(0, 0, 1);
        glVertex3f(p.x(), p.y(), p.z());
    }
    glEnd();

    if(show_seeds){
        glPointSize(15);
        glBegin(GL_POINTS);
        glColor3f(1, 1, 1);
        const Vertex* seed = sl->seed;
        const Vector3d seed_cord = seed->cords;
        glVertex3f(seed->x(), seed->y(), seed->z());
        glEnd();
    }

    glPopMatrix();
}

inline void draw_arrows( PathLine* pl )
{
    glColor3f(arrow_color[0], arrow_color[1], arrow_color[2]);
    for(const Vertex* v : pl->verts){
        const Vector3d& p = v->cords;
        const Vector3d& vel = v->vels.begin()->second;
        draw_arrow(p, vel);
    }
}

inline void draw_opague_boundary_tris(double alpha, vector<Triangle*> tris)
{
    glEnable(GL_BLEND); //Enable blending.
    glDepthMask( GL_FALSE );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending function.

    glColor4f( 0, 0, 1, alpha );
    draw_triangles(tris);

    glDepthMask( GL_TRUE );
    glDisable( GL_BLEND );
}


inline void draw_isosurfaces(const Isosurface* isosurface, const double min, const double max)
{
    if(isosurface == NULL){
        qDebug() << "draw_isosurfaces: isosurface is null!";
        return;
    }

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    double d = max-min;

    float color[ ] = { 1.,0.,0. };
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, color );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, color );

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glBegin(GL_TRIANGLES);
    for(Triangle* tri : isosurface->tris){
        const Vector3d& normal = tri->cal_normal();
        glNormal3f(normal.x(), normal.y(), normal.z());

        Vector3d v1 = tri->verts[0]->cords;
        Vector3d v2 = tri->verts[1]->cords;
        Vector3d v3 = tri->verts[2]->cords;
        glVertex3f(v1.x(), v1.y(), v1.z());
        glVertex3f(v2.x(), v2.y(), v2.z());
        glVertex3f(v3.x(), v3.y(), v3.z());
    }
    glEnd();

    glPopMatrix();

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
}


inline void decide_color(unsigned short type){
    switch(type){
    case 1: // source node
    case 5: // repelling focus source
        glColor3f(1, 0, 0); // source-like red
        break;
    case 2: // sink node
    case 6: // repelling saddle focus
        glColor3f(0, 0, 1); // sink-like blue
        break;
    case 3:
    case 4:
    case 7:
    case 8:
        glColor3f(1, 1, 0); // saddle-like
        break;
    default:
        glColor3f(0, 0, 0); // weird criticle points
        break;
    }
}

inline void draw_singularities(const vector<Singularity*> fixed_pts)
{
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glPointSize(15);
    glBegin(GL_POINTS);
    for(Singularity* pt : fixed_pts){
        decide_color(pt->type);
        glVertex3f(pt->x(), pt->y(), pt->z());
    }
    glEnd();

    glPopMatrix();
}


inline void draw_ECG_connections( ECG* ecg ){
    for(ECG_NODE* node : ecg->get_nodes()){
        set<ECG_EDGE*> inEdges = node->in_edges;
        set<ECG_EDGE*> outEdges = node->out_edges;

        for(ECG_EDGE* edge : inEdges){
            draw_streamline(edge->sl, 0, 0);
        }
        for(ECG_EDGE* edge : outEdges){
            draw_streamline(edge->sl, 0, 0);
        }
    }
}

inline void color_tets_with_fixedPts(const vector<Tet*> & tets){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glBegin(GL_TRIANGLES);
    glColor3f(0,0,0);
    for(const Tet* tet: tets){
        for(const Triangle* tri:tet->tris){
            Vector3d v1 = tri->verts[0]->cords;
            Vector3d v2 = tri->verts[1]->cords;
            Vector3d v3 = tri->verts[2]->cords;
            glVertex3f(v1.x(), v1.y(), v1.z());
            glVertex3f(v2.x(), v2.y(), v2.z());
            glVertex3f(v3.x(), v3.y(), v3.z());
        }
    }
    glEnd();

    glPopMatrix();
}

#endif // DRAW_H
