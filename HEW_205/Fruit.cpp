#include "Fruit.h"
#include "Geometory.h"
#include <DirectXMath.h>
#include"Defines.h"
#include "Collision.h"

namespace
{
	const float Fruit_GRAVITY = 0.002f; // 毎フレーム速度に足す量
	const float BOUNCE_SPEED  = 0.1f;  // 跳ね返り直後の速さ（1フレームあたり）
	const float HIT_OFFSET_Y  = 0.0f;   // m_pos から判定球の中心までの高さ（既存の +0.5f）
	const float HIT_RADIUS    = 0.05f;  // 判定球の半径（既存の radius/10）
	const float PUSH_OUT      = 0.001f; // 跳ね返し後に台から浮かせる量
	const float OUT_Y         = -9.0f;  // 画面外に飛ばす仮の高さ（既存）
	const float TANGENT_KEEP  = 0.5f;  // 面に沿った方向の勢いを残す割合（0=前と同じ、1=鏡面反射）
	const float SURFACE_GAIN  = 0.1f;  // 面（台）の動きを乗せる割合（0=無視）
	const float IMPACT_GAIN   = 0.05f;  // 落下の勢いを跳ね返りに足す割合（0=固定速度）
}

CFruit::CFruit()
	:m_pos(0.0f, 3.0f, 0.0f)
	,m_prevPos(0.0f,0.0f,0.0f)
	,m_vel(0.0f,0.0f,0.0f)
	, radius(0.5f)
	, m_pModel(nullptr)
	, m_pCamera(nullptr)
	, m_pStand(nullptr)
	, m_pGround(nullptr)
{
	m_pModel = new Model();

	if (!m_pModel->Load("Assets/Model/apple/apple.obj", 0.2f))
	{
		MessageBox(NULL, "モデル読み込み失敗", "Load Error", MB_OK);
	}

}

CFruit::~CFruit()
{
	m_pCamera = nullptr;
	m_pGround = nullptr;
	m_pStand = nullptr;
	if (m_pModel)
	{
		delete m_pModel;
		m_pModel = nullptr;
	}
}

void CFruit::Update()
{
	if (m_pos.y <= OUT_Y) return;//画面外に落ちた物は何もしない

	//動かす前に判定球の中心
	DirectX::XMFLOAT3 prev = { m_pos.x, m_pos.y + HIT_OFFSET_Y, m_pos.z };

	//重力→速度→位置
	m_vel.y -= Fruit_GRAVITY;
	m_pos.x += m_vel.x;
	m_pos.y += m_vel.y;
	
	DirectX::XMFLOAT3 cur = { m_pos.x, m_pos.y + HIT_OFFSET_Y, m_pos.z };

	//台との判定
	if (m_pStand)
	{
		const DirectX::XMFLOAT3 pp = m_pStand->GetPrevPos();
		const DirectX::XMFLOAT3 cp = m_pStand->GetPos();
		const DirectX::XMMATRIX prevW = DirectX::XMMatrixRotationZ(m_pStand->GetPrevAngle()) * DirectX::XMMatrixTranslation(pp.x, pp.y, pp.z);
		const DirectX::XMMATRIX curW  = DirectX::XMMatrixRotationZ(m_pStand->GetAngle()) * DirectX::XMMatrixTranslation(cp.x, cp.y, cp.z);
		//前の位置は前フレームの台から、今の位置は今フレームの台から見た座標にする
		DirectX::XMFLOAT3 localPrev, localCur;
		DirectX::XMStoreFloat3(&localPrev, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&prev), DirectX::XMMatrixInverse(nullptr, prevW)));
		DirectX::XMStoreFloat3(&localCur, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&cur), DirectX::XMMatrixInverse(nullptr, curW)));

		const DirectX::XMFLOAT3 size = m_pStand->GetSize();
		const float h = size.y * 0.5f + HIT_RADIUS;
		Collision::Plane plane = { { 0.0f, h, 0.0f }, { 0.0f, 1.0f, 0.0f } };
		Collision::Line  line = { localPrev, localCur };

		//表側から裏側へ横切ったか、横切った位置が台の長さの範囲内か
		Collision::Result r = Collision::Hit(plane, line);
		if (r.isHit && fabsf(r.point.x) <= size.x * 0.5f)
		{
			//ローカルの交差点を今フレームと前フレームのワールドに戻す
			const DirectX::XMVECTOR vHit = DirectX::XMLoadFloat3(&r.point);
			DirectX::XMFLOAT3 hitNow, hitBefore, n;
			DirectX::XMStoreFloat3(&hitNow, DirectX::XMVector3TransformCoord(vHit, curW));
			DirectX::XMStoreFloat3(&hitBefore, DirectX::XMVector3TransformCoord(vHit, prevW));
			DirectX::XMStoreFloat3(&n, DirectX::XMVector3TransformNormal(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), curW)); //台のY軸

			//台のこの点の１フレームの移動量
			const DirectX::XMFLOAT3 surfaceVel = { hitNow.x - hitBefore.x, hitNow.y - hitBefore.y, 0.0f };

			//速度を先に計算してから、面の上に置き直す
			m_vel = CalcBounceVelocity(n, surfaceVel);
			m_pos.x = hitNow.x + n.x * PUSH_OUT;
			m_pos.y = hitNow.y + n.y * PUSH_OUT - HIT_OFFSET_Y;


		}
	
	}
	
	
	
	//重力
	//const float GRAVITY = 0.05f;
	//m_pos.y -= GRAVITY;

	//各値の取得
	Collision::Sphere FruitSphere = Collision::Sphere{ {m_pos.x,(m_pos.y + 0.5f),m_pos.z} , radius / 10 };// 通常値だと当たる前にFlyしやがったので変更。(無理やり)
	Collision::Box GroundBox = m_pGround->GetBox();
	//Collision::Box StandBox = m_pStand->GetBox();

	//Collision::Result ResultStand = Collision::Result{ Collision::Hit(FruitSphere,StandBox) };
	////台と当たったら、仮で座標を3.0fに飛ばす。
	//if (ResultStand.isHit)
	//{
	//	m_pos.y = 3.0f;
	//}

	Collision::Result RsultGround = Collision::Result{ Collision::Hit(FruitSphere,GroundBox) };
	//グラウンドと当たったら仮でy座標を-9.0(画面外)に飛ばす。
	if (RsultGround.isHit)
	{
		m_pos.y = -9.0f;
	}
}

