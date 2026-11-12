#include "Stand.h"
#include "Geometory.h"
#include <DirectXMath.h>
#include"Defines.h"


CStand::CStand()
	:m_pos(0.0f, 0.0f, 0.0f)
	, angle(0)
	, LeftPos{ 0.0f,0.0f }
	, RightPos{ 0.0f,0.0f }
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
	//•ÏŠ·s—ñ‚ÌŒvZ
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(0);
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(0);
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(angle);
	DirectX::XMMATRIX R = Rx * Ry * Rz;
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(3.0f, 0.5f, 0.6f);
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

//“ñl‚ÌêŠ‚©‚çêŠEŠp“x‚ÌŒvZ
void CStand::SetPos(DirectX::XMFLOAT4 Pos)
{
	float dx = Pos.z - Pos.x;
	float dy = Pos.w - Pos.y;

	angle = atan2f(dy, dx);

	m_pos.x = (Pos.z + Pos.x) / 2.0f;
	m_pos.y = ((Pos.w + Pos.y) / 2.0f) + 0.5f;

}

