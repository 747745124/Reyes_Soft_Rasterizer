#include "../utils/Rheader.hpp"

/********************************************/
/********************************************/
void Tunnel()
{	
	using namespace gl;
	RiBegin(RI_NULL);
		RiFormat(800, 800, 1.0);
		//Change output here
		RiDisplay("../Tunnel.png", "", "");
		RiPixelSamples(2,2);
		
		RiFrameBegin(0);
			/* set the perspective transformation */
			float fov = 45.0;
			RiProjection(RI_PERSPECTIVE, "fov", &fov);
			//RiProjection(RI_ORTHOGRAPHIC);

			RiWorldBegin();
				RiPhongMaterial(vec3(0.2),0.2,vec3(0.4),vec3(0.9),32);
				RiTransformBegin();
					RtColor color = {1,0,0};
					RiColor(color);
					RiTranslate(0, 0.5, 7.0);
					RiRotate(60, 1, 0, 0);
					RiTorus(1, .25, 0, 360, 360);
				RiTransformEnd();
				RiTransformBegin();
					color[0] = 0; color[1] = 1;
					RiColor(color);
					RiTranslate(-0.5,1.2, 8.0);
					RiRotate(60, 1, 0, 0);
					RiRotate(30, 0, 1, 0);
					RiCylinder(1, -1, 1, 360);
				RiTransformEnd();
				RiTransformBegin();
					color[1] = 0; color[2] = 1;
					RiColor(color);
					RiTranslate(0, 1, 9.0);
					RiRotate(60, 1, 0, 0);
					RiSphere(1.0, -1.0, 1.0, 360);
				RiTransformEnd();
				RiTransformBegin();
					color[0] = 1; color[1] = .4; color[2] = .4;
					RiColor(color);
					RiSurface(SURFACE_TYPE::CHECKER,10,10);
					RiTranslate(0, -1, 8.5);
					RiRotate(-160, 1, 0, 0);
					RiRotate(-30, 0, 1, 0);
					RiCone(2, 1, 360);
				RiTransformEnd();
				RiTransformBegin();
                    RiSurface(SURFACE_TYPE::CHECKER,40,40);
					RiTranslate(0, 0, 7.0);
					RiCylinder(3, 0, 10, 360);
				RiTransformEnd();
			RiWorldEnd();

		RiFrameEnd();

	RiEnd();
}
