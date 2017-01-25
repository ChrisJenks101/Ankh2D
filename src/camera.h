#ifndef CAMERA_H
#define CAMERA_H

#include <glm\mat4x4.hpp>
#include <glm\vec2.hpp>
#include <glm\gtx\transform.hpp>
#include "transform.h"
#include <SDL.h>

#include "sprite.h"

enum CDir
{
	CAM_STILL,
	CAM_LEFT,
	CAM_RIGHT,
	CAM_UP,
	CAM_DOWN
};

typedef struct 
{
	bool freeCamera;
	transform* transform;
	CDir camState;
	const uint8_t* keys;
	float speed; //camera speed when free (not attached to player)
	float screenWidth, screenHeight;
	glm::mat4 projection;
} camera;

extern camera* mainCamera;

camera* camera_create(float _speed);
void camera_create();
void camera_update();
void camera_update(camera* _camera);
void camera_update(sprite* _sprite);
void camera_update(camera* _camera, sprite* _sprite);
void camera_movement(camera* _camera);
void camera_destroy();
void camera_destroy(camera* _camera);

#endif