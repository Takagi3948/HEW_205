
#include <DirectXMath.h>
#include "Shader.h"
#include "MeshBuffer.h"
#include "LeftPlayer.h"
#include "RightPlayer.h"

class CPlayer {
private:
	CLeftPlayer* m_pLeftPlayer;
	CRightPlayer* m_pRightPlayer;
	DirectX::XMFLOAT4 Position;

public:
	CPlayer();
	~CPlayer();

	//XVˆ—
	void Update();

	//•`‰æˆ—
	void Draw();

	DirectX::XMFLOAT4 GetPos();

};