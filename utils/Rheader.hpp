#pragma once
#include "../base/SceneManager.hpp"
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

    static void RiBegin()
    {
        _scene_manager = std::make_unique<SceneManager>();
        // default params
        _scene_manager->setWidth(800, 800);
        _scene_manager->setClippingPlane(0.1, 50.0f);
    };

    static void RiFormat(RtInt width, RtInt height, RtFloat pixel_aspect = 1.0f)
    {
        assert(pixel_aspect == 1.0f && "This pixel aspect ratio is not supported yet");
        _scene_manager->setWidth(width, height);
    };

    //??
    static void RiFrameAspectRatio(RtFloat aspect)
    {
        _scene_manager->setWidth(_scene_manager->width, _scene_manager->width / aspect);
    };

    static void RiPixelSamples(RtInt x, RtInt y)
    {
        _scene_manager->setSpp(x, y);
    };

    static void RiClipping(RtFloat near, RtFloat far)
    {
        _scene_manager->setClippingPlane(near, far);
    };

    static void RiProjection(RtToken name, ...)
    {

        va_list args;
        va_start(args, name);

        for (const char *str = name;
             str != RI_NULL;
             str = va_arg(args, const char *))
        {
            if (strcmp(name, RI_PERSPECTIVE) == 0)
                _scene_manager->setCAM_TYPE(CAM_TYPE::PERSPECTIVE);
            else if (strcmp(name, RI_ORTHOGRAPHIC) == 0)
                _scene_manager->setCAM_TYPE(CAM_TYPE::ORTHOGRAPHIC);
            else if (strcmp(name, "fov") == 0)
            {
                _scene_manager->setFov(*va_arg(args, RtFloat *));
            }
            else
                assert(false && "Unknown parameter in RiProjection");
        }

        va_end(args);
        _scene_manager->initCamera();
    }

    static void RiTranslate(RtFloat dx, RtFloat dy, RtFloat dz)
    {
        _scene_manager->root.position.r() += dx;
        _scene_manager->root.position.g() += dy;
        _scene_manager->root.position.b() += dz;
    }

    //this one takes parameter in degree
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

    static void RiEnd()
    {
        _scene_manager = nullptr;
    };

    
};