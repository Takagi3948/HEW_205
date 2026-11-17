#include "Fruit.h"
#include "Geometory.h"
#include <DirectXMath.h>
#include"Defines.h"
#include "Collision.h"

namespace
{
	const float Fruit_GRAVITY      = 0.004f; // 毎フレーム速度に足す量
	const float BOUNCE_SPEED = 0.15f;  // 跳ね返り直後の速さ（1フレームあたり）
	const float HIT_OFFSET_Y = 0.5f;   // m_pos から判定球の中心までの高さ（既存の +0.5f）
	const float HIT_RADIUS   = 0.05f;  // 判定球の半径（既存の radius/10）
	const float PUSH_OUT     = 0.001f; // 跳ね返し後に台から浮かせる量
	const float OUT_Y        = -9.0f;  // 画面外に飛ばす仮の高さ（既存）
}

CFruit::CFruit()
	:m_pos(0.0f, 3.0f, 0.0f)
	,m_vel(0.0f,0.0f,0.0f)
	, radius(0.5f)
	, m_pModel(nullptr)
	, m_pCamera(nullptr)
	, m_pStand(nullptr)
	, m_pGround(nullptr)
{
	m_pModel = new Model();
	m_pCamera = new CameraDebug();
	//m_pStand = new CStand();
	m_pGround = new Ground();

	if (!m_pModel->Load("Assets/Model/apple/apple.obj", 0.2f))
	{
		MessageBox(NULL, "モデル読み込み失敗", "Load Error", MB_OK);
	}

}

CFruit::~CFruit()
{
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
		DirectX::XMFLOAT3 hitPos, n;
		if (m_pStand->SweptHit(prev, cur, HIT_RADIUS, hitPos, n))
		{
			//平面の上に置きなおして、入射角に関係なく台のY軸方向へ固定速度で飛ばす
			m_pos.x = hitPos.x + n.x * PUSH_OUT;
			m_pos.y = hitPos.y + n.y * PUSH_OUT - HIT_OFFSET_Y;
			m_vel.x = n.x * BOUNCE_SPEED;
			m_vel.y = n.y * BOUNCE_SPEED;
		}
	}
	
	
	//重力
	//const float GRAVITY = 0.05f;
	//m_pos.y -= GRAVITY;

	//各値の取得
	Collision::Sphere FruitSphere = Collision::Sphere{ {m_pos.x,(m_pos.y+0.5f),m_pos.z} , radius/10};// 通常値だと当たる前にFlyしやがったので変更。(無理やり)
	Collision::Box GroundBox = m_pGround->GetBox();
	//Collision::Box StandBox = m_pStand->GetBox();

	//Collision::Result ResultStand = Collision::Result{ Collision::Hit(FruitSphere,StandBox) };
	////台と当たったら、仮で座標を3.0fに飛ばす。
	//if (ResultStand.isHit)
	//{
	//	m_pos.y = 3.0f;
	//}

	Collision::Result RsultGround = Collision::Result{Collision::Hit(FruitSphere,GroundBox)};
	//グラウンドと当たったら仮でy座標を-9.0(画面外)に飛ばす。
	if (RsultGround.isHit)
	{
		m_pos.y = -9.0f;
	}
}

void CFruit::Draw(Camera* camera)
{
	m_pCamera = camera;

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
	if (m_pModel)
	{

		//シェーダーに必要なデータを設定
		ShaderList::SetWorld(fMat);
		ShaderList::SetView(m_pCamera->GetViewMatrix());
		ShaderList::SetProjection(m_pCamera->GetProjectionMatrix());

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



}

void CFruit::SetPos(float x, float y, float z)
{
	m_pos.x = x;
	m_pos.y = y;
	m_pos.z = z;
}

void CFruit::SetStand(CStand* stand)
{
	m_pStand = stand;
}

