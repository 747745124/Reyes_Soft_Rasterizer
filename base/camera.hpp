#pragma once
#include "../utils/matrix.hpp"
#include "../utils/quat.hpp"
#include "../utils/transformations.hpp"
#include "../base/object3D.hpp"

// abstract camera class
class Camera : public Object3D
{
public:
    gl::mat4 getViewMat() const;
    virtual gl::mat4 getProjectionMat() const = 0;
};

class PerspectiveCamera : public Camera
{
public:
    float fov;
    float aspect;
    float zNear;
    float zFar;
    PerspectiveCamera(float fov, float aspect, float zNear, float zFar);
    ~PerspectiveCamera() = default;
    gl::mat4 getProjectionMat() const override;
};

class OrthographicCamera : public Camera
{
public:
    float left;
    float right;
    float bottom;
    float top;
    float zNear;
    float zFar;

public:
    OrthographicCamera(float left, float right, float bottom, float top, float zNear, float zFar);

    ~OrthographicCamera() = default;

    gl::mat4 getProjectionMat() const override;
};