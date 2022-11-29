#ifndef MATRIX3X3_H
#define MATRIX3X3_H

#include "Others/Vector2d.h"
#include "Others/Vector3d.h"


// start for class Matrix3x3
class Matrix3x3 {
public:
    inline Matrix3x3();
    inline Matrix3x3(double x);
    inline Matrix3x3(const Matrix3x3 &that);
    inline Matrix3x3(const Vector2d &v1, const Vector2d &v2, const Vector2d &v3);

    inline Matrix3x3(double M00, double M01, double M02,
                                         double M10, double M11, double M12,
                                         double M20, double M21, double M22);
    inline Matrix3x3(double M[3][3]);

    inline Matrix3x3 &set      (const double d);
    inline Matrix3x3 &operator=(const double d);

    inline Matrix3x3 &set      (const Matrix3x3 &that);
    inline Matrix3x3 &operator=(const Matrix3x3 &that);

    inline Matrix3x3 &set			 (double M[3][3]);
    inline Matrix3x3 &operator=(double M[3][3]);

    inline Matrix3x3 &set     (const Vector2d &v1, const Vector2d &v2, const Vector2d &v3);
    inline Matrix3x3 &set			(double M00, double M01, double M02,
                                                     double M10, double M11, double M12,
                                                             double M20, double M21, double M22);
    inline int operator!=(const Matrix3x3 &that)const;
    inline int operator==(const Matrix3x3 &that)const;

    inline int operator==(double d) const;
    inline int operator!=(double d) const;

    inline Matrix3x3 &operator+=(double d);
    inline Matrix3x3 &operator-=(double d);
    inline Matrix3x3 &operator*=(double d);

    // component-wise operations.
    inline Matrix3x3 &operator+=(const Matrix3x3 &that);
    inline Matrix3x3 &operator-=(const Matrix3x3 &that);
    inline Matrix3x3 &operator*=(const Matrix3x3 &that);

    // Left : this = that x this
    // Right: this = this x that
    Matrix3x3 &leftMultiply (const Matrix3x3 &that);
    Matrix3x3 &rightMultiply(const Matrix3x3 &that);

    inline Matrix3x3 &setIdentity     ();

public:
    double entry[3][3];

};

inline Matrix3x3 operator+(const Matrix3x3 &a, double b);
inline Matrix3x3 operator-(const Matrix3x3 &a, double b);
inline Matrix3x3 operator*(const Matrix3x3 &a, double b);

inline Matrix3x3 operator+(const Matrix3x3 &a, const Matrix3x3 &b);
inline Matrix3x3 operator-(const Matrix3x3 &a, const Matrix3x3 &b);
inline Matrix3x3 operator*(const Matrix3x3 &a, const Matrix3x3 &b);

inline Matrix3x3 multiply(const Matrix3x3 &a, const Matrix3x3 &b);
inline Matrix3x3 conjugate(const Matrix3x3 &a, const Matrix3x3 &b);
inline Matrix3x3 othoconjugate(const Matrix3x3 &a, const Matrix3x3 &b);
inline Vector2d   operator*(const Matrix3x3 &a, const Vector2d   &b);
inline Vector2d   operator*(const Vector2d   &a, const Matrix3x3 &b);

inline double determinant(const Matrix3x3 &a);

inline Matrix3x3 transpose(const Matrix3x3 &a);
inline Matrix3x3   inverse(const Matrix3x3 &a);

inline Matrix3x3::Matrix3x3() {
  entry[0][0] = 1;
  entry[0][1] = 0;
  entry[0][2] = 0;
  entry[1][0] = 0;
  entry[1][1] = 1;
  entry[1][2] = 0;
  entry[2][0] = 0;
  entry[2][1] = 0;
  entry[2][2] = 1;
}

