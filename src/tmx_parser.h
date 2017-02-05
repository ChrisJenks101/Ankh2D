#ifndef TMX_PARSER_H
#define TMX_PARSER_H

#include <stdint.h>
#include <vector>
#include "tinyxml2.h"
#include "logger.h"

//this is a lite version of the tmx file format
//uses tinyxml2 and tiled map editor (0.18)
//http://doc.mapeditor.org/reference/tmx-map-format/ 

//improvements
//only loads XML formatted tmx files (look into CSV, base64)
//remove the tilesize in the individual tilesets (should be the same as the map tilesize)
//add rotational code to the tmx parser for the TILE data
//adjust the buffer size of the names and filenames
//concatenate the image filename dir path (some are wild)

typedef struct
{
	uint8_t firstGid;
	char name[128];
	uint8_t tile_width;
	uint8_t tile_height;
	char filename[128];
} TILESET;

typedef struct
{
	uint16_t tile_id;
} TILE;

typedef struct
{
	char name[128];
	uint16_t width;
	uint16_t height;
	TILE* data;
} LAYER;

typedef struct
{
	uint16_t map_width;
	uint16_t map_height;
	uint8_t tile_width;
	uint8_t tile_height;
	TILESET tileset;
	LAYER layer;
} tmx_map;

tmx_map* tmx_parser_create(const char* _filename);
void tmx_parser_destroy(tmx_map* _tmxm);

#endif