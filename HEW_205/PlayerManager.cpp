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
	, rightVelocityY(0.0f)
	, leftVelocityY(0.0f)
	, rightJump(false)
	, leftJump(false)
	, L_isGround(false)
	, R_isGround(false)
	, R_IsSideGround(false)
	, L_IsSideGround(false)
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
	//m_pLeftPlayer->SetLineStart(L_Pos);
	//m_pRightPlayer->SetLineStart(R_Pos);

	// 移動速度
	const float MOVE_SPEED = 0.03f;

	// 重力
	const float PLAYERGRAVITY = 0.012f;
	const float JUMPGRAVITY = 0.015f;

	// ジャンプ力
	const float JUMP_POWER = 0.15f;

	// 壁にぶつかっていないときのみプレイヤー移動
	if (IsKeyPress('A') && !L_IsSideGround)
	{
		L_Pos.x -= MOVE_SPEED;
		R_Pos.x -= MOVE_SPEED;
	}
	if (IsKeyPress(VK_LEFT) && !L_IsSideGround)
	{
		L_Pos.x -= MOVE_SPEED;
		R_Pos.x -= MOVE_SPEED;
	}


	if (IsKeyPress('D') && !R_IsSideGround)
	{
		L_Pos.x += MOVE_SPEED;
		R_Pos.x += MOVE_SPEED;
	}
	if (IsKeyPress(VK_RIGHT) && !R_IsSideGround)
	{
		L_Pos.x += MOVE_SPEED;
		R_Pos.x += MOVE_SPEED;
	}

	//地面にいるときだけジャンプ
	// 左プレイヤージャンプ
	if (IsKeyTrigger('W') && !leftJump && L_isGround)
	{
		leftVelocityY = JUMP_POWER;
		leftJump = true;
		L_isGround = false;
	}

	// 右プレイヤージャンプ
	if (IsKeyTrigger(VK_UP) && !rightJump && R_isGround)
	{
		rightVelocityY = JUMP_POWER;
		rightJump = true;
		R_isGround = false;
	}

	//地面にいないかつジャンプしていなければ重力の処理
	if (!L_isGround && !leftJump)
	{
		L_Pos.y -= leftVelocityY;
		leftVelocityY += PLAYERGRAVITY;
	}
		
	if (!R_isGround && !rightJump)
	{
		R_Pos.y -= rightVelocityY;
		rightVelocityY += PLAYERGRAVITY;
	}
		
	// 左ジャンプ更新
	if (leftJump)
	{
		L_Pos.y += leftVelocityY;
		leftVelocityY -= JUMPGRAVITY;
	}

	// 右ジャンプ更新
	if (rightJump)
	{
		R_Pos.y += rightVelocityY;
		rightVelocityY -= JUMPGRAVITY;
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

//右当たり判定（横）
Collision::Info CPlayerManager::RightGetSideCollision()
{
	return m_pRightPlayer->GetSideCollision();
}

//左当たり判定
Collision::Info CPlayerManager::LeftGetCollision()
{
	return m_pLeftPlayer->GetCollision();
}

//左当たり判定（横）
Collision::Info CPlayerManager::LeftGetSideCollision()
{
	return m_pLeftPlayer->GetSideCollision();
}

void CPlayerManager::RightOnCollision(Collision::Result collision)
{
	rightJump = false;
	rightVelocityY = 0.0f;

	R_Pos.y = collision.point.y + 0.5f;
	m_pRightPlayer->SetPos(R_Pos.x, R_Pos.y, 0.0f);

	R_isGround = true;
}

void CPlayerManager::LeftOnCollision(Collision::Result collision)
{
	leftJump = false;
	leftVelocityY = 0.0f;

	L_Pos.y = collision.point.y + 0.5f;
	m_pLeftPlayer->SetPos(L_Pos.x, L_Pos.y, 0.0f);

	L_isGround = true;
}


void CPlayerManager::RightOnSideCollision(Collision::Result collision, int side)
{
	R_IsSideGround = true;

	const float PLAYER_DISTANCE = 2.0f;

	switch (side)
	{
	case 0:
		R_Pos.x = collision.point.x - 0.25f;
		break;

	case 1:
		R_Pos.x = collision.point.x + 0.25f;
		break;
	}

	// 二人の距離を一定にする
	L_Pos.x = R_Pos.x - PLAYER_DISTANCE;

	m_pLeftPlayer->SetPos(L_Pos.x, L_Pos.y, 0.0f);
	m_pRightPlayer->SetPos(R_Pos.x, R_Pos.y, 0.0f);
}

void CPlayerManager::LeftOnSideCollision(Collision::Result collision, int side)
{
	L_IsSideGround = true;

	const float PLAYER_DISTANCE = 2.0f;

	switch (side)
	{
	case 0:
		L_Pos.x = collision.point.x - 0.25f;
		break;

	case 1:
		L_Pos.x = collision.point.x + 0.25f;
		break;
	}

	// 二人の距離を一定にする
	R_Pos.x = L_Pos.x + PLAYER_DISTANCE;

	// 座標を反映
	m_pLeftPlayer->SetPos(L_Pos.x, L_Pos.y, 0.0f);
	m_pRightPlayer->SetPos(R_Pos.x, R_Pos.y, 0.0f);
}


void CPlayerManager::Reset()
{
	L_isGround = false;
	R_isGround = false;
	L_IsSideGround = false;
	R_IsSideGround = false;
}





