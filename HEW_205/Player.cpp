#include "Player.h"
#include "Geometory.h"
#include <DirectXMath.h>
#include "Defines.h"
#include "Input.h"

CPlayer::CPlayer()
    : m_pos{ 0.0f,0.0f,0.0f }
    , m_speedY(0.012f)
    , m_isGround(false)
    , m_collision()
    , m_sidecollision()
{
    m_collision.type = Collision::eLine;
    m_sidecollision.type = Collision::eLine;
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update()
{
    /*
    // 移動速度
    const float MOVE_SPEED = 0.05f;
    const float JUMP_MOVE_SPEED = 0.02f;

    // ジャンプ中かどうかで移動速度を変更
    float moveSpeed = MOVE_SPEED;

    if (!m_isGround)
    {
        moveSpeed = JUMP_MOVE_SPEED;
    }

    // 移動処理
    if (IsKeyPress('A') && IsKeyPress(VK_LEFT))
    {
        m_pos[0] -= moveSpeed;
    }

    if (IsKeyPress('D') && IsKeyPress(VK_RIGHT))
    {
        m_pos[0] += moveSpeed;
    }
    */


    // 当たり判定の更新
    m_collision.line.start = {
        m_pos[0],
        m_pos[1],
        m_pos[2]
    };

    m_collision.line.end = {
        m_pos[0],
        m_pos[1] - 0.55f,
        m_pos[2]
    };

    //横方向
    m_sidecollision.line.start = {
        m_pos[0] - 0.25f,
        m_pos[1],
        m_pos[2]
    };

    //移動方向に線分を
    m_sidecollision.line.end = {
       m_pos[0] + 0.25f ,
       m_pos[1],
       m_pos[2]
    };

}

void CPlayer::Draw()
{
    // 変換行列の計算
    DirectX::XMMATRIX T =
        DirectX::XMMatrixTranslation(
            m_pos[0], m_pos[1], m_pos[2]);

    DirectX::XMMATRIX Rx =
        DirectX::XMMatrixRotationX(0);

    DirectX::XMMATRIX Ry =
        DirectX::XMMatrixRotationY(0);

    DirectX::XMMATRIX Rz =
        DirectX::XMMatrixRotationZ(0);

    DirectX::XMMATRIX R = Rx * Ry * Rz;

    DirectX::XMMATRIX S =
        DirectX::XMMatrixScaling(0.5f, 1.0f, 0.5f);

    DirectX::XMMATRIX mat = S * R * T;

    // GPUに渡すための前処理
    mat = DirectX::XMMatrixTranspose(mat);

    DirectX::XMFLOAT4X4 fMat;
    DirectX::XMStoreFloat4x4(&fMat, mat);

    // 描画処理
    Geometory::SetWorld(fMat);
    Geometory::DrawBox();
}

void CPlayer::SetPos(float x, float y, float z)
{
    m_pos[0] = x;
    m_pos[1] = y;
    m_pos[2] = z;
}

DirectX::XMFLOAT2 CPlayer::GetPos()
{
    DirectX::XMFLOAT2 Pos = {
        m_pos[0],
        m_pos[1]
    };

    return Pos;
}

void CPlayer::SetLineStart(DirectX::XMFLOAT3 Pos)
{
    m_collision.line.start = Pos;
}