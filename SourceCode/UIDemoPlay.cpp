
//=============================================================================
//	@file	UIDemoPlay.cpp
//	@brief	デモプレイUI
//	@autor	Takuya Ochi
//	@date	2018/1/15
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "UIDemoPlay.h"
#include "Common.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const int		UIDemoPlay::MIN_ALPHA = 128;			//	アルファ値の最小
const int		UIDemoPlay::MAX_ALPHA = 255;			//	アルファ値の最大
const int		UIDemoPlay::ALPHA_SPEED = 5;			//	アルファ値の速度

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
UIDemoPlay::UIDemoPlay()
{
	//	処理なし
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
UIDemoPlay::~UIDemoPlay()
{
	//	最終的な解放
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成
//-----------------------------------------------------------------------------
void UIDemoPlay::Create()
{
	//	タイトルロゴの初期化
	m_titleLogo.m_spriteHendle = LoadGraph("Data/Sprite/DemoPlay/DemoLog.png");
	CommonDebug::Assert((m_titleLogo.m_spriteHendle <= -1), " [ UIDemoPlay.cpp ] : error : sprite loading failed.");
	m_titleLogo.m_pos = VGet(1600.0f, 0.0f, 0.0f);

	//	スタートの初期化
	m_start.m_spriteHendle = LoadGraph("Data/Sprite/DemoPlay/Start.png");
	CommonDebug::Assert((m_start.m_spriteHendle <= -1), " [ UIDemoPlay.cpp ] : error : sprite loading failed.");
	m_start.m_pos = VGet(550.0f, 925.0f, 0.0f);
	m_start.m_flashingTime = 0.0f;

	//	デモプレイUI
	m_demoPlayUI.m_spriteHendle = LoadGraph("Data/Sprite/DemoPlay/DemoPlay.png");
	CommonDebug::Assert((m_demoPlayUI.m_spriteHendle <= -1), " [ UIDemoPlay.cpp ] : error : sprite loading failed.");
	m_demoPlayUI.m_pos = VGet(575.0f, 475.0f, 0.0f);
	m_alpha = MAX_ALPHA;
	m_isChangeAlpha = false;
}

//-----------------------------------------------------------------------------
//	@brief	解放
//-----------------------------------------------------------------------------
void UIDemoPlay::Release()
{
	//	スプライトのアンロード
	DeleteGraph(m_titleLogo.m_spriteHendle);
	DeleteGraph(m_start.m_spriteHendle);
	DeleteGraph(m_demoPlayUI.m_spriteHendle);
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void UIDemoPlay::Update()
{
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
void UIDemoPlay::Draw()
{
	//点滅描画
	_FlashingDraw(m_start);

	//	タイトルロゴ描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, MIN_ALPHA);
	DrawGraph((int)m_titleLogo.m_pos.x, (int)m_titleLogo.m_pos.y, m_titleLogo.m_spriteHendle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//	デモプレイUIの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);
	DrawGraph((int)m_demoPlayUI.m_pos.x , (int)m_demoPlayUI.m_pos.y, m_demoPlayUI.m_spriteHendle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//-----------------------------------------------------------------------------
//	@brief	点滅描画
//-----------------------------------------------------------------------------
void UIDemoPlay::_FlashingDraw(UI& _ui)
{
	const bool isDraw = _ui.m_flashingTime >= 0.0f && _ui.m_flashingTime <= 30.0f;
	const bool isReset = _ui.m_flashingTime >= 40.0f;

	if (isDraw) { DrawGraph((int)_ui.m_pos.x, (int)_ui.m_pos.y, _ui.m_spriteHendle, TRUE); }
	if (isReset) { _ui.m_flashingTime = 0.0f; }

	_ui.m_flashingTime++;
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放
//-----------------------------------------------------------------------------
void UIDemoPlay::_FinalRelease()
{
	//	スプライトのアンロード
	DeleteGraph(m_titleLogo.m_spriteHendle);
	DeleteGraph(m_start.m_spriteHendle);
	DeleteGraph(m_demoPlayUI.m_spriteHendle);
}
