#include "./unit_test.hpp"
//  #define TEST_API
//  #define TEST_SCENE_MANAGER
//  #define TEST_TEXTURE_SHADOW
//  #define TEST_PRIMITIVE_NORMAL
//  #define TEST_ZBUFFER_VIS
//  #define TEST_PERLIN_DISPLACEMENT
//  #define TEST_PERLIN
//  #define TEST_FILTER_CHECKER
//  #define TEST_TEXTURE
//  #define TEST_PLANE
//  #define TEST_PHONG
//  #define TEST_DICING
//  #define TEST_ALPHA
//  #define TEST_BILINEAR
//  #define TEST_SAMPLING
//  #define TEST_SAMPLE_VIS
//  #define TEST_FRAMEBUFFER
//  #define TEST_MVP
//  #define TEST_VEC
int main()
{

#ifdef TEST_API
    using namespace gl;
    RiBegin();
    RiFormat(400, 400, 1.0);
    RiPixelSamples(1, 1);
    RiFrameAspectRatio(4.0 / 3.0);
    RiFrameBegin(0);
    /* set the perspective transformation */
    float fov = 45.0;
    RiProjection(RI_PERSPECTIVE, "fov", &fov);
    RiTranslate(0, 0, 40.0);
    RiWorldBegin();
    RiSphere(5.0, 5.0, 5.0, 2 * M_PI);
    RiWorldEnd();
    RiFrameEnd();
    RiEnd();
#endif

#ifdef TEST_SCENE_MANAGER
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
#endif

#ifdef TEST_TEXTURE_SHADOW
    TextureShadow tex(2000, 2000);
    // OrthographicCamera shadow_cam(-10.f, 10.f, -10.f, 10.f, 0.1f, 50.f);
    PerspectiveCamera shadow_cam(gl::to_radian(45.f), 1.0f, 7.f, 9.0f);
    PerspectiveCamera cam(gl::to_radian(45.f), 1.0f, 0.1f, 50.0f);
    FrameBuffer fb(400, 400, 2, 2);

    Light light;
    light.position = gl::vec3(0.f, 0.0f, 0.f);
    shadow_cam.position = light.position;

    Sphere sphere(1.0, -1.0, 1.0, gl::to_radian(360.0f));
    gl::Quat q = gl::Quat::fromAxisAngle(gl::vec3(0.0f, 1.0f, 0.0f), gl::to_radian(90.0f));
    sphere.dice(100, 100, 2.f);
    sphere.scale = gl::vec3(1.f);
    sphere.position = gl::vec3(0, 0, 8.0f);
    sphere.rotation = q * sphere.rotation;

    Sphere sphere2(1.0, -1.0, 1.0, gl::to_radian(360.0f));
    sphere2.dice(100, 100, 2.f);
    sphere2.scale = gl::vec3(1.f);
    sphere2.position = gl::vec3(1.5f, 0, 4.0f);
    sphere2.rotation = q * sphere2.rotation;

    gl::mat4 lightspace = shadow_cam.getProjectionMat() * gl::getViewMat(light.position, gl::vec3(0, 0, 8.f), gl::vec3(0, 1, 0));
    gl::mat4 mvp1 = cam.getProjectionMat() * cam.getViewMat() * sphere.getModelMat();
    gl::mat4 mvp2 = cam.getProjectionMat() * cam.getViewMat() * sphere2.getModelMat();
    tex.renderToTextureShadow(sphere, lightspace);
    // tex.renderToTextureShadow(sphere2, lightspace);

    auto tmp = tex.to_cv_mat();
    cv::imshow("image", tmp);
    cv::waitKey();

    {
        PhongMaterial mat;
        mat.ka = 0.15f;
        mat.kd = gl::vec3(0.5f);
        mat.ks = gl::vec3(0.5f);
        mat.shininess = 32.0f;
        std::vector<Light> lights = {light};

        gl::BlinnPhong(sphere, lights, mat, cam.position, &tex, lightspace);
        gl::BlinnPhong(sphere2, lights, mat, cam.position, &tex, lightspace);
    }

    {
        // loop over micropolygons of sphere
        const auto [w, h] = sphere.getResolution();
        for (int i = 0; i < w; i++)
        {
            for (int j = 0; j < h; j++)
            {
                auto mp = sphere.getMicropolygon(i, j);
                fb.updateBufferByMicropolygon(mp, mvp1, LERP_MODE::CORNER);
            }
        }
    }

    const auto [w, h] = sphere2.getResolution();
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            auto mp = sphere2.getMicropolygon(i, j);
            fb.updateBufferByMicropolygon(mp, mvp2, LERP_MODE::CORNER);
        }
    }

    fb.setPixelColorFromBuffer();
    auto img = fb.to_cv_mat();
    cv::imshow("image", img);
    cv::waitKey();
#endif

