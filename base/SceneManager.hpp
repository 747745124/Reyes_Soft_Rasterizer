#pragma once
#include <memory>
#include <any>
#include <stack>
#include <cstdarg>
#include "./camera.hpp"
#include "./framebuffer.hpp"
#include "./mesh.hpp"
#include "./primitive.hpp"
#include "./texture.hpp"
#include "./shader.hpp"
#include "./light.hpp"

enum class CAM_TYPE
{
    PERSPECTIVE,
    ORTHOGRAPHIC
};

enum class DISPLAY_METHOD
{
    IMMEDIATE,
    FILE
};

enum class RENDERING_TARGET
{
    COLOR,
    DEPTH,
    SHADOW
};

enum class ILLUMINATION_TYPE
{
    PHONG,
    PHONG_SHADOW,
    CLOTH,
    PBR
};

enum class SURFACE_TYPE
{
    TEXTURE,
    NORMAL,
    CHECKER,
    DEFAULT
};

enum class DISPLACEMENT_TYPE
{
    PERLIN,
    PERLIN_FRACT,
    DEFAULT
};

// this specifies the context of the rendering
struct Context
{
    ILLUMINATION_TYPE illumination_type = ILLUMINATION_TYPE::PHONG;
    SURFACE_TYPE surface_type = SURFACE_TYPE::DEFAULT;
    DISPLACEMENT_TYPE displacement_type = DISPLACEMENT_TYPE::DEFAULT;

    std::vector<Texture2D> textures = {};
    gl::vec4 color = gl::vec4(1.0f);
    PBRMaterial pbr_material;
    PhongMaterial phong_material;

    uint perlin_tile = 20;
    uint activeTexture = 0;
    float perlin_scale = 0.1f;
    uint checkerBoard_x = 10, checkerBoard_y = 10;

    void setTexture2D(int index, Texture2D &tex)
    {
        assert(index >= 0 && index < textures.size());
        textures[index] = tex;
    }

    void loadTexture(const std::string path, int index = 0)
    {
        Texture2D tex(path);
        textures.push_back(tex);
    };

    Texture2D getTexture2D(int index)
    {
        assert(index >= 0 && index < textures.size());
        return textures[index];
    }

    void setIlluminationType(ILLUMINATION_TYPE type)
    {
        illumination_type = type;
    }

    void setSurfaceType(SURFACE_TYPE type)
    {
        surface_type = type;
    }

    void setDisplacementType(DISPLACEMENT_TYPE type)
    {
        displacement_type = type;
    }

    void setColor(gl::vec4 c)
    {
        color = c;
    }

    void setColor(float r, float g, float b)
    {
        color = gl::vec4(r, g, b, 1.0f);
    }

    void setOpacity(float a)
    {
        color.a() = a;
    }
};

class SceneManager
{
public:
    SceneManager() = default;
    ~SceneManager() = default;
    uint width = 400, height = 400;
    uint spp_x = 2, spp_y = 2;
    uint dice_factor = 4.0f;
    float fovy = gl::to_radian(45.f), znear = 0.1, zfar = 50.0f;

    // output image
    RENDERING_TARGET target = RENDERING_TARGET::COLOR;
    DISPLAY_METHOD display_method = DISPLAY_METHOD::IMMEDIATE;
    Context context;

    char *FILE_NAME = nullptr;

    CAM_TYPE cam_type = CAM_TYPE::PERSPECTIVE;
    Object3D root;
    std::stack<Object3D> _stack;
    std::shared_ptr<Camera> _camera = nullptr;
    std::unique_ptr<OrthographicCamera> _shadow_cam = nullptr;
    std::unique_ptr<FrameBuffer> _framebuffer = nullptr; //
    std::vector<std::unique_ptr<Mesh>> _meshes = {};
    std::vector<Light> _lights = {Light(), Light()};
    TextureShadow _shadow_tex;
    gl::mat4 lightSpaceMatrix;

    void setDicingFactor(uint factor)
    {
        dice_factor = factor;
    }

    void setCAM_TYPE(CAM_TYPE type)
    {
        cam_type = type;
    }

    void setWidth(uint x, uint y)
    {
        width = x;
        height = y;
    }

    void setTarget(RENDERING_TARGET t)
    {
        target = t;
    }

    void setFileName(char *name)
    {
        FILE_NAME = name;
    }

    void setFov(float fov)
    {
        fovy = fov;
    }

