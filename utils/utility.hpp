#pragma once
#include "./transformations.hpp"
#include "./matrix.hpp"
#include <opencv2/opencv.hpp>
#include <random>

namespace gl
{
    static float VanDerCorput(uint bits)
    {
        bits = (bits << 16u) | (bits >> 16u);
        bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
        bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
        bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
        bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
        return float(bits) * 2.3283064365386963e-10; // / 0x100000000
    }

    static gl::vec2 Hammersley(uint i, uint N)
    {
        return gl::vec2(float(i) / float(N), VanDerCorput(i));
    }

    static float step(float edge, float x)
    {
        return x < edge ? 0.0f : 1.0f;
    }

    // convolve kernel
    static float filtered_step(float edge, float x, float w)
    {
        return std::clamp((x - edge + w / 2.0f) / w, 0.0f, 1.0f);
    }

    static float sign(gl::vec2 p1, gl::vec2 p2, gl::vec2 p3)
    {
        return (p1.x() - p3.x()) * (p2.y() - p3.y()) - (p2.x() - p3.x()) * (p1.y() - p3.y());
    }

    // check if a point is inside a rectangle
    // note that p1 p2 p3 p4 must be in clockwise order or counter clockwise order
    static bool is_inside_rect(gl::vec2 sample, gl::vec2 p1, gl::vec2 p2, gl::vec2 p3, gl::vec2 p4)
    {
        auto d1 = sign(sample, p1, p2);
        auto d2 = sign(sample, p2, p3);
        auto d3 = sign(sample, p3, p4);
        auto d4 = sign(sample, p4, p1);

        auto all_neg = (d1 <= 0) && (d2 <= 0) && (d3 <= 0) && (d4 <= 0);
        auto all_pos = (d1 >= 0) && (d2 >= 0) && (d3 >= 0) && (d4 >= 0);

        return all_neg || all_pos;
    }

    static bool is_convex(gl::vec2 p1, gl::vec2 p2, gl::vec2 p3, gl::vec2 p4)
    {
        auto d1 = sign(p4, p2, p1);
        auto d2 = sign(p1, p3, p2);
        auto d3 = sign(p2, p4, p3);
        auto d4 = sign(p3, p1, p4);

        auto all_neg = (d1 <= 0) && (d2 <= 0) && (d3 <= 0) && (d4 <= 0);
        auto all_pos = (d1 >= 0) && (d2 >= 0) && (d3 >= 0) && (d4 >= 0);
        return all_neg || all_pos;
    }

    // random number from 0 to 1
    static float rand_num()
    {
        std::random_device rd;  // Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
        std::uniform_real_distribution<> dist(0, 1);
        return dist(gen);
    }

    // random number from 0 to end_point
    static float rand_num(float end_point)
    {
        std::random_device rd;  // Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
        std::uniform_real_distribution<> dist(0, end_point);
        return dist(gen);
    }

    // random number from start_point to end_point
    static float rand_num(float start_point, float end_point)
    {
        std::random_device rd;  // Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
        std::uniform_real_distribution<> dist(start_point, end_point);
        return dist(gen);
    }

    // biliner interpolation
    static float bilinear(float w1, float w2, float w3, float w4, float q1, float q2, float q3, float q4)
    {
        return (w1 * q1 + w2 * q2 + w3 * q3 + w4 * q4) / (w1 + w2 + w3 + w4);
    }

    // biliner interpolation
    template <typename T>
    static T bilinear(float w1, float w2, float w3, float w4, T q1, T q2, T q3, T q4)
    {
        return (w1 * q1 + w2 * q2 + w3 * q3 + w4 * q4) / (w1 + w2 + w3 + w4);
    }

    // biliner interpolation
    template <typename T>
    static T bilinear(gl::vec2 uv, T p1, T p2, T p3, T p4)
    {
        T p = p4 + (p3 - p4) * uv.u();
        T q = p1 + (p2 - p1) * uv.u();
        return p + (q - p) * uv.v();
    }

