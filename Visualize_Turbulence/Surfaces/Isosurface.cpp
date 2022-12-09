#include "Surfaces/Isosurface.h"
#include "Others/Utilities.h"


Isosurface::Isosurface()
{
    this->time = 0;
}

Isosurface::~Isosurface()
{
    UL i;
    for(i = 0; i < this->num_tris(); i++){
        Triangle* tri = this->tris[i];
        if(tri != NULL){
            delete tri;
            this->tris[i] = NULL;
        }
    }

    tris.clear();
}


void constuct_isosurfaces()
{
    // interpolate vertices at all t=n*0.1 and 0<t<num_time_steps
    mesh->interpolate_vertices();

    // calculate actual surface level using surface_level_ratio
    calc_actual_surface_levels_for_all_t();

    // for each vertex, check if it is above the surface level or below and mark them
    classify_vertex_levels_for_all_t();

    // for each tet, calculate the marching tetrahedron index
    calc_marching_indices_for_all_t();

    // for each tet, create triangles based on the index
    create_isosurface_tris_for_all_t();
}


// needs to be called after mesh->interpolate_vertices()
// we are currently using vorticity magnitude
// so we calculate min and max vorticity magnitude at all vertices
// using surface_level_ratio to calculate the actual levels we want for all t
void calc_actual_surface_levels_for_all_t()
{
    mesh->calc_vor_min_max_at_verts_for_all_t();

    double time = 0.;
    while(time < mesh->num_time_steps - 1.)
    {
        pair<double, double> min_max = mesh->min_max_at_verts_for_all_t[time];
        const double& min = min_max.first;
        const double& max = min_max.second;

        const double actual_surface_level = surface_level_ratio * (max-min) + min;
        surface_level_vals[time] = actual_surface_level;

        time += time_step_size; // increment time
    }
}


// assume all vertices are interpolated for all time levels
void classify_vertex_levels_for_all_t()
{
    double time = 0.;
    while(time < mesh->num_time_steps - 1.)
    {
        // for each vert
        for(Vertex* vert : mesh->verts){
            // check if this vert is above the surface level (>=)
            const double vert_vor_mag = length( vert->vors.at(time) );
            if(vert_vor_mag >= surface_level_vals.at(time)){
                vert->is_above_surface[time] = true;
            }
            else{
                vert->is_above_surface[time] = false;
            }
        }

        time += time_step_size; // increment time
    }
}


void calc_marching_indices_for_all_t()
{
    for(Tet* tet:mesh->tets){
        tet->calc_marching_indices();
    }
}


void create_isosurface_tris_for_all_t()
{
    double time = 0.;
    while(time < mesh->num_time_steps - 1.)
    {
        qDebug() << "Building isosurface for time" << time;
        Isosurface* isosurf= new Isosurface();
        for(Tet* tet:mesh->tets){
            vector<Triangle*> new_tris = tet->create_isosurface_tris(time);
            isosurf->add_tri(new_tris);
        }

        isosurfaces_for_all_t[time] = isosurf;
        time += time_step_size; // increment time
    }
}
