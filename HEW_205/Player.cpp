#include "Player.h"
#include "Geometory.h"
#include <DirectXMath.h>
#include"Defines.h"
#include "Input.h"

CPlayer::CPlayer()
	:m_pLeftPlayer(new CLeftPlayer())
	,m_pRightPlayer(new CRightPlayer())
{
	m_pLeftPlayer->SetPos(-1.0f, 0.5f, 0.0f);
	//m_pLeftPlater->SetPos(-1.0f, 0.5f, 0.0f);

	m_pRightPlayer->SetPos(1.0f, 0.5f, 0.0f);
	//m_pRightPlater->SetPos(1.0f, 0.5f, 0.0f);
}


CPlayer::~CPlayer()
{
	if (m_pLeftPlayer)
	{
		delete m_pLeftPlayer;
		m_pLeftPlayer = nullptr;
	}

	if (m_pRightPlayer)
	{
		delete m_pRightPlayer;
		m_pRightPlayer = nullptr;
	}
}

void CPlayer::Update()
{
	m_pLeftPlayer->Update();
	m_pRightPlayer->Update();
}

void CPlayer::Draw()
{
	m_pLeftPlayer->Draw();
	m_pRightPlayer->Draw();
}



