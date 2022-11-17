#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <math.h>
#include <stdlib.h>

class Vector3d{
public:
    double entry[3];

    inline Vector3d();
    inline Vector3d(const double d);
    inline Vector3d(const double d0,const double d1,const double d2);

    inline Vector3d(const Vector3d &a);
    inline Vector3d(const double    *a);

    inline Vector3d &set(double d);
    inline Vector3d &set(double d0, double d1,double d2);

    inline Vector3d &set(const Vector3d &a);
    inline Vector3d &set(const double    *a);

    inline Vector3d &operator=(double d);
    inline Vector3d &operator=(const Vector3d &a);
    inline Vector3d &operator=(const double    *a);

    inline int operator==(const Vector3d &a) const;
    inline int operator!=(const Vector3d &a) const;

    inline int operator==(double d) const;
    inline int operator!=(double d) const;

    inline Vector3d &operator+=(double d);
    inline Vector3d &operator-=(double d);
    inline Vector3d &operator*=(double d);
    inline Vector3d &operator/=(double d);

    inline Vector3d &operator+=(const Vector3d &a);
    inline Vector3d &operator-=(const Vector3d &a);
    inline Vector3d &operator*=(const Vector3d &a);
    inline Vector3d &operator/=(const Vector3d &a);
    inline double    length(const Vector3d &a);
    inline void normalize(Vector3d &a);

    inline double      dot(const Vector3d &a,const Vector3d &b);
    inline Vector3d cross(const Vector3d &a);
};

inline Vector3d operator-(const Vector3d &a);

inline Vector3d operator+(const Vector3d &a, const Vector3d &b);
inline Vector3d operator-(const Vector3d &a, const Vector3d &b);

inline Vector3d operator+(const Vector3d &a, double b);
inline Vector3d operator-(const Vector3d &a, double b);
inline Vector3d operator*(const Vector3d &a, double b);

inline Vector3d operator+(double a, const Vector3d &b);
inline Vector3d operator-(double a, const Vector3d &b);
inline Vector3d operator*(double a, const Vector3d &b);

inline double    length(const Vector3d &a);
inline void normalize(Vector3d &a);


inline double   dot(const Vector3d &a,const Vector3d &b);
inline Vector3d cross(const Vector3d &a, const Vector3d &b);

inline Vector3d::Vector3d() {
    entry[0] = entry[1] = entry[2] = 0.0;
}


inline Vector3d::Vector3d(const double d) {
    entry[0] = entry[1] = entry[2] = d;
}


inline Vector3d::Vector3d(const double d0,const double d1, const double d2) {
    entry[0] = d0;
    entry[1] = d1;
    entry[2] = d2;
}


inline Vector3d::Vector3d(const Vector3d &a) {
    entry[0] = a.entry[0];
    entry[1] = a.entry[1];
    entry[2] = a.entry[2];
}


inline Vector3d::Vector3d(const double *a) {
    entry[0] = a[0];
    entry[1] = a[1];
    entry[1] = a[2];
}

//-------------------------------------------------------------------

inline Vector3d &Vector3d::set(double d) {
    entry[0] = d;
    entry[1] = d;
    entry[2] = d;
    return (*this);
}

inline Vector3d &Vector3d::set(double d0, double d1, double d2) {
    entry[0] = d0;
    entry[1] = d1;
    entry[2] = d2;
    return (*this);
}

inline Vector3d &Vector3d::set(const Vector3d &a) {
    entry[0] = a.entry[0];
    entry[1] = a.entry[1];
    entry[2] = a.entry[2];
    return (*this);
}

inline Vector3d &Vector3d::set(const double *a) {
    entry[0] = a[0];
    entry[1] = a[1];
    entry[2] = a[2];
    return (*this);
}

inline Vector3d operator-(const Vector3d &a) {
    return Vector3d(-a.entry[0],-a.entry[1],-a.entry[2]);
}

inline Vector3d &Vector3d::operator=(double d) {
    return set(d);
}

inline Vector3d &Vector3d::operator=(const Vector3d &a) {
    return set(a);
}

inline Vector3d &Vector3d::operator=(const double *a) {
    return set(a);
}

//-------------------------------------------------------------------

