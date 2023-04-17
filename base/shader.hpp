#pragma once
#include "./mesh.hpp"
#include "./light.hpp"
#include "./material.hpp"
namespace gl
{   
    static void setOpacity(Mesh &mesh, float opacity){
        const auto [w, h] = mesh.getResolution();
        for (int i = 0; i <= w; i++)
        {
            for (int j = 0; j <= h; j++)
            {
                mesh.setVertex(i,j).baseColor.a() = opacity;
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


    static void BlinnPhong(Mesh& mesh, std::vector<Light> lights,PhongMaterial material,gl::vec3 view_pos){
        const auto [w, h] = mesh.getResolution();
        for (int i = 0; i <= w; i++)
        {
            for (int j = 0; j <= h; j++)
            {
                auto& vertex = mesh.setVertex(i,j);
                auto normal = vertex.normal.normalize();
                auto view = (view_pos-vertex.position).normalize();
                auto color = gl::vec3(0.0f);
                vec3 ambient = material.ka*vertex.baseColor.rgb();

                for(auto& light:lights){
                    auto light_dir = light.position - vertex.position;
                    auto light_color = light.color;
                    auto diffuse = std::max(0.0f, dot(normal, light_dir))*vertex.baseColor.rgb();
                    auto specular = std::max(0.0f, dot(normalize(light_dir + view), normal));
                    specular = pow(specular, material.shininess);
                    color += light_color * (ambient+diffuse * material.kd + specular * material.ks);
                }
                vertex.baseColor = gl::vec4(color, vertex.baseColor.a());
            }
        }
    };

    static void Bumpy(Mesh& mesh){

    };
};