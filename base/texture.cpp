#include "./texture.hpp"

Texture2D::Texture2D(const std::string path, int FORMAT = CV_32FC3) : _path(path)
{
    // read image from given path
    auto _image = cv::imread(_path.c_str(), cv::IMREAD_COLOR);
    if (_image.empty())
        throw std::runtime_error("Failed to load texture");
    _image.convertTo(_image, FORMAT,1.0 / 255.0);
    cv::cvtColor(_image, _image, cv::COLOR_BGR2RGB);

    // get the width and height of the image
    this->_width = _image.cols;
    this->_height = _image.rows;

    if (FORMAT == CV_32FC3)
    {
        // read the image into the texels
        // note: it's row major in opencv
        for (uint j = 0; j < _width; j++)
        {
            std::vector<gl::vec3> row;
            for (uint i = 0; i < _height; i++)
            {
                cv::Vec3f color = _image.at<cv::Vec3f>(i, j);
                row.push_back(gl::vec3(color[2], color[1], color[0]));
            }
            texels.push_back(row);
        }
    }
    else if (FORMAT == CV_8UC3)
    {
        // read the image into the texels
        for (uint j = 0; j < _width; j++)
        {
            std::vector<gl::vec3> row;
            for (uint i = 0; i < _height; i++)
            {
                cv::Vec3b color = _image.at<cv::Vec3b>(i, j);
                row.push_back(gl::vec3(color[0], color[1], color[2]));
            }
        }
    }
    else
    {
        throw std::runtime_error("Unsupported format");
    }

    _image.release();
    return;
};

gl::vec3 Texture2D::getTexelColor(float u, float v,LERP_MODE mode)
{   
    uint x = 0;
    uint y = 0;
    if(mode==LERP_MODE::NEAREST)
    {
        // get the texel coordinate
        x = (uint)(u * _width);
        y = (uint)(v * _height);
    }

    // clamp the texel coordinate
    x = std::clamp(x, 0u, _width - 1);
    y = std::clamp(y, 0u, _height - 1);

    return texels[x][y];
};