#include "./utils/Rheader.hpp"
using namespace gl;


/********************************************/
/********************************************/
void ShaderTest()
{
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
				RiTransformEnd();
			RiWorldEnd();
		RiFrameEnd();
	RiEnd();
}

int main()
{
    ShaderTest();
}