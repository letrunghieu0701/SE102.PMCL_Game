#pragma once
#include <vector>
#include "Tile.h"
#include "tinyxml.h"

using namespace std;

class CTileLayer
{
private:
	vector<LPTILE> layer;
public:
	CTileLayer(vector<LPTILE> tiles) { this->layer = tiles; }
	void Render();
};
typedef CTileLayer* LPTILELAYER;
