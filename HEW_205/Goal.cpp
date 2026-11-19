#include "Goal.h"
#include "Fruit.h"
#include "Geometory.h"
#include <DirectXMath.h>
#include"Defines.h"

Goal::Goal()
	:m_pos{ 0.0f,0.0f,0.0f }
	, m_time(0)	
{
	m_box.center = DirectX::XMFLOAT3(m_pos[0], m_pos[1], m_pos[2]);
	m_box.size = DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f);
}

Goal::~Goal()
{
}

void Goal::Update(CFruit* fruit)
{
	m_time += 1.0f / FPS;

	// ゴールの当たり判定情報
	m_box.center = DirectX::XMFLOAT3(m_pos[0], m_pos[1], m_pos[2]);

	// フルーツの当たり判定情報
	Collision::Sphere sphere;

	//sphere.center = fruit->GetPos();
	//sphere.radius = fruit->GetRadius();

	// 球と四角の当たり判定
	Collision::Result result = Collision::Hit(sphere, m_box);

	if (result.isHit)
	{
		OutputDebugString("GOAL!\n");
	}
}

void Goal::Draw()
{
	//変換行列の計算
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(m_pos[0], m_pos[1], m_pos[2]);
	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(m_time);
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(m_time);
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(m_time);
	DirectX::XMMATRIX R = Rx * Ry * Rz;
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.0f,1.0f,1.0f);
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

void Goal::SetPos(float x, float y, float z)
{
	m_pos[0] = x;
	m_pos[1] = y;
	m_pos[2] = z;

	m_box.center = DirectX::XMFLOAT3(x, y, z);
}

