#include "MainCamera.h"


MainCamera::MainCamera()
	//:m_pLeftPlayer(nullptr)
	: m_radXZ(0.0f)
	, m_radY(0.0f)
	, m_radius(5.0f)
{
	//m_pLeftPlayer = new CLeftPlayer();
}

MainCamera::~MainCamera()
{

}

void MainCamera::Update()
{

}

void MainCamera::SetPos(DirectX::XMFLOAT4 Pos)
{
	m_pos.x = (Pos.x + Pos.z) / 2;
	m_look.x = (Pos.x + Pos.z) / 2;
}

