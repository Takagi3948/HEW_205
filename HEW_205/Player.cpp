#include "Player.h"
#include "Geometory.h"
#include <DirectXMath.h>
#include"Defines.h"
#include "Input.h"

CPlayer::CPlayer()
	:m_pos{ 0.0f,0.0f,0.0f }
	,m_speedY(0.012f)
	,m_isGround(false)
	,m_collision()
{
	// “–‚½‚è”»’è‚Ìí—Ş‚ğü•ª‚É‚·‚é
	m_collision.type = Collision::eLine;
}

CPlayer::~CPlayer()
{
}


void CPlayer::Update()
{

	//“–‚½‚è”»’è‚ÌXV

	m_collision.line.end = {
		m_pos[0],
		m_pos[1] - 0.5f,
		m_pos[2]
	};

}

void CPlayer::Draw()
{
	//•ÏŠ·s—ñ‚ÌŒvZ
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(m_pos[0], m_pos[1], m_pos[2]);
	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(0);
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(0);
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(0);
	DirectX::XMMATRIX R = Rx * Ry * Rz;
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(0.5f, 1.0f, 0.5f);
	DirectX::XMMATRIX mat = S * R * T;

	//•ÏŠ·s—ñ‚ğGPU‚É“n‚·‚½‚ß‚Ì‘Oˆ—
	mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fMat;
	//mat‚ğfMat‚ÉŠi”[
	DirectX::XMStoreFloat4x4(&fMat, mat);


	//•`‰æˆ—
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
	DirectX::XMFLOAT2 Pos = { m_pos[0],m_pos[1] };
	return Pos;
}

void CPlayer::SetLineStart(DirectX::XMFLOAT3 Pos)
{
	m_collision.line.start = Pos;
}