#include "TileMap.h"



void CTileMap::MakeLayers()
{
	// Load xml file 
	TiXmlDocument doc(this->filePath.c_str());
	if (!doc.LoadFile())
	{
		printf("%s", doc.ErrorDesc());
		return;
	}

	// Lay thong tin node goc
	TiXmlElement* tilemap_root = doc.RootElement();

	// Lấy tên file tileset để mở file đó và lấy các thông tin cần thiết
	TiXmlElement* tileset_layer = tilemap_root->FirstChildElement("tileset");
	string img_tileset_path = tileset_layer->Attribute("source");

	TiXmlDocument doc_img(img_tileset_path.c_str());
	if (!doc_img.LoadFile())
	{
		printf("%s", doc_img.ErrorDesc());
		return;
	}

	TiXmlElement* tile_set_root = doc_img.RootElement();
	int spacing, margin, tileWidth, tileHeight, column;
	tile_set_root->QueryIntAttribute("spacing", &spacing);
	tile_set_root->QueryIntAttribute("margin", &margin);
	tile_set_root->QueryIntAttribute("tilewidth", &tileWidth);
	tile_set_root->QueryIntAttribute("tileheight", &tileHeight);
	tile_set_root->QueryIntAttribute("columns", &column);


	for (auto layer = tilemap_root->FirstChildElement("layer"); layer != NULL; layer = layer->NextSiblingElement("layer"))
	{
		int id, width, height;
		layer->QueryIntAttribute("id", &id);
		layer->QueryIntAttribute("width", &width);
		layer->QueryIntAttribute("height", &height);

		string content = layer->FirstChildElement("data")->GetText();
		vector<string> num_tokens = split(content, ",");


		vector<LPTILE> tiles;
		for (int i = 0; i < num_tokens.size(); i++)
		{
			int sprite_ith = stoi(num_tokens[i]);
			if (sprite_ith != 0)	// Khác 0 tức là vị trí này có sprite
			{
				int x_th, y_th; // Bắt đầu từ số 0
				if (sprite_ith % column > 0) // chia dư, tức là sprite này nằm không nằm ở cột cuối cùng của tileset
				{
					x_th = sprite_ith % column - 1;	// vị trí của sprite trong tileset (sprite_ith) bắt đầu từ 1 nên cần trừ 1 để chuyển về bắt đầu từ 0
					y_th = sprite_ith / column;
				}
				else // nếu chia ko dư thì vị trí x_th của sprite nằm ở cột cuối cùng
				{
					x_th = column;
					y_th = sprite_ith / column - 1;
				}

				int x = margin + x_th * (tileWidth + spacing);
				int y = margin + y_th * (tileHeight + spacing);
				LPTILE tile = new CTile(x, y, sprite_ith);

				tiles.push_back(tile);
			}
		}
		LPTILELAYER tile_layer = new CTileLayer(tiles);
		this->layers[id] = tile_layer;
	}
}
