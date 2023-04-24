#include "../utils/Rheader.hpp"
using namespace gl;

void snowman(){
    SceneManager scene;
    Texture2D tex("../assets/textures/earth.jpg");
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
    std::unique_ptr<Sphere> body = std::make_unique<Sphere>(0.8, -0.8, 0.8, gl::to_radian(360.0f));
    std::unique_ptr<Sphere> face = std::make_unique<Sphere>(0.5, -0.5, 0.5, gl::to_radian(360.0f));
    std::unique_ptr<Sphere> left_eye = std::make_unique<Sphere>(0.1, -0.1, 0.1, gl::to_radian(360.0f));
    std::unique_ptr<Sphere> right_eye = std::make_unique<Sphere>(0.1, -0.1, 0.1, gl::to_radian(360.0f));
    std::unique_ptr<Sphere> mouth1 = std::make_unique<Sphere>(0.05, -0.1, 0.1, gl::to_radian(360.0f));
    std::unique_ptr<Sphere> mouth2 = std::make_unique<Sphere>(0.05, -0.1, 0.1, gl::to_radian(360.0f));
    std::unique_ptr<Sphere> mouth3 = std::make_unique<Sphere>(0.05, -0.1, 0.1, gl::to_radian(360.0f));
    std::unique_ptr<Sphere> mouth4 = std::make_unique<Sphere>(0.05, -0.1, 0.1, gl::to_radian(360.0f));
    std::unique_ptr<Cone> nose = std::make_unique<Cone>(0.17, 0.06, gl::to_radian(360.0f));
    std::unique_ptr<Cylinder> hat = std::make_unique<Cylinder>(0.4, 0.f, 0.5, gl::to_radian(360.0f));
    std::unique_ptr<Disk> hat_base = std::make_unique<Disk>(0.2, 0.9f, gl::to_radian(360.0f));
    std::unique_ptr<Disk> bg = std::make_unique<Disk>(0.2, 2.3f, gl::to_radian(360.0f));
    std::unique_ptr<Cylinder> scarf = std::make_unique<Cylinder>(0.68, 0.0f, 0.1, gl::to_radian(360.0f));
    std::unique_ptr<Cylinder> left_hand = std::make_unique<Cylinder>(0.05, 0.0f, 1.5, gl::to_radian(360.0f));
    std::unique_ptr<Cylinder> right_hand = std::make_unique<Cylinder>(0.05, 0.0f, 1.5, gl::to_radian(360.0f));

    Light light, light2, light3;
    PhongMaterial mat;
    PBRMaterial pbr_mat;
    PBRMaterial hat_mat;
    PBRMaterial eye_mat;
    PBRMaterial nose_mat;
    PBRMaterial hand_mat;

    {
        pbr_mat.albedo = gl::vec3(0.5f); // actually it uses per vertex color as albedo
        pbr_mat.roughness = 0.3f;
        pbr_mat.metallic = 0.4f;
        pbr_mat.ao = 0.4f;

        hat_mat.albedo = gl::vec3(0.9f); // actually it uses per vertex color as albedo
        hat_mat.roughness = 0.2f;
        hat_mat.metallic = 1.0f;
        hat_mat.ao = 0.1f;

        eye_mat.albedo = gl::vec3(0.05f); // actually it uses per vertex color as albedo
        eye_mat.roughness = 0.2f;
        eye_mat.metallic = 1.0f;
        eye_mat.ao = 0.1f;

        nose_mat.albedo = gl::vec3(0.9f); // actually it uses per vertex color as albedo
        nose_mat.roughness = 0.1f;
        nose_mat.metallic = 0.4f;
        nose_mat.ao = 0.1f;

        hand_mat.albedo = gl::vec3(0.9f); // actually it uses per vertex color as albedo
        hand_mat.roughness = 0.8f;
        hand_mat.metallic = 0.0f;
        hand_mat.ao = 0.1f;

        gl::Quat q = gl::Quat::fromAxisAngle(gl::vec3(0.0f, 1.0f, 0.0f), gl::to_radian(90.0f));
        gl::Quat q2 = gl::Quat::fromAxisAngle(gl::vec3(1.0f, 0.0f, 0.0f), gl::to_radian(-90.0f));
        gl::Quat q3 = gl::Quat::fromAxisAngle(gl::vec3(1.0f, 0.0f, 0.0f), gl::to_radian(90.0f));
        gl::Quat q4 = gl::Quat::fromAxisAngle(gl::vec3(0.0f, 0.0f, 1.0f), gl::to_radian(45.0f));

        base->scale = gl::vec3(1.f);
        base->position = gl::vec3(0.f, -2.0f, 8.0f);
        base->rotation = q * base->rotation;

        body->scale = gl::vec3(1.f);
        body->position = gl::vec3(0.f, 0.0f, 8.0f);
        body->rotation = q * body->rotation;

        face->scale = gl::vec3(1.f);
        face->position = gl::vec3(0.f, 1.0f, 8.0f);
        face->rotation = q * face->rotation;

        hat->position = gl::vec3(0.0f, 0.8f, 8.0f);
        hat->rotation = q2 * hat->rotation;

        bg->position = gl::vec3(0.0f, 0.8f, 8.0f);
        hat_base->position = gl::vec3(0.0, 0.6f, 8.0f);
        hat_base->rotation = q2 * hat_base->rotation;

        left_eye->scale = gl::vec3(1.0f);
        right_eye->scale = gl::vec3(1.0f);
        left_eye->position = gl::vec3(-0.25f, 0.3f, 7.0f);
        right_eye->position = gl::vec3(0.25f, 0.3f, 7.0f);
        mouth1->position = gl::vec3(-0.25f, -0.13f, 7.0f);
        mouth2->position = gl::vec3(-0.10f, -0.18f, 7.0f);
        mouth3->position = gl::vec3(0.10f, -0.18f, 7.0f);
        mouth4->position = gl::vec3(0.25f, -0.13f, 7.0f);
        nose->position = gl::vec3(0.0f, 0.0f, 6.9f);
        nose->rotation = q2 * nose->rotation;

        scarf->position = gl::vec3(0.0f, -.5f, 7.5f);
        scarf->rotation = q2 * scarf->rotation;
    }

    mat.ka = 0.3f;
    mat.kd = gl::vec3(0.5f);
    mat.ks = gl::vec3(0.5f);
    mat.shininess = 32.0f;

    scene._lights.clear();
    light.position = gl::vec3(-2.f, 3.f, 9.0f);
    light.color = gl::vec3(1.0f, .843f, 0.0f);
    light.intensity = 600.0f;
    light2.position = gl::vec3(2.f, 3.f, 9.0f);
    light2.color = gl::vec3(0.8f, 0.15f, 0.0f);
    light2.intensity = 400.0f;
    light3.position = gl::vec3(0.f, 1.0f, 4.5f);
    light3.color = gl::vec3(1.0f, 1.0f, 1.0f);
    light3.intensity = 160.0f;

    scene.dice(*base);
    scene.dice(*body);
    scene.dice(*face);
    scene.dice(*hat);
    scene.dice(*bg);
    scene.dice(*hat_base);
    scene.dice(*left_eye);
    scene.dice(*right_eye);
    scene.dice(*mouth1);
    scene.dice(*mouth2);
    scene.dice(*mouth3);
    scene.dice(*mouth4);
    scene.dice(*nose);
    scene.dice(*scarf);

    // non-lighting shader
    gl::displacementPerlin(*base, 200U, 0.2f);
    gl::displacementPerlin(*body, 100U, 0.15f);
    gl::displacementPerlin(*nose, 5U, 0.015f);

    gl::setColor(*base, gl::vec4(0.28, 0.38, 1.0f, 1.0f));
    gl::setColor(*body, gl::vec4(0.2, 0.3, 0.7f, 1.0f));
    gl::setColor(*hat, gl::vec4(0.05, 0.05, 0.05f, 1.0f));
    gl::setColor(*bg, gl::vec4(0.95, 0.23, 0.05f, 1.0f));
    gl::setColor(*hat_base, gl::vec4(0.05, 0.05, 0.05f, 1.0f));
    gl::setColor(*left_eye, gl::vec4(0.05, 0.05, 0.05f, 1.0f));
    gl::setColor(*right_eye, gl::vec4(0.05, 0.05, 0.05f, 1.0f));
    gl::setColor(*mouth1, gl::vec4(0.05, 0.05, 0.05f, 1.0f));
    gl::setColor(*mouth2, gl::vec4(0.05, 0.05, 0.05f, 1.0f));
    gl::setColor(*mouth3, gl::vec4(0.05, 0.05, 0.05f, 1.0f));
    gl::setColor(*mouth4, gl::vec4(0.05, 0.05, 0.05f, 1.0f));
    gl::setColor(*nose, gl::vec4(1.0, 0.44, 0.05f, 1.0f));
    gl::setColor(*scarf, gl::vec4(0.68, 0.137, 0.16, 1.0f));

    // finialize mesh,lighting shader starts
    scene.addMesh(std::move(base));
    scene.addMesh(std::move(body));
    scene.addMesh(std::move(hat));
    scene.addMesh(std::move(bg));
    scene.addMesh(std::move(hat_base));
    scene.addMesh(std::move(left_eye));
    scene.addMesh(std::move(right_eye));
    scene.addMesh(std::move(mouth1));
    scene.addMesh(std::move(mouth2));
    scene.addMesh(std::move(mouth3));
    scene.addMesh(std::move(mouth4));
    scene.addMesh(std::move(nose));
    scene.addMesh(std::move(scarf));

    // finalize light
    scene.addLight(light);
    scene.addLight(light2);
    scene.addLight(light3);

    scene.clothShader(0, pbr_mat);
    scene.clothShader(1, pbr_mat);
    scene.SimplePBRShader(2, hat_mat);
    scene.SimplePBRShader(4, hat_mat);
    scene.SimplePBRShader(5, eye_mat);
    scene.SimplePBRShader(6, eye_mat);
    scene.SimplePBRShader(11, nose_mat);
    scene.clothShader(12, pbr_mat);

    scene.drawMeshesDeferred();
    scene.setPixelColorBuffer();
    scene.showImage();
};

int main(){
    snowman();
}