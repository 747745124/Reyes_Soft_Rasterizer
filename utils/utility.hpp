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
};
