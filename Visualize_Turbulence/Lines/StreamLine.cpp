#include "Lines/StreamLine.h"
#include "Others/Utilities.h"

StreamLine::StreamLine()
{
    this->time = 0.;
}


StreamLine::~StreamLine()
{
    UL i;

    if(seed != NULL) {
        delete seed;
        seed = NULL;
    }

    for(i = 0; i < this->num_fw_verts(); i++){
        if(this->fw_verts[i] != NULL) delete this->fw_verts[i];
    }
    this->fw_verts.clear();

    for(i = 0; i < this->num_bw_verts(); i++){
        if(this->bw_verts[i] != NULL) delete this->bw_verts[i];
    }
    this->bw_verts.clear();
}


void tracing_streamlines()
{
    for(unsigned int i = 0; i < meshes.size(); i++){
        auto& mesh = meshes[i];
        qDebug()<< "Tracing streamlines for mesh"<< i+1;
        // interpolate vertices at all t=n*0.1 and 0<t<num_time_steps
        mesh->interpolate_vertices();

        // place inital seeds
        place_seeds(mesh);

        // trace seeds and form pathlines
        // mainwindow.cpp will clear the memory of pathlines and streamlines
        build_streamlines_from_seeds(mesh);
    }

}


// now every streamline at any time has a seed point as a starting point, we want to calculate their trajectory individually
void build_streamlines_from_seeds( Mesh* mesh )
{
    // for each time step
    double cur_time = 0;
    while( cur_time < mesh->num_time_steps - 1. ){
        vector<StreamLine*> sls = mesh->streamlines_for_all_t.at(cur_time);
        // for each (the beginning of) trajectory
        for( StreamLine* sl : sls ){
            // forward tracing
            {
                Vertex* seed_vert = sl->seed;
                Vector3d cords = seed_vert->cords;
                Tet* tet = seed_vert->tets[0];
                for(UI i = 0; i < max_num_steps; i++){
                    Vector3d newCords = trace_one_dist_step(cords, seed_vert->vels.at(cur_time)); // trace 1 time step
                    double ds[4]; // saving barycentric coordinates
                    Tet* newTet = mesh->inWhichTet(newCords, tet, ds); // find the corresponding tet
                    if(newTet == NULL) {
                        break; // newTet is null means we couldn't proceed
                    }
                    // interpolate vertex's vel, vor, mu at time t
                    Vertex* newVert = newTet->get_vert_at(newCords, cur_time, ds); // interpolate new cords in the tet
                    if(newVert == NULL) throwErrorMessage("build_pathlines_from_seeds: newVert is NULL!");
                    newVert->add_tet(newTet);
                    sl->fw_verts.push_back(newVert); // new vert into the streamline
                    cords = newCords; // update cords for the next iteration
                    seed_vert = newVert;
                }
            }

            // backward tracing
            {
                Vertex* vert = sl->seed;
                Tet* tet = vert->tets[0];
                for(UI i = 0; i < max_num_steps; i++){
                    Vector3d vel = Vector3d( vert->vels.at(cur_time) ) * (- 1.); // -1 means backward
                    Vector3d cords = vert->cords;
                    Vector3d newCords = trace_one_dist_step(cords, vel); // trace 1 time step
                    double ws[4]; // saving barycentric coordinates
                    Tet* newTet = mesh->inWhichTet(newCords, tet, ws); // find the corresponding tet
                    if(newTet == NULL) {
                        break; // newTet is null means we couldn't proceed
                    }
                    // interpolate vertex's vel, vor, mu at time t
                    Vertex* newVert = newTet->get_vert_at(newCords, cur_time, ws); // interpolate new cords in the tet
                    if(newVert == NULL) throwErrorMessage("build_pathlines_from_seeds: newVert is NULL!");
                    newVert->add_tet(newTet);
                    sl->bw_verts.push_back(newVert); // new vert into the streamline
                    vert = newVert;
                }
            }
        }
        cur_time += time_step_size;
    }
}


inline Vector3d trace_one_dist_step(const Vector3d& start_cords, const Vector3d& vel)
{
    Vector3d v = Vector3d(vel);
    normalize(v);
    return start_cords + v*dist_step_size;
}


inline void place_seeds(Mesh* mesh)
{
    mesh->streamlines_for_all_t.reserve(mesh->num_time_steps * frames_per_sec);
    // make sure we are using same seeds every time step
    vector<UL> seeds = generate_unique_random_Tet_idx(mesh);

    // for each time step, place streamline
    double time = 0.;
    while(time < mesh->num_time_steps - 1.)
    {
        vector<StreamLine*> sls; // create a vector of streamlines to store seeds
        sls.reserve(NUM_SEEDS); // we have num_seeds streamlines for each time step

        unsigned int cur_num_seeds = 0;
        while(cur_num_seeds < NUM_SEEDS)
        {
            // set up the streamline
            StreamLine* SL = new StreamLine();
            SL->fw_verts.reserve(max_num_steps);
            SL->bw_verts.reserve(max_num_steps);
            SL->time = (double) time;

            UL tet_idx = seeds[cur_num_seeds];
            Tet* rdm_tet = mesh->tets[tet_idx];
            const Vector3d& center_pt = rdm_tet->center;
            double ws[4];
            Vertex* center_vert = rdm_tet->get_vert_at(center_pt, (double)time, ws, true);
            SL->set_seed( center_vert );
            cur_num_seeds ++;
            sls.push_back(SL);
        }
        mesh->streamlines_for_all_t[time] = sls;

        time += time_step_size; // increment time
    }
}
