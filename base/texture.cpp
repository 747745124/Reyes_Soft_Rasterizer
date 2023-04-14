#include "./texture.hpp"

Texture2D::Texture2D(const std::string path,int FORMAT=CV_32FC3) : _path(path)
{
    //read image from given path
    this->_image = cv::imread(_path.c_str(), cv::IMREAD_COLOR);
    if (this->_image.empty())
        throw std::runtime_error("Failed to load texture");
    _image.convertTo(_image,FORMAT);

    return;
};