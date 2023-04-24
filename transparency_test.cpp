#include "./utils/Rheader.hpp"
using namespace gl;

void TransparencyTest()
{
	RiBegin(RI_NULL);
		//RiFormat(960, 720, 1.0);
		RiFormat(400, 400, 1.0);
		//RiFormat(200, 150, 1.0);
		RiDisplay("../transparency_test.png", "file", "");
		RiPixelSamples(2,2);
		
		RiFrameBegin(0);
			/* set the perspective transformation */
			float fov = 45.0;
			RiProjection(RI_PERSPECTIVE, "fov", &fov);
			//RiProjection(RI_ORTHOGRAPHIC);

			RiWorldBegin();
            //Red
				RiTransformBegin();
					RtColor color = {1,0,0};
					RtColor opacity = {.4,.4,.4};
					RiOpacity(opacity);
					RiColor(color);
					RiTranslate(0, 0.5, 7.0);
					RiRotate(60, 1, 0, 0);
					RiTorus(1, .25, 0, 250, 270);
				RiTransformEnd();
            //Green
				RiTransformBegin();
					color[0] = 0; color[1] = 1;
					opacity[0] = 0.4; opacity[1] = 0.4; opacity[2] = 0.4;
					RiOpacity(opacity);
					RiColor(color);
					RiTranslate(0, 0, 8.0);
					RiRotate(60, 1, 0, 0);
					RiRotate(30, 0, 1, 0);
					RiCylinder(1, -1, 1, 300);
				RiTransformEnd();
            //Blue
				RiTransformBegin();
					color[1] = 0; color[2] = 1;
					opacity[0] = .7; opacity[1] = .7; opacity[2] = .7;
					RiOpacity(opacity);
					RiColor(color);
					RiTranslate(0, 1, 9.0);
					RiRotate(60, 1, 0, 0);
					RiSphere(1.8, -1.0, 1.0, 250);
				RiTransformEnd();
            
				RiTransformBegin();
					color[0] = 1; color[1] = .6; color[2] = .6;
					RiColor(color);
					opacity[0] = .6; opacity[1] = .6; opacity[2] = .6;
					RiOpacity(opacity);
					RiTranslate(0, -1, 8.5);
					RiRotate(-160, 1, 0, 0);
					RiRotate(30, 0, 1, 0);
					RiRotate(140, 0, 0, 1);
					RiCone(2.5, 1, 270);
				RiTransformEnd();
			RiWorldEnd();
		RiFrameEnd();

	RiEnd();
};

int main()
{
    TransparencyTest();
}