#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL.h>
#include <GL/glew.h>
#include <stdint.h>
#include <sstream>

#include "logger.h"

class Graphics
{
	public:
		Graphics(const char* _title, uint16_t _w, uint16_t _h, bool _fs);
		~Graphics();

		void clear();
		void update();
		bool isClosed(){return m_closed;}
		void getFPS();

		uint16_t getWidth(){return m_width;}
		uint16_t getHeight(){return m_height;}

		SDL_GLContext getContext(){return m_glContext;}
		SDL_Window* getWindow(){return m_window;}
	private:
		const char* m_title;
		uint32_t m_startTime, m_endTime;
		uint32_t m_numFrames;
		uint16_t m_width, m_height;
		bool m_closed;
		SDL_Window* m_window;
		SDL_GLContext m_glContext;
};

#endif