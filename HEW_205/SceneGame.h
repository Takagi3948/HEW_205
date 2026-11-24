#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "Goal.h"
#include "Scene.h"
#include "Camera.h"
#include "Model.h"
#include "PlayerManager.h"
#include "Stand.h"
#include "Fruit.h"
#include "StageManager.h"


class SceneGame : public Scene
{
public:
	SceneGame();
	~SceneGame();
	void Update() final;
	void Draw() final;
	void ModelDraw(Model* model,float posX, float posY, float posZ, float sizeX, float sizeY, float sizeZ);
	void R_IsHitGround();
	void L_IsHitGround();


private:
	Goal* m_pGoal; //ゴールオブジェクト
	Camera* m_pCamera; //継承元のカメラクラスのみ
	Model* m_pModel;
	CPlayerManager* m_pPlayerManager;
	CStand* m_pStand;
	CFruit* m_pFruit;
	CStageManager* m_pStageManager;

	

};

#endif // __SCENE_GAME_H__