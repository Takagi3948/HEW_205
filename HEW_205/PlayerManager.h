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

public:
	CPlayerManager();
	~CPlayerManager();

	//XVˆ—
	void Update();

	//•`‰æˆ—
	void Draw();

	DirectX::XMFLOAT4 GetPos();

};