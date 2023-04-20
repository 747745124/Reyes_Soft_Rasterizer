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
    SceneManager() = default;
    uint width = 400, height = 400;
    uint spp_x = 2, spp_y = 2;
    float fovy = gl::to_radian(45.f), znear = 0.1, zfar = 50.0f;

    std::shared_ptr<Camera> _camera;
    std::unique_ptr<OrthographicCamera> _shadow_cam;
    std::unique_ptr<FrameBuffer> _framebuffer;
    std::vector<std::unique_ptr<Mesh>> _meshes;
    std::vector<Light> _lights;
    TextureShadow _shadow_tex;
    gl::mat4 lightSpaceMatrix;

    void setWidth(uint x, uint y)
    {
        width = x;
        height = y;
    }

    void blinnPhong(int mesh_index, PhongMaterial &material, bool enable_shadow = false)
    {
        assert(mesh_index >= 0 && mesh_index < _meshes.size());
        Mesh &mesh = *_meshes[mesh_index];
        if (!enable_shadow)
            gl::BlinnPhong(mesh, _lights, material, _camera->position);
        else
            gl::BlinnPhong(mesh, _lights, material, _camera->position, &_shadow_tex, lightSpaceMatrix);
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

    void initShadowSetting()
    {
        _shadow_tex = TextureShadow(2000, 2000);
        _shadow_cam = std::make_unique<OrthographicCamera>(-30.0f * (float)width / (float)height, 30.0f * (float)width / (float)height, -30.0f, 30.0f, 0.1, 50.f);
    }

    void generateDepthTex()
    {
        // only use single light for now
        assert(_lights.size() >= 1 && _meshes.size() >= 1);
        this->lightSpaceMatrix = _shadow_cam->getProjectionMat() * gl::getViewMat(_lights[0].position, gl::vec3(0, 0, 1), gl::vec3(0, 1, 0));
        for (auto &mesh : _meshes)
            _shadow_tex.renderToTextureShadow(*mesh, lightSpaceMatrix);
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

    void addLight(Light &light)
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