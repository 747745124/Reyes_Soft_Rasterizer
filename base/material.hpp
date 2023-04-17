#pragma once
#include "../utils/matrix.hpp"

struct PhongMaterial
{
    gl::vec3 albedo;
    float ka;
    gl::vec3 kd;
    gl::vec3 ks;
    float shininess;

    PhongMaterial() = default;
};

struct PBRMaterial
{
    gl::vec3 albedo;
    float metallic;
    float roughness;
    float ao;

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
        float sin2h = std::max(1.0 - cos2h, 0.0078125); // 2^(-14/2), so sin2h^2 > 0 in fp16
        float sin4h = sin2h * sin2h;
        float cot2 = -cos2h / (a2 * sin2h);
        return 1.0 / (M_PI * (4.0 * a2 + 1.0) * sin4h) * (4.0 * exp(cot2) + sin4h);
    }

    // ue4 2013
    float shadowMaskingUnreal(gl::vec3 N, gl::vec3 V, gl::vec3 L, float roughness)
    {
        float NdotV = std::max(dot(N, V), 0.0f);
        float NdotL = std::max(dot(N, L), 0.0f);
        float ggx1 = GeometrySchlickGGX(NdotL);
        float ggx2 = GeometrySchlickGGX(NdotV);
        return ggx1 * ggx2;
    }
};
