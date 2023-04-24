#include "../utils/Rheader.hpp"
using namespace gl;

void Basic_Shadow()
{
    SceneManager scene;
    scene.setSpp(2, 2);
    scene.fovy = gl::to_radian(45.0f);
    scene.znear = 0.1f;
    scene.zfar = 50.0f;
    scene.initCamera(CAM_TYPE::PERSPECTIVE);
    scene.setWidth(800, 800);
    scene.setSpp(2, 2);
    scene.setDicingFactor(5u);
    scene.initCamera();
    scene.initFramebuffer();
    scene.initShadowSetting();

    Sphere sphere(1.0, -1.0, 1.0, gl::to_radian(360.0f));
    gl::Quat q = gl::Quat::fromAxisAngle(gl::vec3(0.0f, 1.0f, 0.0f), gl::to_radian(90.0f));
    sphere.dice(100, 100, 2.f);
    sphere.scale = gl::vec3(1.f);
    sphere.position = gl::vec3(0, 0, 8.0f);
    sphere.rotation = q * sphere.rotation;

    Sphere sphere2(1.0, -1.0, 1.0, gl::to_radian(360.0f));
    sphere2.dice(100, 100, 2.f);
    sphere2.scale = gl::vec3(1.f);
    sphere2.position = gl::vec3(1.3f, 0, 4.0f);
    sphere2.rotation = q * sphere2.rotation;

    scene.dice(sphere);
    scene.dice(sphere2);

    scene.addMesh(std::make_unique<Sphere>(sphere));
    scene.addMesh(std::make_unique<Sphere>(sphere2));

    scene.generateDepthTex();
    
    scene.context.setIlluminationType(ILLUMINATION_TYPE::PHONG_SHADOW);
    scene.context.setColor(gl::vec4(0.28, 0.38, 1.0f, 1.0f));
    scene.drawMesh(std::make_unique<Sphere>(sphere));
    scene.drawMesh(std::make_unique<Sphere>(sphere2));
    scene.setPixelColorBuffer();
    scene.showImage();
}

int main()
{
    Basic_Shadow();
}