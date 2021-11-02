#include "..\Mario_Project\TileMap.h"
#include "TileMap.h"


CTileMap::CTileMap(string tileMapPath)
{
	this->imgTileMapPath = tileMapPath;
}


vector<vector<int>> CTileMap::GetInfoOnMap(TiXmlElement* root)
{
	TiXmlElement* layer_child = root->FirstChildElement("layer");

	layer_child->QueryIntAttribute("width", &this->numCellWidth);
	layer_child->QueryIntAttribute("height", &this->numCellHeight);

	string content = layer_child->FirstChildElement("data")->GetText();
	vector<string> num_tokens = Split(content, ",");

	// Vị trí thứ i của num_tokens sẽ là ô có sprite
	// Giá trị tại vị trí i:
	// Bằng 0: ô này ko có sprite
	// Khác 0: ô này có sprite, giá trị này là số thứ tự (bắt đầu từ 1, ko phải 0) của sprite trong ảnh texture

	// Điền vào grid theo trình tự: trái -> phải, trên -> dưới
	vector<vector<int>> grid(numCellWidth, vector<int>(numCellHeight));
	ofstream MyFile("filename__2.txt");
	for (int j = 0; j < numCellHeight; j++)
	{
		for (int i = 0; i < numCellWidth; i++)
		{
			grid[i][j] = stoi(num_tokens[i + (numCellWidth * j)]);

			if (grid[i][j] > 0)
				MyFile << i << " " << j << " " << grid[i][j] << endl;
		}
	}
	MyFile.close();

	return grid;
}

void CTileMap::GetSpritesInfo(TiXmlElement* root, vector<vector<int>> grid)
{
	// Lấy tên file tileset để mở file đó và lấy các thông tin cần thiết
	TiXmlElement* tileset_layer = root->FirstChildElement("tileset");
	this->tilesetPath = tileset_layer->Attribute("source");

	TiXmlDocument doc_tileset(this->tilesetPath.c_str());
	if (!doc_tileset.LoadFile())
	{
		printf("%s", doc_tileset.ErrorDesc());
		return;
	}

	// Lấy tên ảnh texture
	TiXmlElement* img_tileset_root = doc_tileset.RootElement();
	TiXmlElement* img_layer = img_tileset_root->FirstChildElement();
	this->imgTilesetPath = img_layer->Attribute("source");

	// Thêm texture vào database
	string text_path = "textures\\" + this->imgTilesetPath;
	CTextures::GetInstance()->Add(ID_FIRST_WORLD_MAP_1_1, ToLPCWSTR(text_path));

	TiXmlElement* texture_root = doc_tileset.RootElement();
	texture_root->QueryIntAttribute("spacing", &this->spacing);
	texture_root->QueryIntAttribute("margin", &this->margin);
	texture_root->QueryIntAttribute("tilewidth", &this->tileWidth);
	texture_root->QueryIntAttribute("tileheight", &this->tileHeight);
	texture_root->QueryIntAttribute("tilecount", &this->tilecount);
	texture_root->QueryIntAttribute("columns", &this->column);

	TiXmlElement* texture_img_layer = texture_root->FirstChildElement();
	/*int texture_width, texture_height;
	texture_img_layer->QueryIntAttribute("width", &texture_width);
	texture_img_layer->QueryIntAttribute("height", &texture_height);*/


	CSprites* sprites = CSprites::GetInstance();
	LPTEXTURE texMap = CTextures::GetInstance()->Get(ID_FIRST_WORLD_MAP_1_1);

	ofstream MyFile("filename.txt");
	int spriteIDsForMap = 0;
	for (int j = 0; j < this->numCellHeight; j++)
	{
		for (int i = 0; i < this->numCellWidth; i++)
		{
			int sprite_ith = grid[i][j];	// Vị trí của sprite (ith) trong ảnh texture bắt đầu từ số 1, ko phải số 0
			if (sprite_ith != 0)	// Khác 0 tức là vị trí này có sprite
			{
				int x_th, y_th; // Bắt đầu từ số 0
				if (sprite_ith % column > 0)
				{
					x_th = sprite_ith % column - 1;
					y_th = sprite_ith / column;
				}
				else
				{
					x_th = column;
					y_th = sprite_ith / column - 1;
				}

				int left = margin + x_th * (tileWidth + spacing);
				int top = margin + y_th * (tileHeight + spacing);
				int right = left + tileWidth - 1;
				int bottom = top + tileHeight - 1;


				SpriteInfo* newSprite = new SpriteInfo();
				newSprite->id = spriteIDsForMap;
				newSprite->x = i;
				newSprite->y = j;

				//this->spriteInfos[sprite_ith] = newSprite;
				this->spriteInfos.push_back(newSprite);

				/*this->spriteInfos[sprite_ith].id = spriteIDsForMap;
				this->spriteInfos[sprite_ith].x = i;
				this->spriteInfos[sprite_ith].y = j;*/

				
				// Thêm sprite vào database
				sprites->Add(spriteIDsForMap, left, top, right, bottom, texMap);
				DebugOut(L"So luong sprite hien tai la: %d \n", spriteIDsForMap);
				spriteIDsForMap++;

				

				// Create and open a text file


				// Write to the file
				MyFile << i << " " << j << " " << grid[i][j] << endl;

				// Close the file


			}
		}
	}
	MyFile.close();
}

void CTileMap::ReadMap()
{
	// Load xml file
	TiXmlDocument doc(this->imgTileMapPath.c_str());
	//TiXmlDocument doc("Authors.xml");
	if (!doc.LoadFile())
	{
		printf("%s", doc.ErrorDesc());
		return;
	}

	// Lay thong tin node goc
	TiXmlElement* root = doc.RootElement();

	// Lấy tọa độ những chỗ cần vẽ trên map
	vector<vector<int>> grid = GetInfoOnMap(root);

	// Lấy tọa độ của các sprite cần vẽ
	GetSpritesInfo(root, grid);
}

void CTileMap::MakeLayers()
{
}

void CTileMap::Render()
{
	CSprites* sprites = CSprites::GetInstance();

	//DebugOut(L"Totel sprite of map: %d \n", spriteInfos.size());
	//for (auto itr = spriteInfos.begin(); itr != spriteInfos.end(); itr++)
	//{
	//	// itr works as a pointer to pair<key, value>
	//	// type itr->first stores the key part and
	//	// itr->second stores the value part
	//	int spriteID = itr->second->id;
	//	int x = itr->second->x * 16;
	//	int y = itr->second->y * 16;
	//	sprites->Get(spriteID)->Draw(x, y);


	//}


	for (int i = 0; i < spriteInfos.size(); i++) {
		int spriteID = spriteInfos[i]->id;
		int x = spriteInfos[i]->x * 16;
		int y = spriteInfos[i]->y * 16;
		sprites->Get(spriteID)->Draw(x, y);
	}
}

string CTileMap::GetImgTilesetPath()
{
	return this->imgTilesetPath;
}