inline Matrix3x3::Matrix3x3(double x) {
  entry[0][0] = x;
  entry[0][1] = x;
  entry[0][2] = x;
  entry[1][0] = x;
  entry[1][1] = x;
  entry[1][2] = x;
  entry[2][0] = x;
  entry[2][1] = x;
  entry[2][2] = x;
}

inline Matrix3x3::Matrix3x3(double M00, double M01, double M02,
                                                                double M10, double M11, double M12,
                                                                double M20, double M21, double M22) {
  entry[0][0] = M00;
  entry[0][1] = M01;
  entry[0][2] = M02;
  entry[1][0] = M10;
  entry[1][1] = M11;
  entry[1][2] = M12;
  entry[2][0] = M20;
  entry[2][1] = M21;
  entry[2][2] = M22;
};

inline Matrix3x3::Matrix3x3(const Matrix3x3 &that) {
  entry[0][0] = that.entry[0][0];
  entry[0][1] = that.entry[0][1];
  entry[0][2] = that.entry[0][2];
  entry[1][0] = that.entry[1][0];
  entry[1][1] = that.entry[1][1];
  entry[1][2] = that.entry[1][2];
  entry[2][0] = that.entry[2][0];
  entry[2][1] = that.entry[2][1];
  entry[2][2] = that.entry[2][2];
};

inline Matrix3x3::Matrix3x3(const Vector2d &v1, const Vector2d &v2, const Vector2d &v3) {
    entry[0][0] = v1.entry[0];
    entry[0][1] = v1.entry[1];
    entry[0][2] = v1.entry[2];
    entry[1][0] = v2.entry[0];
    entry[1][1] = v2.entry[1];
    entry[1][2] = v2.entry[2];
    entry[2][0] = v3.entry[0];
    entry[2][1] = v3.entry[1];
    entry[2][2] = v3.entry[2];
}

inline Matrix3x3 &Matrix3x3::set(const double d) {
  return (*this)=d;
}

inline Matrix3x3 &Matrix3x3::operator=(const double d) {
  entry[0][0] = d;
  entry[0][1] = d;
  entry[0][2] = d;

  entry[1][0] = d;
  entry[1][1] = d;
  entry[1][2] = d;

  entry[2][0] = d;
  entry[2][1] = d;
  entry[2][2] = d;

  return (*this);
};

inline Matrix3x3 &Matrix3x3::set(const Matrix3x3 &that) {
  return (*this)=that;
}

inline Matrix3x3 &Matrix3x3::operator=(const Matrix3x3 &that) {
  entry[0][0] = that.entry[0][0];
  entry[0][1] = that.entry[0][1];
  entry[0][2] = that.entry[0][2];
  entry[1][0] = that.entry[1][0];
  entry[1][1] = that.entry[1][1];
  entry[1][2] = that.entry[1][2];
  entry[2][0] = that.entry[2][0];
  entry[2][1] = that.entry[2][1];
  entry[2][2] = that.entry[2][2];
  return (*this);
};

inline Matrix3x3 &Matrix3x3::set(double M[3][3]) {
  return (*this)=M;
}

inline Matrix3x3 &Matrix3x3::operator=(double M[3][3]) {
  entry[0][0] = M[0][0];
  entry[0][1] = M[0][1];
  entry[0][2] = M[0][2];

  entry[1][0] = M[1][0];
  entry[1][1] = M[1][1];
  entry[1][2] = M[1][2];

  entry[2][0] = M[2][0];
  entry[2][1] = M[2][1];
  entry[2][2] = M[2][2];
return (*this);
};

inline Matrix3x3 &Matrix3x3::set(const Vector2d &v1, const Vector2d &v2, const Vector2d &v3) {
    entry[0][0] = v1.entry[0];
    entry[0][1] = v1.entry[1];
    entry[0][2] = v1.entry[2];
    entry[1][0] = v2.entry[0];
    entry[1][1] = v2.entry[1];
    entry[1][2] = v2.entry[2];
    entry[2][0] = v3.entry[0];
    entry[2][1] = v3.entry[1];
    entry[2][2] = v3.entry[2];
    return (*this);
}

