******************************************

** AdvComputerGraphics	(CS657/718)
** Anna Fischer		(Spring 2018)
**
** Project 1: Ray Casting
** ReadME File

******************************************

For raytraceResult1.ppm:

vec3 viewpoint = {1.5, 1.0, 2.0};

Material myobj_mat[] = {
	{{0.00784, 0.49411, 0.85882}, 0.2, 1.0, 0.0, 28, 0.0 },
	{{0.00784, 0.49411, 0.85882}, 0.2, 1.0, 0.0, 28, 0.0 },
	{{0.00784, 0.49411, 0.85882}, 0.2, 1.0, 0.0, 28, 0.0 }, //floor
};

vec3 light_pos[] = {
	{  5.0, 6.0, -0.5 },
	{ -5.0, 3.0, -4.5 },
};

vec3 light_color[] = {
	{ 0.5, 0.5, 0.5 },
	{ 0.7, 0.3, 0.1 },
};


In order to achieve the result from
raytraceResult2.ppm:

Change the light color (in regards to the redness of the light) for the second light source to 0.0
Thus,

vec3 light_color[] = {
	{ 0.5, 0.5, 0.5 },
	{ 0.0, 0.3, 0.1 }, //RGB
};
 
******************************************