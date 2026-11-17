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
	bool leftJump;
	bool rightJump;

public:
	CPlayerManager();
	~CPlayerManager();

	//çXêVèàóù
	void Update();

	//ï`âÊèàóù
	void Draw();

	DirectX::XMFLOAT4 GetPos();

	Collision::Info RightGetCollision();
	Collision::Info LeftGetCollision();

	void RightOnCollision(Collision::Result collision);
	void LeftOnCollision(Collision::Result collision);


};