inline Matrix3x3 &Matrix3x3::set			(double M00, double M01, double M02,
                                                 double M10, double M11, double M12,
                                                         double M20, double M21, double M22)
{
    entry[0][0] = M00;
    entry[0][1] = M01;
    entry[0][2] = M02;
    entry[1][0] = M10;
    entry[1][1] = M11;
    entry[1][2] = M12;
    entry[2][0] = M20;
    entry[2][1] = M21;
    entry[2][2] = M22;
    return (*this);
}

inline int Matrix3x3::operator==(double d) const {
  return  ( (entry[0][0] == d) && (entry[0][1] == d) && (entry[0][2] == d) &&
                        (entry[1][0] == d) && (entry[1][1] == d) && (entry[1][2] == d) &&
                        (entry[2][0] == d) && (entry[2][1] == d) && (entry[2][2] == d));
}

inline int Matrix3x3::operator!=(double d) const {
  return  ( (entry[0][0] != d) || (entry[0][1] != d) || (entry[0][2] != d) ||
                        (entry[1][0] != d) || (entry[1][1] != d) || (entry[1][2] != d) ||
                        (entry[2][0] != d) || (entry[2][1] != d) || (entry[2][2] != d));
}

inline int Matrix3x3::operator==(const Matrix3x3 &that)const {
  return ( (entry[0][0] == that.entry[0][0]) && (entry[0][1] == that.entry[0][1]) && (entry[0][2] == that.entry[0][2]) &&
                     (entry[1][0] == that.entry[1][0]) && (entry[1][1] == that.entry[1][1]) && (entry[1][2] == that.entry[1][2]) &&
                     (entry[2][0] == that.entry[2][0]) && (entry[2][1] == that.entry[2][1]) && (entry[2][2] == that.entry[2][2]));
}

inline int Matrix3x3::operator!=(const Matrix3x3 &that)const {
  return ( (entry[0][0] != that.entry[0][0]) || (entry[0][1] != that.entry[0][1]) || (entry[0][2] != that.entry[0][2]) ||
                     (entry[1][0] != that.entry[1][0]) || (entry[1][1] != that.entry[1][1]) || (entry[1][2] != that.entry[1][2]) ||
                     (entry[2][0] != that.entry[2][0]) || (entry[2][1] != that.entry[2][1]) || (entry[2][2] != that.entry[2][2]));
}

inline Matrix3x3 &Matrix3x3::operator+=(double d) {
  entry[0][0] += d; entry[0][1] += d; entry[0][2] += d;
  entry[1][0] += d; entry[1][1] += d; entry[1][2] += d;
  entry[2][0] += d; entry[2][1] += d; entry[2][2] += d;
  return (*this);
}

inline Matrix3x3 &Matrix3x3::operator-=(double d) {
  entry[0][0] -= d; entry[0][1] -= d; entry[0][2] -= d;
  entry[1][0] -= d; entry[1][1] -= d; entry[1][2] -= d;
  entry[2][0] -= d; entry[2][1] -= d; entry[2][2] -= d;
  return (*this);
}

inline Matrix3x3 &Matrix3x3::operator*=(double d) {
  entry[0][0] *= d; entry[0][1] *= d; entry[0][2] *= d;
  entry[1][0] *= d; entry[1][1] *= d; entry[1][2] *= d;
  entry[2][0] *= d; entry[2][1] *= d; entry[2][2] *= d;
  return (*this);
}

inline Matrix3x3 &Matrix3x3::operator+=(const Matrix3x3 &that) {
  entry[0][0] += that.entry[0][0]; entry[0][1] += that.entry[0][1]; entry[0][2] += that.entry[0][2];
  entry[1][0] += that.entry[1][0]; entry[1][1] += that.entry[1][1]; entry[1][2] += that.entry[1][2];
  entry[2][0] += that.entry[2][0]; entry[2][1] += that.entry[2][1]; entry[2][2] += that.entry[2][2];
  return (*this);
}

