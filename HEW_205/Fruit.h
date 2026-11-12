
#include <DirectXMath.h>
#include "Shader.h"
#include "MeshBuffer.h"

class Goal {
private:
	float m_pos[3];		//座標
	float m_time;	//アニメーション経過時間

public:
	Goal();
	~Goal();

	//更新処理
	void Update();

	//描画処理
	void Draw();


	//座標の設定
	void SetPos(float x, float y, float z);

};