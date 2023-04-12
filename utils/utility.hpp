#pragma once
#include "./transformations.hpp"
#include "./matrix.hpp"
#include <opencv2/opencv.hpp>
#include <random>

namespace gl
{
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

    //biliner interpolation
    static float bilinear(float w1,float w2,float w3,float w4,float q1,float q2,float q3,float q4){
        return (w1*q1+w2*q2+w3*q3+w4*q4)/(w1+w2+w3+w4);
    }

    //approximated bilinear interpolation, projected micropolygon may not be rectangular
    static float get_depth_bilinear(gl::vec2 sample_coord, gl::vec3 p1, gl::vec3 p2,gl::vec3 p3,gl::vec3 p4){
        float w1 = (p3.x()-sample_coord.x())*(p3.y()-sample_coord.y())/((p3.x()-p1.x())*(p3.y()-p1.y()));
        float w2 = (sample_coord.x()-p4.x())*(p3.y()-sample_coord.y())/((p3.x()-p1.x())*(p3.y()-p1.y()));
        float w3 = (sample_coord.x()-p1.x())*(sample_coord.y()-p1.y())/((p3.x()-p1.x())*(p3.y()-p1.y()));
        float w4 = (p2.x()-sample_coord.x())*(sample_coord.y()-p1.y())/((p3.x()-p1.x())*(p3.y()-p1.y()));
        float q1 = p1.z();
        float q2 = p2.z();
        float q3 = p3.z();
        float q4 = p4.z();
        return bilinear(w1,w2,w3,w4,q1,q2,q3,q4);
    };
};
