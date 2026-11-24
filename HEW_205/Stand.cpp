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
	,m_prevPos(0.0f,0.0f,0.0f)
	,m_prevAngle(0.0f)
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
	//更新前の姿勢を保存（前フレーム）
	m_prevPos = m_pos;
	m_prevAngle = angle;

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
		m_prevPos = m_pos; //最初の１回は前フレーム＝今のフレーム
		m_prevAngle = angle;
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

bool CStand::SweptHit(const DirectX::XMFLOAT3& prev, const DirectX::XMFLOAT3& cur, 
	float offset, DirectX::XMFLOAT3& outPoint, DirectX::XMFLOAT3& outNormal) const
{
	const float h = m_size.y * 0.5f + offset;//台の中心から判定面までの高さ

	//前フレームの台から見た、フルーツの前フレーム位置
	const float c0  = cosf(m_prevAngle), s0 = sinf(m_prevAngle);
	const float dx0 = prev.x - m_prevPos.x, dy0 = prev.y - m_prevPos.y;
	const float lx0 = dx0 * c0 + dy0 * s0;		// 台の長さ方向
	const float ly0 = -dx0 * s0 + dy0 * c0;		// 台のY軸方向

	// 今フレームの台から見た、物の今フレームの位置
	const float c1  = cosf(angle), s1 = sinf(angle);
	const float dx1 = cur.x - m_pos.x, dy1 = cur.y - m_pos.y;
	const float lx1 = dx1 * c1 + dy1 * s1;
	const float ly1 = -dx1 * s1 + dy1 * c1;

	// 表側から裏側へ横切ったときだけ当たり（裏側からは無視）
	if (!(ly0 >= h && ly1 < h)) return false;

	// 横切った位置（台の長さ方向）。ly0 - ly1 は必ず正なので0除算にならない
	const float t  = (ly0 - h) / (ly0 - ly1);
	const float lx = lx0 + (lx1 - lx0) * t;
	if (fabsf(lx) > m_size.x * 0.5f) return false;	// 台の長さの外

	// 今フレームの台の上面に置き直す
	outNormal  = DirectX::XMFLOAT3(-s1, c1, 0.0f);	// 台のY軸
	outPoint.x = m_pos.x + lx * c1 + h * outNormal.x;
	outPoint.y = m_pos.y + lx * s1 + h * outNormal.y;
	outPoint.z = cur.z;
	return true;
}

DirectX::XMFLOAT3 CStand::GetMove() const
{
	return m_move;
}

Collision::Box CStand::GetBox()
{
	return Collision::Box{ m_pos,{3.0f,0.5f,0.6f} };
}

DirectX::XMFLOAT3 CStand::GetPos() const
{
	return m_pos;
}

float CStand::GetAngle() const
{
	return angle;
}

DirectX::XMFLOAT3 CStand::GetPrevPos() const
{
	return m_prevPos;
}

float CStand::GetPrevAngle() const
{
	return m_prevAngle;
}

DirectX::XMFLOAT3 CStand::GetSize() const
{
	return m_size;
}
