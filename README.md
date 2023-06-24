### Reyes-Style Renderer

* This renderer is a Reyes-style Architecture Soft Rasterizer. It’s a partial implementation of [Renderman ver. 3.2][http://paulbourke.net/dataformats/rib/RISpec3_2.pdf]specification.

![Snowman](https://s2.loli.net/2023/06/24/Yj3LMpSgbclDiWz.png)



* It supports the features below:

  * Alpha Blending
  * Texture Mapping (Bilinear, Nearest)
  * Orthographic / Perspective Camera
  * ZBuffer
  * Perlin Noise Displacement
  * MSAA
  * Lighting and Shadow-mapping
  * Different render target (Normal, Depth, Albedo etc.)

  <img src="https://s2.loli.net/2023/06/24/2rCl9s5fYuTnymG.png" alt="transparency_test_depth" style="zoom:50%;" />

  * Basic Phong and Microfacet material
  * Basic Primitives:
    * Sphere
    * Disks
    * Cylinder
    * Cone
    * Hyperboloid
    * Paraboloid
    * Torus

  * For all available features, see enum classes in  `./base/RenderManager`.

  

#### Under the hood:

* The rotation uses quaternion to avoid gimbal lock.
* Per Reyes architecture, all primitives are diced (splitted into small pieces based on how many pixels it takes) into quads. This soft-rasterizer is a Quad-based renderer, and uses Bilinear Interpolation by default.
* The coordinate system is left-handed, with the z-axis pointing inward the screen.

<img src="https://s2.loli.net/2023/06/24/WTtRBY4KGL6a8sN.png" alt="image-20230623194458934" style="zoom:33%;" />

* The transformations and matrix APIs are written in a similar fashion to GLM library.



#### Build Instructions:

* This project uses CMake to build and is correctly build under M1 macOS environment. The C++ standard is set to C++ 17.
* Note that the project uses `OpenCV` for image IO (and it’s the only external dependency), and uses CMake’s `find_package` command. So please make sure your OpenCV is downloaded beforehand.
* Once CMake and OpenCV is downloaded, execute shell command below.

```sh
mkdir build
cd build
cmake ..
make .
```



#### Sample Usage:

Below scene creates a sphere with basic texture mapping and perlin noise displacement. It’s compliant to the Renderman 3.2 specification.

<img src="https://s2.loli.net/2023/06/24/2kRNAn4j7up6Wzl.png" alt="image-20230623192900780" style="zoom:50%;" />

```c++
using namespace gl;
RiBegin(RI_NULL);
    RiFormat(600, 600, 1.0);
    //Change output here
    RiDisplay("Earth.png", "", "");
    RiPixelSamples(4,4);

    RiFrameBegin(0);
        /* set the perspective transformation */
        float fov = 45.0;
        RiProjection(RI_PERSPECTIVE, "fov", &fov);
        //RiProjection(RI_ORTHOGRAPHIC);

        RiWorldBegin();
            RiTransformBegin();
                RiMakeTexture("../assets/textures/earth.jpg", 0);
                RiSurface(SURFACE_TYPE::TEXTURE);
                RiActiveTexture(0);
                RtColor blue = { 0, 0, 1 };
                RtColor opacity = { .9, .9, .9 };
                RiColor(blue);
                RiOpacity(opacity);
                RiDisplacement(DISPLACEMENT_TYPE::PERLIN, 8, 0.3f);
                RiTranslate(0, 0, 5.0);
                RiRotate(-175, 0, 1, 0);
                RiRotate(120, 1, 0, 0);
                RiSphere(1, -1, 1, 360);
            RiTransformEnd();
        RiWorldEnd();

    RiFrameEnd();

RiEnd();
```



#### Alternative Usage:

This soft-rasterizer can also be used in a more OOP-fashion. These code creates a basic shadow-mapped image.

```c++
void Basic_Shadow()
{   
    using namespace gl;
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
```
