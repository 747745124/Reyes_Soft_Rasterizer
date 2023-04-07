#pragma once
#include "./matrix.hpp"
namespace gl
{
    // Quaternion class
    template <class T = float>
    class Quaternion
    {
    public:
        T w, x, y, z;
        // constructors
        Quaternion() : w(1), x(0), y(0), z(0) {}
        Quaternion(T w, vec3 v) : w(w), x(v[0]), y(v[1]), z(v[2]) {}
        Quaternion(vec4 v) : w(v[0]), x(v[1]), y(v[2]), z(v[3]) {}
        Quaternion(T w, T x, T y, T z) : w(w), x(x), y(y), z(z) {}
        // methods
        static Quaternion fromAxisAngle(vec3 axis, T angle)
        {
            axis = axis.normalize();
            T s = sin(angle / 2);
            return Quaternion(cos(angle / 2), axis[0] * s, axis[1] * s, axis[2] * s);
        }

        // operators
        Quaternion operator+(Quaternion q) { return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z); }
        Quaternion operator-(Quaternion q) { return Quaternion(w - q.w, x - q.x, y - q.y, z - q.z); }
        Quaternion operator*(Quaternion q) { return Quaternion(w * q.w - x * q.x - y * q.y - z * q.z, w * q.x + x * q.w + y * q.z - z * q.y, w * q.y - x * q.z + y * q.w + z * q.x, w * q.z + x * q.y - y * q.x + z * q.w); }
        Quaternion operator*(T s) { return Quaternion(w * s, x * s, y * s, z * s); }
        Quaternion operator/(T s) { return Quaternion(w / s, x / s, y / s, z / s); }
        // functions
        T norm() { return sqrt(w * w + x * x + y * y + z * z); }
        Quaternion normalize() { return *this / norm(); }
        Quaternion conjugate() { return Quaternion(w, -x, -y, -z); }
        Quaternion inverse() { return conjugate() / (norm() * norm()); }
        // cast
        mat4 to_mat4()
        {
            return mat4{
                1 - 2 * y * y - 2 * z * z, 2 * x * y - 2 * z * w, 2 * x * z + 2 * y * w, 0,
                2 * x * y + 2 * z, 1 - 2 * x * x - 2 * z * z, 2 * y * z - 2 * x * w, 0,
                2 * x * z - 2 * y * w, 2 * y * z + 2 * x * w, 1 - 2 * x * x - 2 * y * y, 0,
                0, 0, 0, 1};
        }
    };

    // Quaternion multiply with vec3
    template <class T>
    vec3 operator*(Quaternion<T> q, vec3 v)
    {
        Quaternion<T> p(0, v);
        Quaternion<T> r = q * p * q.inverse();
        return vec3(r.x, r.y, r.z);
    }

    using Quat = Quaternion<float>;
};