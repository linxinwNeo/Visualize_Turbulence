#ifndef MATRIX2X2_H
#define MATRIX2X2_H

#include "Others/Vector2d.h"

class Matrix2x2 {
public:
    inline Matrix2x2();
    inline Matrix2x2(double x);
    inline Matrix2x2(const Matrix2x2 &that);

    inline Matrix2x2(double M00, double M01,
             double M10, double M11);
    inline Matrix2x2(double M[2][2]);

    inline Matrix2x2 &set      (const double d);
    inline Matrix2x2 &operator=(const double d);

    inline Matrix2x2 &set      (const Matrix2x2 &that);
    inline Matrix2x2 &operator=(const Matrix2x2 &that);

    inline Matrix2x2 &set			 (double M[2][2]);
    inline Matrix2x2 &operator=(double M[2][2]);

    inline int operator!=(const Matrix2x2 &that)const;
    inline int operator==(const Matrix2x2 &that)const;

    inline int operator==(double d) const;
    inline int operator!=(double d) const;

    inline Matrix2x2 &operator+=(double d);
    inline Matrix2x2 &operator-=(double d);
    inline Matrix2x2 &operator*=(double d);

    // component-wise operations.
    inline Matrix2x2 &operator+=(const Matrix2x2 &that);
    inline Matrix2x2 &operator-=(const Matrix2x2 &that);
    inline Matrix2x2 &operator*=(const Matrix2x2 &that);

      // Left : this = that x this
      // Right: this = this x that
      Matrix2x2 &leftMultiply (const Matrix2x2 &that);
      Matrix2x2 &rightMultiply(const Matrix2x2 &that);

      inline Matrix2x2 &setIdentity     ();

public:
    double entry[2][2];

};

inline Matrix2x2 operator+(const Matrix2x2 &a, double b);
inline Matrix2x2 operator-(const Matrix2x2 &a, double b);
inline Matrix2x2 operator*(const Matrix2x2 &a, double b);

inline Matrix2x2 operator+(const Matrix2x2 &a, const Matrix2x2 &b);
inline Matrix2x2 operator-(const Matrix2x2 &a, const Matrix2x2 &b);
inline Matrix2x2 operator*(const Matrix2x2 &a, const Matrix2x2 &b);

inline Matrix2x2 multiply(const Matrix2x2 &a, const Matrix2x2 &b);
inline Vector2d   operator*(const Matrix2x2 &a, const Vector2d   &b);
inline Vector2d   operator*(const Vector2d   &a, const Matrix2x2 &b);

inline double determinant(const Matrix2x2 &a);

inline Matrix2x2 transpose(const Matrix2x2 &a);
inline Matrix2x2   inverse(const Matrix2x2 &a);

inline Matrix2x2::Matrix2x2() {
    entry[0][0] = 1;
    entry[0][1] = 0;
    entry[1][0] = 0;
    entry[1][1] = 1;
}

inline Matrix2x2::Matrix2x2(double x) {
    entry[0][0] = x;
    entry[0][1] = x;
    entry[1][0] = x;
    entry[1][1] = x;
}

inline Matrix2x2::Matrix2x2(double M00, double M01,
                double M10, double M11) {
    entry[0][0] = M00;
    entry[0][1] = M01;
    entry[1][0] = M10;
    entry[1][1] = M11;
};

inline Matrix2x2::Matrix2x2(const Matrix2x2 &that) {
    entry[0][0] = that.entry[0][0];
    entry[0][1] = that.entry[0][1];
    entry[1][0] = that.entry[1][0];
    entry[1][1] = that.entry[1][1];
};

inline Matrix2x2 &Matrix2x2::set(const double d) {
    return (*this)=d;
}

inline Matrix2x2 &Matrix2x2::operator=(const double d) {
    entry[0][0] = d;
    entry[0][1] = d;

    entry[1][0] = d;
    entry[1][1] = d;
    return (*this);
};

inline Matrix2x2 &Matrix2x2::set(const Matrix2x2 &that) {
    return (*this)=that;
}

