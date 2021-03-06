#ifndef CAMERA_H
#define CAMERA_H

#if ANKH2D_WIN32
	#include <GL\glew.h>
	#include <GLFW\glfw3.h>
#endif

#include "bmath.h"
#include "transform.h"
#include "sprite.h"

#if ANKH2D_WIN32
	typedef struct 
	{
		bool free_camera;
		transform transform;
		float speed; //camera speed when free (not attached to player)
		float screen_width, screen_height;
		GLFWwindow* window;
	} camera;

	camera camera_create(GLFWwindow* _window);
	void camera_update(camera* _camera, mat4* _proj);
	void camera_update(camera* _camera, sprite* _sprite, mat4* _proj);
	bool camera_movement(camera* _camera);
#elif ANKH2D_PSX

#endif

#endif