inline Matrix3x3 &Matrix3x3::operator-=(const Matrix3x3 &that) {
  entry[0][0] -= that.entry[0][0]; entry[0][1] -= that.entry[0][1]; entry[0][2] -= that.entry[0][2];
  entry[1][0] -= that.entry[1][0]; entry[1][1] -= that.entry[1][1]; entry[1][2] -= that.entry[1][2];
  entry[2][0] -= that.entry[2][0]; entry[2][1] -= that.entry[2][1]; entry[2][2] -= that.entry[2][2];
  return (*this);
}

inline Matrix3x3 &Matrix3x3::operator*=(const Matrix3x3 &that) {
  entry[0][0] *= that.entry[0][0]; entry[0][1] *= that.entry[0][1]; entry[0][2] *= that.entry[0][2];
  entry[1][0] *= that.entry[1][0]; entry[1][1] *= that.entry[1][1]; entry[1][2] *= that.entry[1][2];
  entry[2][0] *= that.entry[2][0]; entry[2][1] *= that.entry[2][1]; entry[2][2] *= that.entry[2][2];
  return (*this);
}

inline Matrix3x3 &Matrix3x3::leftMultiply (const Matrix3x3 &that){
    Matrix3x3 tmp(entry[0][0], entry[0][1], entry[0][2],
                                    entry[1][0], entry[1][1], entry[1][2],
                                    entry[2][0], entry[2][1], entry[2][2]);

    entry[0][0] = that.entry[0][0] * tmp.entry[0][0] + that.entry[0][1] * tmp.entry[1][0] + that.entry[0][2] * tmp.entry[2][0];
    entry[0][1] = that.entry[0][0] * tmp.entry[0][1] + that.entry[0][1] * tmp.entry[1][1] + that.entry[0][2] * tmp.entry[2][1];
    entry[0][2] = that.entry[0][0] * tmp.entry[0][2] + that.entry[0][1] * tmp.entry[1][2] + that.entry[0][2] * tmp.entry[2][2];

    entry[1][0] = that.entry[1][0] * tmp.entry[0][0] + that.entry[1][1] * tmp.entry[1][0] + that.entry[1][2] * tmp.entry[2][0];
    entry[1][1] = that.entry[1][0] * tmp.entry[0][1] + that.entry[1][1] * tmp.entry[1][1] + that.entry[1][2] * tmp.entry[2][1];
    entry[1][2] = that.entry[1][0] * tmp.entry[0][2] + that.entry[1][1] * tmp.entry[1][2] + that.entry[1][2] * tmp.entry[2][2];

    entry[2][0] = that.entry[2][0] * tmp.entry[0][0] + that.entry[2][1] * tmp.entry[1][0] + that.entry[2][2] * tmp.entry[2][0];
    entry[2][1] = that.entry[2][0] * tmp.entry[0][1] + that.entry[2][1] * tmp.entry[1][1] + that.entry[2][2] * tmp.entry[2][1];
    entry[2][2] = that.entry[2][0] * tmp.entry[0][2] + that.entry[2][1] * tmp.entry[1][2] + that.entry[2][2] * tmp.entry[2][2];
    return (*this);
};

