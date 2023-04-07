#include "./object3D.hpp"

gl::vec3 Object3D::getFront() const
{
    return rotation * defaultFront;
}

gl::vec3 Object3D::getUp() const
{
    return rotation * defaultUp;
}

gl::vec3 Object3D::getRight() const
{
    return rotation * defaultRight;
}

gl::mat4 Object3D::getModelMat() const
{
    return gl::getModelMat(position, rotation, scale);
}