#pragma once
#include <DirectXMath.h>
#include "Shader.h"
#include "MeshBuffer.h"
#include "Collision.h"

class CStand {
private:
	DirectX::XMFLOAT3 m_pos;		//座標
	float angle;
	DirectX::XMFLOAT2 LeftPos;
	DirectX::XMFLOAT2 RightPos;
	DirectX::XMFLOAT3 m_size; //新規追加 サイズ。
	DirectX::XMFLOAT3 m_move;//今フレームの台の移動量
	bool m_isFirst;
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

	//台の上面の平面（offset 分だけ外側にずらす。normal は台のY軸）
	Collision::Plane GetTopPlane(float offset)const;

	//点が台の長さの範囲内か
	bool IsWithinLength(const DirectX::XMFLOAT3& p) const;

	//今フレームの台の移動量
	DirectX::XMFLOAT3 GetMove() const;

	//座標の取得(Size,Pos)
	Collision::Box GetBox();
};