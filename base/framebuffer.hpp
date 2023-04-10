#pragma once
#include "../utils/matrix.hpp"
#include "../utils/quat.hpp"
#include "../utils/transformations.hpp"

enum class buffer_type
{
    color,
    depth
};

class FrameBuffer
{
public:
    void setColor(gl::vec2 screen_pos, gl::vec3 color);
    void setDepth(gl::vec2 screen_pos, float depth);

private:
    std::vector<gl::vec3> _color_buffer;
    std::vector<float> _depth_buffer;
    uint width;
    uint height;
};