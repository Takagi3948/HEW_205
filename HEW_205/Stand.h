
#include <DirectXMath.h>
#include "Shader.h"
#include "MeshBuffer.h"
//#include "Collision.h"

class CStand {
private:
	DirectX::XMFLOAT3 m_pos;		//座標
	float angle;
	DirectX::XMFLOAT2 LeftPos;
	DirectX::XMFLOAT2 RightPos;
	//Collision::Info m_collision;

public:
	CStand();
	~CStand();

	//更新処理
	void Update();

	//描画処理
	void Draw();


	//座標の設定
	void SetPos(DirectX::XMFLOAT4 Pos);

	//Collision::Info GetCollision() {
	//	return m_collision;
	//}
	//virtual void OnCollision(Collision::Result collision) {}


};