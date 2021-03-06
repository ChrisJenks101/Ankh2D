#ifndef BMP_H
#define BMP_H

#if ANKH2D_WIN32
	#include <stdint.h>
#elif ANKH2D_PSX
	#include "types.h"
#endif

#include "logger.h"
#include "bmath.h"

#if ANKH2D_WIN32
	static vec3 alpha_colour_key;

	//http://www.dragonwins.com/domains/getteched/bmp/bmpfileformat.htm

	//note: for bmp file format, set current packing alignment value to 1
	//ensures no padding that place (which is bad for this data format)

	//14 bytes
	#pragma pack(push, 1)
	typedef struct
	{
		char type[2];
		uint32_t size;
		uint16_t reserved_1;
		uint16_t reserved_2;
		uint32_t pixel_offset;
	} BMP_FILE_HEADER;
	#pragma pack(pop)

	//40 bytes
	#pragma pack(push, 1)
	typedef struct
	{
		uint32_t size;
		uint32_t width;
		uint32_t height;
		uint16_t planes; //must be 1
		uint16_t bit_count;
		uint32_t compression; //0: uncompressed, 1: 8 bit run-length enc 2: 4 bit run length enc 3: bitfields (32)
		uint32_t image_size;
		uint32_t x_px_per_meter;
		uint32_t y_px_per_meter;
		uint32_t color_used;
		uint32_t color_important;
	} BMP_IMAGE_HEADER;
	#pragma pack(pop)

	#pragma pack(push, 1)
	typedef struct
	{
		uint8_t blue;
		uint8_t green;
		uint8_t red;
	} BMP_PIXEL_24_DATA;
	#pragma pack(pop)

	typedef struct 
	{
		BMP_FILE_HEADER fh;
		BMP_IMAGE_HEADER ih;
		BMP_PIXEL_24_DATA* pd;
	} BMP_24;

	BMP_24* bmp_24_load(const char* _filename);

	typedef struct
	{
		uint8_t red;
		uint8_t green;
		uint8_t blue;
		uint8_t alpha;
	} RGBA_DATA;

	//helper functions
	//convert the bgr format to the preferred rgb format
	RGBA_DATA* bmp_24_bgr_rgba(BMP_24* _bmp);

	//this sets the alpha colour (we loads 24 bit BMP w/
	//no alpha component, so we make it via this colour
	void bmp_set_alpha_color(vec3 _colour);

#elif ANKH2D_PSX
	//Only work with TIM files
#endif

#endif