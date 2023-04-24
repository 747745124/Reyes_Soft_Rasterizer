#pragma once
#include "../utils/matrix.hpp"

struct PhongMaterial
{
    gl::vec3 albedo = gl::vec3(0.4f);
    float ka = 0.4f;
    gl::vec3 kd = gl::vec3(0.2f);
    gl::vec3 ks = gl::vec3(0.6f);
    float shininess = 32.f;
    PhongMaterial() = default;
};

struct PBRMaterial
{
    gl::vec3 albedo = gl::vec3(0.5f);
    float metallic = 0.3f;
    float roughness = 0.3f;
    float ao = 0.4f;

    PBRMaterial() = default;
    float GGX(float NdotH) const
    {
        NdotH = std::max(NdotH, 0.f);
        float a = roughness * roughness;
        float a2 = a * a;
        float NdotH2 = NdotH * NdotH;
        float nom = a2;
        float denom = (NdotH2 * (a2 - 1.f) + 1.f);
        denom = M_PI * denom * denom;
        return nom / denom;
    };

    gl::vec3 FresnelSchlick(float cos_theta, gl::vec3 F0) const
    {
        return F0 + (gl::vec3(1.0) - F0) * pow(1.0 - cos_theta, 5.0);
    };

    float GeometrySchlickGGX(float NdotV) const
    {
        float a = roughness;
        float k = (a * a) / 2.0;

        float nom = NdotV;
        float denom = NdotV * (1.0 - k) + k;

        return nom / denom;
    };

    float GeometrySmith(gl::vec3 N, gl::vec3 V, gl::vec3 L) const
    {
        float NdotV = std::max(dot(N, V), 0.0f);
        float NdotL = std::max(dot(N, L), 0.0f);
        float ggx2 = GeometrySchlickGGX(NdotV);
        float ggx1 = GeometrySchlickGGX(NdotL);

        return ggx1 * ggx2;
    };

    float D_Ashikhmin(float NoH)
    {
        // Ashikhmin 2007, "Distribution-based BRDFs"
        // Cloth BRDF
        float a2 = roughness * roughness;
        float cos2h = NoH * NoH;
        float sin2h = std::max(1.0 - cos2h, 0.0078125);
        float sin4h = sin2h * sin2h;
        float cot2 = -cos2h / (a2 * sin2h);
        return 1.0 / (M_PI * (4.0 * a2 + 1.0) * sin4h) * (4.0 * exp(cot2) + sin4h);
    }

    // ue4 2013
    float shadowMaskingUnreal(gl::vec3 N, gl::vec3 V, gl::vec3 L)
    {
        float NdotV = std::max(dot(N, V), 0.0f);
        float NdotL = std::max(dot(N, L), 0.0f);
        float ggx1 = GeometrySchlickGGX(NdotL);
        float ggx2 = GeometrySchlickGGX(NdotV);
        return ggx1 * ggx2;
    }

    gl::vec3 ImportanceSamplingGGX(gl::vec2 Xi, gl::vec3 N)
    {
        using namespace gl;
        float a = roughness * roughness;
        float phi = 2.0f * M_PI * Xi.x();
        float cos_theta = sqrt((1.0f - Xi.y()) / (1.0f + (a * a - 1.0f) * Xi.y()));
        float sin_theta = sqrt(1.0f - cos_theta * cos_theta);

        // spherical coordinates to cartesian coordinates
        vec3 H;
        H.x() = cos(phi) * sin_theta;
        H.y() = sin(phi) * sin_theta;
        H.z() = cos_theta;

        // tangent space to world space
        vec3 up = fabs(N.z()) < 0.999f ? vec3(0.0f, 0.0f, 1.0f) : vec3(1.0f, 0.0f, 0.0f);
        vec3 tangent = normalize(cross(up, N));
        vec3 bitangent = cross(N, tangent);

        vec3 sample_vec = tangent * H.x() + bitangent * H.y() + N * H.z();
        return normalize(sample_vec);
    };
};


