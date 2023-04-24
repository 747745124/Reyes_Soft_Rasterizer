#pragma once
#include "../utils/Rheader.hpp"

/********************************************/
/********************************************/
void ShaderTest()
{	
	using namespace gl;
	RiBegin(RI_NULL);
		//RiFormat(960, 720, 1.0);
		RiFormat(800, 800, 1.0);
		//RiFormat(200, 150, 1.0);
		RiDisplay("ShaderTest.png", "", "");
		RiPixelSamples(2,2);
		
		RiFrameBegin(0);
			/* set the perspective transformation */
			float fov = 45.0;
			RiProjection(RI_PERSPECTIVE, "fov", &fov);
			//RiProjection(RI_ORTHOGRAPHIC);

			RiWorldBegin();
				RiAddLight(vec3(0.5,0.5,6.0),vec3(1.0f),100.f);
				RiTransformBegin();
					RtColor color = {1,0,0};
					RtColor opacity = {.4,.4,.4};
					RiOpacity(opacity);
					RiColor(color);
					uint CHECK_SIZE_X = 20;
					uint CHECK_SIZE_Y = 10;
					RiSurface(SURFACE_TYPE::CHECKER, CHECK_SIZE_X, CHECK_SIZE_Y);
					RiTranslate(0, 0.5, 11);
					RiRotate(70, 0, 1, 0);
					RiTorus(2.5, .5, 0, 360, 360);
				RiTransformEnd();
				RiTransformBegin();
					color[0] = 0; color[1] = 1; color[2] = 0;
					opacity[0] = 1; opacity[1] = 1; opacity[2] = 1;
					RiOpacity(opacity);
					RiSurface(nullptr);
					RiColor(color);
					RiTranslate(3, -1.5, 11);
					RiRotate(90, 1, 0, 0);
					RiCylinder(1, -1, 1, 360);
				RiTransformEnd();
				RiTransformBegin();
					opacity[0] = 1; opacity[1] = 1; opacity[2] = 1;
					RiOpacity(opacity);
					RiMakeTexture("../assets/textures/earth.jpg", 0);
					RiActiveTexture(0);
					RiSurface(SURFACE_TYPE::TEXTURE);
					RiDisplacement(DISPLACEMENT_TYPE::PERLIN,6,0.01f);
					RiTranslate(3, 1, 11.0);
					RiRotate(-40, 0, 0, 1);
					RiRotate(-100, 0, 1, 0);
					RiRotate(110, 1, 0, 0);
					RiSphere(1, -1.5, 1.5, 360);
				RiTransformEnd();
				RiTransformBegin();
					opacity[0] = .8; opacity[1] = .8; opacity[2] = .8;
					RiOpacity(opacity);
					RiSurface(SURFACE_TYPE::TEXTURE);
					RiDisplacement(nullptr);
					RiTranslate(-2.5, 1, 11.0);
					RiRotate(-175, 0, 1, 0);
					RiRotate(110, 1, 0, 0);
					RiSphere(1.5, -1.5, 1.5, 360);
				RiTransformEnd();
				RiTransformBegin();
					color[0] = 0.4; color[1] = 0.2; color[2] = 0;
					RiColor(color);
					opacity[0] = 1; opacity[1] = 1; opacity[2] = 1;
					RiOpacity(opacity);
					RiSurface(nullptr);
					RiDisplacement(DISPLACEMENT_TYPE::PERLIN,6,0.5f);
					RiTranslate(-2.5, -2.5, 11);
					RiRotate(-90, 1, 0, 0);
					RiRotate(90, 0, 0, 1);
					RiCone(2.5, 1, 360);
				RiTransformEnd();
			RiWorldEnd();
		RiFrameEnd();
	RiEnd();
}
