
#include <DirectXMath.h>
#include "Shader.h"
#include "MeshBuffer.h"
#include "LeftPlayer.h"
#include "RightPlayer.h"

class CPlayer {
private:
	CLeftPlayer* m_pLeftPlayer;
	CRightPlayer* m_pRightPlayer;

public:
	CPlayer();
	~CPlayer();

	//XVˆ—
	void Update();

	//•`‰æˆ—
	void Draw();

};