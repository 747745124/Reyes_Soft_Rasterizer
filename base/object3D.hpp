#pragma once
#include "../utils/matrix.hpp"
#include "../utils/quat.hpp"
#include "../utils/transformations.hpp"

class Object3D {
public:
	gl::vec3 position = { 0.0f, 0.0f, 0.0f };
	gl::Quat rotation = { 1.0f, 0.0f, 0.0f, 0.0f };
	gl::vec3 scale = { 1.0f, 1.0f, 1.0f };

	gl::vec3 defaultFront{ 0.0f, 0.0f, 1.0f };
	gl::vec3 defaultUp{ 0.0f, 1.0f, 0.0f };
	gl::vec3 defaultRight{ 1.0f, 0.0f, 0.0f };

	gl::vec3 getFront() const;

	gl::vec3 getUp() const;

	gl::vec3 getRight() const;

	gl::mat4 getModelMat() const;
};