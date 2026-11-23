#include "SceneGame.h"
#include "Geometory.h"
#include "MainCamera.h"
#include "ShaderList.h"


SceneGame::SceneGame()
	:m_pGoal(nullptr)
	,m_pCamera(nullptr)
	,m_pModel(nullptr)
	,m_pPlayerManager(nullptr)
	,m_pStand(nullptr)
	,m_pFruit(nullptr)
	,m_pStageManager(nullptr)
{
	m_pStageManager = new CStageManager(1, 1);

	m_pGoal = new Goal;
	m_pGoal->SetPos(0.0f, 1.0f, 0.0f);

	//m_pCamera = new CameraDebug(); //継承されたDebugのクラスを入れる

	m_pCamera = new MainCamera();

	m_pModel = new Model();

	m_pPlayerManager = new CPlayerManager();

	m_pStand = new CStand();



	m_pFruit = new CFruit();
	m_pFruit->SetCamera(m_pCamera);
	//m_pFruit->SetGround(m_pGround);
	m_pFruit->SetStand(m_pStand);


}

SceneGame::~SceneGame()
{
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
	if (m_pStageManager)
	{
		delete m_pStageManager;
		m_pStageManager = nullptr;
	}
}

void SceneGame::Update()
{
	m_pStageManager->Update();

	m_pGoal->Update();
	
	m_pPlayerManager->Update();
	m_pPlayerManager->Reset();

	//プレイヤーと地面の当たり判定
	//長すぎるので別の関数へ左遷
	R_IsHitGround();
	L_IsHitGround();

	m_pCamera->SetPos(m_pPlayerManager->GetPos());
	m_pCamera->Update();

	m_pStand->SetPos(m_pPlayerManager->GetPos());
	m_pStand->Update();

	m_pFruit->Update();

}

void SceneGame::Draw()
{
	//深度テストの有効化
	SetDepthTest(DEPTH_ENABLE);


	Geometory::SetView(m_pCamera->GetViewMatrix());
	Geometory::SetProjection(m_pCamera->GetProjectionMatrix());
	ShaderList::SetView(m_pCamera->GetViewMatrix());
	ShaderList::SetProjection(m_pCamera->GetProjectionMatrix());

	m_pStageManager->Draw();
	//Geometory::DrawBox(); 
	//m_pGoal->Draw();
	m_pPlayerManager->Draw();
	m_pStand->Draw();

	m_pFruit->Draw();

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


//右PlayerとGroundの当たり判定
void SceneGame::R_IsHitGround()
{
	int groundCount = m_pStageManager->GetGroundCount();

	for (int g = 0; g < groundCount; g++)
	{
		Collision::Info groundCollision = m_pStageManager->GetGroundCollision(g);

		Collision::Result Rightresult = Collision::Hit(m_pPlayerManager->RightGetCollision(), groundCollision);
		
		//判定
		if (Rightresult.isHit)
		{
			//  点の情報
			Collision::Point point;
			point.pos = Rightresult.point; //  平面と線分の交点を設定
			//---  斜面の情報（四角形ではなく、三角形に分割）
			Collision::Triangle* triangle = m_pStageManager->GetGroundTriangle(g);

			if (!triangle) continue;

			for (int i = 0; i < 2; ++i) {
				Rightresult = Collision::Hit(point, triangle[i]);
				if (Rightresult.isHit) {
					m_pPlayerManager->RightOnCollision(Rightresult);
					break;
				}
			}
		}
	}

	

}

//左PlayerとGroundの当たり判定
void SceneGame::L_IsHitGround()
{
	int groundCount = m_pStageManager->GetGroundCount();

	for (int g = 0; g < groundCount; g++)
	{
		Collision::Info groundCollision = m_pStageManager->GetGroundCollision(g);

		Collision::Result Leftresult = Collision::Hit(m_pPlayerManager->LeftGetCollision(), groundCollision);
		
		//判定
		if (Leftresult.isHit)
		{
			//  点の情報
			Collision::Point point;
			point.pos = Leftresult.point; //  平面と線分の交点を設定
			//---  斜面の情報（四角形ではなく、三角形に分割）
			Collision::Triangle* triangle = m_pStageManager->GetGroundTriangle(g);

			if (!triangle) continue;

			//地面に当たってるか判定を取る
			for (int i = 0; i < 2; ++i) {
				Leftresult = Collision::Hit(point, triangle[i]);
				if (Leftresult.isHit) {
					m_pPlayerManager->LeftOnCollision(Leftresult);
					break;
				}

			}
		}
	}
}


