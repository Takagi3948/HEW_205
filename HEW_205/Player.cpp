#include "Player.h"
#include "Geometory.h"
#include <DirectXMath.h>
#include"Defines.h"
#include "Input.h"

CPlayer::CPlayer()
	:m_pLeftPlayer(new CLeftPlayer())
	,m_pRightPlayer(new CRightPlayer())
{
	m_pLeftPlayer->SetPos(-1.0f, 0.5f, 0.0f);
	//m_pLeftPlater->SetPos(-1.0f, 0.5f, 0.0f);

	m_pRightPlayer->SetPos(1.0f, 0.5f, 0.0f);
	//m_pRightPlater->SetPos(1.0f, 0.5f, 0.0f);
}


CPlayer::~CPlayer()
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

void CPlayer::Update()
{
    //
    // プレイヤー座標
    static float leftX = -1.0f;
    static float leftY = 0.5f;

    static float rightX = 1.0f;
    static float rightY = 0.5f;

    // ジャンプ用
    static float leftVelocityY = 1.0f;
    static float rightVelocityY = 0.0f;

    static bool leftJump = false;
    static bool rightJump = false;

    // 移動速度
    const float MOVE_SPEED = 0.05f;

    // 重力
    const float GRAVITY = 0.012f;

    // ジャンプ力
    const float JUMP_POWER = 0.25f;

    // 左プレイヤー移動
    if (IsKeyPress('A') && IsKeyPress(VK_LEFT))
    {
        leftX -= MOVE_SPEED;
    }

    if (IsKeyPress('D') && IsKeyPress(VK_RIGHT))
    {
        leftX += MOVE_SPEED;
    }

    // 右プレイヤー移動
    if (IsKeyPress('A') && IsKeyPress(VK_LEFT))
    {
        rightX -= MOVE_SPEED;
    }

    if (IsKeyPress('D') && IsKeyPress(VK_RIGHT))
    {
        rightX += MOVE_SPEED;
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

    // 左ジャンプ更新
    if (leftJump)
    {
        leftY += leftVelocityY;
        leftVelocityY -= GRAVITY;

        if (leftY <= 0.5f)
        {
            leftY = 0.5f;
            leftVelocityY = 0.0f;
            leftJump = false;
        }
    }

    // 右ジャンプ更新
    if (rightJump)
    {
        rightY += rightVelocityY;
        rightVelocityY -= GRAVITY;

        if (rightY <= 0.5f)
        {
            rightY = 0.5f;
            rightVelocityY = 0.0f;
            rightJump = false;
        }
    }

    // 座標を反映
    m_pLeftPlayer->SetPos(leftX, leftY, 0.0f);
    m_pRightPlayer->SetPos(rightX, rightY, 0.0f);
}

void CPlayer::Draw()
{
	m_pLeftPlayer->Draw();
	m_pRightPlayer->Draw();
}



