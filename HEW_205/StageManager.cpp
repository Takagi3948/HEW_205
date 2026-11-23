#include "StageManager.h"

CStageManager::CStageManager()
	:World(0)
	, Stage(0)
	, type(0)
	, Pos{ 0.0f,0.0f,0.0f }
	, Size{ 0.0f,0.0f,0.0f }
	, Angle(0.0f)
	, v_Ground()
	, v_Enemy()
{
}

CStageManager::~CStageManager()
{
}

void CStageManager::Update(int World, int Stage)
{
}

void CStageManager::Draw(int World, int Stage)
{
}

Collision::Info CStageManager::GetGroundCollision(int num)
{
	return Collision::Info();
}

Collision::Info CStageManager::GetEnemyCollision(int num)
{
	return Collision::Info();
}