#ifdef TEST_PRIMITIVE_NORMAL
    // linearize depth visualize
    uint width = 800;
    uint height = 800;

    PerspectiveCamera p_cam(gl::to_radian(45.0f), (float)width / (float)height, 0.1f, 50.0f);

    FrameBuffer fb(width, height, 1, 1);
    Hyperboloid hyper({-1, -1, -1}, {1, 1, 1}, gl::to_radian(360.f));
    Sphere sphere(1.0, -1.0, 1.0, gl::to_radian(360.0f));

    {
        hyper.scale = gl::vec3(1.f);
        hyper.position = gl::vec3(1.0f, 0.0f, 8.0f);
        gl::Quat q = gl::Quat::fromAxisAngle(gl::vec3(0.0f, 1.0f, 0.0f), gl::to_radian(45.0f));
        gl::Quat q2 = gl::Quat::fromAxisAngle(gl::vec3(1.0f, 0.0f, 0.0f), gl::to_radian(0.0f));
        hyper.rotation = q2 * q * hyper.rotation;
    }

    gl::mat4 mvp_hp = p_cam.getProjectionMat() * p_cam.getViewMat() * hyper.getModelMat();
    {
        auto span = hyper.getProjectedBoundingVolumeSpan(mvp_hp, width, height);
        hyper.dice((uint)span.x(), (uint)span.y(), 1.0f);
        gl::visualizeNormal(hyper);
    }

    fb.clearColor();
    fb.clearBuffer(1.0f, gl::vec4(gl::vec3(0.0f), 1.0f));

    {
        PhongMaterial mat;
        mat.ka = 0.2f;
        mat.kd = gl::vec3(0.5f);
        mat.ks = gl::vec3(0.5f);
        mat.shininess = 32.0f;

        Light light1, light2;
        light1.position = gl::vec3(-4.f, 2.f, 1.0f);
        light1.color = gl::vec3(1.0f, 1.f, 1.f);
        light2.position = gl::vec3(0.f, 0.f, 2.0f);
        light2.color = gl::vec3(1.0f, 1.0f, 1.0f);

        gl::BlinnPhong(hyper, std::vector<Light>{light1}, mat, p_cam.position);
    }

    {
        // loop over micropolygons of primitive
        const auto [w, h] = hyper.getResolution();
        for (int i = 0; i < w; i++)
        {
            for (int j = 0; j < h; j++)
            {
                auto mp = hyper.getMicropolygon(i, j);
                fb.updateBufferByMicropolygon(mp, mvp_hp, LERP_MODE::CORNER);
            }
        }
    }

    fb.setPixelColorFromBuffer();
    auto img = fb.to_cv_mat();
    cv::imshow("image", img);
    cv::waitKey();
#endif

#ifdef TEST_ZBUFFER_VIS
    // linearize depth visualize
    uint width = 800;
    uint height = 800;

    PerspectiveCamera p_cam(gl::to_radian(45.0f), (float)width / (float)height, 0.1f, 50.0f);

    // MSAA 4x
    FrameBuffer fb(width, height, 2, 2);
    Sphere sphere(1.0, -1.0, 1.0, gl::to_radian(360.0f));

    sphere.scale = gl::vec3(2.f);
    sphere.position = gl::vec3(0.0f, 0.0f, 8.0f);
    gl::Quat q = gl::Quat::fromAxisAngle(gl::vec3(0.0f, 1.0f, 0.0f), gl::to_radian(90.0f));
    sphere.rotation = q * sphere.rotation;

    gl::mat4 mvp = p_cam.getProjectionMat() * p_cam.getViewMat() * sphere.getModelMat();
    auto span = sphere.getProjectedBoundingVolumeSpan(mvp, width, height);
    sphere.dice((uint)span.x(), (uint)span.y(), 2.0f);

    fb.clearColor();
    fb.clearBuffer(1.0f, gl::vec4(gl::vec3(0.0f), 1.0f));

    // loop over micropolygons of sphere
    const auto [w, h] = sphere.getResolution();
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            auto mp = sphere.getMicropolygon(i, j);
            fb.updateBufferByMicropolygon(mp, mvp, LERP_MODE::CORNER);
        }
    }

    fb.setPixelDepthFromBuffer(1.f, 50.f);
    // fb.setPixelColorFromBuffer();
    auto img = fb.to_cv_mat();
    cv::imshow("image", img);
    cv::waitKey();
#endif

