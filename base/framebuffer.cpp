#include "./framebuffer.hpp"

FrameBuffer::FrameBuffer(uint width, uint height, uint m, uint n)
{
    this->width = width;
    this->height = height;
    this->_color_buffer.resize(width);
    this->_depth_buffer.resize(width);
    // this->_samples.resize(width);
    for (uint i = 0; i < width; i++)
    {
        this->_color_buffer[i].resize(height);
        this->_depth_buffer[i].resize(height);
        // this->_samples[i].resize(height);
    }

    setSampleNumber(m, n);
}

void FrameBuffer::setPixelColor(uint u, uint v, gl::vec3 color)
{
    this->_color_buffer[u][v] = color;
}

void FrameBuffer::setSampleNumber(uint m, uint n)
{
    this->sample_offset.clear();
    // sample_offset offset in the grid
    for (uint i = 0; i < m; i++)
    {
        for (uint j = 0; j < n; j++)
        {
            float i_seg = (float)i / (float)m;
            float i_seg_next = (float)(i + 1) / (float)m;
            float j_seg = (float)j / (float)n;
            float j_seg_next = (float)(j + 1) / (float)n;
            gl::vec2 offset = {gl::rand_num(i_seg, i_seg_next), gl::rand_num(j_seg, j_seg_next)};
            this->sample_offset.push_back(offset);
        }
    }
};

void FrameBuffer::setPixelDepth(uint u, uint v, float depth)
{
    this->_depth_buffer[u][v] = depth;
}

cv::Mat FrameBuffer::to_cv_mat()
{
    cv::Mat image(getWidth(), getHeight(), CV_32FC3, cv::Scalar(0.0f, 0.0f, 0.0f));
    image.convertTo(image, CV_8UC3, 1.0f);
    for (uint i = 0; i < getWidth(); i++)
    {
        for (uint j = 0; j < getHeight(); j++)
        {
            auto color = getPixelColor(i, j);
            image.at<cv::Vec3b>(i, j) = cv::Vec3b(color.x() * 255, color.y() * 255, color.z() * 255);
        }
    }
    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
    return image;
}