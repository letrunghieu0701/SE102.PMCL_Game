#pragma once

#include <unordered_map>
#include <fstream>
#include "Utils.h"
#include "debug.h"
#include "Sprites.h"
#include "Textures.h"

#define ID_FIRST_WORLD_MAP_1_1 30


struct SpriteInfo
{
	int id;
	int x, y;
};

typedef SpriteInfo* LPSpriteInfo;

class CTileMap
{
private:
	string imgTileMapPath;
	string tilesetPath;
	string imgTilesetPath;
	int numCellWidth, numCellHeight;
	int spacing, margin, tileWidth, tileHeight, tilecount, column;
	int textureWidth, textureHeight;
	
	//unordered_map<int, LPSpriteInfo> spriteInfos;
	vector<LPSpriteInfo> spriteInfos;

public:
	CTileMap(string tileMapPath);
	vector<vector<int>> GetInfoOnMap(TiXmlElement* root);
	void GetSpritesInfo(TiXmlElement* root, vector<vector<int>> grid);
	void ReadMap();
	void Render();
	string GetImgTilesetPath();

};

