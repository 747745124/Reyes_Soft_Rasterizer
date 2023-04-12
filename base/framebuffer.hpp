#pragma once
#include "../utils/matrix.hpp"
#include "../utils/quat.hpp"
#include "../utils/transformations.hpp"
#include "../utils/utility.hpp"
#include "./vertex.hpp"
#include <opencv2/opencv.hpp>

//data needed per sample
struct Sample
{
    float depth;
    //in rgba format
    gl::vec3 color;

    void setDepth(float d)
    {
        this->depth = d;
    };

    void setColor(gl::vec3 c)
    {
        this->color = c;
    };

    Sample()
    {
        this->depth = 1.0;
        this->color = gl::vec3(0.0);
    }
};

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
    //use sample buffer to set pixel color
    void setPixelColorFromBuffer();
    //set sample number
    void setSampleNumber(uint m, uint n);
    //initialize all sample buffers
    void initBuffers();

    // given a micropolygon, test it with the framebuffer, update buffers accordingly
    void updateBufferByMicropolygon(Micropolygon &mp,gl::mat4 mvp);

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

    //reset all pixel color to 0.0
    void clearColor(){
        //set all pixel color to 0.0
        for (uint i = 0; i < width; i++)
        {
            for (uint j = 0; j < height; j++)
            {
                this->_pixel_color[i][j] = gl::vec3(0.0);
            }
        }
    }

    //reset all samples in the buffer to 0.0
    void clearBuffer(buffer_type b_type);

    // the input should be 2 integers
    gl::vec3 getPixelColor(uint u, uint v) const
    {
        return this->_pixel_color[u][v];
    };
    
    // suppose color is vec3 and range 0~1
    cv::Mat to_cv_mat();

private:
    // 1st 2nd index for pixel, 3rd index for vector of samples
    // std::vector<std::vector<std::vector<gl::vec2>>> _samples;
    std::vector<gl::vec2> sample_offset;

    // per pixel color buffer, all 0s by constructor default
    std::vector<std::vector<gl::vec3>> _pixel_color;
    // per sample buffers (color,depth), index by pixel coordinate, and then sample index
    std::vector<std::vector<std::vector<Sample>>> _buffers;

    uint width;
    uint height;
};

