#include "tmx_parser.h"

TMX_Parser::TMX_Parser(const char* _name)
{
	tinyxml2::XMLDocument m_xmlMap;
	m_xmlMap.LoadFile(_name);

	if (m_xmlMap.Error()){
		//tinyxml2 has shuddy error logging
		log_fprint("%s", m_xmlMap.GetErrorStr1());
	}

	else
	{
		//get the generic map data
		tinyxml2::XMLNode* map = m_xmlMap.FirstChildElement("map");
		tinyxml2::XMLElement* mapElem = map->ToElement();

		m_map.map_height = mapElem->IntAttribute("height");
		m_map.map_width = mapElem->IntAttribute("width");
		m_map.tile_width = mapElem->IntAttribute("tilewidth");
		m_map.tile_height = mapElem->IntAttribute("tileheight");

		//get the tilesets used in the map
		for (tinyxml2::XMLElement* childTileset = map->FirstChildElement("tileset"); childTileset != NULL; 
			childTileset = childTileset->NextSiblingElement("tileset"))
		{
			tinyxml2::XMLElement* tilesetElement = childTileset->ToElement();

			TILESET newSet;
			newSet.firstGid = tilesetElement->IntAttribute("firstgid");
			
			char* name = const_cast<char*>(tilesetElement->Attribute("name"));
			memcpy(newSet.name, name, sizeof(newSet.name));
			
			newSet.tile_width = tilesetElement->IntAttribute("tilewidth");
			newSet.tile_height = tilesetElement->IntAttribute("tileheight");

			char* filename = const_cast<char*>(tilesetElement->FirstChildElement("image")->Attribute("source"));
			memcpy(newSet.filename, filename, sizeof(newSet.filename));

			//enter our new tileset data
			m_map.tileset.push_back(newSet);
		}

		//get all the layers in the map
		for (tinyxml2::XMLElement* childLayer = map->FirstChildElement("layer"); childLayer != NULL; 
			childLayer = childLayer->NextSiblingElement("layer"))
		{
			tinyxml2::XMLElement* layerElement = childLayer->ToElement();
			LAYER newLayer;

			char* name = const_cast<char*>(layerElement->Attribute("name"));
			memcpy(newLayer.name, name, sizeof(newLayer.name));

			newLayer.width = layerElement->IntAttribute("width");
			newLayer.height = layerElement->IntAttribute("height");

			//get all the data from this layer
			tinyxml2::XMLNode* data = childLayer->FirstChildElement("data");
			for (tinyxml2::XMLElement* child = data->FirstChildElement("tile"); child != NULL; 
				child = child->NextSiblingElement("tile"))
			{
				TILE newTile;
				newTile.tile_id = child->IntAttribute("gid");
				newLayer.data.push_back(newTile);

				//Add all of the rotation tile code here...
			}

			m_map.layer.push_back(newLayer);
		}
	}
}

TMX_Parser::~TMX_Parser()
{

}