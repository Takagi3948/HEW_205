#pragma once
#include <DirectXMath.h>
#include "Shader.h"
#include "MeshBuffer.h"
#include "Collision.h"

class CPlayer {
private:
	float m_pos[3];		//座標
	Collision::Info m_collision; //  当たり判定の情報
	Collision::Info m_sidecollision; //  横の当たり判定の情報
	float m_speedY;
	bool  m_isGround;

public:
	CPlayer();
	~CPlayer();

	Collision::Info GetCollision() {
		return m_collision;
	}

	Collision::Info GetSideCollision() {
		return m_sidecollision;
	}

	//更新処理
	void Update();

	//描画処理
	void Draw();


	//座標の設定
	void SetPos(float x, float y, float z);
	DirectX::XMFLOAT2 GetPos();

	void SetLineStart(DirectX::XMFLOAT3 Pos);



};