#pragma once
#include <DirectXMath.h>
#include "Shader.h"
#include "MeshBuffer.h"
#include "Collision.h"

class CStand {
private:
	DirectX::XMFLOAT3 m_pos;		//座標
	float angle;
	DirectX::XMFLOAT2 LeftPos;
	DirectX::XMFLOAT2 RightPos;
	DirectX::XMFLOAT3 m_size; //新規追加 サイズ。
	//Collision::Info m_collision;

public:
	CStand();
	~CStand();

	//更新処理
	void Update();

	//描画処理
	void Draw();


	//座標の設定
	void SetPos(DirectX::XMFLOAT4 Pos);

	//Collision::Info GetCollision() {
	//	return m_collision;
	//}
	//virtual void OnCollision(Collision::Result collision) {}

	//座標の取得(Size,Pos)
	Collision::Box GetBox();

	//角度の取得
	float GetRad();
};