#include "SceneGame.h"
#include "Geometory.h"
#include "CameraDebug.h"
#include "ShaderList.h"


SceneGame::SceneGame()
	:m_pGround(nullptr)
	,m_pGoal(nullptr)
	,m_pCamera(nullptr)
	,m_pModel(nullptr)
	,m_pPlayer(nullptr)
	,m_pStand(nullptr)
{
	m_pGround = new Ground;
	m_pGround->SetPos(0.0f, -0.25f, 0.0f);
	m_pGround->SetScale(20.0f, 0.5f, 3.0f);

	m_pGoal = new Goal;
	m_pGoal->SetPos(0.0f, 1.0f, 0.0f);

	m_pCamera = new CameraDebug(); //継承されたDebugのクラスを入れる

	m_pModel = new Model();

	m_pPlayer = new CPlayer();

	m_pStand = new CStand();

	//深度テストの有効化
	SetDepthTest(DEPTH_ENABLE);
}

SceneGame::~SceneGame()
{
	if (m_pGround)
	{
		delete m_pGround;
		m_pGround = nullptr;
	}
	if (m_pGoal)
	{
		delete m_pGoal;
		m_pGoal = nullptr;
	}
	if (m_pCamera)
	{
		delete m_pCamera;
		m_pCamera = nullptr;
	}
	if (m_pModel)
	{
		delete m_pModel;
		m_pModel = nullptr;
	}
	if (m_pPlayer)
	{
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}
	if (m_pStand)
	{
		delete m_pStand;
		m_pStand = nullptr;
	}

}

void SceneGame::Update()
{
	m_pGround->Update();
	m_pGoal->Update();
	m_pCamera->Update();
	m_pPlayer->Update();

	m_pStand->SetPos(m_pPlayer->GetPos());
	m_pStand->Update();
}

void SceneGame::Draw()
{
	Geometory::SetView(m_pCamera->GetViewMatrix());
	Geometory::SetProjection(m_pCamera->GetProjectionMatrix());

	//Geometory::DrawBox(); 
	m_pGround->Draw();
	//m_pGoal->Draw();
	m_pPlayer->Draw();
	m_pStand->Draw();

}





void SceneGame::ModelDraw(Model* model,float posX,float posY,float posZ, float sizeX,float sizeY,float sizeZ)
{
	if (model)
	{

		//モデルを表示するためのワールド行列を変換
		DirectX::XMFLOAT4X4 fWorld;

		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(posX, posY, posZ);
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(sizeX, sizeY, sizeZ);
		DirectX::XMMATRIX world = S * T;

		world = DirectX::XMMatrixTranspose(world);
		DirectX::XMStoreFloat4x4(&fWorld, world);


		//シェーダーに必要なデータを設定
		ShaderList::SetWorld(fWorld);
		ShaderList::SetView(m_pCamera->GetViewMatrix());
		ShaderList::SetProjection(m_pCamera->GetProjectionMatrix());

		// モデルに使用する各種シェーダーを設定 
		model->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
		model->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));

		// メッシュ別にモデルを表示 
		for (int i = 0; i < model->GetMeshNum(); ++i) {

			// モデルのメッシュを取得 
			const Model::Mesh* mesh = model->GetMesh(i);

			// メッシュに割り当てられているマテリアルを取得 
			Model::Material material = *model->GetMaterial(mesh->materialID);


			// シェーダーへマテリアルを設定 
			ShaderList::SetMaterial(material);

			// モデルの描画(内部的に一括で行っていた描画処理を、メッシュごとの描画処理に変更) 
			model->Draw(i);
		}
	}
	else
	{
		return;
	}
}
