#pragma once
#include "Defines.h"
#include <DirectXMath.h>
#include "Shader.h"
#include "MeshBuffer.h"

//カメラ基本クラス
class Camera {
protected:
	DirectX::XMFLOAT3 m_pos;	//座標
	DirectX::XMFLOAT3 m_look;	//注視点
	DirectX::XMFLOAT3 m_up;		//上方ベクトル
	float m_fovy;
	float m_aspect;
	float m_near;
	float m_far;

public:
	Camera();
	virtual ~Camera();  //デストラクタはvirtualを付ける

	virtual void Update() = 0; //純粋仮想関数
	DirectX::XMFLOAT4X4 GetViewMatrix(bool transpose = true);
	DirectX::XMFLOAT4X4 GetProjectionMatrix(bool transpose = true);


	DirectX::XMFLOAT3 GetPos();
	DirectX::XMFLOAT3 GetLook();

};