#include <DirectXMath.h>
#include "Shader.h"
#include "MeshBuffer.h"
#include "Collision.h"


class CEnemy {
private:
	float m_pos[3];		//座標
	float m_scale[3];	//拡大率
	int type;			//タイプ名
	Collision::Info m_collision;		//当たり判定の情報

public:
	CEnemy(float posx, float posy, float posz, float sizex, float sizey, float sizez,int type);
	~CEnemy();

	//更新処理
	void Update();

	//描画処理
	void Draw();


	//座標の設定
	void SetPos(float x, float y, float z);

	//拡大率の設定
	void SetScale(float x, float y, float z);


	Collision::Info GetCollision() {
		return m_collision;
	}
};