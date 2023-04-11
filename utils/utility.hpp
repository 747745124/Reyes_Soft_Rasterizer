#pragma once
#include "./transformations.hpp"
#include "./matrix.hpp"
#include "../base/framebuffer.hpp"
#include <opencv2/opencv.hpp>

namespace gl
{
    // suppose color is vec3 and range 0~1
    static cv::Mat to_cv_mat(FrameBuffer &fb)
    {
        cv::Mat image(fb.getWidth(), fb.getHeight(), CV_32FC3, cv::Scalar(0.0f, 0.0f, 0.0f));
        image.convertTo(image, CV_8UC3, 1.0f);
        for (uint i = 0; i < fb.getWidth(); i++)
        {
            for (uint j = 0; j < fb.getHeight(); j++)
            {
                auto color = fb.getPixelColor(i, j);
                image.at<cv::Vec3b>(i, j) = cv::Vec3b(color.x() * 255, color.y() * 255, color.z() * 255);
            }
        }
        cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
        return image;
    };

    float sign(gl::vec2 p1, gl::vec2 p2, gl::vec2 p3)
    {
        return (p1.x() - p3.x()) * (p2.y() - p3.y()) - (p2.x() - p3.x()) * (p1.y() - p3.y());
    }

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

};
