#include "Stand.h"
#include "Geometory.h"
#include <DirectXMath.h>
#include"Defines.h"


CStand::CStand()
	:m_pos(0.0f, 0.0f, 0.0f)
	, angle(0)
	, LeftPos{ 0.0f,0.0f }
	, RightPos{ 0.0f,0.0f }
	,m_size(3.0f,0.5f,0.6f)
	,m_move(0.0f,0.0f,0.0f)
	,m_isFirst(true)
	//, m_collision()
{
	/*m_collision.type = Collision::eBox;
	m_collision.box = {
	 m_pos, DirectX::XMFLOAT3(3.0f, 0.5f, 0.6f)
	};*/

}

CStand::~CStand()
{
}

void CStand::Update()
{

}

//void CStand::Update()
//{
//	m_collision.box.center = m_pos;
//}

void CStand::Draw()
{
	//変換行列の計算
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(0);
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(0);
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(angle);
	DirectX::XMMATRIX R = Rx * Ry * Rz;
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(m_size.x,m_size.y, m_size.z);//新たにm_sizeを作ったのでこちらで代用。
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

//二人の場所から場所・角度の計算
void CStand::SetPos(DirectX::XMFLOAT4 Pos)
{
	float dx = Pos.z - Pos.x;
	float dy = Pos.w - Pos.y;

	angle = atan2f(dy, dx);

	DirectX::XMFLOAT3 old = m_pos;//更新前の位置

	m_pos.x = (Pos.z + Pos.x) / 2.0f;
	m_pos.y = ((Pos.w + Pos.y) / 2.0f) + 0.5f;

	//台の移動量
	if (m_isFirst)
	{
		m_move = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_isFirst = false;
	}
	else
	{
		m_move = DirectX::XMFLOAT3(m_pos.x - old.x, m_pos.y - old.y, 0.0f);
	}

}

// 【追加】台の上面の平面
Collision::Plane CStand::GetTopPlane(float offset) const
{
	const float nx = -sinf(angle);
	const float ny =  cosf(angle);
	const float h = m_size.y * 0.5f + offset;

	Collision::Plane p;
	p.pos = DirectX::XMFLOAT3(m_pos.x + nx * h, m_pos.y + ny * h, m_pos.z);
	p.normal = DirectX::XMFLOAT3(nx, ny, 0.0f);

	return p;
}

bool CStand::IsWithinLength(const DirectX::XMFLOAT3& p) const
{
	//台のローカルｘ軸方向の位置
	const float lx = (p.x - m_pos.x) * cosf(angle) + (p.y - m_pos.y);
	return fabsf(lx) <= m_size.x * 0.5f;
}

DirectX::XMFLOAT3 CStand::GetMove() const
{
	return m_move;
}

Collision::Box CStand::GetBox()
{
	return Collision::Box{ m_pos,{3.0f,0.5f,0.6f} };
}
