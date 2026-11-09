#include "CameraDebug.h"
#include "Input.h"
#include "Defines.h"
#include "Geometory.h"

CameraDebug::CameraDebug()
	:m_radXZ(0.0f)
	,m_radY(0.0f)
	,m_radius(5.0f)
{

}

CameraDebug::~CameraDebug()
{

}

void CameraDebug::Update()
{
	//キー入力に合わせた注視点の移動距離
	//直接数字で移動速度を設定するより、
	//１秒間の移動距離から１フレームの速度が計算できるとよい
	//
	const float speed = (5.0f) / fFPS;
	if (IsKeyPress(VK_UP))		{ m_look.z += speed; }
	if (IsKeyPress(VK_DOWN))	{ m_look.z -= speed; }
	if (IsKeyPress(VK_LEFT))	{ m_look.x -= speed; }
	if (IsKeyPress(VK_RIGHT))	{ m_look.x += speed; }
	if (IsKeyPress(VK_SHIFT))	{ m_look.y += speed; }
	if (IsKeyPress(VK_CONTROL))	{ m_look.y -= speed; }

	//カメラ位置の移動
	//回り込みの処理
	const float rotate = DirectX::XMConvertToRadians(180.0f / FPS);
	if (IsKeyPress('A')) { m_radXZ += rotate; }
	if (IsKeyPress('D')) { m_radXZ -= rotate; }
	if (IsKeyPress('W')) { m_radY += rotate; }
	if (IsKeyPress('S')) { m_radY -= rotate; }

	if (m_radY >= DirectX::XMConvertToRadians(85.0f))
		m_radY = DirectX::XMConvertToRadians(85.0f);

	if (m_radY < DirectX::XMConvertToRadians(-85.0f))
		m_radY = DirectX::XMConvertToRadians(-85.0f);

	//カメラの距離を変える処理
	const float radiusspeed = METER(1.0f) / FPS;
	if (IsKeyPress('Q')) { m_radius += radiusspeed; }
	if (IsKeyPress('E')) { m_radius -= radiusspeed; }

	//カメラ位置の計算
	m_pos.x = cosf(m_radY) * -sinf(m_radXZ) * m_radius + m_look.x;
	m_pos.y = sinf(m_radY) * m_radius + m_look.y;
	m_pos.z = cosf(m_radY) * -cosf(m_radXZ) * m_radius + m_look.z;

}