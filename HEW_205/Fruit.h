#pragma once
#include <DirectXMath.h>
#include "Shader.h"
#include "MeshBuffer.h"
#include "ShaderList.h"
#include "Model.h"
#include "Camera.h"
#include "CameraDebug.h"
#include "Ground.h"
#include "Stand.h"
#include "Collision.h"

class CFruit {
private:
	DirectX::XMFLOAT3 m_pos;		//座標
	DirectX::XMFLOAT3 m_vel;
	float radius;	//半径
	Model* m_pModel;
	Camera* m_pCamera;
	Ground* m_pGround;
	CStand* m_pStand;
	Collision::Info m_GroundCollision;	//対地面
	Collision::Info m_EnemyCollision;	//対エネミー

public:

	CFruit();
	~CFruit();

	//更新処理
	void Update();

	//描画処理
	void Draw();

	//Enemy
	void OnEnemyCollision(Collision::Result collision);

	//Ground
	void OnGroundCollision(Collision::Result collision);

	Collision::Info GetEnemyCollision() { return m_EnemyCollision; }
	Collision::Info GetGroundCollision() { return m_GroundCollision; }


	//座標の設定
	void SetPos(float x, float y, float z);

	void SetCamera(Camera* pCamera)
	{
		m_pCamera = pCamera;
	}

	void SetStand(CStand* pStand)
	{
		m_pStand = pStand;
	}

	void SetGround(Ground* pGround)
	{
		m_pGround = pGround;
	}

};
