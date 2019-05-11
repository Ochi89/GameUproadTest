﻿
//=============================================================================
//	@file	UILife.cpp
//	@brief	UILife
//	@autor	Takuya Ochi
//	@date	2018/12/15
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "UILife.h"
#include "PlayerManager.h"
#include "PlayerBase.h"
#include "Common.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const float UILife::LEFT_UI_INTEVAL = 90.0f;				//	ライフUIの間隔

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
UILife::UILife()
{
	//	処理なし
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
UILife::~UILife()
{
	//	最終的な解放
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成
//-----------------------------------------------------------------------------
void UILife::Create()
{
	//	ライフUI
	m_lifeUI.m_spriteHendle = LoadGraph("Data/Sprite/Life/Life.png");
	CommonDebug::Assert((m_lifeUI.m_spriteHendle <= -1), " [ UILife.cpp ] : error : sprite loading failed.");
	m_lifeUI.m_pos = VGet(50.0f, 50.0f, 0.0f);
	m_lifeUI.m_scale = 0.2f;
}

//-----------------------------------------------------------------------------
//	@brief	解放
//-----------------------------------------------------------------------------
void UILife::Release()
{
	//	ライフUIのスプライトのアンロード
	DeleteGraph(m_lifeUI.m_spriteHendle);
}

//-----------------------------------------------------------------------------
//	@brief	描画
//-----------------------------------------------------------------------------
void UILife::Draw(PlayerManager& _playerManager)
{
	//	ライフUIの描画
	PlayerBase* player = _playerManager.GetPlayerPtr();
	for (int i = 0; i < player->GetLife(); i++)
	{
		DrawRotaGraph((int)(m_lifeUI.m_pos.x + (LEFT_UI_INTEVAL * i)), (int)(m_lifeUI.m_pos.y), (double)m_lifeUI.m_scale, 0.0, m_lifeUI.m_spriteHendle, TRUE);
	}
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放
//-----------------------------------------------------------------------------
void UILife::_FinalRelease()
{
	//	ライフUIのスプライトのアンロード
	DeleteGraph(m_lifeUI.m_spriteHendle);
}