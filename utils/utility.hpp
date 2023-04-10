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
                auto color = fb.getColor(gl::vec2(i, j));
                image.at<cv::Vec3b>(i, j) = cv::Vec3b(color.x() * 255, color.y() * 255, color.z() * 255);
            }
        }
        cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
        return image;
    };

};
