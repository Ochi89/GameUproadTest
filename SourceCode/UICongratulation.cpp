
//=============================================================================
//	@file	UICongratulation.h
//	@brief	ゲームオーバーUI
//	@autor	Takuya Ochi
//	@date	2018/12/30
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "UICongratulation.h"
#include "PlayerManager.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const float		UICongratulation::MAAX_CLEAR_WAIT_TIME = 300.0f;	//	クリア時の遅延時間の最大
const int		UICongratulation::MIN_ALPHA = 0;					//	アルファ値の最小
const int		UICongratulation::MAX_ALPHA = 255;					//	アルファ値の最大
const int		UICongratulation::ALPHA_SPEED = 2;					//	アルファ値の速度

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
UICongratulation::UICongratulation()
{
	//	処理なし
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
UICongratulation::~UICongratulation()
{
	//	最終的な解放処理
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成処理
//-----------------------------------------------------------------------------
void UICongratulation::Create()
{
	//	クリアUI
	m_congratulationUI.m_spriteHendle = LoadGraph("Data/Sprite/Congratulation/Congratulation.png");
	CommonDebug::Assert((m_congratulationUI.m_spriteHendle <= -1), " [ UICongratulation.cpp ] : error : sprite loading failed.");
	m_congratulationUI.m_pos = VGet(350.0f, 200.0f, 0.0f);
	m_alpha = MIN_ALPHA;
	m_isClear = false;
	m_clearWaitTime = 0.0f;
}

//-----------------------------------------------------------------------------
//	@brief	解放処理
//-----------------------------------------------------------------------------
void UICongratulation::Release()
{
	//	スプライトのアンロード
	DeleteGraph(m_congratulationUI.m_spriteHendle);
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void UICongratulation::Update(PlayerManager & _playerManager)
{
	//	クリアした
	const bool isClear = _playerManager.GetIsClear();
	if (isClear) 
	{
		m_clearWaitTime++;
		const bool isActive = m_clearWaitTime >= MAAX_CLEAR_WAIT_TIME;
		if (isActive) { m_isClear = true; }
 	}

	//	アルファ値の加算
	if (m_isClear) { m_alpha += ALPHA_SPEED; }

	//	最大処理
	const bool isMaxVal = m_alpha >= MAX_ALPHA;
	if (isMaxVal) { m_alpha = MAX_ALPHA; }
}

//-----------------------------------------------------------------------------
//	@brief	描画処理
//-----------------------------------------------------------------------------
void UICongratulation::Draw()
{
	if (m_isClear)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);
		DrawGraph((int)m_congratulationUI.m_pos.x, (int)m_congratulationUI.m_pos.y, m_congratulationUI.m_spriteHendle, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放処理
//-----------------------------------------------------------------------------
void UICongratulation::_FinalRelease()
{
	//	スプライトのアンロード
	DeleteGraph(m_congratulationUI.m_spriteHendle);
}
