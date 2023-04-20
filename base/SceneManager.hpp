#pragma once
#include <memory>
#include "./camera.hpp"
#include "./framebuffer.hpp"
#include "./mesh.hpp"
#include "./primitive.hpp"
#include "./texture.hpp"
#include "./shader.hpp"
#include "./light.hpp"
#include <any>

class SceneManager
{
public:
    uint width = 400, height = 400;
    uint spp_x = 2, spp_y = 2;
    float fovy = gl::to_radian(45), znear = 0.1, zfar = 100.0;

    std::shared_ptr<Camera> _camera;
    std::unique_ptr<FrameBuffer> _framebuffer;
    std::vector<std::unique_ptr<Mesh>> _meshes;
    std::vector<Light> _lights;

    void setWidth(uint x, uint y)
    {
        width = x;
        height = y;
    }

    void blinnPhong(Mesh &mesh, PhongMaterial &material)
    {
        gl::BlinnPhong(mesh, _lights, material, _camera->position);
    }

    void clothShader(Mesh &mesh, PBRMaterial &material)
    {
        gl::clothShader(mesh, _lights, material, _camera->position);
    }

    void setSpp(uint x, uint y)
    {
        spp_x = x;
        spp_y = y;
    }

    void initCamera()
    {
        _camera = std::make_shared<PerspectiveCamera>(fovy, (float)width / (float)height, znear, zfar);
    };

    void initFramebuffer()
    {
        _framebuffer = std::make_unique<FrameBuffer>(width, height, spp_x, spp_y);
    };

    void setPixelColorBuffer()
    {
        _framebuffer->setPixelColorFromBuffer();
    };

    void showImage()
    {
        auto img = _framebuffer->to_cv_mat();
        cv::imshow("image", img);
        cv::waitKey();
    }

    void drawMeshes()
    {
        for (auto &mesh : _meshes)
        {
            drawMesh(std::move(mesh));
        }
    }

    void drawMesh(std::unique_ptr<Mesh> mesh, LERP_MODE mode = LERP_MODE::CORNER)
    {
        auto mvp = getMVP(*mesh);
        const auto [w, h] = mesh->getResolution();
        for (int i = 0; i < w; i++)
        {
            for (int j = 0; j < h; j++)
            {
                auto mp = mesh->getMicropolygon(i, j);
                _framebuffer->updateBufferByMicropolygon(mp, mvp, LERP_MODE::CORNER);
            }
        }
    }

    void addMesh(std::unique_ptr<Mesh> mesh)
    {
        _meshes.push_back(std::move(mesh));
    }

    void addLight(Light& light)
    {
        _lights.push_back(light);
    }

    gl::mat4 getMVP(Mesh &mesh)
    {
        return _camera->getProjectionMat() * _camera->getViewMat() * mesh.getModelMat();
    };

    void dice(Mesh &mesh)
    {
        auto mvp = getMVP(mesh);
        auto span = mesh.getProjectedBoundingVolumeSpan(mvp, width, height);
        mesh.dice((uint)span.x(), (uint)span.y());
    };
};