#include "SceneGame.h"
#include "Geometory.h"
#include "MainCamera.h"
#include "ShaderList.h"


SceneGame::SceneGame()
	:m_pGround(nullptr)
	,m_pGoal(nullptr)
	,m_pCamera(nullptr)
	,m_pModel(nullptr)
	,m_pPlayerManager(nullptr)
	,m_pStand(nullptr)
	,m_pFruit(nullptr)
	,m_pEnemy{nullptr, nullptr, nullptr}
{
	m_pGround = new Ground;
	m_pGround->SetPos(0.0f, -0.25f, 0.0f);
	m_pGround->SetScale(20.0f, 0.5f, 3.0f);

	m_pGoal = new Goal;
	m_pGoal->SetPos(0.0f, 1.0f, 0.0f);

	//m_pCamera = new CameraDebug(); //継承されたDebugのクラスを入れる

	m_pCamera = new MainCamera();

	m_pModel = new Model();

	m_pPlayerManager = new CPlayerManager();

	m_pStand = new CStand();

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		m_pEnemy[i] = new CEnemy;
	}
	m_pEnemy[0]->SetPos(5.0f, 3.0f, 0.0f);
	m_pEnemy[1]->SetPos(0.0f, 5.0f, 0.0f);
	m_pEnemy[2]->SetPos(-7.0f, 4.0f, 0.0f);

	m_pFruit = new CFruit();
	m_pFruit->SetCamera(m_pCamera);
	m_pFruit->SetGround(m_pGround);
	m_pFruit->SetStand(m_pStand);


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
	if (m_pPlayerManager)
	{
		delete m_pPlayerManager;
		m_pPlayerManager = nullptr;
	}
	if (m_pStand)
	{
		delete m_pStand;
		m_pStand = nullptr;
	}
	if (m_pFruit)
	{
		delete m_pFruit;
		m_pFruit = nullptr;
	}
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (m_pEnemy[i])
		{
			delete m_pEnemy[i];
			m_pEnemy[i] = nullptr;
		}
	}
}

void SceneGame::Update()
{
	m_pGround->Update();
	m_pGoal->Update();
	
	m_pPlayerManager->Update();
	m_pPlayerManager->Reset();


	//PlayerとGroundの当たり判定
	Collision::Result Rightresult = Collision::Hit(m_pPlayerManager->RightGetCollision(), m_pGround->GetCollision());
	//判定
	if (Rightresult.isHit)
	{
		//  点の情報
		Collision::Point point;
		point.pos = Rightresult.point; //  平面と線分の交点を設定
		//---  斜面の情報（四角形ではなく、三角形に分割）
		Collision::Triangle* triangle = m_pGround->GetTriangle();
		

		for (int i = 0; i < 2; ++i) {
			Rightresult = Collision::Hit(point, triangle[i]);
			if (Rightresult.isHit) {
				m_pPlayerManager->RightOnCollision(Rightresult);
				break;
			}
		}
	}
	
	Collision::Result Leftresult = Collision::Hit(m_pPlayerManager->LeftGetCollision(), m_pGround->GetCollision());
	//判定
	if (Leftresult.isHit)
	{
		//  点の情報
		Collision::Point point;
		point.pos = Leftresult.point; //  平面と線分の交点を設定
		//---  斜面の情報（四角形ではなく、三角形に分割）
		Collision::Triangle* triangle = m_pGround->GetTriangle();


		//地面に当たってるか判定を取る
		for (int i = 0; i < 2; ++i) {
			Leftresult = Collision::Hit(point, triangle[i]);
			if (Leftresult.isHit) {
				m_pPlayerManager->LeftOnCollision(Leftresult);
				break;
			}
			
		}
	}
	

	m_pCamera->SetPos(m_pPlayerManager->GetPos());
	m_pCamera->Update();

	m_pStand->SetPos(m_pPlayerManager->GetPos());
	m_pStand->Update();

	m_pFruit->Update();

	m_pEnemy[0]->Update();
	m_pEnemy[1]->Update();
	m_pEnemy[2]->Update();
}

void SceneGame::Draw()
{
	//深度テストの有効化
	SetDepthTest(DEPTH_ENABLE);


	Geometory::SetView(m_pCamera->GetViewMatrix());
	Geometory::SetProjection(m_pCamera->GetProjectionMatrix());
	ShaderList::SetView(m_pCamera->GetViewMatrix());
	ShaderList::SetProjection(m_pCamera->GetProjectionMatrix());


	//Geometory::DrawBox(); 
	m_pGround->Draw();
	//m_pGoal->Draw();
	m_pPlayerManager->Draw();
	m_pStand->Draw();

	m_pFruit->Draw();

	m_pEnemy[0]->Draw();
	m_pEnemy[1]->Draw();
	m_pEnemy[2]->Draw();
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