// 3 different shaders to play with
#ifdef TEST_PERLIN_DISPLACEMENT
    Texture2D tex("../assets/textures/aki.jpeg", CV_32FC3);
    uint width = 800;
    uint height = 800;

    PerspectiveCamera p_cam(gl::to_radian(45.0f), (float)width / (float)height, 0.1f, 100.0f);

    // MSAA 4x
    FrameBuffer fb(width, height, 4, 4);
    Sphere sphere(1.0, -1.0, 1.0, gl::to_radian(360.0f));

    sphere.scale = gl::vec3(0.9f);
    sphere.position = gl::vec3(0.0f, 0.0f, 6.0f);
    gl::Quat q = gl::Quat::fromAxisAngle(gl::vec3(0.0f, 1.0f, 0.0f), gl::to_radian(90.0f));
    sphere.rotation = q * sphere.rotation;

    gl::mat4 mvp = p_cam.getProjectionMat() * p_cam.getViewMat() * sphere.getModelMat();
    auto span = sphere.getProjectedBoundingVolumeSpan(mvp, width, height);
    sphere.dice((uint)span.x(), (uint)span.y(), 10.0f);

    {
        PhongMaterial mat;
        mat.ka = 0.3f;
        mat.kd = gl::vec3(0.5f);
        mat.ks = gl::vec3(0.5f);
        mat.shininess = 32.0f;

        PBRMaterial pbr_mat;
        pbr_mat.roughness = 0.2f;
        pbr_mat.metallic = 0.4f;

        Light light1, light2, light3, light4;
        light1.position = gl::vec3(-2.0f, -1.0f, 3.0f);
        light1.color = gl::vec3(1.0f);
        light1.intensity = 200.0f;
        light2.position = gl::vec3(0.0f, 0.0f, 1.0f);
        light2.color = gl::vec3(1.0f, 1.0f, 1.0f);
        light2.intensity = 40.0f;
        light3.position = gl::vec3(3.0f, -3.0f, 2.0f);
        light3.color = gl::vec3(1.0f, 1.0f, 1.0f);
        light3.intensity = 40.0f;
        light4.position = gl::vec3(1.0f, 0.0f, 2.0f);
        light4.color = gl::vec3(1.0f, 1.0f, 1.0f);
        light4.intensity = 200.0f;

        gl::displacementPerlin(sphere, 400, 0.4f);
        gl::setColor(sphere, gl::vec4(0.28, 0.38, 1.0f, 1.0f));
        // gl::SimplePBRShader(sphere, std::vector<Light>{light1, light2, light3, light4}, pbr_mat, p_cam.position);
        gl::clothShader(sphere, std::vector<Light>{light1, light2, light3, light4}, pbr_mat, p_cam.position);
        // gl::BlinnPhong(sphere, std::vector<Light>{light1, light2, light3, light4}, mat, p_cam.position);
    }

    fb.clearColor();
    fb.clearBuffer(1.0f, gl::vec4(gl::vec3(0.0f), 1.0f));

    // loop over micropolygons of sphere
    const auto [w, h] = sphere.getResolution();
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            auto mp = sphere.getMicropolygon(i, j);
            fb.updateBufferByMicropolygon(mp, mvp, LERP_MODE::CORNER);
        }
    }

    fb.setPixelColorFromBuffer();
    auto img = fb.to_cv_mat();
    cv::imshow("image", img);
    cv::waitKey();
#endif

#ifdef TEST_PERLIN
    uint width = 400;
    uint height = 400;

    PerspectiveCamera p_cam(gl::to_radian(45.0f), (float)width / (float)height, 0.1f, 100.0f);
    FrameBuffer fb(width, height, 1, 1);

    fb.clearColor();
    fb.clearBuffer(1.0f, gl::vec4(gl::vec3(0.0f), 1.0f));

    for (uint i = 0; i < width; i++)
    {
        for (uint j = 0; j < height; j++)
        {
            float x = 8.f * (float)i / width;
            float y = 8.f * (float)j / height;
            float noise = gl::fractal_perlin_2D({x, y}, 4);
            gl::vec3 color = gl::vec3(noise);
            fb.setPixelColor(i, j, color);
        }
    }

    auto img = fb.to_cv_mat();
    cv::imshow("image", img);
    cv::waitKey();
#endif

#ifdef TEST_FILTER_CHECKER
    Texture2D tex("../assets/textures/aki.jpeg", CV_32FC3);
    uint width = 600;
    uint height = 600;

    PerspectiveCamera p_cam(gl::to_radian(45.0f), (float)width / (float)height, 0.1f, 100.0f);

    // MSAA 4x
    FrameBuffer fb(width, height, 2, 2);
    Sphere sphere(1.0, -1.0, 1.0, gl::to_radian(360.0f));

    sphere.scale = gl::vec3(0.9f);
    sphere.position = gl::vec3(1.0f, 0.0f, 6.0f);
    gl::Quat q = gl::Quat::fromAxisAngle(gl::vec3(0.0f, 1.0f, 0.0f), gl::to_radian(-90.0f));
    sphere.rotation = q * sphere.rotation;

    gl::mat4 mvp = p_cam.getProjectionMat() * p_cam.getViewMat() * sphere.getModelMat();
    auto span = sphere.getProjectedBoundingVolumeSpan(mvp, width, height);
    sphere.dice((uint)span.x(), (uint)span.y(), 3.0f);

    {
        PhongMaterial mat;
        mat.ka = 0.15f;
        mat.kd = gl::vec3(0.5f);
        mat.ks = gl::vec3(0.5f);
        mat.shininess = 32.0f;

        Light light1, light2;
        light1.position = gl::vec3(6.0f, 4.0f, 2.0f);
        light1.color = gl::vec3(1.0f);
        light2.position = gl::vec3(20.0f, 1.0f, 5.0f);
        light2.color = gl::vec3(1.0f, 0.0f, 1.0f);

        gl::checkerBoard(sphere, 80);
        gl::displacement(sphere, 1.0f);
        gl::BlinnPhong(sphere, std::vector<Light>{light1, light2}, mat, p_cam.position);
    }

    fb.clearColor();
    fb.clearBuffer(1.0f, gl::vec4(gl::vec3(0.0f), 1.0f));

    // loop over micropolygons of sphere
    const auto [w, h] = sphere.getResolution();
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            auto mp = sphere.getMicropolygon(i, j);
            fb.updateBufferByMicropolygon(mp, mvp, LERP_MODE::CORNER);
        }
    }

    fb.setPixelColorFromBuffer();
    auto img = fb.to_cv_mat();
    cv::imshow("image", img);
    cv::waitKey();
