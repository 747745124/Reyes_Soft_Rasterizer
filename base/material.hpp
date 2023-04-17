#pragma once
#include "../utils/matrix.hpp"

struct PhongMaterial {
    gl::vec3 albedo;
    float ka;
    gl::vec3 kd;
    gl::vec3 ks;
    float shininess;

    PhongMaterial() = default;
};

struct PBRMaterial {
    gl::vec3 albedo;
    float metallic;
    float roughness;
    float ao;

    PBRMaterial() = default;
};