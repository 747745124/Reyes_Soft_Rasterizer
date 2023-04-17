#pragma once
#include <memory>
#include "./camera.hpp"
#include "./framebuffer.hpp"
#include "./mesh.hpp"
#include "./primitive.hpp"
#include "./texture.hpp"
#include "./shader.hpp"
#include "./light.hpp"

class SceneManager
{
public:
    std::unique_ptr<Camera> _camera;
    std::unique_ptr<FrameBuffer> _framebuffer;

    // a singleton contains all the world information, passed to shaders
    struct Scene
    {
        std::vector<std::unique_ptr<Mesh>> _meshes;
        std::vector<std::unique_ptr<Light>> _lights;
    };
};