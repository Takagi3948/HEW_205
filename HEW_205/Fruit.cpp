#include "Fruit.h"
#include "Geometory.h"
#include <DirectXMath.h>
#include"Defines.h"

#include "Collision.h"//判定用追加

CFruit::CFruit()
	:m_pos(0.0f, 3.0f, 0.0f)
	, radius(0.5f)
	, m_pModel(nullptr)
	, m_pCamera(nullptr)
	, m_pStand(nullptr)
{
	m_pModel = new Model();
	m_pCamera = new CameraDebug();
	m_pStand = new CStand();

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

	if (m_pStand)
	{
		delete m_pStand;
		m_pStand = nullptr;
	}
}

void CFruit::Update()
{
	//重力
	const float GRAVITY = 0.02f;

	m_pos.y -= GRAVITY;
	DirectX::XMFLOAT3 StandPos = m_pStand->GetPos();
	//ここから collisionチェック用
	if (CheckCollisionRtoR(m_pos.x,m_pos.y,radius,radius,StandPos.x,StandPos.y,1.0f,1.0f))
	{
		m_pos.y = 3.0f;
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
