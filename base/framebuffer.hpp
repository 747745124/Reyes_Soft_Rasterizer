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
    void setColor(gl::vec2 screen_pos, gl::vec3 color);
    void setDepth(gl::vec2 screen_pos, float depth);

    uint getWidth() const
    {
        return this->width;
    };

    uint getHeight() const
    {
        return this->height;
    };

    // the input should be 2 integers
    gl::vec3 getColor(gl::vec2 screen_pos) const
    {
        return this->_color_buffer[(int)screen_pos.x()][(int)screen_pos.y()];
    };

private:
    std::vector<std::vector<gl::vec3>> _color_buffer;
    std::vector<std::vector<float>> _depth_buffer;
    uint width;
    uint height;
};