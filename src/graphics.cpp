#include "graphics.h"

Graphics::Graphics(const char* _title, uint16_t _w, uint16_t _h, bool _fs)
{
	//clear previous logs
	log_fclear();

	m_title = _title;
	m_width = _w;
	m_height = _h;
	log_fprint("Window Size: %i x %i", _w, _h);

	//setup SDL+GL windows and contexts
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	log_fprint("OpenGL 3.1");

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	log_fprint("RGBA: 32 bits");

	//disable vsync
	SDL_GL_SetSwapInterval(0);
	log_fprint("VSync: disabled");

	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	log_fprint("Buffer size: 32 bits");
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	log_fprint("Double buffering is enabled");

	if (_fs)
	{
		m_window = SDL_CreateWindow(_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			_w, _h, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
		log_fprint("Fullscreen mode");
	}

	else
	{
		m_window = SDL_CreateWindow(_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			_w, _h, SDL_WINDOW_OPENGL | SDL_WINDOW_MAXIMIZED |SDL_WINDOW_RESIZABLE);
		log_fprint("Windowed mode");
	}

	m_glContext = SDL_GL_CreateContext(m_window);

	//get all GL functions for this OS/GPU
	GLenum status = glewInit();
	if (status != GLEW_OK)
		log_fprint("Glew failed to init");

	m_closed = false;

	m_numFrames = 0;

	//get the starting time
	m_startTime = SDL_GetTicks();
}

void Graphics::clear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics::update()
{
	SDL_GL_SwapWindow(m_window);

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			m_closed = true;

		if (event.key.keysym.sym == SDLK_ESCAPE)
			m_closed = true;
	}

	this->getFPS();
}

void Graphics::getFPS()
{
	m_endTime = SDL_GetTicks();
	m_numFrames++;

	//limit the drawing of fps
	if ((m_endTime - m_startTime > 1000) && m_numFrames > 10)
	{
		double fps = (double)m_numFrames / (m_endTime / m_startTime);
		//update our starting time again
		m_startTime = m_endTime;
		//reset the frames
		m_numFrames = 0;

		std::stringstream ss;
		ss << m_title;
		ss << " | ";
		ss << fps;
		ss << "fps";
		SDL_SetWindowTitle(m_window, ss.str().c_str());
	}
}

Graphics::~Graphics()
{
	SDL_GL_DeleteContext(m_glContext);
	log_fprint("Destroyed GL context");
	SDL_DestroyWindow(m_window);
	log_fprint("Destroyed Window");
	SDL_Quit();
}