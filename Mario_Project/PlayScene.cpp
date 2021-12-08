#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Platform.h"
#include "InvisiblePlatform.h"
#include "Mushroom.h"
#include "WingGoomba.h"
#include "Koopa.h"
#include "ChangeDirectionOnPlatform.h"
#include "Pipe.h"

#include "SampleKeyEventHandler.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y, object_type);
		player = (CMario*)obj;

		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(x, y, object_type); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(x, y, object_type); break;
	case OBJECT_TYPE_COIN:
	{
		obj = new CCoin(x, y, object_type);
		break;
	}
	case OBJECT_TYPE_BREAKABLE_BRICK:
	{
		int coin_life_time = atoi(tokens[3].c_str());
		obj = new CBreakableBrick(x, y, object_type, coin_life_time);

		breakable_bricks.push_back((CBreakableBrick*)obj);
		break;
	}
	case OBJECT_TYPE_PIPE:
	{
		obj = new CPipe(x, y, object_type);
		break;
	}
	case OBJECT_TYPE_KOOPA:
	{
		int id_CDOP = atoi(tokens[3].c_str());
		obj = new CKoopa(x, y, object_type, id_CDOP);
		break;
	}
	case OBJECT_TYPE_CDOP:
	{
		int id = atoi(tokens[3].c_str());
		obj = new CChangeDirectionOnPlatform(x, y, object_type, id);

		itemsInside.insert(make_pair(id, obj));
		break;
	}
	case OBJECT_TYPE_WING_GOOMBA:
	{
		float walking_distance = (float)atof(tokens[3].c_str());

		obj = new CWingGoomba(x, y, object_type, walking_distance);
		break;
	}
	case OBJECT_TYPE_QUESTION_BRICK:
	{
		int item_id;
		if (tokens.size() > 3)
		{
			item_id = atoi(tokens[3].c_str());
			obj = new CQuestionBrick(x, y, object_type, item_id);
		}
		else
			obj = new CQuestionBrick(x, y, object_type);

		break;
	}
	case OBJECT_TYPE_MUSHROOM:
	{
		int mushroom_id = atoi(tokens[3].c_str());
		obj = new CMushroom(x, y, object_type, mushroom_id);

		itemsInside.insert(make_pair(mushroom_id, obj));
		break;
	}
	case OBJECT_TYPE_PLATFORM:
	{
		bool base = false;
		if (tokens.size() > 9)	// Nếu đây là platform base - camera không dịch chuyển theo trục y khi Mario đứng trên platform này (giúp player không bị chóng mặt)
			base = true;

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CPlatform(
			x, y, object_type, base,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		if (base == true)
		{
			int base_platform_ID = atoi(tokens[10].c_str());
			itemsInside.insert(make_pair(base_platform_ID, obj));
		}
		break;
	}
	case OBJECT_TYPE_INVISIBLE_PLATFORM:
	{
		int width = atoi(tokens[3].c_str());
		int height = atoi(tokens[4].c_str());
		obj = new CInvisiblePlatform(x, y, object_type, width, height);
		break;
	}
	case OBJECT_TYPE_IMAGEMAP:
	{
		int id = atoi(tokens[3].c_str());
		obj = new CImageMap(x, y, object_type, id);
		DebugOut(L"[INFO] Image map object has been created!\n");

		break;
	}
	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id, object_type);
	}
	break;


	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);


	objects.push_back(obj);
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way
	/* "dirty" là vì chưa xử dụng phân hoạch không gian(Grid, Quad Tree, ...) để xử lý va chạm cho ít game object hơn.
		 Xử lý ít hơn là để cho game chạy nhanh hơn, mà cũng là vì những object không ở gần cửa sổ game thì cũng không cần update làm chi
		 (đỡ tốn thời gian tính toán cho CPU).
		 Nên mới cần phân hoạch không gian để chọn ra những object thực sự cần thiết để update.
	*/


	CMario* mario = dynamic_cast<CMario*>(objects[0]);

	if (mario->IsStopUpdate() == false)
	{
		vector<LPGAMEOBJECT> coObjects;
		for (size_t i = 1; i < objects.size(); i++)
		{
			coObjects.push_back(objects[i]);
		}

		for (size_t i = 0; i < objects.size(); i++)
		{
			objects[i]->Update(dt, &coObjects);
		}

		// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
		if (player == NULL) return;

		// Update camera to follow mario
		float cx, cy;
		player->GetPosition(cx, cy);

		CGame* game = CGame::GetInstance();
		cx -= game->GetBackBufferWidth() / 2;
		//cy -= game->GetBackBufferHeight() / 2;


		CPlatform* base_platform = dynamic_cast<CPlatform*>(this->itemsInside[DEFAULT_ID_BASE_PLATFORM]);
		float base_platform_x;
		float base_platform_y;
		base_platform->GetPosition(base_platform_x, base_platform_y);

		// Nếu Mario vẫn nằm trong khoảng giữa base platform và screen height, thì giữ nguyên camera cách base platform một khoảng bằng screen height
		if (((base_platform_y - game->GetBackBufferHeight()) <= cy) &&
			(cy <= base_platform_y))
			cy = base_platform_y + base_platform->GetCellHeight() - game->GetBackBufferHeight();
		else
			//cy -= game->GetBackBufferHeight() / 2;
			;

		if (cx < 0) cx = 0;

		CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);
		/*DebugOutTitle(L"Camera: %0.2f, %0.2f", cx, cy);*/

		PurgeDeletedObjects();
	}
}

void CPlayScene::Render()
{
	// Vẽ theo chiều ngược lại vì Mario thuộc vị trí đầu tiên của objects, theo sau là các object và cuối cùng là map
	for (int i = objects.size() - 1; i > -1; i--)
		objects[i]->Render();
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];
	objects.clear();

	// Các item trong itemsInside đã được xóa cùng với các game object khác khi xóa các game object trong objects
	// Nên bây giờ chỉ cần clear itemsInside mà thôi
	itemsInside.clear();

	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}