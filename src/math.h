#ifndef MATH_H
#define MATH_H

#include <math.h>
#include <stdint.h>

#define PI 3.14159265359f

float degToRad(float _deg);
float radToDeg(float _rad);

//matrices: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/

typedef struct
{
	float x;
	float y;
} vec2;

vec2 vec2_create(float _x, float _y);

typedef struct
{
	float x;
	float y;
	float z;
} vec3;

vec3 vec3_create(float _x, float _y, float _z);

//OpenGL is column major when dealing w/ mat4's
//http://www.songho.ca/opengl/gl_matrix.html

//[COLUMN] [ROW]

typedef struct
{
	float element[4][4];
} mat4;

mat4 mat4_create();
mat4 mat4_identity();
mat4 mat4_orthographic(float _left, float _right, float _bottom, float _top, float _zNear, float _zFar);
void mat4_translate(mat4& _model, vec2 _pos);
void mat4_scale(mat4& _model, vec2 _scale);
void mat4_rotate(mat4& _model, float _degree);

#endif