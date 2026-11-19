#pragma once
#include <DirectXMath.h>
#include "Shader.h"
#include "MeshBuffer.h"
#include "ShaderList.h"
#include "Model.h"
#include "Camera.h"
#include "CameraDebug.h"
#include "Ground.h"
#include "Stand.h"

class CFruit {
private:
	DirectX::XMFLOAT3 m_pos;		//座標
	DirectX::XMFLOAT3 m_vel;
	float radius;	//半径
	Model* m_pModel;
	Camera* m_pCamera;
	Ground* m_pGround;
	CStand* m_pStand;

public:


	CFruit();
	~CFruit();

	//更新処理
	void Update();

	//描画処理
	void Draw(Camera*);


	//座標の設定
	void SetPos(float x, float y, float z);

	void SetStand(CStand* stand);
};
