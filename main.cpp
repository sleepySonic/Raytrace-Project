#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define nelem(a) (sizeof(a) / (sizeof(a[0])))
#define min(x,y) ((x) < (y) ? (x) : (y))
#define max(x,y) ((x) < (y) ? (y) : (x))

#define PI 3.1415926536
#define WIDTH 500
#define HEIGHT 500

typedef unsigned char rgb[3];

struct vec3{
    float x, y, z;
    vec3 operator+(int i){
    vec3 v = {x + i, y + i, z + i};
    return v;
}
// vector addition
vec3 operator+(vec3 v){
    vec3 nv = {x + v.x, y + v.y, z + v.z};
    return nv;
}
// vector subtraction (neg.)
vec3 operator-(){
    vec3 v = {-x, -y, -z};
    return v;
}
// vector subtract scalar
vec3 operator-(float d){
    vec3 v = {x - d, y - d, z - d};
    return v;
}
// vector subtract vector
vec3 operator-(vec3 v){
    vec3 nv = {x - v.x, y - v.y, z - v.z};
    return nv;
}
// vector multi. scalar
vec3 operator*(float d) const{
    vec3 v = {x * d, y * d, z * d};
    return v;
}
// vector multi. vector
vec3 operator*(vec3 v){
    vec3 nv = {x * v.x, y * v.y, z * v.z};
    return nv;
}
// vector divide scalar
vec3 operator/(float d){
    vec3 v = {x / d, y / d, z / d};
    return v;
}
// dot product
float dot(vec3 v){
    return (x * v.x) + (y * v.y) + (z * v.z);
}
// square root operation
vec3 root(){
    vec3 v = {(float) sqrt(x), (float) sqrt(y), (float) sqrt(z)};
    return v;
}
// square operation
vec3 sqr(){
    vec3 v = {x * x, y * y, z * z};
    return v;
}
};
// add vector and scalar
vec3 operator+(int i, vec3 v) {
    return  v + i;
}
// multiply vector and scalar
vec3 operator*(float d, vec3 v) {
    return v * d;
}

struct Material {
    vec3 color;
    float ka, kd, ks, ns, kr;
};

//output image
rgb *fb;

vec3 viewpoint = {1.5, 1.0, 2.0};

//viewplane;
vec3 vp_u = {1.0, 0.0, 0.0};
vec3 vp_v = {0.0, 1.0, 0.0};
vec3 vp_w = {0.0, 0.0, -1.0};

//focal length
float vp_d = 1.0;

//set of spheres
vec3 mysph_pos[] = {
    { 2.0, 1.0, -7.0},
    {-1.0, 1.0, -6.0},
};

//material properties
Material myobj_mat[] = {
    {{0.00784, 0.49411, 0.85882}, 0.2, 1.0, 0.0, 28, 0.0 },
    {{0.0, 0.8, 0.67843}, 0.4, 0.8, 0.7, 28, 0.0 },
    {{1.0, 1.0, 1.0}, 0.7, 0.7, 0.7, 28, 0.5 },  //floor
};

//background color
vec3 bgcolor = { 0.0, 0.0, 0.0 };
vec3 ambient = { 0.2, 0.2, 0.2 };

//set of lights
vec3 light_pos[] = {
    {  5.0, 6.0, -0.5 },
    { -5.0, 3.0, -4.5 },
};
// lights colors
vec3 light_color[] = {
    { 0.5, 0.5, 0.5 },
    { 0.7, 0.3, 0.1 },
};

