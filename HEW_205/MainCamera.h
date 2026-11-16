#pragma once

#include "Camera.h"
#include "Input.h"


// クラス
class MainCamera : public Camera
{
private:
	//CLeftPlayer* m_pLeftPlayer;
	float m_radXZ;
	float m_radY;
	float m_radius;

public:
	MainCamera();
	~MainCamera();

	// 更新
	void Update() final;

	void SetPos(DirectX::XMFLOAT4 Pos);

	
};