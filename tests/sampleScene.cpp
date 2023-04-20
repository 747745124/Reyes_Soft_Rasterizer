// #include <stdio.h>
// #include "Ri.h"





// void SampleScene1(void) {
// 	int i;
// 	int nf;
// 	float slopex,slopey,slopez;
// 	char name[50];

// 	RtColor red={1,0,0};
// 	RtColor green={0,1,0};
// 	RtColor blue={0,0,1};
// 	RtColor white={1,1,1};


// 	RtPoint p1={30,0,10}; /* ball's initial position */
// 	RtPoint p2={0,20,10}; /* ball's final position  */


// 	RtFloat fov = 45;
// 	RtFloat intensity1=0.1;
// 	RtFloat intensity2=1.5;
// 	RtInt init=0,end=1;
	

// 	nf = 100; /* number of frames to output */
// 	slopex = (p2[0]-p1[0])/nf;
// 	slopey = (p2[1]-p1[1])/nf;
// 	slopez = (p2[2]-p1[2])/nf;
	
// 	RiBegin(RI_NULL);
// 		RiFormat(320,240,1);
// 		RiPixelSamples(2,2);
// 		RiShutter(0,1);

// 		/* loop through all the frames */
// 		for (i = 1; i <= nf; i++) {
// 			RiFrameBegin(i);
// 				sprintf(name,"image_%02d.tif", i-1);
// 				RiDisplay(name, "file", "rgb", RI_NULL);

// 				RiProjection("perspective", "fov", &fov, RI_NULL);
// 				RiTranslate(0,-5,60);
// 				RiRotate(-120,1,0,0);
// 				RiRotate(25,0,0,1);

// 				RiWorldBegin();
// 					RiColor(blue);
// 					RiTransformBegin();
// 						RiCylinder(1, 0, 20, 360, RI_NULL);
// 						RiTranslate(0, 0, 20);
// 						RiCone(2, 2, 360, RI_NULL);
// 					RiTransformEnd();

// 					RiColor(green);
// 					RiTransformBegin();
// 						RiRotate(-90, 1, 0, 0);
// 						RiCylinder(1, 0, 20, 360, RI_NULL);
// 						RiTranslate(0, 0, 20);
// 						RiCone(2, 2, 360, RI_NULL);
// 					RiTransformEnd();

// 					RiColor(red);
// 					RiTransformBegin();
// 						RiRotate(90, 0, 1, 0);
// 						RiCylinder(1, 0, 20, 360, RI_NULL);
// 						RiTranslate(0, 0, 20);
// 						RiCone(2, 2, 360, RI_NULL);
// 					RiTransformEnd();


// 					RiColor(white);
// 						RiTransformBegin();
// 						RiTranslate(p1[0] + slopex * (i-1), p1[1] + slopey * (i-1), p1[2] + slopez * (i-1));
// 						RiSphere(5, -5, 5, 360, RI_NULL);
// 					    RiTransformEnd();
// 				RiWorldEnd();

// 			/* when you hit this command you should output the final image for this frame */
// 			RiFrameEnd();
// 		}
// 	RiEnd();
// };


// int main(void) {
// 	// select which scene to render
// 	SampleScene1();
// }