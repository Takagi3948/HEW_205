#include "Fruit.h"
#include "Geometory.h"
#include <DirectXMath.h>
#include"Defines.h"
#include "Collision.h"

CFruit::CFruit()
	:m_pos(0.0f, 3.0f, 0.0f)
	, radius(0.5f)
	, m_pModel(nullptr)
	, m_pCamera(nullptr)
	, m_pStand(nullptr)
	, m_pGround(nullptr)
{
	m_pModel = new Model();
	m_pCamera = new CameraDebug();
	m_pStand = new CStand();
	m_pGround = new Ground();

	if (!m_pModel->Load("Assets/Model/apple/apple.obj", 0.2f))
	{
		MessageBox(NULL, "���f���ǂݍ��ݎ��s", "Load Error", MB_OK);
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
	//�d��
	const float GRAVITY = 0.05f;

	m_pos.y -= GRAVITY;

	//�e�l�̎擾
	Collision::Sphere FruitSphere = Collision::Sphere{ {m_pos.x,(m_pos.y+0.5f),m_pos.z} , radius/10};// �ʏ�l���Ɠ�����O��Fly���₪�����̂ŕύX�B(�������)
	Collision::Box GroundBox = m_pGround->GetBox();
	//Collision::Box StandBox = m_pStand->GetBox();

	//Collision::Result ResultStand = Collision::Result{ Collision::Hit(FruitSphere,StandBox) };
	////��Ɠ���������A���ō��W��3.0f�ɔ�΂��B
	//if (ResultStand.isHit)
	//{
	//	m_pos.y = 3.0f;
	//}

	Collision::Result RsultGround = Collision::Result{Collision::Hit(FruitSphere,GroundBox)};
	//�O���E���h�Ɠ��������牼��y���W��-9.0(��ʊO)�ɔ�΂��B
	if (RsultGround.isHit)
	{
		m_pos.y = -9.0f;
	}
}

void CFruit::Draw(Camera* camera)
{
	m_pCamera = camera;

	//�ϊ��s��̌v�Z
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(0);
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(0);
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(0);
	DirectX::XMMATRIX R = Rx * Ry * Rz;
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(3.0f, 3.0f, 3.0f);
	DirectX::XMMATRIX mat = S * R * T;

	//�ϊ��s���GPU�ɓn�����߂̑O����
	mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fMat;
	//mat��fMat�Ɋi�[
	DirectX::XMStoreFloat4x4(&fMat, mat);

	//���f��
	if (m_pModel)
	{

		//�V�F�[�_�[�ɕK�v�ȃf�[�^��ݒ�
		ShaderList::SetWorld(fMat);
		ShaderList::SetView(m_pCamera->GetViewMatrix());
		ShaderList::SetProjection(m_pCamera->GetProjectionMatrix());

		// ���f���Ɏg�p����e��V�F�[�_�[��ݒ�
		m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
		m_pModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));

		// ���b�V���ʂɃ��f����\��
		for (int i = 0; i < m_pModel->GetMeshNum(); ++i) {

			// ���f���̃��b�V����擾
			const Model::Mesh* mesh = m_pModel->GetMesh(i);

			// ���b�V���Ɋ��蓖�Ă��Ă���}�e���A����擾
			Model::Material material = *m_pModel->GetMaterial(mesh->materialID);


			// �V�F�[�_�[�փ}�e���A����ݒ�
			ShaderList::SetMaterial(material);

			// ���f���̕`��(����I�Ɉꊇ�ōs���Ă����`�揈����A���b�V�����Ƃ̕`�揈���ɕύX)
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

