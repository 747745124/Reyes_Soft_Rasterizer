#pragma once
#include <string>
#include "../utils/matrix.hpp"
#include <opencv2/opencv.hpp>

enum class LERP_MODE
{
	NEAREST,
	BILINEAR
};

class Texture2D
{
public:
	Texture2D(const std::string path, int FORMAT);
	~Texture2D() = default;

	// get the texel color, get to the nearest texel
	// bilinear interpolation is in progress
	gl::vec3 getTexelColor(float u, float v, LERP_MODE mode = LERP_MODE::NEAREST);

private:
	std::string _path;
	std::vector<std::vector<gl::vec3>> texels;
	uint _width;
	uint _height;
};