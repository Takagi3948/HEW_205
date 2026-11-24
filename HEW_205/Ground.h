#pragma once
#include <DirectXMath.h>
#include "Shader.h"
#include "MeshBuffer.h"
#include "Collision.h"

class Ground {
private:
	float m_pos[3];		//座標
	float m_scale[3];	//拡大率
	float m_angle;	//角度
	int type;			//タイプ
	Collision::Info m_collision;		//当たり判定の情報
	Collision::Triangle m_triangle[2]; //三角形の情報

	Collision::Info m_sideCollision[2];	//横方向の当たり判定
	Collision::Triangle m_sideTriangle[4];	//横方向の三角形の情報

public:
	Ground(float posx, float posy, float posz, float sizex, float sizey, float sizez, float angle,int type );
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

	// 三角形2つを取得
	Collision::Triangle* GetTriangle();

	Collision::Info GetCollision() {
		return m_collision;
	}


	//横方向の情報
	Collision::Info* GetSideCollision()
	{
		return m_sideCollision;
	}

	Collision::Triangle* GetSideTriangle()
	{
		return m_sideTriangle;
	}

};