    // approximated bilinear interpolation, projected micropolygon may not be rectangular
    static float get_depth_bilinear(gl::vec2 sample_coord, gl::vec3 p1, gl::vec3 p2, gl::vec3 p3, gl::vec3 p4)
    {
        float w1 = (p3.x() - sample_coord.x()) * (p3.y() - sample_coord.y()) / ((p3.x() - p1.x()) * (p3.y() - p1.y()));
        float w2 = (sample_coord.x() - p4.x()) * (p3.y() - sample_coord.y()) / ((p3.x() - p1.x()) * (p3.y() - p1.y()));
        float w3 = (sample_coord.x() - p1.x()) * (sample_coord.y() - p1.y()) / ((p3.x() - p1.x()) * (p3.y() - p1.y()));
        float w4 = (p2.x() - sample_coord.x()) * (sample_coord.y() - p1.y()) / ((p3.x() - p1.x()) * (p3.y() - p1.y()));
        float q1 = p1.z();
        float q2 = p2.z();
        float q3 = p3.z();
        float q4 = p4.z();
        return bilinear(w1, w2, w3, w4, q1, q2, q3, q4);
    };

    // https://stackoverflow.com/questions/808441/inverse-bilinear-interpolation
    // The good thing is the order of the points is preserved after projection
    // If the points is not passed in clockwise or counter clockwise order, the result will be wrong
    // This returns the interpolated params u,v
    // Use u,v we can interpolate depth, normal, color, etc.
    static gl::vec2 inverseBilinear(gl::vec2 sample, gl::vec2 p1, gl::vec2 p2, gl::vec2 p3, gl::vec2 p4)
    {
        using namespace gl;
        vec2 res(0.0);

        if (!is_convex(p1, p2, p3, p4))
            throw std::runtime_error("Non convex polygon!");

        vec2 e = p2 - p1;
        vec2 f = p4 - p1;
        vec2 g = p1 - p2 + p3 - p4;
        vec2 h = sample - p1;

        float k0 = h.x() * e.y() - h.y() * e.x();
        float k1 = e.x() * f.y() - e.y() * f.x() + h.x() * g.y() - h.y() * g.x();
        float k2 = g.x() * f.y() - g.y() * f.x();

        // special case: parallel edges
        if (fabs(k2) < 1e-4)
            return res = vec2((h.x() * k1 + f.x() * k0) / (e.x() * k1 - g.x() * k0), -k0 / k1);

        // quadric equation

        float delta = k1 * k1 - 4 * k0 * k2;

        if (delta < 0.f)
        {
            // no solution
            std::cout << "no solution" << std::endl;
            return res = vec2(-1.f, -1.f);
        }

        delta = sqrtf(delta);
        float v = (-k1 - delta) / (2 * k2);
        float u = (h.x() - f.x() * v) / (e.x() + g.x() * v);

        if (u < 0.0 || u > 1.0 || v < 0.0 || v > 1.0)
        {
            v = (-k1 + delta) / (2 * k2);
            u = (h.x() - f.x() * v) / (e.x() + g.x() * v);
        }

        // degrade to nearest
        if (u < 0.0 || u > 1.0 || v < 0.0 || v > 1.0)
        {
            return res = vec2(0.f, 0.f);
        }

        return res = vec2(u, v);
    }

};

class Plane
{
public:
    Plane(gl::vec3 p1, gl::vec3 p2, gl::vec3 p3)
    {
        gl::vec3 v1 = p2 - p1;
        gl::vec3 v2 = p3 - p1;
        normal = cross(v1, v2);
        normal.normalized();
        d = dot(-normal, p1);
    }
    Plane(gl::vec3 normal, gl::vec3 p)
    {
        this->normal = normal;
        this->normal.normalize();
        d = dot(-normal, p);
    }
    Plane()
    {
        normal = gl::vec3(0, 0, 1);
        d = 0;
    }

    float get_x(float y, float z)
    {
        return -(normal.y() * y + normal.z() * z + d) / normal.x();
    }

    float get_y(float x, float z)
    {
        return -(normal.x() * x + normal.z() * z + d) / normal.y();
    }

    float get_z(float x, float y)
    {
        return -(normal.x() * x + normal.y() * y + d) / normal.z();
    }

    gl::vec3 get_normal()
    {
        return normal;
    }

    float get_d()
    {
        return d;
    }

private:
    gl::vec3 normal;
    float d;
};
