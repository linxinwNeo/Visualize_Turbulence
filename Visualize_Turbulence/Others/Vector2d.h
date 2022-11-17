#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <math.h>
#include <stdlib.h>

class Vector2d{
public:
    double entry[2];

    inline Vector2d();
    inline Vector2d(double d);
    inline Vector2d(double d0,double d1);

    inline Vector2d(const Vector2d &a);
    inline Vector2d(const double    *a);

    inline Vector2d &set(double d);
    inline Vector2d &set(double d0, double d1);

    inline Vector2d &set(const Vector2d &a);
    inline Vector2d &set(const double    *a);

    inline Vector2d &operator=(double d);
    inline Vector2d &operator=(const Vector2d &a);
    inline Vector2d &operator=(const double    *a);

    inline int operator==(const Vector2d &a) const;
    inline int operator!=(const Vector2d &a) const;

    inline int operator==(double d) const;
    inline int operator!=(double d) const;

    inline Vector2d &operator+=(double d);
    inline Vector2d &operator-=(double d);
    inline Vector2d &operator*=(double d);
    inline Vector2d &operator/=(double d);

    inline Vector2d &operator+=(const Vector2d &a);
    inline Vector2d &operator-=(const Vector2d &a);
    inline Vector2d &operator*=(const Vector2d &a);
    inline Vector2d &operator/=(const Vector2d &a);
    inline double    length(const Vector2d &a);
    inline void normalize(Vector2d &a);

    inline double      dot(const Vector2d &a,const Vector2d &b);
    inline Vector2d cross(const Vector2d &a);
};

inline Vector2d operator-(const Vector2d &a);

inline Vector2d operator+(const Vector2d &a, const Vector2d &b);
inline Vector2d operator-(const Vector2d &a, const Vector2d &b);

inline Vector2d operator+(const Vector2d &a, double b);
inline Vector2d operator-(const Vector2d &a, double b);
inline Vector2d operator*(const Vector2d &a, double b);

inline Vector2d operator+(double a, const Vector2d &b);
inline Vector2d operator-(double a, const Vector2d &b);
inline Vector2d operator*(double a, const Vector2d &b);

inline double    length(const Vector2d &a);
inline void normalize(Vector2d &a);


inline double   dot(const Vector2d &a,const Vector2d &b);
inline Vector2d cross(const Vector2d &a);

inline Vector2d::Vector2d() {
    entry[0] = entry[1] = 0.0;
}
inline Vector2d::Vector2d(double d) {
    entry[0] = entry[1] = d;
}

inline Vector2d::Vector2d(double d0,double d1) {
    entry[0] = d0;
    entry[1] = d1;
}

inline Vector2d::Vector2d(const Vector2d &a) {
    entry[0] = a.entry[0];
    entry[1] = a.entry[1];
}

inline Vector2d::Vector2d(const double *a) {
    entry[0] = a[0];
    entry[1] = a[1];
}

inline Vector2d &Vector2d::set(double d) {
    entry[0] = d;
    entry[1] = d;
    return (*this);
}

inline Vector2d &Vector2d::set(double d0, double d1) {
    entry[0] = d0;
    entry[1] = d1;
    return (*this);
}

inline Vector2d &Vector2d::set(const Vector2d &a) {
    entry[0] = a.entry[0];
    entry[1] = a.entry[1];
    return (*this);
}

inline Vector2d &Vector2d::set(const double *a) {
    entry[0] = a[0];
    entry[1] = a[1];
    return (*this);
}

inline Vector2d operator-(const Vector2d &a) {
    return Vector2d(-a.entry[0],-a.entry[1]);
}

inline Vector2d &Vector2d::operator=(double d) {
    return set(d);
}

inline Vector2d &Vector2d::operator=(const Vector2d &a) {
    return set(a);
}

inline Vector2d &Vector2d::operator=(const double *a) {
    return set(a);
}

//-------------------------------------------------------------------

inline int Vector2d::operator==(const Vector2d &a) const {
    return ((entry[0] == a.entry[0]) &&
      (entry[1] == a.entry[1]));
}

inline int Vector2d::operator!=(const Vector2d &a) const {
    return ((entry[0] != a.entry[0]) ||
      (entry[1] != a.entry[1]));
}

inline int Vector2d::operator==(double d) const {
    return ((entry[0] == d) &&
      (entry[1] == d));
}

inline int Vector2d::operator!=(double d) const {
    return ((entry[0] != d) ||
      (entry[1] != d));
}

//-------------------------------------------------------------------

inline Vector2d &Vector2d::operator+=(double d) {
    entry[0] += d;
    entry[1] += d;
    return (*this);
}

inline Vector2d &Vector2d::operator-=(double d) {
    entry[0] -= d;
    entry[1] -= d;
    return (*this);
}

inline Vector2d &Vector2d::operator*=(double d) {
    entry[0] *= d;
    entry[1] *= d;
    return (*this);
}

inline Vector2d &Vector2d::operator+=(const Vector2d &a) {
    entry[0] += a.entry[0];
    entry[1] += a.entry[1];
    return (*this);
}

inline Vector2d &Vector2d::operator-=(const Vector2d &a) {
    entry[0] -= a.entry[0];
    entry[1] -= a.entry[1];
    return (*this);
}

inline Vector2d &Vector2d::operator*=(const Vector2d &a) {
    entry[0] *= a.entry[0];
    entry[1] *= a.entry[1];
    return (*this);
}

//-------------------------------------------------------------------

inline Vector2d operator+(const Vector2d &a,const Vector2d &b) {
    return Vector2d(a.entry[0] + b.entry[0], a.entry[1] + b.entry[1]);
}

inline Vector2d operator-(const Vector2d &a,const Vector2d &b) {
    return Vector2d(a.entry[0] - b.entry[0], a.entry[1] - b.entry[1]);
}

inline Vector2d operator+(const Vector2d &a,double b){
    return Vector2d(a.entry[0] + b, a.entry[1] + b);
}

inline Vector2d operator-(const Vector2d &a,double b){
    return Vector2d(a.entry[0] - b, a.entry[1] - b);
}

inline Vector2d operator*(const Vector2d &a,double b){
    return Vector2d(a.entry[0] * b, a.entry[1] * b);
}

inline Vector2d operator+(double a,const Vector2d &b){
    return Vector2d(a + b.entry[0], a + b.entry[1]);
}

inline Vector2d operator-(double a,const Vector2d &b){
    return Vector2d(a - b.entry[0], a - b.entry[1]);
}

inline Vector2d operator*(double a,const Vector2d &b){
    return Vector2d(a * b.entry[0], a * b.entry[1]);
}

inline double length(const Vector2d &a) {
    return sqrt(a.entry[0] * a.entry[0] + a.entry[1] * a.entry[1]);
}

inline void normalize(Vector2d &a) {
    double m = length(a);
    if (m != 0) a *= (1/m);
}

inline double dot(const Vector2d &a,const Vector2d &b) {
    return (a.entry[0] * b.entry[0] + a.entry[1] * b.entry[1]);
}

inline Vector2d cross(const Vector2d &a) {
    return Vector2d(-a.entry[1], a.entry[0]);
}


#endif // VECTOR2D_H
