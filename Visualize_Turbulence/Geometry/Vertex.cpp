#include "Geometry/Vertex.h"

// destructor
Vertex::~Vertex()
{
    this->edges.clear();
    this->faces.clear();
    this->edges.clear();

    for(const auto& v : this->vels){
        if(v.second == NULL) continue;
        delete v.second;
    }
    this->vels.clear();

    for(const auto& v : this->vors){
        if(v.second == NULL) continue;
        delete v.second;
    }

    this->vors.clear();
}


// set vel at time t
void Vertex::set_vel(const double time, Vector3d *vel_ptr)
{
    if(has_vel_at_t(time)) {
        qDebug() << "trying to add vel at time " << time << ", which already exists";
        return;
    }
    if(vel_ptr == NULL) return;

    this->vels[time] = vel_ptr;
}


// set vel at time t
void Vertex::set_vel(const double time, const double vx, const double vy, const double vz)
{
    if(has_vel_at_t(time)) {
        qDebug() << "trying to add vel at time " << time << ", which already exists";
        return;
    }

    Vector3d* new_vel = new Vector3d(vx, vy, vz);
    this->set_vel(time, new_vel);
}


// set vor at time t
void Vertex::set_vor(const double time, Vector3d *vor_ptr)
{
    if(has_vor_at_t(time)) {
        qDebug() << "trying to add vor at time " << time << ", which already exists";
        return;
    }

    if(vor_ptr == NULL) return;
    this->vors[time] = vor_ptr;
}


// set vor at time t
void Vertex::set_vor(const double time, const double vx, const double vy, const double vz)
{
    if(has_vor_at_t(time)) {
        qDebug() << "trying to add vor at time " << time << ", which already exists";
        return;
    }

    Vector3d* new_vor = new Vector3d(vx, vy, vz);
    this->set_vor(time, new_vor);
}


// set mu at time t
void Vertex::set_mu(const double time, const double mu)
{
    if(has_mu_at_t(time)) {
        qDebug() << "trying to add mu at time " << time << ", which already exists";
        return;
    }
    this->mus[time] = mu;
}


// using linear interpolation to find the state of the vertex at target_t
Vector3d* Vertex::linear_interpolate_vel(const double target_t)
{
    int t1 = floor(target_t);
    int t2 = ceil(target_t);

    if(target_t < t1 || target_t > t2 || t1 > t2) return NULL;
    if(target_t == t1) {
        if ( this->has_vel_at_t(t1) ) return this->vels.at(t1);
        else return NULL;
    }

    if(target_t == t2) {
        if ( this->has_vel_at_t(t2) ) return this->vels.at(t2);
        else return NULL;
    }

    if(this->has_vel_at_t(target_t)) return this->vels.at(target_t); // return the velocity vector if exists

    // at this point we know that the target_t is never calculated and is somewhere between t1 and t2, exclusively.
    Vector3d v1 = Vector3d(vels.at(t1));
    Vector3d v2 = Vector3d(vels.at(t2));
    Vector3d dv = v2 - v1;
    double dt = t2 - t1; // dt will never be 0 because t1 != t2
    // calculate rate of change in x direction
    double dx_dt = dv.x() / dt;
    // calculate rate of change in y direction
    double dy_dt = dv.y() / dt;
    // calculate rate of change in z direction
    double dz_dt = dv.z() / dt;
    // here we need to create a new velcoity vector by linear interpolation
    Vector3d* v = new Vector3d(v1);
    double dt2 = target_t - (double) t1;
    v->entry[0] = v->x() + dx_dt * dt2;
    v->entry[1] = v->y() + dy_dt * dt2;
    v->entry[2] = v->z() + dz_dt * dt2;

    this->set_vel(target_t, v);
    return v;
}


Vector3d* Vertex::linear_interpolate_vor(const double target_t)
{
    int t1 = floor(target_t);
    int t2 = ceil(target_t);

    if(target_t < t1 || target_t > t2 || t1 > t2) return NULL;
    if(target_t == t1) return this->vors.at(t1);
    if(target_t == t2) return this->vors.at(t2);
    if(this->has_vor_at_t(target_t)) return this->vors.at(target_t); // return the vorticity vector if exists

    // at this point we know that the target_t is never calculated and is somewhere between t1 and t2, exclusively.
    Vector3d v1 = Vector3d(vors.at(t1));
    Vector3d v2 = Vector3d(vors.at(t2));
    Vector3d dv = v2 - v1;
    double dt = t2 - t1; // dt will never be 0 because t1 != t2
    // calculate rate of change in x direction
    double dx_dt = dv.x() / dt;
    // calculate rate of change in y direction
    double dy_dt = dv.y() / dt;
    // calculate rate of change in z direction
    double dz_dt = dv.z() / dt;
    // here we need to create a new vorticity vector by linear interpolation
    Vector3d* v = new Vector3d(v1);
    double dt2 = target_t - (double) t1;
    v->entry[0] = v->x() + dx_dt * dt2;
    v->entry[1] = v->y() + dy_dt * dt2;
    v->entry[2] = v->z() + dz_dt * dt2;

    this->set_vor(target_t, v);
    return v;
}


double Vertex::linear_interpolate_mu(const double target_t)
{
    int t1 = floor(target_t);
    int t2 = ceil(target_t);

    if(target_t < t1 || target_t > t2 || t1 > t2) return 0.0;
    if(target_t == t1) return this->mus.at(t1);
    if(target_t == t2) return this->mus.at(t2);
    if(this->has_mu_at_t(target_t)) return this->mus.at(target_t); // return the velocity vector if exist

    // at this point we know that the target_t is never calculated and is somewhere between t1 and t2, exclusively.
    double mu1 = mus.at(t1);
    double mu2 = mus.at(t2);
    double dmu = mu2 - mu1;
    double dt = t2 - t1; // dt will never be 0 because t1 != t2
    // calculate rate of change
    double dmu_dt = dmu / dt;

    double dt2 = target_t - (double) t1; // difference between target_t and t1

    double target_mu = mu1 + dmu_dt * dt2;

    this->set_mu(target_t, target_mu);
    return target_mu;
}


QString Vertex::vel_str( const double time ) const
{
    if(vels.find(time) == vels.end()) return "time does not exist!";

    Vector3d* vel = vels.at(time);
    QString str = QString( "%1, %2, %3" ).arg(vel->entry[0]).arg(vel->entry[1]).arg(vel->entry[2]);
    return str;
}


QString Vertex::vor_str( const double time ) const
{
    if(vors.find(time) == vors.end()) return "time does not exist!";

    Vector3d* vor = vors.at(time);
    QString str = QString( "%1, %2, %3" ).arg(vor->entry[0]).arg(vor->entry[1]).arg(vor->entry[2]);
    return str;
}

