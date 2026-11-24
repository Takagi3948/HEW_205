#include "Enemy.h"
#include "Geometory.h"
#include <DirectXMath.h>

CEnemy::CEnemy(float posx, float posy, float posz, float sizex, float sizey, float sizez, int type)
	:m_pos{ posx,posy,posz }
	,m_scale{ sizex,sizey,sizez }
	,type(type)
	,m_collision()
{
	m_collision.type = Collision::eBox;
}

CEnemy::~CEnemy()
{
}

//更新処理
void CEnemy::Update()
{
	//座標の設定
	m_collision.box.center.x = m_pos[0];
	m_collision.box.center.y = m_pos[1];
	m_collision.box.center.z = m_pos[2];

	//サイズの設定
	m_collision.box.size.x = m_scale[0];
	m_collision.box.size.y = m_scale[1];
	m_collision.box.size.z = m_scale[2];

}

//描画処理
void CEnemy::Draw()
{
	//変換行列の計算
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(m_pos[0], m_pos[1], m_pos[2]);
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(m_scale[0], m_scale[1], m_scale[2]);
	DirectX::XMMATRIX mat = S * T;

	//変換行列をGPUに渡すための前処理
	mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fMat;
	//matをfMatに格納
	DirectX::XMStoreFloat4x4(&fMat, mat);


	//描画処理
	Geometory::SetWorld(fMat);
	Geometory::DrawBox();
}


//座標の設定
void CEnemy::SetPos(float x, float y, float z)
{
	m_pos[0] = x;
	m_pos[1] = y;
	m_pos[2] = z;
}

//拡大率の設定
void CEnemy::SetScale(float x, float y, float z)
{
	m_scale[0] = x;
	m_scale[1] = y;
	m_scale[2] = z;
}