inline Matrix2x2 &Matrix2x2::operator=(const Matrix2x2 &that) {
    entry[0][0] = that.entry[0][0];
    entry[0][1] = that.entry[0][1];

    entry[1][0] = that.entry[1][0];
    entry[1][1] = that.entry[1][1];
    return (*this);
};

inline Matrix2x2 &Matrix2x2::set(double M[2][2]) {
    return (*this)=M;
}

inline Matrix2x2 &Matrix2x2::operator=(double M[2][2]) {
    entry[0][0] = M[0][0];
    entry[0][1] = M[0][1];

    entry[1][0] = M[1][0];
    entry[1][1] = M[1][1];
    return (*this);
};

inline int Matrix2x2::operator==(double d) const {
    return  ( (entry[0][0] == d) &&
        (entry[0][1] == d) &&
        (entry[1][0] == d) &&
        (entry[1][1] == d) );
}

inline int Matrix2x2::operator!=(double d) const {
    return  ( (entry[0][0] != d) ||
        (entry[0][1] != d) ||
        (entry[1][0] != d) ||
        (entry[1][1] != d) );
}

inline int Matrix2x2::operator==(const Matrix2x2 &that)const {
    return ( (entry[0][0] == that.entry[0][0]) &&
       (entry[0][1] == that.entry[0][1]) &&
       (entry[1][0] == that.entry[1][0]) &&
       (entry[1][1] == that.entry[1][1]) );
}

inline int Matrix2x2::operator!=(const Matrix2x2 &that)const {
    return ( (entry[0][0] != that.entry[0][0]) ||
       (entry[0][1] != that.entry[0][1]) ||
       (entry[1][0] != that.entry[1][0]) ||
       (entry[1][1] != that.entry[1][1]) );
}

inline Matrix2x2 &Matrix2x2::operator+=(double d) {
    entry[0][0] += d; entry[1][0] += d;
    entry[0][1] += d; entry[1][1] += d;
    return (*this);
}

inline Matrix2x2 &Matrix2x2::operator-=(double d) {
    entry[0][0] -= d; entry[1][0] -= d;
    entry[0][1] -= d; entry[1][1] -= d;
    return (*this);
}

inline Matrix2x2 &Matrix2x2::operator*=(double d) {
    entry[0][0] *= d; entry[1][0] *= d;
    entry[0][1] *= d; entry[1][1] *= d;
    return (*this);
}

inline Matrix2x2 &Matrix2x2::operator+=(const Matrix2x2 &that) {
    entry[0][0] += that.entry[0][0]; entry[1][0] += that.entry[1][0];
    entry[0][1] += that.entry[0][1]; entry[1][1] += that.entry[1][1];
    return (*this);
}

inline Matrix2x2 &Matrix2x2::operator-=(const Matrix2x2 &that) {
    entry[0][0] -= that.entry[0][0]; entry[1][0] -= that.entry[1][0];
    entry[0][1] -= that.entry[0][1]; entry[1][1] -= that.entry[1][1];
    return (*this);
}

inline Matrix2x2 &Matrix2x2::operator*=(const Matrix2x2 &that) {
    entry[0][0] *= that.entry[0][0]; entry[1][0] *= that.entry[1][0];
    entry[0][1] *= that.entry[0][1]; entry[1][1] *= that.entry[1][1];
    return (*this);
}

inline Matrix2x2 &Matrix2x2::leftMultiply (const Matrix2x2 &that){
    Matrix2x2 tmp(entry[0][0], entry[0][1], entry[1][0], entry[1][1]);

    entry[0][0] = that.entry[0][0] * tmp.entry[0][0] + that.entry[0][1] * tmp.entry[1][0];
    entry[0][1] = that.entry[0][0] * tmp.entry[0][1] + that.entry[0][1] * tmp.entry[1][1];
    entry[1][0] = that.entry[1][0] * tmp.entry[0][0] + that.entry[1][1] * tmp.entry[1][0];
    entry[1][1] = that.entry[1][0] * tmp.entry[0][1] + that.entry[1][1] * tmp.entry[1][1];
    return (*this);
};

