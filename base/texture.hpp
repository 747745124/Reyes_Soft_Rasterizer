#pragma once
#include <string>
#include "../utils/matrix.hpp"
#include <opencv2/opencv.hpp>
// TODO: Shadowmapping
enum class LERP_MODE
{
	NEAREST,
	BILINEAR,
	CORNER
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

class TextureShadow
{
public:
	TextureShadow(uint width, uint height, int FORMAT);
	~TextureShadow() = default;
	void setTexelColor(float u, float v, gl::vec3 color, LERP_MODE mode = LERP_MODE::NEAREST);
	gl::vec3 getTexelColor(float u, float v, LERP_MODE mode = LERP_MODE::NEAREST);

private:
	std::vector<std::vector<gl::vec3>> texels;
	uint _width;
	uint _height;
};