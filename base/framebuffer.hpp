#pragma once
#include "../utils/matrix.hpp"
#include "../utils/quat.hpp"
#include "../utils/transformations.hpp"
#include "../utils/utility.hpp"
#include "./vertex.hpp"
#include "./texture.hpp"
#include <opencv2/opencv.hpp>

// data needed per sample
struct Sample
{
    float depth;
    // in rgba format
    gl::vec4 color;

    void setDepth(float d)
    {
        this->depth = d;
    };

    void setColor(gl::vec4 c)
    {
        this->color = c;
    };

    Sample()
    {
        this->depth = 1.0;
        this->color = gl::vec4(gl::vec3(0.0f), 1.0f);
    }

    Sample(float depth, gl::vec4 color)
    {
        this->depth = depth;
        this->color = color;
    }
};

enum class buffer_type
{
    color,
    depth
};

//Default FrameBuffer
class FrameBuffer
{
public:
    FrameBuffer(uint width, uint height, uint m = 1, uint n = 1);
    void setPixelColor(uint u, uint v, gl::vec3 color);
    // use sample buffer to set pixel color
    void setPixelColorFromBuffer();
    // use sample depth to set pixel color, visualizing z-buffer (per pixel make average)
    void setPixelDepthFromBuffer(float znear,float zfar);
    // set sample number
    void setSampleNumber(uint m, uint n);
    // initialize all sample buffers
    void initBuffers();
    //  set samples buffer,to ensure the same sample across all different framebuffers
    void setSampleOffset(std::vector<gl::vec2> & offsets){
        this->sample_offset = offsets;
    };
    // given a micropolygon, test it with the framebuffer, update buffers accordingly
    void updateBufferByMicropolygon(Micropolygon &mp, gl::mat4 mvp,LERP_MODE mode = LERP_MODE::CORNER);

    uint getSampleNumber() const
    {
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

    // reset all pixel color to 0.0
    void clearColor()
    {
        // set all pixel color to 0.0
        for (uint i = 0; i < width; i++)
        {
            for (uint j = 0; j < height; j++)
            {
                this->_pixel_color[i][j] = gl::vec3(0.0);
            }
        }
    }

    // reset all samples in the buffer to 0.0
    void clearBuffer(float bg_depth, gl::vec4 bg_color);

    // the input should be 2 integers
    gl::vec3 getPixelColor(uint u, uint v) const
    {
        return this->_pixel_color[u][v];
    };

    // suppose color is vec3 and range 0~1
    // conversion happens if the dest format is 8UC3, which loses precision
    cv::Mat to_cv_mat(int FORMAT = CV_32FC3);

private:
    // 1st 2nd index for pixel, 3rd index for vector of samples
    // std::vector<std::vector<std::vector<gl::vec2>>> _samples;
    std::vector<gl::vec2> sample_offset;

    // per pixel color buffer, all 0s by constructor default
    std::vector<std::vector<gl::vec3>> _pixel_color;
    // per sample buffers (color,depth), index by pixel coordinate
    // and then sample index, and then all the intersected data for that sample
    std::vector<std::vector<std::vector<std::vector<Sample>>>> _buffers;

    uint width;
    uint height;
};
