#pragma once
#include "TileLayer.h"
#include "Utils.h"

class CTileMap
{
private:
	unordered_map<int, LPTILELAYER> layers;
	string filePath;
public:
	CTileMap(string path) { this->filePath = path; };
	void MakeLayers();
	void Render();
	//void Clear();
};
typedef CTileMap LPTILEMAP;

