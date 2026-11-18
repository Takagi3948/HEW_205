#pragma once
#include <DirectXMath.h>
#include "Shader.h"
#include "MeshBuffer.h"
#include "Player.h"

class CPlayerManager {
private:
	CPlayer* m_pLeftPlayer;
	CPlayer* m_pRightPlayer;
	DirectX::XMFLOAT4 Position;
	DirectX::XMFLOAT3 R_Pos;
	DirectX::XMFLOAT3 L_Pos;
	float leftVelocityY;		//ジャンプ用
	float rightVelocityY;		//ジャンプ用
	bool leftJump;
	bool rightJump;
	bool L_isGround;
	bool R_isGround;

public:
	CPlayerManager();
	~CPlayerManager();

	//更新処理
	void Update();

	//描画処理
	void Draw();

	DirectX::XMFLOAT4 GetPos();

	Collision::Info RightGetCollision();
	Collision::Info LeftGetCollision();

	void RightOnCollision(Collision::Result collision);
	void LeftOnCollision(Collision::Result collision);


};