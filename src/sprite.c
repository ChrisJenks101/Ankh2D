#include "sprite.h"

#if ANKH2D_WIN32
	sprite sprite_create(texture2d* _texture, shader* _shader, transform _trans)
	{
		sprite new_sprite;
		//new_sprite.window = _window;
		new_sprite.shader = _shader;
		new_sprite.texture = _texture;
		new_sprite.transform = _trans;
	
		new_sprite.model = shader_get_uniform_location(new_sprite.shader, "model");
		new_sprite.projection = shader_get_uniform_location(new_sprite.shader, "projection");

		//transparency on the font
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

		//new_sprite.speed = 100.0f;

		GLfloat vertices[] = {
			// Pos      // Tex
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f, 
    
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};

		//quad is normalised for positional and tex coords
		glGenVertexArrays(1, &new_sprite.vao);
		glBindVertexArray(new_sprite.vao);
		//allocate two vertex buffers (vert + texcoord)
		glGenBuffers(1, &new_sprite.vbo);
    
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, new_sprite.vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		return new_sprite;
	}

	/*bool sprite_update(sprite* _sprite, btime* _time)
	{
		vec2 pos = vec2_create(0.0f, 0.0f);
		bool sprite_update = false;

		float sprite_speed = _sprite->speed * _time->delta_time;

		if (glfwGetKey(_sprite->window, GLFW_KEY_W) == GLFW_PRESS)
		{
			pos = vec2_create(pos.x + 0.0f, pos.y + -sprite_speed);
			sprite_update = true;
		}

		if (glfwGetKey(_sprite->window, GLFW_KEY_A) == GLFW_PRESS)
		{
			pos = vec2_create(pos.x + -sprite_speed, pos.y + 0.0f);
			sprite_update = true;
		}

		if (glfwGetKey(_sprite->window, GLFW_KEY_S) == GLFW_PRESS)
		{
			pos = vec2_create(pos.x + 0.0f, pos.y + sprite_speed);
			sprite_update = true;
		}

		if (glfwGetKey(_sprite->window, GLFW_KEY_D) == GLFW_PRESS)
		{
			pos = vec2_create(pos.x + sprite_speed, pos.y + 0.0f);
			sprite_update = true;
		}

		//only update if we move
		if (sprite_update)
		{
			_sprite->transform.position = vec2_create(_sprite->transform.position.x + pos.x, 
				_sprite->transform.position.y + pos.y);
		}

		return sprite_update;
	}*/

	/*void sprite_map_intersect(tmx_sprite* _tmx_map, sprite* _sprite, bool _sprite_update)
	{
		//dont waste cycles if we havent moved the sprite
		if (_sprite_update)
		{
			if (_tmx_map->map.num_collisions != 0)
			{
				for (uint8_t i = 0; i < _tmx_map->map.num_collisions; i++)
				{
					uint8_t current_x = _tmx_map->map.collision_coords[i].x;
					uint8_t current_y = _tmx_map->map.collision_coords[i].y;

					// - (_tmx_map->transform.scale.x / 2) is equal to how far we're into a tile in the coords 
					// if 1 tile is 50 px wide and the map is 5 tiles wide and you place it at vec2(0,0), this would mean
					// you're 25 pixels into the middle tile. (This is a prob, need to work it out realtime based off position)
					float x1 = (current_x * _tmx_map->transform.scale.x) - 
						(_tmx_map->transform.scale.x / 2) + _tmx_map->transform.position.x;
					if (_sprite->transform.position.x >= x1)
					{
						float x2 = ((current_x * _tmx_map->transform.scale.x) + _tmx_map->transform.scale.x) - 
							(_tmx_map->transform.scale.x / 2) + _tmx_map->transform.position.x;
						if (_sprite->transform.position.x <= x2)
						{
							float y1 = (current_y * _tmx_map->transform.scale.y) - 
								(_tmx_map->transform.scale.y / 2) + _tmx_map->transform.position.y;
							if (_sprite->transform.position.y >= y1)
							{
								float y2 = ((current_y * _tmx_map->transform.scale.y) + _tmx_map->transform.scale.y) - 
									(_tmx_map->transform.scale.y / 2) + _tmx_map->transform.position.y;
								if (_sprite->transform.position.y <= y2)
								{
									//revert back the pre-collision position
									_sprite->transform.position = _sprite->old_pos;
									break;
								}
							}
						}				
					}
				}
			}
		}

		//update the old position (used for collisions)
		_sprite->old_pos = _sprite->transform.position;
	}*/

	void sprite_draw(mat4* _projection, sprite* _sprite)
	{
		//bind our program
		glUseProgram(_sprite->shader->program);
		texture2d_bind(_sprite->texture);

		mat4 model_matrix = transform_get_model_matrix(_sprite->transform);
		shader_set_uniform_mat4(_sprite->model, &model_matrix, true);

		shader_set_uniform_mat4(_sprite->projection, _projection, false);

		//bind and draw our object
		glBindVertexArray(_sprite->vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	void sprite_destroy(sprite* _sprite)
	{
		_sprite->texture = NULL;
		_sprite->shader = NULL;
		glDeleteBuffers(1, &_sprite->vao);
		glDeleteBuffers(1, &_sprite->vbo);
	}

#elif ANKH2D_PSX

	void sprite_create(sprite* _sprite, uint32_t* _tpage, uint32_t* _clut)
	{
		//create the rect and GsImage for the GsSprite
		texture2d_create(&_sprite->texture, _tpage, _clut);

		// 16 bit CLUT, all options off (0x1 = 8-bit, 0x2 = 16-bit)
		_sprite->gs_sprite.attribute = 0x2000000;
		
		//get positional data through the transform
		_sprite->gs_sprite.x = _sprite->transform.position.x;
		_sprite->gs_sprite.y = _sprite->transform.position.y;
		
		//get the w/h through the rect
		_sprite->gs_sprite.w = _sprite->texture.tim.pw;
   		_sprite->gs_sprite.h = _sprite->texture.tim.ph;
		
		//texture page 
		//texture mode 
		//0 = 4 bit
		//1 = 8 bit
		//2 = 16 bit
		//semi-transparent rate ???
		//texture x in framebuffer
		//texture y in framebuffer
		_sprite->gs_sprite.tpage = GetTPage(2, 2, 320, 200);
		
		//colour (rgb) overlay of texture
		_sprite->gs_sprite.r = 128;
		_sprite->gs_sprite.g = 128;
		_sprite->gs_sprite.b = 128;
		
		//position in tim file for sprite
		_sprite->gs_sprite.u = 0;
		_sprite->gs_sprite.v = 0;
		
		//CLUT loc x and y
		_sprite->gs_sprite.cx = _sprite->texture.tim.cx;
		_sprite->gs_sprite.cy = _sprite->texture.tim.cy;
		
		//normal luminosity???
		//_sprite->gs_sprite.r = 128;
		//_sprite->gs_sprite.g = 128;
		//_sprite->gs_sprite.b = 128;

		//ADD TO THE TRANSFORM!!!
		//rotational x and y coord
		_sprite->gs_sprite.mx = 0;
		_sprite->gs_sprite.my = 0;
		//not sure!!
		_sprite->gs_sprite.rotate = 0;

		//ADD TO THE TRANSFORM!!!
		//scale (1 = 100%)
		_sprite->gs_sprite.scalex = 1; 
		_sprite->gs_sprite.scaley = 1;

		//wait for all to be drawn
		//not sure if needed here...
		DrawSync(0);
	}

	void sprite_draw(sprite* _sprite, GsOT* _ot)
	{
		// insert sprites into the ordering table ready for drawing
	   GsSortSprite(&_sprite->gs_sprite, _ot, 0);
	}

#endif