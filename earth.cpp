#include "../utils/Rheader.hpp"
using namespace gl;

void Earth()
{
	RiBegin(RI_NULL);
		//RiFormat(960, 720, 1.0);
		RiFormat(600, 600, 1.0);
		//RiFormat(200, 150, 1.0);
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
}

int main(){
    Earth();
}