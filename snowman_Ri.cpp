#include "../utils/Rheader.hpp"
using namespace gl;
RtColor blue = {0.28, 0.38, 1.0f};
RtColor orange = {0.95, 0.23, 0.05f};
RtColor lightblue = {0.2, 0.3, 0.7f};
RtColor matt = {0.05, 0.05, 0.05};
RtColor yellow = {1.0, 0.44, 0.05f};
RtColor red = {0.68, 0.137, 0.16};

void snowman_ri()
{
    RiBegin(RI_NULL);
    RiFormat(1000, 1000, 1.0);
    RiDisplay("../Snowman.png", "file", "rgb");
    RiPixelSamples(4, 4);
    RiDiceCoeff(6.0f);
    
    RiFrameBegin(0);
    /* set the perspective transformation */
    float fov = 45.0;
    RiProjection(RI_PERSPECTIVE, "fov", &fov);

    RiWorldBegin();
    RiClearLight();
    RiAddLight(vec3(-2.f, 3.f, 9.f), vec3(1.0, 0.843, 0.0), 600.f);
    RiAddLight(vec3(2.f, 3.f, 9.f), vec3(0.8, 0.15, 0.0), 400.f);
    RiAddLight(vec3(0.f, 1.f, 4.5f), vec3(1.0, 1.0, 1.0), 160.f);
    RiTransformBegin();
        //base
        RiColor(blue);
        RiIlluminate(ILLUMINATION_TYPE::CLOTH);
        RiPBRMaterial(vec3(0.5), 0.4, 0.3, 0.4);
        RiDisplacement(DISPLACEMENT_TYPE::PERLIN, 200, 0.2f);
        RiTranslate(0, -2.0, 8.0);
        RiRotate(90, 0, 1.0, 0);
        RiSphere(1.5, -1.5, 1.5, 360);
        //head
        RiColor(lightblue);
        RiDisplacement(DISPLACEMENT_TYPE::PERLIN, 100, 0.15f);
        RiTranslate(0, 2.0, 0.0);
        RiSphere(0.8,-0.8,0.8,360);
        RiRotate(-90,0,1,0);

        //hat
        RiTransformBegin();
            RiPBRMaterial(vec3(0.9f),1.0f,0.2f,0.1f);
            RiIlluminate(ILLUMINATION_TYPE::PBR);
            RiDisplacement(nullptr);
            RiColor(matt);
            RiTranslate(0, 0.8, 0.0);
            RiRotate(-90,1,0,0);
            RiCylinder(0.4,0,0.5,360);
            RiTranslate(0,-0.2f,0.0f);
            RiDisk(0.2,0.9,360);
        RiTransformEnd();

        //background
        RiTransformBegin();
            RiIlluminate(ILLUMINATION_TYPE::NONE);
            RiColor(orange);
            RiTranslate(0, 0.8, 0.0);
            RiDisk(0.2,2.3,360);
        RiTransformEnd();

        //face
        RiTransformBegin();
            RiIlluminate(ILLUMINATION_TYPE::PBR);
            RiColor(matt);
            RiPBRMaterial(vec3(0.05),1.0,0.2,0.1);
            RiTranslate(-0.25, 0.3, -1.0);
            RiSphere(0.1,-0.1,0.1,360);

            RiTranslate(0.5, 0.0, 0.0);
            RiSphere(0.1,-0.1,0.1,360);
            
            //4 dots of mouth
            RiTranslate(-0.5, -0.43, 0.0);
            RiSphere(0.05,-0.05,0.05,360);

            RiTranslate(0.15, -0.05, 0.0);
            RiSphere(0.05,-0.05,0.05,360);

            RiTranslate(0.2, 0, 0.0);
            RiSphere(0.05,-0.05,0.05,360);

            RiTranslate(0.15, 0.05, 0.0);
            RiSphere(0.05,-0.05,0.05,360);
        RiTransformEnd();

        //nose
        RiTransformBegin();
            RiPBRMaterial(vec3(0.9),0.4,0.1,0.1);
            RiDisplacement(DISPLACEMENT_TYPE::PERLIN, 5, 0.015f);
            RiColor(yellow);
            RiTranslate(0,0,-1.1f);
            RiRotate(-90,1,0,0);
            RiCone(0.17,0.06,360);
            RiDisplacement(nullptr);
        RiTransformEnd();

        //scarf
        RiTransformBegin();
            RiPBRMaterial(vec3(0.9),0.4,0.1,0.1);
            RiColor(red);
            RiTranslate(0,-0.5,-0.5f);
            RiRotate(-90,1,0,0);
            RiCylinder(0.68,0.0,0.1,360);
        RiTransformEnd();
    RiTransformEnd();
    RiWorldEnd();
    RiFrameEnd();
    RiEnd();
}

int main()
{   
    auto timeit = make_decorator(snowman_ri);
    timeit();
}