    void Shading(Mesh &mesh)
    {
        gl::setColor(mesh, context.color);

        switch (context.displacement_type)
        {
        case DISPLACEMENT_TYPE::PERLIN:
            gl::displacementPerlin(mesh, context.perlin_tile, context.perlin_scale);
            break;
        case DISPLACEMENT_TYPE::PERLIN_FRACT:
            gl::displacementFractalPerlin(mesh, context.perlin_tile, context.perlin_scale);
            break;
        case DISPLACEMENT_TYPE::DEFAULT:
            break;
        }

        switch (context.surface_type)
        {
        case SURFACE_TYPE::TEXTURE:
            gl::simpleTextureMapping(mesh, context.getTexture2D(context.activeTexture), LERP_MODE::BILINEAR);
            break;
        case SURFACE_TYPE::NORMAL:
            gl::visualizeNormal(mesh);
            break;
        case SURFACE_TYPE::CHECKER:
            gl::checkerBoard(mesh, context.checkerBoard_x);
            break;
        case SURFACE_TYPE::DEFAULT:
            gl::setColor(mesh, context.color);
            break;
        }

        switch (context.illumination_type)
        {
        case ILLUMINATION_TYPE::PHONG:
            gl::BlinnPhong(mesh, _lights, context.phong_material, _camera->position);
            break;
        case ILLUMINATION_TYPE::PHONG_SHADOW:
            gl::BlinnPhong(mesh, _lights, context.phong_material, _camera->position, &_shadow_tex, lightSpaceMatrix);
            break;
        case ILLUMINATION_TYPE::CLOTH:
            gl::clothShader(mesh, _lights, context.pbr_material, _camera->position);
            break;
        case ILLUMINATION_TYPE::PBR:
            gl::SimplePBRShader(mesh, _lights, context.pbr_material, _camera->position);
            break;
        }
    }

    void setClippingPlane(float near, float far)
    {
        znear = near;
        zfar = far;
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

    void clothShader(int mesh_index, PBRMaterial &material)
    {
        assert(mesh_index >= 0 && mesh_index < _meshes.size());
        Mesh &mesh = *_meshes[mesh_index];
        gl::clothShader(mesh, _lights, material, _camera->position);
    }

    void SimplePBRShader(int mesh_index, PBRMaterial &material)
    {
        assert(mesh_index >= 0 && mesh_index < _meshes.size());
        Mesh &mesh = *_meshes[mesh_index];
        gl::SimplePBRShader(mesh, _lights, material, _camera->position);
    }

    void setSpp(uint x, uint y)
    {
        spp_x = x;
        spp_y = y;
    }

    void initCamera(CAM_TYPE type = CAM_TYPE::PERSPECTIVE)
    {
        if (type == CAM_TYPE::PERSPECTIVE)
            _camera = std::make_shared<PerspectiveCamera>(fovy, (float)height / (float)width, znear, zfar);
        else
            _camera = std::make_shared<OrthographicCamera>(-30.0f * (float)height / (float)width, 30.0f * (float)height / (float)width, -30.0f, 30.0f, 0.1, 50.f);
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
        this->lightSpaceMatrix = _shadow_cam->getProjectionMat() * gl::getViewMat(gl::vec3(0, 0, 0), gl::vec3(0, 0, 1), gl::vec3(0, 1, 0));
        for (auto &mesh : _meshes)
            _shadow_tex.renderToTextureShadow(*mesh, lightSpaceMatrix);
    };

    void visualizeShadowTex()
    {
        auto img = _shadow_tex.to_cv_mat();
        cv::imshow("shadow", img);
    };

    void initFramebuffer()
    {
        _framebuffer = std::make_unique<FrameBuffer>(width, height, spp_x, spp_y);
    };

    void writeToFile()
    {
        auto img = _framebuffer->to_cv_mat();
        cv::imwrite(FILE_NAME, 255.0f * img);
    }

    void setPixelColorBuffer()
    {
        _framebuffer->setPixelColorFromBuffer();
    };

    void setPixelDepthBuffer()
    {
        _framebuffer->setPixelDepthFromBuffer(znear, zfar);
    };

    void setPixelShadowBuffer()
    {
        for (int i = 0; i < width; i++)
            for (int j = 0; j < height; j++)
            {
                float u = (float)i / (float)width;
                float v = (float)j / (float)height;
                _framebuffer->setPixelColor(i, j, _shadow_tex.getTexelDepth(u, v));
            }
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

    // this one is draw combined shading
    void drawMesh(std::unique_ptr<Mesh> mesh, LERP_MODE mode = LERP_MODE::CORNER)
    {
        Shading(*mesh);
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
        _shadow_tex.renderToTextureShadow(*mesh, lightSpaceMatrix);
        mesh.release();
    }

    void drawMeshesDeferred()
    {
        for (auto &mesh : _meshes)
        {
            drawMeshDeferred(std::move(mesh));
        }
    }

    // this is happened when we separate shading from drawing
    void drawMeshDeferred(std::unique_ptr<Mesh> mesh, LERP_MODE mode = LERP_MODE::CORNER)
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
        mesh.release();
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
        mesh.dice((uint)span.x(), (uint)span.y(), dice_factor);
    };
};
