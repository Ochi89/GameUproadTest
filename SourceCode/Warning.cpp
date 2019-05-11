
//=============================================================================
//	@file	Warning.cpp
//	@brief	警告処理
//	@autor	Takuya Ochi
//	@date	2018/1/6
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "Warning.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const int		Warning::ALPHA_SPEED = 10;				//	アルファ速度
const int		Warning::MAX_ALPHA = 255;				//	アルファ値の最大
const int		Warning::MIN_ALPHA = 0;					//	アルファ値の最小
const float		Warning::MAX_WARNING_TIME = 300.0f;		//	警告時間の最大

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
Warning::Warning()
{
	//	初期化
	m_spriteHandle = LoadGraph("Data/Sprite/Warning/Warning.png");
	m_pos = VGet(500.0f, 450.0f, 0.0f);
	m_alpha = 0;
	m_warningTime = 0.0f;
	m_isWarningStart = false;
	m_isSubAlpha = false;
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
Warning::~Warning()
{
	//	最終的な解放処理
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
void Warning::Update()
{
	if (m_isWarningStart)
	{
		//	アルファ値
		_Alpha();

		//	警告の終了
		m_warningTime++;
		const bool isEndWarning = m_warningTime >= MAX_WARNING_TIME;
		if (isEndWarning) { m_warningTime = 0.0f; m_isWarningStart = false; }
	}
}

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
void Warning::Draw()
{
	if (m_isWarningStart)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);
		DrawGraph((int)m_pos.x, (int)m_pos.y, m_spriteHandle, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

//-----------------------------------------------------------------------------
//	@brief	アルファ値
//-----------------------------------------------------------------------------
void Warning::_Alpha()
{
	//	加算減算
	if (m_isSubAlpha) { m_alpha -= ALPHA_SPEED; }
	else { m_alpha += ALPHA_SPEED; }

	//	最大最小
	const bool isMaxAlpha = m_alpha >= MAX_ALPHA;
	const bool isMinAlpha = m_alpha <= MIN_ALPHA;
	if (isMaxAlpha) { m_alpha = MAX_ALPHA; m_isSubAlpha = true; }
	if (isMinAlpha) { m_alpha = MIN_ALPHA; m_isSubAlpha = false; }
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放
//-----------------------------------------------------------------------------
void Warning::_FinalRelease()
{
	//	スプライトのアンロード
	DeleteGraph(m_spriteHandle);
}