#endif

#ifdef TEST_PHONG
    Texture2D tex("../assets/textures/aki.jpeg", CV_32FC3);
    uint width = 800;
    uint height = 800;

    PerspectiveCamera p_cam(gl::to_radian(45.0f), (float)width / (float)height, 0.1f, 100.0f);

    // MSAA 4x
    FrameBuffer fb(width, height, 1, 1);
    Sphere sphere(1.0, -1.0, 1.0, gl::to_radian(360.0f));

    sphere.scale = gl::vec3(1.4f);
    sphere.position = gl::vec3(0.0f, 0.0f, 6.0f);
    gl::Quat q = gl::Quat::fromAxisAngle(gl::vec3(0.0f, 1.0f, 0.0f), gl::to_radian(135.0f));
    sphere.rotation = q * sphere.rotation;

    gl::mat4 mvp = p_cam.getProjectionMat() * p_cam.getViewMat() * sphere.getModelMat();
    auto span = sphere.getProjectedBoundingVolumeSpan(mvp, width, height);
    sphere.dice((uint)span.x(), (uint)span.y(), 1.0f);

    // gl::checkerBoard(sphere, 60);
    // gl::setOpacity(sphere, 0.1f);
    {
        PhongMaterial mat;
        mat.ka = 0.15f;
        mat.kd = gl::vec3(0.2f);
        mat.ks = gl::vec3(0.5f);
        mat.shininess = 32.0f;

        PBRMaterial pbr_mat;
        pbr_mat.albedo = gl::vec3(0.5f); // actually it uses per vertex color as albedo
        pbr_mat.roughness = 0.3f;
        pbr_mat.metallic = 0.5f;
        pbr_mat.ao = 1.0f;

        Light light1, light2, light3, light4;
        light1.position = gl::vec3(-1.0f, 0.0f, 7.0f);
        light1.color = gl::vec3(1.0f);
        light1.intensity = 10.f;

        light2.position = gl::vec3(-0.0f, 1.0f, 4.0f);
        light2.color = gl::vec3(1.0f, 1.0f, 1.0f);
        light2.intensity = 10.f;

        light3.position = gl::vec3(-0.0f, 0.0f, 2.0f);
        light3.color = gl::vec3(1.0f, 1.0f, 1.0f);
        light3.intensity = 10.f;

        light4.position = gl::vec3(-0.5f, -0.0f, 4.0f);
        light4.color = gl::vec3(1.0f, 1.0f, 1.0f);
        light4.intensity = 20.f;

        gl::simpleTextureMapping(sphere, tex);
        gl::BlinnPhong(sphere, std::vector<Light>{light1}, mat, p_cam.position);
        // gl::clothShader(sphere, std::vector<Light>{light1, light2, light3, light4}, pbr_mat, p_cam.position);
    }

    fb.clearColor();
    // fb.clearBuffer(1.0f, gl::vec4(gl::vec3(0.4, 0.6, 0.2), 1.0));

    // loop over micropolygons of sphere
    const auto [w, h] = sphere.getResolution();
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            auto mp = sphere.getMicropolygon(i, j);
            fb.updateBufferByMicropolygon(mp, mvp);
        }
    }

    fb.setPixelColorFromBuffer();
    auto img = fb.to_cv_mat();
    cv::imshow("image", img);
    cv::waitKey();

#endif

#ifdef TEST_PLANE
    {
        using namespace gl;
        vec3 p1(1.0f, 1.0f, 1.0f);
        vec3 p2(0.0f, 0.0f, 0.0f);
        vec3 p3(0.0f, 1.0f, 0.0f);

        Plane plane(p1, p2, p3);
        // std::cout << plane.get_x(0.0,0.5) << std::endl;
    }
#endif

