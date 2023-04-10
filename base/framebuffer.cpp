#include "./framebuffer.hpp"

FrameBuffer::FrameBuffer(uint width, uint height){
    
    this->width = width;
    this->height = height;
    this->_color_buffer.resize(width);
    this->_depth_buffer.resize(width);
    for (uint i = 0; i < width; i++)
    {
        this->_color_buffer[i].resize(height);
        this->_depth_buffer[i].resize(height);
    }

}