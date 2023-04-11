#pragma once
#include "../utils/matrix.hpp"
#include "../utils/quat.hpp"
#include "../utils/transformations.hpp"
#include "../utils/utility.hpp"
#include <opencv2/opencv.hpp>

enum class buffer_type
{
    color,
    depth
};

class FrameBuffer
{
public:
    FrameBuffer(uint width, uint height, uint m = 1, uint n = 1);
    void setPixelColor(uint u, uint v, gl::vec3 color);
    void setPixelDepth(uint u, uint v, float depth);
    void setSampleNumber(uint m, uint n);

    uint getSampleNumber() const
    {
        // return this->_samples[0][0].size();
        return this->sample_offset.size();
    };

    std::vector<gl::vec2> getSampleOffset() const
    {
        return this->sample_offset;
    };

    uint getWidth() const
    {
        return this->width;
    };

    uint getHeight() const
    {
        return this->height;
    };

    // the input should be 2 integers
    gl::vec3 getPixelColor(uint u, uint v) const
    {
        return this->_color_buffer[u][v];
    };

    float getPixelDepth(uint u, uint v) const
    {
        return this->_depth_buffer[u][v];
    };

    // suppose color is vec3 and range 0~1
    cv::Mat to_cv_mat();

private:
    // 1st 2nd index for pixel, 3rd index for vector of samples
    // std::vector<std::vector<std::vector<gl::vec2>>> _samples;
    std::vector<gl::vec2> sample_offset;

    // pixel color buffer
    std::vector<std::vector<gl::vec3>> _color_buffer;
    // pixel depth buffer
    std::vector<std::vector<float>> _depth_buffer;
    uint width;
    uint height;
};