#ifdef TEST_TEXTURE
    {
        Texture2D tex("../assets/textures/abstract_wind_tex.png", CV_32FC3);
        uint width = 800;
        uint height = 800;

        PerspectiveCamera p_cam(gl::to_radian(45.0f), (float)width / (float)height, 0.1f, 100.0f);

        // MSAA 4x
        FrameBuffer fb(width, height, 2, 2);
        Sphere sphere(1.0, -1.0, 1.0, gl::to_radian(360.0f));

        sphere.scale = gl::vec3(0.9f);
        sphere.position = gl::vec3(0.0f, 0.0f, 6.0f);
        gl::Quat q = gl::Quat::fromAxisAngle(gl::vec3(0.0f, 1.0f, 0.0f), gl::to_radian(90.0f));
        // gl::Quat q2 = gl::Quat::fromAxisAngle(gl::vec3(0.0f,0.0f,1.0f), gl::to_radian(90.0f));
        // gl::Quat q3 = gl::Quat::fromAxisAngle(gl::vec3(1.0f, 0.0f, 0.0f), gl::to_radian(180.0f));
        // sphere.rotation = q * sphere.rotation;

        gl::mat4 mvp = p_cam.getProjectionMat() * p_cam.getViewMat() * sphere.getModelMat();
        auto span = sphere.getProjectedBoundingVolumeSpan(mvp, width, height);
        sphere.dice((uint)span.x(), (uint)span.y(), 2.0f);

        // apply texture
        {
            const auto [w, h] = sphere.getResolution();
            for (int i = 0; i <= w; i++)
            {
                for (int j = 0; j <= h; j++)
                {
                    auto color = tex.getTexelColor(i / (float)w, j / (float)h, LERP_MODE::BILINEAR);
                    sphere.setVertexColor(i, j, gl::vec4(color, 1.0f));
                }
            }
        }

        fb.clearColor();
        fb.clearBuffer(1.0f, gl::vec4(gl::vec3(0.4, 0.6, 0.2), 1.0));

        // loop over micropolygons of sphere
        const auto [w, h] = sphere.getResolution();
        for (int i = 0; i < w; i++)
        {
            for (int j = 0; j < h; j++)
            {
                auto mp = sphere.getMicropolygon(i, j);
                fb.updateBufferByMicropolygon(mp, mvp);
            }
        }

        fb.setPixelColorFromBuffer();
        auto img = fb.to_cv_mat();
        cv::imshow("image", img);
        cv::waitKey();
    }
#endif

    //////////////////////////////////////////
    // Below are general test code for Adaptive Dicing
    //////////////////////////////////////////

#ifdef TEST_DICING
    {
        uint width = 800;
        uint height = 800;

        PerspectiveCamera p_cam(gl::to_radian(45.0f), (float)width / (float)height, 0.1f, 100.0f);

        // MSAA 4x
        FrameBuffer fb(width, height, 2, 2);
        Sphere sphere(1.0, -1.0, 1.0, gl::to_radian(360.0f));

        {
            sphere.scale = gl::vec3(0.9f);
            sphere.position = gl::vec3(0.0f, 0.0f, 6.0f);
            gl::Quat q = gl::Quat::fromAxisAngle(gl::vec3(0.0f, 1.0f, 0.0f), gl::to_radian(90.0f));
            sphere.rotation = q * sphere.rotation;

            {
                gl::mat4 mvp = p_cam.getProjectionMat() * p_cam.getViewMat() * sphere.getModelMat();
                auto span = sphere.getProjectedBoundingVolumeSpan(mvp, width, height);
                sphere.dice((uint)span.x(), (uint)span.y(), 1.f);
            }
        }

        Cylinder cylinder(1.0, 0.0, 1.0, gl::to_radian(360.0f));

        {
            cylinder.scale = gl::vec3(0.7f);
            cylinder.position = gl::vec3(-0.0f, 0.f, 3.5f);
            gl::Quat q = gl::Quat::fromAxisAngle(gl::vec3(0.0f, 1.0f, 0.0f), gl::to_radian(.0f));
            cylinder.rotation = q * sphere.rotation;

            {
                gl::mat4 mvp = p_cam.getProjectionMat() * p_cam.getViewMat() * cylinder.getModelMat();
                auto span = cylinder.getProjectedBoundingVolumeSpan(mvp, width, height);
                cylinder.dice((uint)span.x(), (uint)span.y(), 2.f);
            }
        }

        fb.clearColor();
        fb.clearBuffer(1.0f, gl::vec4(gl::vec3(0.4, 0.6, 0.2), 1.0));

        // loop over micropolygons of sphere
        {
            gl::mat4 mvp = p_cam.getProjectionMat() * p_cam.getViewMat() * sphere.getModelMat();
            const auto [w, h] = sphere.getResolution();
            for (int i = 0; i < w; i++)
            {
                for (int j = 0; j < h; j++)
                {
                    auto mp = sphere.getMicropolygon(i, j);
                    fb.updateBufferByMicropolygon(mp, mvp);
                }
            }
        }

        // loop over micropolygons of cylinder
        {
            gl::mat4 mvp = p_cam.getProjectionMat() * p_cam.getViewMat() * cylinder.getModelMat();
            const auto [w, h] = cylinder.getResolution();
            for (int i = 0; i < w; i++)
            {
                for (int j = 0; j < h; j++)
                {
                    auto mp = cylinder.getMicropolygon(i, j);
                    fb.updateBufferByMicropolygon(mp, mvp);
                }
            }
        }

        fb.setPixelColorFromBuffer();
        auto img = fb.to_cv_mat();
        cv::imshow("image", img);
        cv::waitKey();
    }