inline Matrix3x3 &Matrix3x3::rightMultiply(const Matrix3x3 &that){
    Matrix3x3 tmp(entry[0][0], entry[0][1], entry[0][2],
                                    entry[1][0], entry[1][1], entry[1][2],
                                    entry[2][0], entry[2][1], entry[2][2]);

    entry[0][0] = tmp.entry[0][0] * that.entry[0][0] + tmp.entry[0][1] * that.entry[1][0] + tmp.entry[0][2] * that.entry[2][0];
    entry[0][1] = tmp.entry[0][0] * that.entry[0][1] + tmp.entry[0][1] * that.entry[1][1] + tmp.entry[0][2] * that.entry[2][1];
    entry[0][2] = tmp.entry[0][0] * that.entry[0][2] + tmp.entry[0][1] * that.entry[1][2] + tmp.entry[0][2] * that.entry[2][2];

    entry[1][0] = tmp.entry[1][0] * that.entry[0][0] + tmp.entry[1][1] * that.entry[1][0] + tmp.entry[1][2] * that.entry[2][0];
    entry[1][1] = tmp.entry[1][0] * that.entry[0][1] + tmp.entry[1][1] * that.entry[1][1] + tmp.entry[1][2] * that.entry[2][1];
    entry[1][2] = tmp.entry[1][0] * that.entry[0][2] + tmp.entry[1][1] * that.entry[1][2] + tmp.entry[1][2] * that.entry[2][2];

    entry[2][0] = tmp.entry[2][0] * that.entry[0][0] + tmp.entry[2][1] * that.entry[1][0] + tmp.entry[2][2] * that.entry[2][0];
    entry[2][1] = tmp.entry[2][0] * that.entry[0][1] + tmp.entry[2][1] * that.entry[1][1] + tmp.entry[2][2] * that.entry[2][1];
    entry[2][2] = tmp.entry[2][0] * that.entry[0][2] + tmp.entry[2][1] * that.entry[1][2] + tmp.entry[2][2] * that.entry[2][2];
    return (*this);
};

inline Matrix3x3 &Matrix3x3::setIdentity() {
  entry[0][0] = 1; entry[0][1] = 0; entry[0][2] = 0;
  entry[1][0] = 0; entry[1][1] = 1; entry[1][2] = 0;
  entry[2][0] = 0; entry[2][1] = 0; entry[2][2] = 1;
  return (*this);
};

inline Matrix3x3 operator+(const Matrix3x3 &a,double b) {
  return (Matrix3x3(a)+=b);
}

inline Matrix3x3 operator-(const Matrix3x3 &a,double b) {
  return (Matrix3x3(a)-=b);
}

inline Matrix3x3 operator*(const Matrix3x3 &a,double b) {
  return (Matrix3x3(a)*=b);
}

inline Matrix3x3 operator+(double a, const Matrix3x3 &b) {
return b+a;
}

inline Matrix3x3 operator-(double a, const Matrix3x3 &b) {
  return Matrix3x3(a-b.entry[0][0],a-b.entry[0][1],a-b.entry[0][2],
                                         a-b.entry[1][0],a-b.entry[1][1],a-b.entry[1][2],
                                         a-b.entry[2][0],a-b.entry[2][1],a-b.entry[2][2]);
}

inline Matrix3x3 operator*(double a, const Matrix3x3 &b) {
  return b*a;
}

inline Matrix3x3 operator+(const Matrix3x3 &a,const Matrix3x3 &b) {
  return (Matrix3x3(a)+=b);
}

inline Matrix3x3 operator-(const Matrix3x3 &a,const Matrix3x3 &b) {
  return (Matrix3x3(a)-=b);
}

inline Matrix3x3 operator*(const Matrix3x3 &a,const Matrix3x3 &b) {
  return (Matrix3x3(a)*=b);
}

inline Matrix3x3 multiply(const Matrix3x3 &a,const Matrix3x3 &b) {
  Matrix3x3 tmp(a);
  tmp.rightMultiply(b);
  return tmp;
}

inline Matrix3x3 conjugate(const Matrix3x3 a, const Matrix3x3 &b) {
  Matrix3x3 tmp(a);
    Matrix3x3 c = inverse(b);
  tmp.rightMultiply(b);
    tmp.leftMultiply(c);
  return tmp;
}

