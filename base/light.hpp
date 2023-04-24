#pragma once
#include "./object3D.hpp"

class Light : public Object3D
{
public:
    Light(){
        position = gl::vec3(0.f, 3.f, 7.0f);
        color = gl::vec3(1.0f, 1.0f, 1.0f);
        intensity = 400.0f;
    };
    Light(const gl::vec3 &position, const gl::vec3 &color, float intensity)
    {
        this->color = {1.0f, 1.0f, 1.0f};
        this->intensity = 1.0f;
    }
    ~Light() = default;

public:
    float intensity = 1.0f;
    gl::vec3 color = {1.0f, 1.0f, 1.0f};
};

class DirectionalLight : public Light
{
public:
    DirectionalLight() = default;
    ~DirectionalLight() = default;
};

class SpotLight : public Light
{
public:
    SpotLight() = default;
    ~SpotLight() = default;

public:
    float angle = gl::to_radian(180.0f);
    float kc = 1.0f;
    float kl = 0.0f;
    float kq = 0.2f;
};