inline Matrix2x2 &Matrix2x2::rightMultiply(const Matrix2x2 &that){
    Matrix2x2 tmp(entry[0][0], entry[0][1], entry[1][0], entry[1][1]);

    entry[0][0] = tmp.entry[0][0] * that.entry[0][0] + tmp.entry[0][1] * that.entry[1][0];
    entry[0][1] = tmp.entry[0][0] * that.entry[0][1] + tmp.entry[0][1] * that.entry[1][1];
    entry[1][0] = tmp.entry[1][0] * that.entry[0][0] + tmp.entry[1][1] * that.entry[1][0];
    entry[1][1] = tmp.entry[1][0] * that.entry[0][1] + tmp.entry[1][1] * that.entry[1][1];
    return (*this);
};

inline Matrix2x2 &Matrix2x2::setIdentity() {
    entry[0][0] = 1; entry[0][1] = 0;
    entry[1][0] = 0; entry[1][1] = 1;
    return (*this);
};

inline Matrix2x2 operator+(const Matrix2x2 &a,double b) {
    return (Matrix2x2(a)+=b);
}

inline Matrix2x2 operator-(const Matrix2x2 &a,double b) {
    return (Matrix2x2(a)-=b);
}

inline Matrix2x2 operator*(const Matrix2x2 &a,double b) {
    return (Matrix2x2(a)*=b);
}

inline Matrix2x2 operator+(double a, const Matrix2x2 &b) {
    return b+a;
}

inline Matrix2x2 operator-(double a, const Matrix2x2 &b) {
    return Matrix2x2(a-b.entry[0][0],a-b.entry[0][1],
             a-b.entry[1][0],a-b.entry[1][1]);
}

inline Matrix2x2 operator*(double a, const Matrix2x2 &b) {
    return b*a;
}

inline Matrix2x2 operator+(const Matrix2x2 &a,const Matrix2x2 &b) {
    return (Matrix2x2(a)+=b);
}

inline Matrix2x2 operator-(const Matrix2x2 &a,const Matrix2x2 &b) {
    return (Matrix2x2(a)-=b);
}

inline Matrix2x2 operator*(const Matrix2x2 &a,const Matrix2x2 &b) {
    return (Matrix2x2(a)*=b);
}

inline Matrix2x2 multiply(const Matrix2x2 &a,const Matrix2x2 &b) {
    Matrix2x2 tmp(a);
    tmp.rightMultiply(b);
    return tmp;
}

inline Vector2d operator*(const Matrix2x2 &a,const Vector2d &b) {
    return Vector2d(b.entry[0]*a.entry[0][0] + b.entry[1]*a.entry[0][1],
           b.entry[0]*a.entry[1][0] + b.entry[1]*a.entry[1][1]);
}

inline Vector2d operator*(const Vector2d &a,const Matrix2x2 &b) {
    return Vector2d(a.entry[0]*b.entry[0][0] + a.entry[1]*b.entry[1][0],
           a.entry[0]*b.entry[0][1] + a.entry[1]*b.entry[1][1]);
}

inline double determinant(const Matrix2x2 &a) {
    return ( a.entry[0][0] * a.entry[1][1] - a.entry[0][1] * a.entry[1][0] );
}

inline Matrix2x2 transpose(const Matrix2x2 &a) {
    Matrix2x2 tmp(a);

    tmp.entry[0][1] = a.entry[1][0];
    tmp.entry[1][0] = a.entry[0][1];
    return tmp;
}

inline Matrix2x2 inverse(const Matrix2x2 &a) {
    Matrix2x2 tmp;
    double dmt;

    if ((dmt=determinant(a))!= 0.0) {
        tmp.entry[0][0] = a.entry[1][1]/dmt;
        tmp.entry[0][1] = -a.entry[0][1]/dmt;
        tmp.entry[1][0] = -a.entry[1][0]/dmt;
        tmp.entry[1][1] = a.entry[0][0]/dmt;
    }
    return tmp;
}

#endif // MATRIX2X2_H
