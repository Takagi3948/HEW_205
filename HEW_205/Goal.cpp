#include "Goal.h"
#include "Geometory.h"
#include <DirectXMath.h>
#include"Defines.h"

Goal::Goal()
	:m_pos{ 0.0f,0.0f,0.0f }
	, m_time(0)
{
}

Goal::~Goal()
{
}

void Goal::Update()
{
	m_time += 1.0f / FPS;
}

void Goal::Draw()
{
	//•ÏŠ·s—ñ‚ÌŒvZ
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(m_pos[0], m_pos[1], m_pos[2]);
	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(m_time);
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(m_time);
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(m_time);
	DirectX::XMMATRIX R = Rx * Ry * Rz;
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.0f,1.0f,1.0f);
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

void Goal::SetPos(float x, float y, float z)
{
	m_pos[0] = x;
	m_pos[1] = y;
	m_pos[2] = z;
}

