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


// using linear interpolation to find the
Vector3d Vertex::linear_interpolate_vel(const int t1, const int t2, const double target_t)
{
    if(this->has_vel_at_t(target_t)) return this->vels.at(target_t);


}


Vector3d Vertex::linear_interpolate_vor(const int t1, const int t2, const double target_t)
{
    if(this->has_vor_at_t(target_t)) return this->vors.at(target_t);


}


double Vertex::linear_interpolate_mu(const int t1, const int t2, const double target_t)
{
    if(this->has_mu_at_t(target_t)) return this->mus.at(target_t);


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
