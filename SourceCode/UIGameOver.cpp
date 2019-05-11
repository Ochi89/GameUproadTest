
//=============================================================================
//	@file	UIGameOver.cpp
//	@brief	UIGameOver
//	@autor	Takuya Ochi
//	@date	2018/12/15
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "UIGameOver.h"
#include "PlayerManager.h"
#include "PlayerBase.h"
#include "Common.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const int		UIGameOver::MIN_ALPHA = 128;			//	アルファ値の最小
const int		UIGameOver::MAX_ALPHA = 255;			//	アルファ値の最大
const int		UIGameOver::ALPHA_SPEED = 5;			//	アルファ値の速度

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
UIGameOver::UIGameOver()
{
	//	処理なし
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
UIGameOver::~UIGameOver()
{
	//	最終的な解放
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成
//-----------------------------------------------------------------------------
void UIGameOver::Create()
{
	//	ゲームオーバーUI
	m_gameOverUI.m_spriteHendle = LoadGraph("Data/Sprite/GameOver/GameOver.png");
	CommonDebug::Assert((m_gameOverUI.m_spriteHendle <= -1), " [ UIGameOver.cpp ] : error : sprite loading failed.");

	m_gameOverUI.m_pos = VGet(450.0f, 500.0f, 0.0f);
	m_alpha = MAX_ALPHA;
	m_isChangeAlpha = false;
	m_isGameOver = false;
}

//-----------------------------------------------------------------------------
//	@brief	解放
//-----------------------------------------------------------------------------
void UIGameOver::Release()
{
	//	ゲームオーバーUIのスプライトのアンロード
	DeleteGraph(m_gameOverUI.m_spriteHendle);
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void UIGameOver::Update(PlayerManager& _playerManager)
{
	//	ゲームオーバーUIの描画
	PlayerBase* player = _playerManager.GetPlayerPtr();
	const bool isGameOver = !player->GetIsAlive();
	if (isGameOver) { m_isGameOver = true; }

	//	アルファ値の加算減算の切り替え
	if (m_isChangeAlpha) { m_alpha -= ALPHA_SPEED; }
	else { m_alpha += ALPHA_SPEED; }

	//	最大最小処理
	const bool isMaxVal = m_alpha >= MAX_ALPHA;
	const bool isMinVal = m_alpha <= MIN_ALPHA;
	if (isMaxVal) { m_alpha = MAX_ALPHA; m_isChangeAlpha = true; }
	if (isMinVal) { m_alpha = MIN_ALPHA; m_isChangeAlpha = false; }

}

//-----------------------------------------------------------------------------
//	@brief	描画処理
//-----------------------------------------------------------------------------
void UIGameOver::Draw()
{
	if (m_isGameOver)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);
		DrawGraph((int)m_gameOverUI.m_pos.x, (int)m_gameOverUI.m_pos.y, m_gameOverUI.m_spriteHendle, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放
//-----------------------------------------------------------------------------
void UIGameOver::_FinalRelease()
{
	//	ゲームオーバーライフUIのスプライトのアンロード
	DeleteGraph(m_gameOverUI.m_spriteHendle);
}