inline Matrix3x3 othoconjugate(const Matrix3x3 a, const Matrix3x3 &b) {
  Matrix3x3 tmp(a);
    Matrix3x3 c = transpose(b);
  tmp.rightMultiply(b);
    tmp.leftMultiply(c);
  return tmp;
}

inline Vector2d operator*(const Matrix3x3 &a,const Vector2d &b) {
  return Vector3d(b.entry[0]*a.entry[0][0] + b.entry[1]*a.entry[0][1] + b.entry[2]*a.entry[0][2],
                                     b.entry[0]*a.entry[1][0] + b.entry[1]*a.entry[1][1] + b.entry[2]*a.entry[1][2],
                                     b.entry[0]*a.entry[2][0] + b.entry[1]*a.entry[2][1] + b.entry[2]*a.entry[2][2]);
}

inline Vector2d operator*(const Vector2d &a,const Matrix3x3 &b) {
  return Vector3d(a.entry[0]*b.entry[0][0] + a.entry[1]*b.entry[1][0] + a.entry[2]*b.entry[2][0],
                                     a.entry[0]*b.entry[0][1] + a.entry[1]*b.entry[1][1] + a.entry[2]*b.entry[2][1],
                                     a.entry[0]*b.entry[0][2] + a.entry[1]*b.entry[1][2] + a.entry[2]*b.entry[2][2]);
}

inline double determinant(const Matrix3x3 &a) {
  return ( a.entry[0][0] * a.entry[1][1] * a.entry[2][2] - a.entry[2][0] * a.entry[1][1] * a.entry[0][2]
             + a.entry[1][0] * a.entry[2][1] * a.entry[0][2] - a.entry[0][0] * a.entry[2][1] * a.entry[1][2]
                 + a.entry[2][0] * a.entry[0][1] * a.entry[1][2] - a.entry[1][0] * a.entry[0][1] * a.entry[2][2]);
}

inline Matrix3x3 transpose(const Matrix3x3 &a) {
  Matrix3x3 tmp(a);

    tmp.entry[0][1] = a.entry[1][0];
    tmp.entry[1][0] = a.entry[0][1];

    tmp.entry[0][2] = a.entry[2][0];
    tmp.entry[2][0] = a.entry[0][2];

    tmp.entry[2][1] = a.entry[1][2];
    tmp.entry[1][2] = a.entry[2][1];
  return tmp;
}

inline Matrix3x3 inverse(const Matrix3x3 &a) {
    Matrix3x3 tmp;
    double dmt;

    if ((dmt=determinant(a))!= 0.0) {
        tmp.entry[0][0] = (a.entry[1][1] * a.entry[2][2] - a.entry[2][1] * a.entry[1][2])/dmt;
        tmp.entry[0][1] = (a.entry[2][1] * a.entry[0][2] - a.entry[0][1] * a.entry[2][2])/dmt;
        tmp.entry[0][2] = (a.entry[0][1] * a.entry[1][2] - a.entry[1][1] * a.entry[0][2])/dmt;

        tmp.entry[1][0] = (a.entry[1][2] * a.entry[2][0] - a.entry[2][2] * a.entry[1][0])/dmt;
        tmp.entry[1][1] = (a.entry[2][2] * a.entry[0][0] - a.entry[0][2] * a.entry[2][0])/dmt;
        tmp.entry[1][2] = (a.entry[0][2] * a.entry[1][0] - a.entry[1][2] * a.entry[0][0])/dmt;

        tmp.entry[2][0] = (a.entry[1][0] * a.entry[2][1] - a.entry[2][0] * a.entry[1][1])/dmt;
        tmp.entry[2][1] = (a.entry[2][0] * a.entry[0][1] - a.entry[0][0] * a.entry[2][1])/dmt;
        tmp.entry[2][2] = (a.entry[0][0] * a.entry[1][1] - a.entry[1][0] * a.entry[0][1])/dmt;
    }
    return tmp;
}

#endif // MATRIX3X3_H
