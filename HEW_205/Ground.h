#pragma once
#include <DirectXMath.h>
#include "Shader.h"
#include "MeshBuffer.h"
#include "Collision.h"

class Ground {
private:
	float m_pos[3];		//座標
	float m_scale[3];	//拡大率

public:
	Ground();
	~Ground();

	//更新処理
	void Update();

	//描画処理
	void Draw();


	//座標の設定
	void SetPos(float x, float y, float z);

	//拡大率の設定
	void SetScale(float x, float y, float z);

	// Box型を渡す処理(Pos,Scale)
	Collision::Box GetBox();
};
