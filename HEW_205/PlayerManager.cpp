#include "PlayerManager.h"
#include "Geometory.h"
#include <DirectXMath.h>
#include"Defines.h"
#include "Input.h"

CPlayerManager::CPlayerManager()
	:m_pLeftPlayer(new CPlayer())
	, m_pRightPlayer(new CPlayer())
	, Position{ 0.0f,0.0f,0.0f,0.0f }
	, R_Pos{ 1.0f,0.5f,0.0f }
	, L_Pos{ -1.0f,0.5f,0.0f }
	, rightJump(false)
	, leftJump(false)
{
	m_pLeftPlayer->SetPos(L_Pos.x, L_Pos.y, L_Pos.z);
	m_pRightPlayer->SetPos(R_Pos.x, R_Pos.y, R_Pos.z);
}


CPlayerManager::~CPlayerManager()
{
	if (m_pLeftPlayer)
	{
		delete m_pLeftPlayer;
		m_pLeftPlayer = nullptr;
	}

	if (m_pRightPlayer)
	{
		delete m_pRightPlayer;
		m_pRightPlayer = nullptr;
	}
}

void CPlayerManager::Update()
{
	//移動前の座標をセット
	m_pLeftPlayer->SetLineStart(L_Pos);
	m_pRightPlayer->SetLineStart(R_Pos);

	// ジャンプ用
	static float leftVelocityY = 1.0f;
	static float rightVelocityY = 0.0f;


	// 移動速度
	const float MOVE_SPEED = 0.025f;

	// 重力
	const float GRAVITY = 0.012f;

	// ジャンプ力
	const float JUMP_POWER = 0.125f;

	// 左プレイヤー移動
	if (IsKeyPress('A'))
	{
		L_Pos.x -= MOVE_SPEED;
		R_Pos.x -= MOVE_SPEED;
	}
	if (IsKeyPress(VK_LEFT))
	{
		L_Pos.x -= MOVE_SPEED;
		R_Pos.x -= MOVE_SPEED;
	}


	if (IsKeyPress('D'))
	{
		L_Pos.x += MOVE_SPEED;
		R_Pos.x += MOVE_SPEED;
	}
	if (IsKeyPress(VK_RIGHT))
	{
		L_Pos.x += MOVE_SPEED;
		R_Pos.x += MOVE_SPEED;
	}


	// 左プレイヤージャンプ
	if (IsKeyTrigger('W') && !leftJump)
	{
		leftVelocityY = JUMP_POWER;
		leftJump = true;
	}

	// 右プレイヤージャンプ
	if (IsKeyTrigger(VK_UP) && !rightJump)
	{
		rightVelocityY = JUMP_POWER;
		rightJump = true;
	}

	L_Pos.y -= GRAVITY;
	R_Pos.y -= GRAVITY;

	// 左ジャンプ更新
	if (leftJump)
	{
		L_Pos.y += leftVelocityY;


		L_Pos.y = 0.5f;
		leftVelocityY = 0.0f;
		//leftJump = false;
	}

	// 右ジャンプ更新
	if (rightJump)
	{
		R_Pos.y += rightVelocityY;

		R_Pos.y = 0.5f;
		rightVelocityY = 0.0f;
		//rightJump = false;
	}

	// 座標を反映
	m_pLeftPlayer->SetPos(L_Pos.x, L_Pos.y, 0.0f);
	m_pRightPlayer->SetPos(R_Pos.x, R_Pos.y, 0.0f);

	m_pLeftPlayer->Update();
	m_pRightPlayer->Update();


}

void CPlayerManager::Draw()
{
	m_pLeftPlayer->Draw();
	m_pRightPlayer->Draw();
}


DirectX::XMFLOAT4 CPlayerManager::GetPos()
{
	DirectX::XMFLOAT2 Left = m_pLeftPlayer->GetPos();
	DirectX::XMFLOAT2 Right = m_pRightPlayer->GetPos();
	Position = { Left.x,Left.y,Right.x,Right.y };

	return Position;
}

//右当たり判定
Collision::Info CPlayerManager::RightGetCollision()
{
	return m_pRightPlayer->GetCollision();
}

//左当たり判定
Collision::Info CPlayerManager::LeftGetCollision()
{
	return m_pLeftPlayer->GetCollision();
}

void CPlayerManager::RightOnCollision(Collision::Result collision)
{
	rightJump = false;
	R_Pos.y = collision.point.y + 0.5f;
	m_pRightPlayer->SetPos(R_Pos.x, R_Pos.y, 0.0f);
}

void CPlayerManager::LeftOnCollision(Collision::Result collision)
{
	leftJump = false;
	L_Pos.y = collision.point.y + 0.5f;
	m_pLeftPlayer->SetPos(L_Pos.x, L_Pos.y, 0.0f);
}



