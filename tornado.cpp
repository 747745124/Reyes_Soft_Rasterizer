#include "./utils/Rheader.hpp"
using namespace gl;


void Tornado()
{
	RiBegin(RI_NULL);
		//RiFormat(960, 720, 1.0);
		RiFormat(400, 300, 1.0);
		//RiFormat(200, 150, 1.0);
		RiDisplay("Tornado.png", "", "");
		RiPixelSamples(2,2);
		
		RiFrameBegin(0);
			/* set the perspective transformation */
			float fov = 45.0;
			RiProjection(RI_PERSPECTIVE, "fov", &fov);
			//RiProjection(RI_ORTHOGRAPHIC);

			RiWorldBegin();
				RiTransformBegin();
					RtColor gray = { .4, .4, .4 };
					RtColor opacity = { 0.5, 0.5, 0.5 };
					RiColor(gray);
					RiOpacity(opacity);
					uint BUMP_AMPLITUDE = 1;
					uint BUMPY_MIN_FREQ_EXP = 20;
					RiDisplacement(DISPLACEMENT_TYPE::PERLIN, BUMPY_MIN_FREQ_EXP,BUMP_AMPLITUDE);
					RiTranslate(0, 0, 5.0);
					RiRotate(90, 0, 1, 0);
					RiRotate(80, 1, 0, 0);
					RiTranslate(0, 0, -1.5);
					RiCone(3, 1, 360);					
				RiTransformEnd();
			RiWorldEnd();

		RiFrameEnd();

	RiEnd();
};

int main()
{
    Tornado();
}