#endif

    //////////////////////////////////////////
    // Below are general test code for Alpha Blending
    //////////////////////////////////////////

#ifdef TEST_ALPHA

    {

        uint width = 400;
        uint height = 400;

        PerspectiveCamera p_cam(gl::to_radian(45.0f), (float)width / (float)height, 0.1f, 100.0f);

        // MSAA
        FrameBuffer fb(width, height, 2, 2);

        Sphere sphere(1.0, -1.0, 1.0, gl::to_radian(360.0f), 400, 400);
        Cylinder cylinder(1.0, -1.0, 1.0, gl::to_radian(360.0f), 30, 30);

        {
            sphere.scale = gl::vec3(0.9f);
            sphere.position = gl::vec3(0.0f, 0.0f, 2.0f);
            gl::Quat q = gl::Quat::fromAxisAngle(gl::vec3(0.0f, 1.0f, 0.0f), gl::to_radian(90.0f));
            sphere.rotation = q * sphere.rotation;
        }

        {
            cylinder.scale = gl::vec3(0.3f);
            cylinder.position = gl::vec3(-0.0f, 0.f, 3.5f);
            gl::Quat q = gl::Quat::fromAxisAngle(gl::vec3(0.0f, 1.0f, 0.0f), gl::to_radian(.0f));
            cylinder.rotation = q * sphere.rotation;
        }

        gl::mat4 mvp_sp = p_cam.getProjectionMat() * p_cam.getViewMat() * sphere.getModelMat();
        gl::mat4 mvp_cy = p_cam.getProjectionMat() * p_cam.getViewMat() * cylinder.getModelMat();

        gl::setOpacity(sphere, 0.6f);
        gl::checkerBoard(sphere, 10u, 2.0);

        fb.clearColor();
        fb.clearBuffer(1.0f, gl::vec4(gl::vec3(0.4, 0.6, 0.2), 1.0));

        // loop over micropolygons of sphere
        {
            const auto [w, h] = sphere.getResolution();
            for (int i = 0; i < w; i++)
            {
                for (int j = 0; j < h; j++)
                {

                    auto mp_sp = sphere.getMicropolygon(i, j);
                    fb.updateBufferByMicropolygon(mp_sp, mvp_sp);
                }
            }
        }

        fb.setPixelColorFromBuffer();
        auto img = fb.to_cv_mat();
        cv::imshow("image", img);
        cv::waitKey();
    }

#endif

    //////////////////////////////////////////
    // Below are general test code for Bilinear Interpolation
    //////////////////////////////////////////

#ifdef TEST_BILINEAR
    {
        using namespace gl;
        {
            vec3 x2y2(1.0f, 1.0f, 1.0f);
            vec3 x2y1(1.0f, 0.0f, 0.0f);
            vec3 x1y2(0.0f, 1.0f, 0.0f);
            vec3 x1y1(0.0f, 0.0f, 1.0f);

            vec2 screen_coord(0.5, 0.5);

            std::cout << inverseBilinear(screen_coord, x1y1.xy(), x2y1.xy(), x2y2.xy(), x1y2.xy()) << std::endl;
            std::cout << get_depth_bilinear(screen_coord, x1y1, x2y1, x2y2, x1y2) << std::endl;
        }

        {
            vec2 p1(0, 3);
            vec2 p2(3, 4);
            vec2 p3(3, 0);
            vec2 p4(0, 1);

            vec2 screen_coord(1, 2);
            auto uv = inverseBilinear(screen_coord, p1, p2, p3, p4);
            std::cout << uv << std::endl;
            std::cout << bilinear(uv, p1, p2, p3, p4) << std::endl;
        }
    }
#endif

    //////////////////////////////////////////
    // Below are general test code for MSAA
    //////////////////////////////////////////