void CFruit::Draw()
{

	//変換行列の計算
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(0);
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(0);
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(0);
	DirectX::XMMATRIX R = Rx * Ry * Rz;
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(3.0f, 3.0f, 3.0f);
	DirectX::XMMATRIX mat = S * R * T;

	//変換行列をGPUに渡すための前処理
	mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fMat;
	//matをfMatに格納
	DirectX::XMStoreFloat4x4(&fMat, mat);


	//モデル

		//シェーダーに必要なデータを設定
	ShaderList::SetWorld(fMat);

	// モデルに使用する各種シェーダーを設定
	m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
	m_pModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));

	// メッシュ別にモデルを表示
	for (int i = 0; i < m_pModel->GetMeshNum(); ++i) {

		// モデルのメッシュを取得
		const Model::Mesh* mesh = m_pModel->GetMesh(i);

		// メッシュに割り当てられているマテリアルを取得
		Model::Material material = *m_pModel->GetMaterial(mesh->materialID);


		// シェーダーへマテリアルを設定
		ShaderList::SetMaterial(material);

		// モデルの描画(内部的に一括で行っていた描画処理を、メッシュごとの描画処理に変更)
		m_pModel->Draw(i);
	}





}

void CFruit::SetPos(float x, float y, float z)
{
	m_pos.x = x;
	m_pos.y = y;
	m_pos.z = z;
}

DirectX::XMFLOAT3 CFruit::CalcBounceVelocity(const DirectX::XMFLOAT3& normal, const DirectX::XMFLOAT3& surfaceVel) const
{
	const DirectX::XMVECTOR n    = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&normal));
	const DirectX::XMVECTOR vSur = DirectX::XMLoadFloat3(&surfaceVel);
	const DirectX::XMVECTOR vNow = DirectX::XMLoadFloat3(&m_vel);

	//落下の勢い（IMPACT_GAIN 用）：面に近づく速さ。面の動きも含めた相対速度で測る
	const float closing = - DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorSubtract(vNow, vSur), n));

	//面に沿った成分：りんごの速度だけから取る（台の動きは混ぜない）
	const float vn = DirectX::XMVectorGetX(DirectX::XMVector3Dot(vNow, n));
	const DirectX::XMVECTOR vt = DirectX::XMVectorSubtract(vNow, DirectX::XMVectorScale(n, vn));

	//垂直：固定速度＋落下の勢い／面に沿う：勢いを残す／台の動き：SURFACE_GAIN の割合だけ乗せる
	const float up = BOUNCE_SPEED + IMPACT_GAIN * fmaxf(closing, 0.0f);
	DirectX::XMVECTOR v = DirectX::XMVectorScale(n, up);
	v = DirectX::XMVectorAdd(v, DirectX::XMVectorScale(vt, TANGENT_KEEP));
	v = DirectX::XMVectorAdd(v, DirectX::XMVectorScale(vSur, SURFACE_GAIN));

	DirectX::XMFLOAT3 out;
	DirectX::XMStoreFloat3(&out, v);
	return out;
}


