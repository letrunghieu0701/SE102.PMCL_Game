#include <iostream>
#include "tinyxml2.h"
#include<string>
#include <vector>
//#include <sstream>
#include <unordered_map>

using namespace std;
using namespace tinyxml2;

struct rectangle
{
	int left, top, right, bottom;
};

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

vector<vector<int>> GetInfoOnMap(TiXmlElement* root)
{
	TiXmlElement* layer_child = root->FirstChildElement("layer");

	int width, height;
	layer_child->QueryIntAttribute("width", &width);
	layer_child->QueryIntAttribute("height", &height);

	string content = layer_child->FirstChildElement("data")->GetText();
	vector<string> num_tokens = Split(content, ",");

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
			grid[i][j] = stoi(num_tokens[i + (j * width)]);
		}
	}
	return grid;
}


unordered_map<int, rectangle> GetSpritesInfo(TiXmlElement* root, vector<vector<int>> grid)
{
	unordered_map<int, rectangle> spriteInfos;

	// Lấy tên file tileset để mở file đó và lấy các thông tin cần thiết
	TiXmlElement* tileset_layer = root->FirstChildElement("tileset");
	string img_tileset_path = tileset_layer->Attribute("source");

	TiXmlDocument doc_img(img_tileset_path.c_str());
	if (!doc_img.LoadFile())
	{
		printf("%s", doc_img.ErrorDesc());
		return spriteInfos;
	}

	TiXmlElement* texture_root = doc_img.RootElement();
	int spacing, margin, tileWidth, tileHeight, tilecount, column;
	texture_root->QueryIntAttribute("spacing", &spacing);
	texture_root->QueryIntAttribute("margin", &margin);
	texture_root->QueryIntAttribute("tilewidth", &tileWidth);
	texture_root->QueryIntAttribute("tileheight", &tileHeight);
	texture_root->QueryIntAttribute("tilecount", &tilecount);
	texture_root->QueryIntAttribute("columns", &column);


	TiXmlElement* texture_img_layer = texture_root->FirstChildElement();
	int texture_width, texture_height;
	texture_img_layer->QueryIntAttribute("width", &texture_width);
	texture_img_layer->QueryIntAttribute("height", &texture_height);


	int width = grid.size();
	int height = grid[0].size();
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
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

				spriteInfos[sprite_ith].left = margin + x_th * (tileWidth + spacing);
				spriteInfos[sprite_ith].top = margin + y_th * (tileHeight + spacing);
				spriteInfos[sprite_ith].right = spriteInfos[sprite_ith].left + tileWidth - 1;
				spriteInfos[sprite_ith].bottom = spriteInfos[sprite_ith].top + tileHeight - 1;
			}
		}
	}

	return spriteInfos;
}

int main() {

    XMLDocument xmlDoc;
    XMLError eResult = xmlDoc.LoadFile("data.xml");

    if (eResult) {
        cout << XMLDocument::ErrorIDToName(eResult) << endl;
        exit(1);
    }

    XMLElement* root = xmlDoc.RootElement();
    XMLElement* name = root->FirstChildElement();
    XMLElement* comment = name->NextSiblingElement();

    cout << "name: " << name->GetText() << endl;
    cout << "comment: " << comment->GetText() << endl;

    XMLElement* images = root->FirstChildElement("images");
    XMLElement* image = images->FirstChildElement("image");
    const char* filename;

    while (image != nullptr) {
        image->QueryStringAttribute("file", &filename);
        cout << "filename: " << filename << endl;

        int top, left, width, height;
        XMLElement* box = image->FirstChildElement("box");
        while (box != nullptr) {
            box->QueryAttribute("top", &top);
            box->QueryAttribute("left", &left);
            box->QueryAttribute("width", &width);
            box->QueryAttribute("height", &height);

            cout << "\tBox: " << left << " "
                << top << " "
                << width << " "
                << height << " "
                << endl;

            box = box->NextSiblingElement();
        }

        image = image->NextSiblingElement();
    }

    return 0;
}