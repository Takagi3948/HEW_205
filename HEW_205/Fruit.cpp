#include "Fruit.h"
#include "Geometory.h"
#include <DirectXMath.h>
#include"Defines.h"
#include "Collision.h"

namespace
{
	const float Fruit_GRAVITY      = 0.002f; // 毎フレーム速度に足す量
	const float BOUNCE_SPEED = 0.15f;  // 跳ね返り直後の速さ（1フレームあたり）
	const float HIT_OFFSET_Y = 0.0f;   // m_pos から判定球の中心までの高さ（既存の +0.5f）
	const float HIT_RADIUS   = 0.05f;  // 判定球の半径（既存の radius/10）
	const float PUSH_OUT     = 0.001f; // 跳ね返し後に台から浮かせる量
	const float OUT_Y        = -9.0f;  // 画面外に飛ばす仮の高さ（既存）
	const float RESTITUTION = 1.0f; // 反発係数（1.0 = エネルギー損失なし）
	const float BASE_BOUNCE_SPEED = 0.1f;  //基準となる強さ
	const float CENTER_BOUNCE = 1.5f; // 中央の強さ
	const float EDGE_BOUNCE = 0.5f; // 端の強さ

}

CFruit::CFruit()
	:m_pos(0.0f, 3.0f, 0.0f)
	, m_vel(0.0f, 0.0f, 0.0f)
	, radius(0.5f)
	, m_pModel(nullptr)
	, m_pCamera(nullptr)
	, m_pStand(nullptr)
	, m_pGround(nullptr)
	, m_EnemyCollision()
	, m_GroundCollision()
{
	m_pModel = new Model();

	if (!m_pModel->Load("Assets/Model/apple/apple.obj", 0.2f))
	{
		MessageBox(NULL, "モデル読み込み失敗", "Load Error", MB_OK);
	}

	m_EnemyCollision.type = Collision::eSphere;
	m_GroundCollision.type = Collision::eLine;

}

CFruit::~CFruit()
{
	m_pCamera = nullptr;
	m_pGround = nullptr;

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
		/*
		if (m_pStand->SweptHit(prev, cur, HIT_RADIUS, hitPos, n))
		{
			//平面の上に置きなおして、入射角に関係なく台のY軸方向へ固定速度で飛ばす
			m_pos.x = hitPos.x + n.x * PUSH_OUT;
			m_pos.y = hitPos.y + n.y * PUSH_OUT - HIT_OFFSET_Y;
			m_vel.x = n.x * BOUNCE_SPEED;
			m_vel.y = n.y * BOUNCE_SPEED;
		}
		*/
		// 台との判定
		if (m_pStand)
		{
			DirectX::XMFLOAT3 hitPos, n;

			if (m_pStand->SweptHit(prev, cur, HIT_RADIUS, hitPos, n))
			{
				// 衝突直前の速度
				DirectX::XMFLOAT3 inVel = m_vel;

				// 入射角を考慮した反射
				float dot = inVel.x * n.x + inVel.y * n.y;

				m_vel.x = inVel.x - 2.0f * dot * n.x;
				m_vel.y = inVel.y - 2.0f * dot * n.y;

				float reflectX = inVel.x - 2.0f * dot * n.x;
				float reflectY = inVel.y - 2.0f * dot * n.y;

				// 反射方向の長さ
				float length = sqrtf(reflectX * reflectX + reflectY * reflectY);


				// ---------------------------------
				// 当たった位置による反発力の調整
				// ---------------------------------

				// Standの中心位置と半分の幅を取得
				float centerX = m_pStand->GetPos().x;
				float halfWidth = m_pStand->GetSize().x * 0.5f;

				// 中央からの距離を0～1に正規化
				float t = fabsf(hitPos.x - centerX) / halfWidth;

				// 範囲外にならないように制限
				if (t > 1.0f)
				{
					t = 1.0f;
				}

				// 中央1.5倍、端0.3倍
				float bouncePower = CENTER_BOUNCE
					+ (EDGE_BOUNCE - CENTER_BOUNCE) * t;

				// ずっと加算されないように
				if (length > 0.0001f)
				{
					m_vel.x = (reflectX / length) * BASE_BOUNCE_SPEED * bouncePower;
					m_vel.y = (reflectY / length) * BASE_BOUNCE_SPEED * bouncePower;
				}

				// 台へのめり込み防止
				m_pos.x = hitPos.x + n.x * PUSH_OUT;
				m_pos.y = hitPos.y + n.y * PUSH_OUT - HIT_OFFSET_Y;
			}
		}
	}
	
	//当たり判定の計算
	m_EnemyCollision.sphere.center = m_pos;
	m_EnemyCollision.sphere.radius = radius;

	m_GroundCollision.line.start = {
		m_pos.x,
		m_pos.y,
		m_pos.z
	};

	m_GroundCollision.line.end = {
		m_pos.x,
		m_pos.y - 0.25f,
		m_pos.z
	};


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

void CFruit::OnEnemyCollision(Collision::Result collision,Collision::Info enemyBox)
{
	//入射角も含めた反射
	// EnemyのBOXの範囲を取得
	float minX = enemyBox.box.center.x - enemyBox.box.size.x * 0.5f;
	float maxX = enemyBox.box.center.x + enemyBox.box.size.x * 0.5f;
	float minY = enemyBox.box.center.y - enemyBox.box.size.y * 0.5f;
	float maxY = enemyBox.box.center.y + enemyBox.box.size.y * 0.5f;

	// Fruitの中心に最も近いBOX上の点を求める
	float closestX = m_pos.x;
	float closestY = m_pos.y;

	if (closestX < minX) closestX = minX;
	if (closestX > maxX) closestX = maxX;

	if (closestY < minY) closestY = minY;
	if (closestY > maxY) closestY = maxY;

	// 法線を計算
	float nx = m_pos.x - closestX;
	float ny = m_pos.y - closestY;

	float length = sqrtf(nx * nx + ny * ny);

	// BOX内部に中心が入っている場合
	if (length < 0.0001f)
	{
		float left = m_pos.x - minX;
		float right = maxX - m_pos.x;
		float bottom = m_pos.y - minY;
		float top = maxY - m_pos.y;

		float minDist = left;
		nx = -1.0f;
		ny = 0.0f;

		if (right < minDist)
		{
			minDist = right;
			nx = 1.0f;
			ny = 0.0f;
		}

		if (bottom < minDist)
		{
			minDist = bottom;
			nx = 0.0f;
			ny = -1.0f;
		}

		if (top < minDist)
		{
			nx = 0.0f;
			ny = 1.0f;
		}
	}
	else
	{
		nx /= length;
		ny /= length;
	}

	// 衝突直前の速度
	DirectX::XMFLOAT3 inVel = m_vel;

	// 入射角を考慮した反射
	float dot = inVel.x * nx + inVel.y * ny;

	// Enemyから離れる方向なら反射しない
	if (dot >= 0.0f) return;

	m_vel.x = inVel.x - 2.0f * dot * nx;
	m_vel.y = inVel.y - 2.0f * dot * ny;

	// めり込み防止
	m_pos.x = closestX + nx * (radius + PUSH_OUT);
	m_pos.y = closestY + ny * (radius + PUSH_OUT);

}

void CFruit::OnGroundCollision(Collision::Result collision)
{
	m_pos.y = -9.0f;
}

void CFruit::SetPos(float x, float y, float z)
{
	m_pos.x = x;
	m_pos.y = y;
	m_pos.z = z;
}


