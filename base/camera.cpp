#include "./camera.hpp"

gl::mat4 Camera::getViewMat() const
{
    return gl::getViewMat(position, position + getFront(), getUp());
}

PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float zNear, float zFar)
    : fov(fov), aspect(aspect), zNear(zNear), zFar(zFar) {}

gl::mat4 PerspectiveCamera::getProjectionMat() const
{
    return gl::getPerspectiveMatLH(fov, aspect, zNear, zFar);
}

OrthographicCamera::OrthographicCamera(
    float left, float right, float bottom, float top, float zNear, float zFar)
    : left(left), right(right), top(top), bottom(bottom), zNear(zNear), zFar(zFar) {}

gl::mat4 OrthographicCamera::getProjectionMat() const
{
    return gl::getOrthoMat(left, right, bottom, top, zNear, zFar);
}