inline int Vector3d::operator==(const Vector3d &a) const {
    return ((entry[0] == a.entry[0]) &&
        (entry[1] == a.entry[1]) &&
        (entry[2] == a.entry[2]));
}

inline int Vector3d::operator!=(const Vector3d &a) const {
    return ((entry[0] != a.entry[0]) ||
        (entry[1] != a.entry[1]) ||
        (entry[2] != a.entry[2]));
}

inline int Vector3d::operator==(double d) const {
    return ((entry[0] == d) &&
        (entry[1] == d) &&
        (entry[2] == d));
}

inline int Vector3d::operator!=(double d) const {
    return ((entry[0] != d) ||
        (entry[1] != d) ||
         (entry[2] != d));
}

//-------------------------------------------------------------------

inline Vector3d &Vector3d::operator+=(double d) {
    entry[0] += d;
    entry[1] += d;
    entry[2] += d;
    return (*this);
}

inline Vector3d &Vector3d::operator-=(double d) {
    entry[0] -= d;
    entry[1] -= d;
    entry[2] -= d;
    return (*this);
}

inline Vector3d &Vector3d::operator*=(double d) {
    entry[0] *= d;
    entry[1] *= d;
    entry[2] *= d;
    return (*this);
}

inline Vector3d &Vector3d::operator+=(const Vector3d &a) {
    entry[0] += a.entry[0];
    entry[1] += a.entry[1];
    entry[2] += a.entry[2];
    return (*this);
}

inline Vector3d &Vector3d::operator-=(const Vector3d &a) {
    entry[0] -= a.entry[0];
    entry[1] -= a.entry[1];
    entry[2] -= a.entry[2];
    return (*this);
}

inline Vector3d &Vector3d::operator*=(const Vector3d &a) {
    entry[0] *= a.entry[0];
    entry[1] *= a.entry[1];
    entry[2] *= a.entry[2];
    return (*this);
}

//-------------------------------------------------------------------

inline Vector3d operator+(const Vector3d &a,const Vector3d &b) {
    return Vector3d(a.entry[0] + b.entry[0], a.entry[1] + b.entry[1], a.entry[2] + b.entry[2]);
}

inline Vector3d operator-(const Vector3d &a,const Vector3d &b) {
    return Vector3d(a.entry[0] - b.entry[0], a.entry[1] - b.entry[1], a.entry[2] - b.entry[2]);
}

inline Vector3d operator+(const Vector3d &a,double b){
    return Vector3d(a.entry[0] + b, a.entry[1] + b, a.entry[2] + b);
}

inline Vector3d operator-(const Vector3d &a,double b){
    return Vector3d(a.entry[0] - b, a.entry[1] - b, a.entry[2] - b);
}

inline Vector3d operator*(const Vector3d &a,double b){
    return Vector3d(a.entry[0] * b, a.entry[1] * b, a.entry[2] * b);
}

inline Vector3d operator+(double a,const Vector3d &b){
    return Vector3d(a + b.entry[0], a + b.entry[1], a + b.entry[2]);
}

inline Vector3d operator-(double a,const Vector3d &b){
    return Vector3d(a - b.entry[0], a - b.entry[1], a - b.entry[2]);
}

inline Vector3d operator*(double a,const Vector3d &b){
    return Vector3d(a * b.entry[0], a * b.entry[1], a * b.entry[2]);
}

inline double length(const Vector3d &a) {
    return sqrt(a.entry[0] * a.entry[0] + a.entry[1] * a.entry[1] + a.entry[2] * a.entry[2]);
}

inline void normalize(Vector3d &a) {
    double m = length(a);
    if (m != 0) a *= (1/m);
}

inline double dot(const Vector3d &a,const Vector3d &b) {
    return (a.entry[0] * b.entry[0] + a.entry[1] * b.entry[1] + a.entry[2] * b.entry[2]);
}

inline Vector3d cross(const Vector3d &a, const Vector3d &b) {
    return Vector3d(a.entry[1] * b.entry[2] - a.entry[2] * b.entry[1],
                           a.entry[2] * b.entry[0] - a.entry[0] * b.entry[2],
                           a.entry[0] * b.entry[1] - a.entry[1] * b.entry[0]);
}

#endif // VECTOR3D_H
