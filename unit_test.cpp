#include "./unit_test.hpp"
#define TEST_API
//  #define TEST_SCENE_MANAGER
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
    SceneManager scene;
    scene.fovy = gl::to_radian(45.0f);
    scene.znear = 0.1f;
    scene.zfar = 100.0f;
    scene.setWidth(400, 400);
    scene.setSpp(1, 1);
    scene.initCamera();
    scene.initFramebuffer();



#endif

#ifdef TEST_SCENE_MANAGER
    SceneManager scene;
    scene.fovy = gl::to_radian(45.0f);
    scene.znear = 0.1f;
    scene.zfar = 100.0f;
    scene.setWidth(400, 400);
    scene.setSpp(2, 2);
    scene.initCamera();
    scene.initFramebuffer();

    std::unique_ptr<Hyperboloid> hyper = std::make_unique<Hyperboloid>(gl::vec3(-1, -1, -1), gl::vec3(1, 1, 1), gl::to_radian(360.f));
    std::unique_ptr<Sphere> sphere = std::make_unique<Sphere>(1.0, -1.0, 1.0, gl::to_radian(360.0f));
    std::unique_ptr<Disk> disk = std::make_unique<Disk>(0.1, 10.f, gl::to_radian(360.0f));

    std::unique_ptr<Light> light = std::make_unique<Light>();
    std::unique_ptr<PhongMaterial> mat = std::make_unique<PhongMaterial>();

    {
        hyper->scale = gl::vec3(1.f);
        hyper->position = gl::vec3(1.0f, 0.0f, 8.0f);
        gl::Quat q = gl::Quat::fromAxisAngle(gl::vec3(0.0f, 1.0f, 0.0f), gl::to_radian(90.0f));
        gl::Quat q2 = gl::Quat::fromAxisAngle(gl::vec3(1.0f, 0.0f, 0.0f), gl::to_radian(45.0f));
        hyper->rotation = q * hyper->rotation;

        sphere->scale = gl::vec3(1.f);
        sphere->position = gl::vec3(-1.0f, 0.0f, 8.0f);
        sphere->rotation = q * sphere->rotation;

        disk->position = gl::vec3(0.0f, 0.0f, 10.0f);
        disk->rotation = q2 * disk->rotation;
    }

    mat->ka = 0.2f;
    mat->kd = gl::vec3(0.5f);
    mat->ks = gl::vec3(0.5f);
    mat->shininess = 32.0f;

    light->position = gl::vec3(-2.f, 2.f, 1.0f);
    light->color = gl::vec3(1.0f, 1.0f, 1.0f);

    scene.dice(*hyper);
    scene.dice(*sphere);
    scene.dice(*disk);
    // finalize light
    scene.addLight(std::move(light));
    scene.blinnPhong(*hyper, *mat);
    scene.blinnPhong(*disk, *mat);
    scene.blinnPhong(*sphere, *mat);
    gl::displacementPerlin(*sphere, 20u, 0.3f);
    gl::displacementPerlin(*disk, 20u, 0.3f);

    // finialize mesh
    scene.addMesh(std::move(hyper));
    scene.addMesh(std::move(sphere));
    scene.addMesh(std::move(disk));
    scene.drawMeshes();
    scene.setPixelColorBuffer();
    scene.showImage();
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

