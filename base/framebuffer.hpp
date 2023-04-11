#pragma once
#include "../utils/matrix.hpp"
#include "../utils/quat.hpp"
#include "../utils/transformations.hpp"
#include <opencv2/opencv.hpp>

enum class buffer_type
{
    color,
    depth
};

class FrameBuffer
{
public:
    FrameBuffer(uint width, uint height);
    void setPixelColor(uint u, uint v, gl::vec3 color);
    void setPixelDepth(uint u, uint v, float depth);

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

private:
    std::vector<std::vector<gl::vec2>> _samples;
    std::vector<std::vector<gl::vec3>> _color_buffer;
    std::vector<std::vector<float>> _depth_buffer;
    uint width;
    uint height;
    uint spp; // samples per pixel
};