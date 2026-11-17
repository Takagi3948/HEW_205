
#include <DirectXMath.h>
#include "Shader.h"
#include "MeshBuffer.h"
#include "ShaderList.h"
#include "Model.h"
#include "Camera.h"
#include "CameraDebug.h"
#include "Ground.h"
#include "Stand.h"

class CFruit {
private:
	DirectX::XMFLOAT3 m_pos;		//���W
	float radius;	//���a
	Model* m_pModel;
	Camera* m_pCamera;
	Ground* m_pGround;
	CStand* m_pStand;

public:


	CFruit();
	~CFruit();

	//�X�V����
	void Update();

	//�`�揈��
	//�J�����̎擾
	void Draw();


	//���W�̐ݒ�
	void SetPos(float x, float y, float z);

	void SetCamera(Camera* pCamera)
	{
		m_pCamera = pCamera;
	}

	void SetStand(CStand* pStand)
	{
		m_pStand = pStand;
	}

	void SetGround(Ground* pGround)
	{
		m_pGround = pGround;
	}

};