#ifdef TEST_PERLIN_DISPLACEMENT
    Texture2D tex("../assets/textures/aki.jpeg", CV_32FC3);
    uint width = 600;
    uint height = 600;

    PerspectiveCamera p_cam(gl::to_radian(45.0f), (float)width / (float)height, 0.1f, 100.0f);

    // MSAA 4x
    FrameBuffer fb(width, height, 2, 2);
    Sphere sphere(1.0, -1.0, 1.0, gl::to_radian(360.0f));

    sphere.scale = gl::vec3(0.9f);
    sphere.position = gl::vec3(0.0f, 0.0f, 6.0f);
    gl::Quat q = gl::Quat::fromAxisAngle(gl::vec3(0.0f, 1.0f, 0.0f), gl::to_radian(90.0f));
    sphere.rotation = q * sphere.rotation;

    gl::mat4 mvp = p_cam.getProjectionMat() * p_cam.getViewMat() * sphere.getModelMat();
    auto span = sphere.getProjectedBoundingVolumeSpan(mvp, width, height);
    sphere.dice((uint)span.x(), (uint)span.y(), 3.0f);

    {
        PhongMaterial mat;
        mat.ka = 0.3f;
        mat.kd = gl::vec3(0.5f);
        mat.ks = gl::vec3(0.5f);
        mat.shininess = 32.0f;

        Light light1, light2, light3;
        light1.position = gl::vec3(2.0f, -1.0f, 4.0f);
        light1.color = gl::vec3(1.0f);
        light2.position = gl::vec3(2.0f, 4.0f, 2.0f);
        light2.color = gl::vec3(1.0f, 1.0f, 1.0f);
        light3.position = gl::vec3(3.0f, -3.0f, 2.0f);
        light2.color = gl::vec3(1.0f, 1.0f, 1.0f);

        gl::displacementPerlin(sphere, 20, 0.1f);
        gl::setColor(sphere, gl::vec4(0.28, 0.38, 1.0f, 1.0f));
        gl::BlinnPhong(sphere, std::vector<Light>{light1, light2, light3}, mat, p_cam.position);
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
            float noise = gl::fractal_perlin_2D(x, y, 4);
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
    sphere.position = gl::vec3(0.0f, 0.0f, 6.0f);
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

        gl::checkerBoard(sphere, 10);
        gl::Displacement(sphere, 1.0f);
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

        uint width = 800;
        uint height = 800;

        PerspectiveCamera p_cam(gl::to_radian(45.0f), (float)width / (float)height, 0.1f, 100.0f);

        // MSAA
        FrameBuffer fb(width, height, 2, 2);

        Sphere sphere(1.0, -1.0, 1.0, gl::to_radian(360.0f), 30, 30);
        Cylinder cylinder(1.0, -1.0, 1.0, gl::to_radian(360.0f), 30, 30);

        {
            sphere.scale = gl::vec3(0.9f);
            sphere.position = gl::vec3(0.0f, 0.0f, 6.0f);
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

        {
            const auto [w, h] = cylinder.getResolution();
            for (int i = 0; i <= w; i++)
            {
                for (int j = 0; j <= h; j++)
                {
                    auto &vertex = cylinder.setVertex(i, j);
                    vertex.baseColor.a() = 0.1f;
                }
            }
        }

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

        // loop over micropolygons of cylinder

        {
            const auto [w, h] = cylinder.getResolution();
            for (int i = 0; i < w; i++)
            {
                for (int j = 0; j < h; j++)
                {
                    auto mp_cy = cylinder.getMicropolygon(i, j);
                    fb.updateBufferByMicropolygon(mp_cy, mvp_cy);
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
        Sphere sphere(1.0, -1.0, 1.0, gl::to_radian(360.0f), 50, 50);
        sphere.scale = gl::vec3(0.9f);
        sphere.position = gl::vec3(0.0f, 0.0f, 4.0f);
        gl::Quat q = gl::Quat::fromAxisAngle(gl::vec3(0.0f, 1.0f, 0.0f), gl::to_radian(-90.0f));
        sphere.rotation = q * sphere.rotation;
        auto [w, h] = sphere.getResolution();

        uint width = 800;
        uint height = 800;

        PerspectiveCamera p_cam(gl::to_radian(45.0f), 1.0f, 0.1f, 100.0f);
        gl::mat4 mvp = p_cam.getProjectionMat() * p_cam.getViewMat() * sphere.getModelMat();

        int key = 0;
        while (key != 27)
        {
            FrameBuffer fb(width, height);

            for (int i = 0; i < w; i++)
            {
                for (int j = 0; j < h; j++)
                {
                    auto v = sphere.getVertex(i, j);
                    auto v_screen = gl::getScreenCoord(v.position, mvp, width, height);
                    // setting up white color, no sampling
                    fb.setPixelColor((int)v_screen.x(), (int)(v_screen.y()), gl::vec3(1.0f, 1.0f, 1.0f));
                }
            }

            auto img = fb.to_cv_mat();
            cv::imshow("image", img);
            auto key = cv::waitKey(1000);
        }
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