#ifdef TEST_SAMPLING
    {
        // MSAA 16x
        FrameBuffer fb(800, 800, 4, 4);
        Sphere sphere(1.0, -1.0, 1.0, gl::to_radian(360.0f), 100, 100);
        Cylinder cylinder(1.0, 0.0, 1.0, gl::to_radian(360.0f), 100, 100);
        // Hyperboloid sphere(gl::vec3(-1.f,-1.f,-1.f),gl::vec3(1.f,1.f,1.f),gl::to_radian(360.0f), 1000, 1000);

        {
            sphere.scale = gl::vec3(0.9f);
            sphere.position = gl::vec3(0.0f, 0.0f, 4.0f);
            gl::Quat q = gl::Quat::fromAxisAngle(gl::vec3(0.0f, 1.0f, 0.0f), gl::to_radian(90.0f));
            sphere.rotation = q * sphere.rotation;
        }

        {
            cylinder.scale = gl::vec3(0.2f);
            cylinder.position = gl::vec3(0.5f, 0.5f, 2.0f);
            gl::Quat q = gl::Quat::fromAxisAngle(gl::vec3(0.0f, 1.0f, 0.0f), gl::to_radian(.0f));
            cylinder.rotation = q * sphere.rotation;
        }

        const auto [w, h] = sphere.getResolution();
        uint width = 800;
        uint height = 800;

        PerspectiveCamera p_cam(gl::to_radian(45.0f), (float)width / (float)height, 0.1f, 100.0f);
        gl::mat4 mvp_sp = p_cam.getProjectionMat() * p_cam.getViewMat() * sphere.getModelMat();
        gl::mat4 mvp_cy = p_cam.getProjectionMat() * p_cam.getViewMat() * cylinder.getModelMat();

        fb.clearColor();
        fb.clearBuffer();

        // loop over micropolygons
        for (int i = 0; i < w; i++)
        {
            for (int j = 0; j < h; j++)
            {
                auto mp_sp = sphere.getMicropolygon(i, j);
                auto mp_cy = cylinder.getMicropolygon(i, j);
                fb.updateBufferByMicropolygon(mp_cy, mvp_cy);
                fb.updateBufferByMicropolygon(mp_sp, mvp_sp);
            }
        }

        fb.setPixelColorFromBuffer();
        auto img = fb.to_cv_mat();
        cv::imshow("image", img);
        cv::waitKey();
    }

#endif

    //////////////////////////////////////////
    // Below are general test code for visualizing samples
    //////////////////////////////////////////

#ifdef TEST_SAMPLE_VIS
    {
        FrameBuffer fb(800, 800, 4, 4);
        std::cout << fb.getSampleNumber() << std::endl;
        auto sample_offset = fb.getSampleOffset();
        for (auto &offset : sample_offset)
        {
            std::cout << offset.x() << " " << offset.y() << std::endl;
        }

        int key = 0;
        while (key != 27)
        {

            cv::Mat image(100, 100, CV_32FC3, cv::Scalar(0.0f, 0.0f, 0.0f));
            image.convertTo(image, CV_8UC3, 1.0f);
            // set the color of the pixel, of the

            // Vertex v = sphere.getVertex(i);
            //  std::cout << v.position << std::endl;
            for (auto &offset : sample_offset)
            {
                image.at<cv::Vec3b>((int)(offset.y() * 100), (int)(offset.x() * 100)) = cv::Vec3b((int)(255), (int)(offset.y() * 255), 1.0f);
            }

            // show the boundary of the grid
            for (int i = 0; i < 100; i++)
            {
                for (int j = 0; j < 100; j++)
                {
                    if ((i + 1) % 25 == 0 || (j + 1) % 25 == 0)
                        image.at<cv::Vec3b>(i, j) = cv::Vec3b(255, 255, 255);
                }
            }

            cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
            cv::imshow("image", image);
            auto key = cv::waitKey(1000);
        }
    }
#endif

    //////////////////////////////////////////
    // Below are general test code for custom framebuffer class
    //////////////////////////////////////////

#ifdef TEST_FRAMEBUFFER
    {
        Sphere sphere(1.0, -1.0, 1.0, gl::to_radian(360.0f), 20, 20);
        sphere.scale = gl::vec3(0.9f);
        sphere.position = gl::vec3(1.0f, 1.0f, 40.0f);
        gl::Quat q = gl::Quat::fromAxisAngle(gl::vec3(0.0f, 1.0f, 0.0f), gl::to_radian(-90.0f));
        sphere.rotation = q * sphere.rotation;
        auto [w, h] = sphere.getResolution();

        uint width = 600;
        uint height = 600;

        PerspectiveCamera p_cam(gl::to_radian(45.0f), 1.0f, 0.1f, 100.0f);
        gl::mat4 mvp = p_cam.getProjectionMat() * p_cam.getViewMat() * sphere.getModelMat();

        int key = 0;
        FrameBuffer fb(width, height);

        for (int i = 0; i < w; i++)
        {
            for (int j = 0; j < h; j++)
            {
                auto v = sphere.getVertex(i, j);
                auto v_screen = gl::getScreenCoord(v.position, mvp, width, height);
                fb.setPixelColor((int)v_screen.x(), (int)(v_screen.y()), gl::vec3(1.0f, 1.0f, 1.0f));
            }
        }

        auto img = fb.to_cv_mat();
        cv::imshow("image", img);
        cv::waitKey();
    }
#endif

    //////////////////////////////////////////
    // Below are general test code for getScreenCoord function
    //////////////////////////////////////////

