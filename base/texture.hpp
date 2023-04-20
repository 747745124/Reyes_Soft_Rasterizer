#pragma once
#include <string>
#include "../utils/matrix.hpp"
#include "../utils/utility.hpp"
#include "./mesh.hpp"
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
	TextureShadow(uint width=2000, uint height=2000);
	~TextureShadow() = default;
	void setTexelDepth(uint u, uint v, float depth);
	float getTexelDepth(float u, float v, LERP_MODE mode = LERP_MODE::NEAREST);
	void renderToTextureShadow(Mesh &mesh, gl::mat4 lightmatrix);
	void updateByMicropolygon(Micropolygon &mp, gl::mat4 lightmodel);
	cv::Mat to_cv_mat();

private:
	// 1st and 2nd index for texel
	std::vector<std::vector<float>> texels;
	uint _width=2000;
	uint _height=2000;
};