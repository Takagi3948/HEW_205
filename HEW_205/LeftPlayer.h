
#include <DirectXMath.h>
#include "Shader.h"
#include "MeshBuffer.h"

class CLeftPlayer {
private:
	float m_pos[3];		//座標

public:
	CLeftPlayer();
	~CLeftPlayer();

	//更新処理
	void Update();

	//描画処理
	void Draw();


	//座標の設定
	void SetPos(float x, float y, float z);

};