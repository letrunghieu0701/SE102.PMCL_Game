#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"

#include "Brick.h"
#include "QuestionBrick.h"
#include "BreakableBrick.h"

#include "Coin.h"

#include "Mario.h"
#include "Goomba.h"
//#include "Koopas.h"
#include "ImageMap.h"

#define DEFAULT_ID_BASE_PLATFORM 100

class CPlayScene : public CScene
{
protected:
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;

	vector<LPGAMEOBJECT> objects;

	vector<CBreakableBrick*> breakable_bricks;

	// Chứa những cặp (id, game object) của những game object để liên kết với những game object khác,
	// làm vậy để có thể kích hoạt các sự kiện, ví dụ như khi Mario va chạm ? brick từ bên dưới, thì lát nữa kích hoạt sự kiện Mushroom trồi lên
	// VD: một cặp (mushroom_id, mushroom) dùng để liên kết với một ? brick
	unordered_map<int, LPGAMEOBJECT> itemsInside;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void LoadAssets(LPCWSTR assetFile);

public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }
	unordered_map<int, LPGAMEOBJECT> GetItemList() { return itemsInside; }
	vector<CBreakableBrick*> GetBreakableBricks() { return this->breakable_bricks; }

	void AddGameObject(LPGAMEOBJECT obj) { this->objects.push_back(obj); }
	void AddBreakableBricks(CBreakableBrick* obj){this->breakable_bricks.push_back(obj);}

	void ClearBreakableBricks() { this->breakable_bricks.clear(); }
	int GetBBricksSize() { return this->breakable_bricks.size(); }

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CPlayScene* LPPLAYSCENE;

