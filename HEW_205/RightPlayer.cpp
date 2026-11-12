#include "RightPlayer.h"
#include "Geometory.h"
#include <DirectXMath.h>
#include"Defines.h"
#include "Input.h"

CRightPlayer::CRightPlayer()
	:m_pos{ 0.0f,0.0f,0.0f }
{
}

CRightPlayer::~CRightPlayer()
{
}

void CRightPlayer::Update()
{
	//ジャンプ


	//重力処理

	//当たり判定

}

void CRightPlayer::Draw()
{
	//変換行列の計算
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(m_pos[0], m_pos[1], m_pos[2]);
	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(0);
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(0);
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(0);
	DirectX::XMMATRIX R = Rx * Ry * Rz;
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(0.5f, 1.0f, 0.5f);
	DirectX::XMMATRIX mat = S * R * T;

	//変換行列をGPUに渡すための前処理
	mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fMat;
	//matをfMatに格納
	DirectX::XMStoreFloat4x4(&fMat, mat);


	//描画処理
	Geometory::SetWorld(fMat);
	Geometory::DrawBox();

}

void CRightPlayer::SetPos(float x, float y, float z)
{
	m_pos[0] = x;
	m_pos[1] = y;
	m_pos[2] = z;
}

DirectX::XMFLOAT2 CRightPlayer::GetPos()
{
	DirectX::XMFLOAT2 Pos = { m_pos[0],m_pos[1] };
	return Pos;
}