float length(const vec3 &v) {
    return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

vec3 normalize(const vec3 &v)
{
    float n = length(v);
    if(n == 0.0)
        n = FLT_MIN;
    vec3 v2 =  { v.x * (1.0/n), v.y * (1.0/n), v.z * (1.0/n) };
    return v2;
}

int intersection(vec3 origin, vec3 dir, vec3 *interPoint)
{
	int interNum = -1;                 			// intersection point closest to origin point
    vec3 inter = {INFINITY,INFINITY,INFINITY};  // point relative from the origin to the center of object[i]

    for(int i = 0; i < 3; i++) {    // check for each object.

        // object[i] = 2 is the plane.
        // cutoff vector for one axis point and solve for other axis points wrt intersection.
        if(i == 2) {

			vec3 cutoff = origin + (100 * dir);
			float u = origin.y / (origin.y - cutoff.y);
			if (u >= 0) {
				inter.y = 0;
				inter.x = origin.x - ((origin.x - cutoff.x) * u);
				inter.z = origin.z - ((origin.z - cutoff.z) * u);
			}
        // object is not the floor.
        } else {

            // intersection ray-sphere, Lecture 3.
			vec3 p = origin - mysph_pos[i];
			float Tm     = (-p).dot(dir);
            float L2m    = p.dot(p) - pow(p.dot(dir), 2);
            // make sure we do not square root a neg. number
            if (L2m < 1) {
				float deltaT = sqrt(1 - L2m);

				// intersection point 1, point 2.
				vec3 t0 = (p + Tm * dir + deltaT * dir) + mysph_pos[i];
				vec3 t1 = (p + Tm * dir - deltaT * dir) + mysph_pos[i];

				float distance1 = length(t0 - origin);
				float distance2 = length(t1 - origin);

				if(distance1 < distance2) {
					inter.y = t0.y;
					inter.x = t0.x;
					inter.z = t0.z;
				}
				else {
					inter.y = t1.y;
					inter.x = t1.x;
					inter.z = t1.z;
				}
            }
            
        }   //end if-else statement

        // make sure the point in the closest to the viewpoint
        // and assign the interPoint pointer x,y,z to  the values of inter x,y,z.
		if(length((*interPoint)- origin) > length(inter - origin)) {
			interPoint->x = inter.x;
			interPoint->y = inter.y;
			interPoint->z = inter.z;
			interNum = i;
		}

    }   //end for-loop.

    return interNum;
    // the object index is returned so we know which object is 'hit'
    // as well as where the intersection vector point is (memory pointer)

}   //end intersection function.

float clamp(float v, float l, float m)
{
    if(v > m) return m;
    if(v < l) return l;
    return v;
}

vec3 clamp(const vec3 &v)
{
    vec3 r = { clamp(v.x, 0.0, 1.0), clamp(v.y, 0.0, 1.0), clamp(v.z, 0.0, 1.0) };
    return r;
}

vec3 traceray(vec3 origin, vec3 dir)
{
    vec3 fcolor = bgcolor;      // background color (black).

    // find where origin and dir vectors intersect via intersection method
    vec3 p = {INFINITY, INFINITY, INFINITY};
	int y = intersection(origin, dir, &p);

    // if there is an intersection point...
	if(y != -1) {
        
        // ambient lighting
		fcolor = myobj_mat[y].ka * ambient * myobj_mat[y].color;

		for(int light = 0; light < 2; light++) {
			vec3 L = normalize(light_pos[light] - p);
			vec3 garbage = {INFINITY, INFINITY, INFINITY};
			if(intersection(light_pos[light], -L, &garbage) == y) {
				vec3 normal;
				if(y == 2) {
					normal.x = 0;
					normal.y = 1;
					normal.z = 0;
				} else {
					normal.x = p.x - mysph_pos[y].x;
					normal.y = p.y - mysph_pos[y].y;
					normal.z = p.z - mysph_pos[y].z;
				}

				//  reflective ray, Lecture 2 (Slide 42).
				vec3 R = (2*(normal.dot(L))*normal) - L;
                
                // diffuse and specular lighting.
				fcolor = fcolor + (myobj_mat[y].kd * myobj_mat[y].color * max(0, normal.dot(L)) * light_color[light])
							+ (myobj_mat[y].ks * light_color[light] * pow(max(0, (-dir).dot(R)), myobj_mat[y].ns));
			}

		}
	}

// note that, before returning the color, the computed color may be rounded to [0.0, 1.0].
fcolor= clamp(fcolor);
return (fcolor);
}

void render(void)
{
    int x,y;
    vec3 fcolor, origin, dir;

    for(y = 0; y < HEIGHT; y++) {
        for(x = 0; x < WIDTH; x++) {

            origin = viewpoint + (vp_u * ((1.0/WIDTH)*(x+.5)-.5))
                   + (vp_v * ((1.0/HEIGHT)*(y+.5)-.5))
                   + (vp_w * vp_d);
            dir = normalize(origin - viewpoint);

            fcolor = traceray(origin, dir);
            fb[(HEIGHT - y) * WIDTH + x][0] = fcolor.x * 255;
            fb[(HEIGHT - y) * WIDTH + x][1] = fcolor.y * 255;
            fb[(HEIGHT - y) * WIDTH + x][2] = fcolor.z * 255;
        }
    }
}

void WritePPM(char *fn, rgb *dataset)
{
    int i,j;
    FILE *fp;
    if((fp = fopen(fn, "wb")) == NULL) {
        perror(fn);
        return;
    }
    printf("Begin writing to file %s....", fn);
    fflush(stdout);
    fprintf(fp, "P6\n%d %d\n%d\n", WIDTH, HEIGHT, 255);
    for (j=0; j<HEIGHT; j++)
        for (i=0; i<WIDTH; i++) {
            fputc(dataset[j*WIDTH+i][0], fp);
            fputc(dataset[j*WIDTH+i][1], fp);
            fputc(dataset[j*WIDTH+i][2], fp);
        }
    printf("done\n");
    fclose(fp);
}

int main(int argc, char *argv[])
{
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <output_img>\n"
                        "       <output_img>: PPM file\n", argv[0]);
        exit(-1);
    }

    fb = (rgb*)malloc(WIDTH * HEIGHT * sizeof(rgb));
    if(fb == NULL) {
        perror("malloc");
        exit(-1);
    }
	printf("hello\n");
    render();
	printf("hello222\n");
    WritePPM(argv[1], fb);
    free(fb);
    return 0;
}
