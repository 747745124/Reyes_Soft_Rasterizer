#pragma once
#include "../base/SceneManager.hpp"
#include <cmath>
#define RI_NULL nullptr
#define RI_PERSPECTIVE "perspective"
#define RI_ORTHOGRAPHIC "orthographic"
using RtFloat = float;
using RtInt = int;
using RtString = const char *;
using RtToken = const char *;
using RtVoid = void;
using RtBoolean = bool;
using RtColor = gl::vec3;
using RtPoint = gl::vec3;

// header file for C interface binding
namespace gl
{
    static std::unique_ptr<SceneManager> _scene_manager = nullptr;

    static void RiBegin(RtToken token = RI_NULL)
    {
        assert(token == RI_NULL && "This token is not supported yet");
        _scene_manager = std::make_unique<SceneManager>();
        // default params
        _scene_manager->setWidth(800, 800);
        _scene_manager->setClippingPlane(0.1, 50.0f);
    };

    static void RiFormat(RtInt width, RtInt height, RtFloat pixel_aspect = 1.0f)
    {
        assert(pixel_aspect == 1.0f && "This pixel aspect ratio is not supported yet");
        _scene_manager->setWidth(height, width);
    };

    static void RiFrameBegin(RtInt frame)
    {
        _scene_manager->initFramebuffer();
        _scene_manager->initShadowSetting();
        _scene_manager->root = Object3D();
    }

    static void RiFrameEnd()
    {
        if (_scene_manager->target == RENDERING_TARGET::DEPTH)
            _scene_manager->setPixelDepthBuffer();
        else if (_scene_manager->target == RENDERING_TARGET::COLOR)
            _scene_manager->setPixelColorBuffer();

        if (_scene_manager->display_method == DISPLAY_METHOD::FILE)
        {
            _scene_manager->writeToFile();
        }
        else
        {
            _scene_manager->showImage();
        }
    }

    // ??
    static void RiFrameAspectRatio(RtFloat aspect)
    {
        _scene_manager->setWidth(_scene_manager->height * aspect, _scene_manager->width);
    };

    static void RiPixelSamples(RtInt x, RtInt y)
    {
        _scene_manager->setSpp(x, y);
    };

    static void RiClipping(RtFloat near, RtFloat far)
    {
        _scene_manager->setClippingPlane(near, far);
    };

