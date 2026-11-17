
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
	CStand* m_pStand;
	Ground* m_pGround;


public:


	CFruit();
	~CFruit();

	//�X�V����
	void Update();

	//�`�揈��
	//�J�����̎擾
	void Draw(Camera*);


	//���W�̐ݒ�
	void SetPos(float x, float y, float z);


};