#ifndef SINGULARITY_H
#define SINGULARITY_H

#include "Geometry/Vertex.h"
#include "Others/Predefined.h"
#include "Others/Vector3d.h"
#include "Eigen/Dense"
#include <iostream>


#define SOURCE 1;
#define SINK 2;
#define REP_SADD_NODE 3;
#define ATT_SADD_NODE 4;
#define REP_FOC_SOURCE 5;
#define ATT_FOC_SINK 6;
#define REP_SADD_FOCUS 7;
#define ATT_SADD_FOCUS 8;

/*
 * souce = 1
 * sink = 2
 * repelling saddle node = 3
 * attracting saddle node = 4
 * repelling focus source= 5
 * attracting focus sink = 6
 * repelling saddle focus = 7
 * attracting saddle focus = 8
*/

class Singularity
{
public:
    Vector3d cords;
    Tet* in_which_tet;
    unsigned short type;

    Eigen::Matrix3d Jacobian;

    Singularity();

    double x() const;
    double y() const;
    double z() const;
    void classify_this();
    QString get_type() const;
};

inline Singularity::Singularity()
{
    this->in_which_tet = NULL;
    this->type = 0;
}

inline double Singularity::x() const
{
    return this->cords.x();
}

inline double Singularity::y() const
{
    return this->cords.y();
}

inline double Singularity::z() const
{
    return this->cords.z();
}


inline void sort_asecding(vector<double>& reals, vector<double>& imags)
{
    for(UI i = 0; i < reals.size(); i++){
        for(UI j = i+1; j < reals.size(); j++){
            if(reals[i] > reals[j]){
                double temp = reals[j];
                reals[j] = reals[i];
                reals[i] = temp;

                temp = imags[j];
                imags[j] = imags[i];
                imags[i] = temp;
            }
        }
    }
}


// return 1 if its focus, return 2 if its node, return 0 otherwise
inline unsigned short is_Node(const vector<double> imags){
    if(imags[0] == 0 && imags[1] == 0 && imags[2] == 0) return 2;

    long int v1, v2, v3; v1=v2=v3=100;
    for(UI i = 0; i < 3; i++){
        if(imags[i] == 0) v1 = i;
    }

    if(v1 == 100) {
        qDebug() << "couldn't find imag value of 0.";
        return 0;
    }

    for(int i = 0; i < 3; i++){
        for(int j = i+1; j < 3; j++){
            if(imags[i] == - imags[j] && imags[i] !=0)
                return 1;
        }
    }

    return 0;
}

inline void Singularity::classify_this()
{
    Eigen::EigenSolver<Eigen::Matrix3d> ces(this->Jacobian);
    if (ces.info() != Eigen::Success) {
        qDebug() << "fail";
    }

    const Eigen::Matrix<complex<double>, 3, 1> eigenvalues = ces.eigenvalues();

    const complex<double> eigenvalue1 = eigenvalues(0, 0);
    const complex<double> eigenvalue2 = eigenvalues(1, 0);
    const complex<double> eigenvalue3 = eigenvalues(2, 0);
    cout<< this->Jacobian <<endl;
    vector<double> realEigenValues = {eigenvalue1.real(), eigenvalue2.real(), eigenvalue3.real()};
    vector<double> imagEigenValues = {eigenvalue1.imag(), eigenvalue2.imag(), eigenvalue3.imag()};
    sort_asecding(realEigenValues, imagEigenValues);
    cout << eigenvalues <<endl;

    unsigned short value = is_Node(imagEigenValues);
    // check if it is a source, repelling saddle, attracting saddle or sink
    if(realEigenValues[0] > 0 && realEigenValues[1] >= realEigenValues[0]){ // source
        if(value == 1){
            this->type = REP_FOC_SOURCE;
        }
        else if(value == 2){
            this->type = SOURCE;
        }
        else{
            this->type = 0;
        }
    }
    else if(realEigenValues[0] < 0 && realEigenValues[1] > 0){ // rep saddle
        if(value == 1){
            this->type = REP_SADD_FOCUS;
        }
        else if(value == 2){
            this->type = REP_SADD_NODE;
        }
        else{
            this->type = 0;
        }
    }
    else if(realEigenValues[2] > 0 && realEigenValues[1] < 0){ // att saddle
        if(value == 1){
            this->type = ATT_SADD_FOCUS;
        }
        else if(value == 2){
            this->type = ATT_SADD_NODE;
        }
        else{
            this->type = 0;
        }
    }
    else if(realEigenValues[2] < 0){ // sink
        if(value == 1){
            this->type = ATT_FOC_SINK;
        }
        else if(value == 2){
            this->type = SINK;
        }
        else{
            this->type = 0;
        }
    }
    else{
        this->type = 0;
    }
}

inline QString Singularity::get_type() const
{
    switch(type){
    case 1: return "SOURCE"; break;
    case 2: return "SINK"; break;
    case 3: return "REPELLING SADDLE"; break;
    case 4: return "ATTRACTING SADDLE"; break;
    case 5: return "REPELLING FOCUS SOURCE"; break;
    case 6: return "ATTRACTING FOCUS SINK"; break;
    case 7: return "REPELLING SADDLE FOCUS"; break;
    case 8: return "ATTRACTING SADDLE FOCUS"; break;
    default: return "ERROR"; break;
    }
}


#endif