    static void RiSphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, ...)
    {
        auto _ = std::make_unique<Sphere>(radius, zmin, zmax, to_radian(thetamax));
        _->position = _scene_manager->root.position;
        _->rotation = _scene_manager->root.rotation;
        _->scale = _scene_manager->root.scale;
        _scene_manager->dice(*_);
        _scene_manager->drawMesh(std::move(_));
    }

    static void RiCylinder(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, ...)
    {
        auto _ = std::make_unique<Cylinder>(radius, zmin, zmax, to_radian(thetamax));
        _->position = _scene_manager->root.position;
        _->rotation = _scene_manager->root.rotation;
        _->scale = _scene_manager->root.scale;
        _scene_manager->dice(*_);
        _scene_manager->drawMesh(std::move(_));
    }

    static void RiCone(RtFloat height, RtFloat radius, RtFloat thetamax, ...)
    {
        auto _ = std::make_unique<Cone>(height, radius, to_radian(thetamax));
        _->position = _scene_manager->root.position;
        _->rotation = _scene_manager->root.rotation;
        _->scale = _scene_manager->root.scale;
        _scene_manager->dice(*_);
        _scene_manager->drawMesh(std::move(_));
    }

    static void RiTorus(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, ...)
    {
        auto _ = std::make_unique<Torus>(majorrad, minorrad, to_radian(phimin), to_radian(phimax), to_radian(thetamax));
        _->position = _scene_manager->root.position;
        _->rotation = _scene_manager->root.rotation;
        _->scale = _scene_manager->root.scale;
        _scene_manager->dice(*_);
        _scene_manager->drawMesh(std::move(_));
    }

    static void RiDisplacement(DISPLACEMENT_TYPE type, uint tile, float scale)
    {
        _scene_manager->context.setDisplacementType(type);
        _scene_manager->context.perlin_tile = tile;
        _scene_manager->context.perlin_scale = scale;
        _scene_manager->context.setDisplacementType(DISPLACEMENT_TYPE::PERLIN);
    }

    static void RiDisplacement(void *type)
    {
        _scene_manager->context.setDisplacementType(DISPLACEMENT_TYPE::DEFAULT);
    }

    static void RiSurface(SURFACE_TYPE type, uint step_size_x = 10, uint step_size_y = 10)
    {
        _scene_manager->context.setSurfaceType(type);
        if (type == SURFACE_TYPE::CHECKER)
        {
            _scene_manager->context.checkerBoard_x = step_size_x;
            _scene_manager->context.checkerBoard_y = step_size_y;
        }
    }

    static void RiPBRMaterial(vec3 albedo, float metallic, float roughness, float ao)
    {
        _scene_manager->context.pbr_material.albedo = albedo;
        _scene_manager->context.pbr_material.metallic = metallic;
        _scene_manager->context.pbr_material.roughness = roughness;
        _scene_manager->context.pbr_material.ao = ao;
    }

    static void RiPhongMaterial(vec3 albedo, float ka, vec3 kd, vec3 ks, float shininess)
    {
        _scene_manager->context.phong_material.albedo = albedo;
        _scene_manager->context.phong_material.ka = ka;
        _scene_manager->context.phong_material.kd = kd;
        _scene_manager->context.phong_material.ks = ks;
        _scene_manager->context.phong_material.shininess = shininess;
    }

    static void RiActiveTexture(uint index)
    {
        _scene_manager->context.activeTexture = index;
    }

    static void RiSurface(void *type)
    {
        _scene_manager->context.setSurfaceType(SURFACE_TYPE::DEFAULT);
    }

    static void RiColor(RtColor color)
    {
        _scene_manager->context.color.r() = color.x();
        _scene_manager->context.color.g() = color.y();
        _scene_manager->context.color.b() = color.z();
    };

    static void RiIlluminate(ILLUMINATION_TYPE type)
    {
        _scene_manager->context.setIlluminationType(type);
    };

    static void RiIlluminate(void *)
    {
        _scene_manager->context.setIlluminationType(ILLUMINATION_TYPE::PHONG);
    };

    static void RiOpacity(RtColor opacity)
    {
        assert(opacity.x() == opacity.y() && opacity.y() == opacity.z() && "Multichannel Opacity is not supported yet");
        _scene_manager->context.setOpacity(opacity.x());
    };

    static void RiProjection(RtToken Type, RtToken Fov, RtFloat *value = nullptr, ...)
    {

        if (strcmp(Type, RI_PERSPECTIVE) == 0)
            _scene_manager->setCAM_TYPE(CAM_TYPE::PERSPECTIVE);
        else if (strcmp(Type, RI_ORTHOGRAPHIC) == 0)
            _scene_manager->setCAM_TYPE(CAM_TYPE::ORTHOGRAPHIC);
        else
            assert(false && "Unknown parameter in RiProjection");

        if (value != nullptr)
            _scene_manager->setFov(*value);
        _scene_manager->initCamera();
    }

    static void RiMakeTexture(const std::string &name, uint index)
    {
        _scene_manager->context.loadTexture(name, index);
    }

    static void RiWorldEnd()
    {
    }

    static void RiDisplay(RtToken name, RtToken type, RtToken mode, ...)
    {
        va_list args;
        va_start(args, mode);

        for (const char *str = mode;
             str != RI_NULL;
             str = va_arg(args, const char *))
        {
            _scene_manager->setFileName((char *)name);

            if (strcmp(type, "file") == 0)
                _scene_manager->display_method = DISPLAY_METHOD::FILE;
            else 
                _scene_manager->display_method = DISPLAY_METHOD::IMMEDIATE;

            if (strcmp(mode, "rgb") == 0)
                _scene_manager->setTarget(RENDERING_TARGET::COLOR);
            else if (strcmp(mode, "z") == 0)
                _scene_manager->setTarget(RENDERING_TARGET::DEPTH);
            else if (strcmp(mode, "shadow") == 0)
                _scene_manager->setTarget(RENDERING_TARGET::SHADOW);
            else if(strcmp(mode, "") == 0)
                _scene_manager->setTarget(RENDERING_TARGET::COLOR);

            // assert(false && "Unknown mode in RiDisplay");
        }
        va_end(args);
    }

    static void RiTranslate(RtFloat dx, RtFloat dy, RtFloat dz)
    {
        _scene_manager->root.position.r() += dx;
        _scene_manager->root.position.g() += dy;
        _scene_manager->root.position.b() += dz;
    }

    // this one takes parameter in degree
    static void RiRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz)
    {
        auto q = gl::Quat::fromAxisAngle(gl::vec3(dx, dy, dz), gl::to_radian(angle));
        _scene_manager->root.rotation = q * _scene_manager->root.rotation;
    }

    static void RiScale(RtFloat sx, RtFloat sy, RtFloat sz)
    {
        _scene_manager->root.scale.r() *= sx;
        _scene_manager->root.scale.g() *= sy;
        _scene_manager->root.scale.b() *= sz;
    }

    static void RiWorldBegin()
    {
        assert(_scene_manager->_camera != nullptr && "Camera is not initialized");
        assert(_scene_manager->_framebuffer != nullptr && "Framebuffer is not initialized");
        assert(_scene_manager->_shadow_cam != nullptr && "Shadow setting is not initialized");
    }

    static void RiEnd()
    {
        _scene_manager = nullptr;
    };

    static void RiTransformBegin()
    {
        _scene_manager->_stack.push(_scene_manager->root);
    }

    static void RiTransformEnd()
    {
        _scene_manager->root = _scene_manager->_stack.top();
        _scene_manager->_stack.pop();
    }
};