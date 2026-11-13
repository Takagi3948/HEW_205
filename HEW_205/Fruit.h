
#include <DirectXMath.h>
#include "Shader.h"
#include "MeshBuffer.h"
#include "ShaderList.h"
#include "Model.h"
#include "Camera.h"
#include "CameraDebug.h"


class CFruit {
private:
	DirectX::XMFLOAT3 m_pos;		//座標
	float radius;	//半径
	Model* m_pModel;
	Camera* m_pCamera;


public:


	CFruit();
	~CFruit();

	//更新処理
	void Update();

	//描画処理
	void Draw();


	//座標の設定
	void SetPos(float x, float y, float z);

};