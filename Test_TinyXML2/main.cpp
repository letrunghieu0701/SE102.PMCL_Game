#include <iostream>
#include "tinyxml.h"
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>

using namespace std;

class CTile
{
private:
	int x, y, width, height;
public:
	CTile()
	{
		this->x = 0;
		this->y = 0;
		this->width = 16;
		this->height = 16;
	}
	CTile(int x, int y, int width, int height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}
	int GetX() { return this->x; }
	int GetY() { return this->y; }
	int GetWidth() { return this->width; }
	int GetHeight() { return this->height; }
};
typedef CTile* LPTILE;

vector<string> Split(string content, string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	string token;
	vector<string> tokens;

	while ((pos_end = content.find(delimiter, pos_start)) != string::npos) {
		token = content.substr(pos_start, pos_end - pos_start);
		tokens.push_back(token);

		pos_start = pos_end + delim_len;	// Cộng thêm length của delim để trỏ pos_start sang vị trí kí tự đầu tiên của token tiếp theo
	}

	tokens.push_back(content.substr(pos_start));
	return tokens;
}

vector<string> split(string line, string delimeter)
{
	vector<string> tokens;
	size_t last = 0; size_t next = 0;
	while ((next = line.find(delimeter, last)) != string::npos)
	{
		tokens.push_back(line.substr(last, next - last));
		last = next + 1;
	}
	tokens.push_back(line.substr(last));

	return tokens;
}


// Load tag data của layer để điền vào grid 2D [width, height]
vector<vector<int>> GetLayer(TiXmlElement* root, vector<LPTILE> layer)
{

	TiXmlElement* layer_child = root->FirstChildElement("layer");

	int width, height;
	layer_child->QueryIntAttribute("width", &width);
	layer_child->QueryIntAttribute("height", &height);

	string content = layer_child->FirstChildElement("data")->GetText();
	vector<string> num_tokens = split(content, ",");

	// Vị trí thứ i của num_tokens sẽ là ô có sprite
	// Giá trị tại vị trí i:
	// Bằng 0: ô này ko có sprite
	// Khác 0: ô này có sprite, giá trị này là số thứ tự (bắt đầu từ 1, ko phải 0) của sprite trong ảnh texture
	/*for (int i = 0; i < num_tokens.size(); i++)
	{
		if (stoi(num_tokens[i]) != 0)
			cout << "Position: " << i << " number: " << stoi(num_tokens[i]) << endl;
	}*/

	// Điền vào grid theo trình tự: trái -> phải, trên -> dưới
	vector<vector<int>> grid(width, vector<int>(height));
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			grid[i][j] = stoi(num_tokens[i + (j*width)]);
		}
	}
	return grid;
}


void GetSpritesOfLayer(TiXmlElement* root, vector<vector<int>> grid, vector<LPTILE>& sprites_of_layer)
{
	// Lấy tên file tileset để mở file đó và lấy các thông tin cần thiết
	TiXmlElement* tileset_layer = root->FirstChildElement("tileset");
	string img_tileset_path = tileset_layer->Attribute("source");

	TiXmlDocument doc_img(img_tileset_path.c_str());
	if (!doc_img.LoadFile())
	{
		printf("%s", doc_img.ErrorDesc());
		return;
	}

	TiXmlElement* tile_set_ele = doc_img.RootElement();
	int spacing, margin, tileWidth, tileHeight, tilecount, column;
	tile_set_ele->QueryIntAttribute("spacing", &spacing);
	tile_set_ele->QueryIntAttribute("margin", &margin);
	tile_set_ele->QueryIntAttribute("tilewidth", &tileWidth);
	tile_set_ele->QueryIntAttribute("tileheight", &tileHeight);
	tile_set_ele->QueryIntAttribute("tilecount", &tilecount);
	tile_set_ele->QueryIntAttribute("columns", &column);


	TiXmlElement* tileset_img_layer = tile_set_ele->FirstChildElement("image");
	int texture_width, texture_height;
	tileset_img_layer->QueryIntAttribute("width", &texture_width);
	tileset_img_layer->QueryIntAttribute("height", &texture_height);


	int width = grid.size();
	int height = grid[0].size();

	// Duyệt từ trái -> phải, trên -> dưới để có thể dễ dàng debug (vì ta đã biết rõ thứ tự của các sprite trong layer)
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			int sprite_ith = grid[i][j];	// Vị trí của sprite (ith) trong ảnh texture bắt đầu từ số 1, ko phải số 0
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
				LPTILE tile = new CTile(x, y, tileWidth, tileHeight);

				sprites_of_layer.push_back(tile);
			}
		}
	}
}


int main() {

	// Load xml file 
	TiXmlDocument doc("Scenes_1-1.tmx");
	//TiXmlDocument doc("First_half_world_1-1.tmx");
	//TiXmlDocument doc("Authors.xml");
	if (!doc.LoadFile())
	{
		printf("%s", doc.ErrorDesc());
		return -1;
	}

	// Lay thong tin node goc
	TiXmlElement* root = doc.RootElement();
	for (auto layer = root->FirstChildElement("layer"); layer != NULL; layer = layer->NextSiblingElement("layer"))
	{
		cout << layer->Attribute("name");
	}


	/*TiXmlElement* texture_img_layer = root->FirstChildElement("layer");
	cout << texture_img_layer->NextSiblingElement("layer")->Attribute("id");*/

	//// Lấy tọa độ những chỗ cần vẽ trên map
	//vector<vector<int>> grid = GetLayer(root);

	//int width = grid.size();
	//int height = grid[0].size();
	//cout << "Width: " << width << endl;
	//cout << "Height: " << height << endl;

	//for (int j = 0; j < height; j++)
	//{
	//	for (int i = 0; i < width; i++)
	//	{
	//		//if (grid[i][j] > 0)
	//		//{
	//		//	cout /*<< i << " " << j << " "*/ << grid[i][j];
	//		//}

	//		cout << grid[i][j] << ",";
	//	}
	//	cout << endl;
	//}

	// Lấy tọa độ của các sprite cần vẽ

	/*vector<LPTILE> sprites_in_layer;
	GetSpritesOfLayer(root, grid, sprites_in_layer);

	for (auto i = 0; i < sprites_in_layer.size(); i++)
	{
		cout << sprites_in_layer[i]->GetX() << " "
			<< sprites_in_layer[i]->GetY() << " "
			<< sprites_in_layer[i]->GetWidth() << " "
			<< sprites_in_layer[i]->GetHeight() << endl;
	}*/

	return 0;
}