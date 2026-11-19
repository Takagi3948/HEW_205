#pragma once
#include <DirectXMath.h>
#include "Shader.h"
#include "MeshBuffer.h"

#include "Collision.h"

class CFruit;

class Goal {
private:
	float m_pos[3];		//座標
	float m_time;	//アニメーション経過時間

	Collision::Box m_box;
public:
	Goal();
	~Goal();

	//更新処理
	void Update(CFruit* fruit);

	//描画処理
	void Draw();


	//座標の設定
	void SetPos(float x, float y, float z);

};