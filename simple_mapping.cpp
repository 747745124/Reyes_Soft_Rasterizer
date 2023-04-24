#include "./unit_test.hpp"

int main()
{
    SceneManager scene;
    Texture2D tex("../assets/textures/earth.jpg");
    std::cout << "Hi" << std::endl;
    scene.fovy = gl::to_radian(45.0f);
    scene.znear = 0.1f;
    scene.zfar = 50.0f;
    scene.setWidth(800, 800);
    scene.setSpp(2, 2);
    scene.setDicingFactor(5u);
    scene.initCamera();
    scene.initFramebuffer();
    scene.initShadowSetting();
    std::unique_ptr<Sphere> base = std::make_unique<Sphere>(1.5, -1.5, 1.5, gl::to_radian(360.0f));
    gl::Quat q = gl::Quat::fromAxisAngle(gl::vec3(1.0f, 0.0f, 0.0f), gl::to_radian(90.0f));

    base->scale = gl::vec3(1.f);
    base->position = gl::vec3(0.f, 1.0f, 8.0f);
    base->rotation = q * base->rotation;

    scene.dice(*base);
    gl::setColor(*base, gl::vec4(0.28, 0.38, 1.0f, 1.0f));
    gl::simpleTextureMapping(*base, tex);


    scene.addMesh(std::move(base));
    scene.drawMeshesDeferred();
    scene.setPixelColorBuffer();
    scene.showImage();
}