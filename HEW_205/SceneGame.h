#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "Ground.h"
#include "Goal.h"
#include "Scene.h"
#include "Camera.h"
#include "Model.h"

class SceneGame : public Scene
{
public:
	SceneGame();
	~SceneGame();
	void Update() final;
	void Draw() final;

private:
	Ground* m_pGround; //地面オブジェクト
	Goal* m_pGoal; //ゴールオブジェクト
	Camera* m_pCamera; //継承元のカメラクラスのみ
	Model* m_pModel;

};

#endif // __SCENE_GAME_H__