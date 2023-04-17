#pragma once
#include "./mesh.hpp"
#include "./light.hpp"
#include "./material.hpp"
namespace gl
{
    static void setOpacity(Mesh &mesh, float opacity)
    {
        const auto [w, h] = mesh.getResolution();
        for (int i = 0; i <= w; i++)
        {
            for (int j = 0; j <= h; j++)
            {
                mesh.setVertex(i, j).baseColor.a() = opacity;
            }
        }
    }

    static void checkerBoard(Mesh &mesh, uint step_size = 10)
    {
        const auto [w, h] = mesh.getResolution();
        for (int i = 0; i <= w; i++)
        {
            for (int j = 0; j <= h; j++)
            {
                if ((i / step_size + j / step_size) % 2 == 0)
                {
                    mesh.setVertexColor(i, j, gl::vec4(0.0f, 0.0f, 0.0f, 1.0f));
                }
                else
                {
                    mesh.setVertexColor(i, j, gl::vec4(1.0f, 1.0f, 1.0f, 1.0f));
                }
            }
        }
    }

    // assume point light, Ashikhmin 2007
    static void clothShader(Mesh &mesh, std::vector<Light> lights, PBRMaterial material, gl::vec3 view_pos)
    {
        const auto [w, h] = mesh.getResolution();
        for (int i = 0; i <= w; i++)
        {
            for (int j = 0; j <= h; j++)
            {
                auto vertex_color = mesh.getVertex(i, j).baseColor;
                auto &vertex = mesh.setVertex(i, j);
                auto normal = vertex.normal.normalize();
                auto view = (view_pos - vertex.position).normalize();
                auto color = gl::vec3(0.0f);

                for (auto &light : lights)
                {
                    auto light_dir = normalize(light.position - vertex.position);
                    auto halfway = normalize(view + light_dir);
                    float dist = (light.position - vertex.position).length();
                    float attenuation = light.intensity / pow(dist, 2.0f);

                    gl::vec3 radiance = light.color * attenuation;
                    float D = material.D_Ashikhmin(dot(normal, halfway));
                    gl::vec3 upper(D);
                    float NoL = std::max(dot(normal, light_dir), 0.0f);
                    float NoV = std::max(dot(normal, view), 0.0f);
                    float lower = 4.0f * (NoL + NoV - (NoL * NoV)) + 0.0001f;
                    gl::vec3 BRDF = upper / lower;
                    color += radiance * BRDF * NoL * vertex_color.rgb();
                }

                vertex.baseColor = gl::vec4(color, vertex.baseColor.a());
            }
        }
    };

    static void simpleTextureMapping(Mesh &mesh, Texture2D &tex, LERP_MODE mode = LERP_MODE::BILINEAR)
    {
        const auto [w, h] = mesh.getResolution();
        for (int i = 0; i <= w; i++)
        {
            for (int j = 0; j <= h; j++)
            {
                auto color = tex.getTexelColor(i / (float)w, j / (float)h, mode);
                mesh.setVertexColor(i, j, gl::vec4(color, 1.0f));
            }
        }
    }

    // assume point light
    static void BlinnPhong(Mesh &mesh, std::vector<Light> lights, PhongMaterial material, gl::vec3 view_pos)
    {
        const auto [w, h] = mesh.getResolution();
        for (int i = 0; i <= w; i++)
        {
            for (int j = 0; j <= h; j++)
            {
                auto vertex_color = mesh.getVertex(i, j).baseColor;
                auto &vertex = mesh.setVertex(i, j);
                auto normal = vertex.normal.normalize();
                auto view = (view_pos - vertex.position).normalize();
                auto color = gl::vec3(0.0f);
                vec3 ambient = material.ka;

                for (auto &light : lights)
                {
                    auto light_dir = (light.position - vertex.position).normalize();
                    auto light_color = light.color;
                    auto diffuse = std::max(0.0f, dot(normal, light_dir));
                    auto specular = std::max(0.0f, dot(normalize(light_dir + view), normal));
                    specular = pow(specular, material.shininess);
                    color += vertex_color.rgb() * light_color * (ambient + diffuse * material.kd + specular * material.ks);
                }

                vertex.baseColor = gl::vec4(color, vertex.baseColor.a());
            }
        }
    };

    // displace the vertex
    static void Displacement(Mesh &mesh, float bumpy_scale = 0.1f)
    {
        const auto [w, h] = mesh.getResolution();
        for (int i = 0; i <= w; i++)
        {
            for (int j = 0; j <= h; j++)
            {
                auto &vertex = mesh.setVertex(i, j);
                vertex.position += vertex.normal * bumpy_scale;
            }
        }
    };

};