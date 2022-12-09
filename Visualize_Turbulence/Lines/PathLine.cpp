#include "Lines/PathLine.h"
#include "Others/Predefined.h"
#include "Others/Utilities.h"

PathLine::PathLine()
{

}

PathLine::PathLine(vector<Vertex*> vs)
{
    this->verts.reserve(vs.size());

    for(auto& v : vs){
        this->verts.push_back(v);
    }
}

PathLine::~PathLine()
{
    for(UL i = 0; i < this->num_verts(); i++){
        if(this->verts[i] != NULL) delete this->verts[i];
    }

    this->verts.clear();
}



// we try to place seeds randomly in the domain
// for a random tet, if we choose to place a seed,
// a seed will be generated in the center of that tet
// we set number of seeds to be generated
void place_seeds(vector<PathLine*>& pls)
{
    srand((unsigned) time(NULL));

    set<UL> seeded_tets;
    unsigned int cur_num_seeds = 0;

    pls.reserve(NUM_SEEDS);

    while(cur_num_seeds < NUM_SEEDS)
    {
        int random_idx = rand() % mesh->num_tets();
        if( seeded_tets.find(random_idx) != seeded_tets.end() ) continue;
        else{
            seeded_tets.insert(random_idx);
            Tet* tet = mesh->tets[random_idx];
            PathLine* PL = new PathLine();
            const Vector3d& seed = tet->center;
            double ws[4];
            Vertex* seed_v = tet->get_vert_at(seed, 0., ws, true); // interpolate this seed at time 0
            PL->verts.push_back( seed_v );
            pls.push_back(PL);
            cur_num_seeds ++;
        }
    }
}


// now every pathline has a seed point, we want to calculate their path individually
void build_pathlines_from_seeds(){
    // for each pathline, we calculate their trajectories
    for( PathLine* pathline : pathlines ){
        Vertex* seed_vert = pathline->verts[0];
        Vector3d cords = seed_vert->cords;
        Tet* tet = seed_vert->tets[0];

        /* tracing the seed n times
         *      trace 1 step, calculate the new cords
         *      find the corresponding tet
         *      interpolate new cords in the tet
         *      add new vert into the pathline
        */
        double cur_time = 0;
        for(UI i = 0; i < max_num_steps && cur_time < mesh->num_time_steps - 1 - time_step_size; i++){
            Vector3d newCords = trace_one_time_step(cords, seed_vert->vels.at(cur_time)); // trace 1 time step
            double ws[4];
            Tet* newTet = mesh->inWhichTet(newCords, tet, ws); // find the corresponding tet
            if(newTet == NULL) break; // newTet is null means we couldn't proceed
            cur_time += time_step_size;

            // interpolate vertex's vel, vor, mu at time t
            Vertex* newVert = newTet->get_vert_at(newCords, cur_time, ws); // interpolate new cords in the tet
            if(newVert == NULL) throwErrorMessage("build_pathlines_from_seeds: newVert is NULL!");
            newVert->add_tet(newTet);
            pathline->verts.push_back(newVert); // new vert into the pathline
            cords = newCords; // update cords for the next iteration
            seed_vert = newVert;
        }
    }

    qDebug() << "num of pathlines: " << pathlines.size();
}


// tracing the start_cords one time step
// vel is the velocity vector
// return the resultant cords
Vector3d trace_one_time_step(const Vector3d& start_cords, const Vector3d& vel)
{
    return start_cords + vel*time_step_size;
}
