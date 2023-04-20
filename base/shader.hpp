#pragma once
#include "./mesh.hpp"
#include "./light.hpp"
#include "./material.hpp"
#include "../utils/utility.hpp"
#include "../utils/pattern.hpp"
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

    static void setColor(Mesh &mesh, gl::vec4 color)
    {
        const auto [w, h] = mesh.getResolution();
        for (int i = 0; i <= w; i++)
        {
            for (int j = 0; j <= h; j++)
            {
                mesh.setVertex(i, j).baseColor = color;
            }
        }
    }

    float filtered_checkerboard(gl::vec2 uv, gl::vec2 du, gl::vec2 dv)
    {
        using namespace gl;
        vec2 kernel = max(abs(du), abs(dv));
        // integral of the kernel
        float ix = (tri_sig(uv + 0.5 * kernel) - tri_sig(uv - 0.5 * kernel)).x() / kernel.u();
        float iy = (tri_sig(uv + 0.5 * kernel) - tri_sig(uv - 0.5 * kernel)).y() / kernel.v();
        return 0.5f - 0.5f * ix * iy;
    }

    float unfiltered_checkerboard(gl::vec2 uv)
    {
        using namespace gl;
        float sx = sign(fract(uv.u() * 0.5f) - 0.5f);
        float sy = sign(fract(uv.v() * 0.5f) - 0.5f);
        return .5f - .5f * sx * sy;
    }

    static void checkerBoard(Mesh &mesh, uint step_size = 10, uint filter_coeff = 4)
    {
        const auto [w, h] = mesh.getResolution();
        for (int i = 0; i <= w; i++)
        {
            for (int j = 0; j <= h; j++)
            {
                float u = (float)i / (float)w;
                float v = (float)j / (float)h;
                float checker = filtered_checkerboard(gl::vec2(u, v) * step_size, gl::vec2((float)(step_size)*filter_coeff / (float)w, 0.0f), gl::vec2(0.0f, (float)(step_size)*filter_coeff / (float)h));
                mesh.setVertexColor(i, j, gl::vec4(vec3(checker), 1.0f));
            }
        }
    }

    // assume point light, Ashikhmin 2007
    static void clothShader(Mesh &mesh, std::vector<std::unique_ptr<Light>>& lights, PBRMaterial material, gl::vec3 view_pos)
    {
        const auto [w, h] = mesh.getResolution();
        for (int i = 0; i <= w; i++)
        {
            for (int j = 0; j <= h; j++)
            {
                auto vertex_color = mesh.getVertex(i, j).baseColor;
                auto &vertex = mesh.setVertex(i, j);
                // world coord

                auto vertex_pos = mesh.getModelMat() * vec4(vertex.position, 1.0f);
                auto normal = (mesh.getModelMat().inverse().transpose() * vec4(vertex.normal, 0.0f)).xyz().normalize();

                auto view = (view_pos - vertex_pos.xyz()).normalize();
                auto color = gl::vec3(0.0f);

                for (auto &light : lights)
                {
                    auto light_dir = normalize(light->position - vertex_pos.xyz());
                    auto halfway = normalize(view + light_dir);
                    float dist = (light->position - vertex_pos.xyz()).length();
                    float attenuation = light->intensity / pow(dist, 2.0f);

                    gl::vec3 radiance = light->color * attenuation;
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

    // visualize local normal
    static void visualizeNormal(Mesh &mesh)
    {
        const auto [w, h] = mesh.getResolution();
        for (int i = 0; i <= w; i++)
        {
            for (int j = 0; j <= h; j++)
            {
                auto color = mesh.getVertex(i, j).normal;
                mesh.setVertexColor(i, j, gl::vec4(color, 1.0f));
            }
        }
    }

    // note that in the primitive, all vertex position are local coord, so we need to transform them to world coord
    static void BlinnPhong(Mesh &mesh, std::vector<std::unique_ptr<Light>>&lights, PhongMaterial material, gl::vec3 view_pos)
    {
        const auto [w, h] = mesh.getResolution();
        for (int i = 0; i <= w; i++)
        {
            for (int j = 0; j <= h; j++)
            {

                auto vertex_color = mesh.getVertex(i, j).baseColor;
                auto &vertex = mesh.setVertex(i, j);

                // world coord
                auto vertex_pos = mesh.getModelMat() * vec4(vertex.position, 1.0f);
                auto normal = (mesh.getModelMat().inverse().transpose() * vec4(vertex.normal, 0.0f)).xyz().normalize();

                auto view = (view_pos - vertex_pos.xyz()).normalize();
                auto color = gl::vec3(0.0f);
                vec3 ambient = material.ka;

                for (auto &light : lights)
                {
                    auto light_dir = (light->position - vertex_pos.xyz()).normalize();
                    auto light_color = light->color;
                    auto diffuse = std::max(0.0f, dot(normal, light_dir));
                    auto specular = std::max(0.0f, dot(normalize(light_dir + view), normal));
                    specular = pow(specular, material.shininess);
                    color += light->intensity * vertex_color.rgb() * light_color * (ambient + diffuse * material.kd + specular * material.ks);
                }

                vertex.baseColor = gl::vec4(color, vertex.baseColor.a());
            }
        }
    };

    // displace the vertex along normal
    static void displacement(Mesh &mesh, float bumpy_scale = 0.1f)
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

    static void displacementPerlin(Mesh &mesh, uint tile = 20, float scale = 0.1f)
    {
        const auto [w, h] = mesh.getResolution();
        for (int i = 0; i <= w; i++)
        {
            for (int j = 0; j <= h; j++)
            {
                float u = (float)i / (float)w;
                float v = (float)j / (float)h;
                u *= tile;
                v *= tile;
                auto &vertex = mesh.setVertex(i, j);
                vertex.position += vertex.normal * gl::perlin_noise_2D(vec2(u, v)) * scale;
            }
        }
    };

};

template <class>
struct Shader;

template <class Fn, class... Args>
struct Shader<Fn(Args...)>
{
public:
    Shader(std::function<Fn(Args...)> func) : f_(func) {}
    std::function<Fn(Args...)> f_;
    void operator()(Args... args)
    {
        f_(args...);
    }
};

template <class Fn, class... Args>
Shader<Fn(Args...)> make_shader(Fn (*f)(Args...))
{
    return Shader<Fn(Args...)>(std::function<Fn(Args...)>(f));
}