#ifdef TEST_MVP
    {
        Sphere sphere(1.0, -1.0, 1.0, gl::to_radian(360.0f), 100, 100);
        sphere.scale = gl::vec3(0.9f);
        sphere.position = gl::vec3(3.0f, 1.0f, 4.0f);
        gl::Quat q = gl::Quat::fromAxisAngle(gl::vec3(0.0f, 1.0f, 0.0f), gl::to_radian(-90.0f));
        sphere.rotation = q * sphere.rotation;
        auto [w, h] = sphere.getResolution();

        PerspectiveCamera p_cam(gl::to_radian(45.0f), 1.0f, 0.1f, 100.0f);
        gl::mat4 mvp = p_cam.getProjectionMat() * p_cam.getViewMat() * sphere.getModelMat();

        uint width = 800;
        uint height = 800;

        int key = 0;
        while (key != 27)
        {
            cv::Mat image(width, height, CV_32FC3, cv::Scalar(0.0f, 0.0f, 0.0f));
            image.convertTo(image, CV_8UC3, 1.0f);

            auto [w, h] = sphere.getResolution();
            std::cout << gl::getScreenCoord(sphere.position, mvp, width, height) << std::endl;
            for (int i = 0; i < w; i++)
            {
                for (int j = 0; j < h; j++)
                {
                    auto v = sphere.getVertex(i, j);
                    auto v_screen = gl::getScreenCoord(v.position, mvp, width, height);
                    image.at<cv::Vec3b>((int)v_screen.y(), (int)(v_screen.x())) = cv::Vec3b(1.0 * 255, 1.0 * 255, 255);
                }
            }

            cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
            cv::imshow("image", image);
            auto key = cv::waitKey(1000);
        }
    }
#endif

//////////////////////////////////////////
// Below are general test code for vec and mat class
//////////////////////////////////////////
#ifdef TEST_VEC
    {
        gl::vec2 v2(1.0f, 1.0f);
        gl::vec3 v3(1.0f, 2.0f, 3.0f);
        gl::vec4 v4(1.0f, 1.0f, 1.0f, 1.0f);
        gl::vec4 v4_2{2.0f, 2.0f, 2.0f, 2.0f};

        std::cout << v2 + 1.f << std::endl;
        std::cout << v3 - 1.f << std::endl;
        std::cout << v2 * 1.f << std::endl;
        std::cout << 1.f * v2 << std::endl;

        std::cout << v2.normalized() << std::endl;
        std::cout << v3.normalized() << std::endl;
        std::cout << v4.normalized() << std::endl;
        std::cout << v4_2.normalized() << std::endl;

        std::cout << gl::dot(v4, v4_2) << std::endl;
        std::cout << gl::cross(v3, v3) << std::endl;

        // below are test cases for matrix calculation
        gl::mat4 m4(1.0f);
        gl::mat4 m4_2(2.0f);
        gl::mat4 m4_3(3.0f);

        gl::mat2 m2{1.0f, 2.0f, 3.0f, 4.0f};
        gl::mat2 m2_2{gl::vec2(1.0f), gl::vec2(2.0f)};
        gl::mat2 m2_3{{1, 2.0}, {2.0, 1.0}};

        gl::mat3 m3{6, 1, 1, 4, -2, 5, 2, 8, 7};
        gl::mat4 m4_4{13, 2, 0, 0, 2, 2, 0, 0, 1, 1, 4, 1, 1, 1, 1, 4};

        std::cout << m2 << std::endl;
        std::cout << m2_2 << std::endl;
        std::cout << m2_3 << std::endl;

        std::cout << m4_4 << std::endl;

        std::cout << m4 << std::endl;
        std::cout << m4_2 << std::endl;

        std::cout << m4 * m4_2 << std::endl;
        std::cout << m4 * m4_2 * m4_3 << std::endl;

        // matrix * vector
        std::cout << m4_2 * v4 << std::endl;

        // matrix * scalar
        std::cout << m4_2 * 2.0f << std::endl;

        // matrix transpose
        std::cout << m2.transposed() << std::endl;

        // matrix determinant
        std::cout << m3.determinant() << std::endl;
        std::cout << m4_4.determinant() << std::endl;

        // matrix inverse
        std::cout << m4_4.inversed() << std::endl;

        Object3D obj;
        std::cout << obj.getFront() << std::endl;
        std::cout << obj.getUp() << std::endl;
        std::cout << obj.getRight() << std::endl;
        std::cout << obj.getModelMat() << std::endl;

        std::cout << std::endl;
        // rotate the object around the y axis, 45 degrees to radian
        obj.rotation = gl::Quat::fromAxisAngle({0.0f, 1.0f, 0.0f}, gl::to_radian(45.0f));
        std::cout << obj.getFront() << std::endl;
        std::cout << obj.getUp() << std::endl;
        std::cout << obj.getRight() << std::endl;
        std::cout << obj.getModelMat() << std::endl;

        // this test has to pass the points in order to get the correct result
        std::cout << gl::is_inside_rect({0.0, 0.0}, {0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}) << std::endl;
    }
#endif

    return 0;
}