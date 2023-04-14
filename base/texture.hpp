#pragma once
#include <string>
#include "../utils/matrix.hpp"
#include <opencv2/opencv.hpp>

class Texture2D
{
public:
	Texture2D(const std::string path,int FORMAT);
	~Texture2D() = default;

private:
	std::string _path;
    cv::Mat _image;
};