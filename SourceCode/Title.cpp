
//=============================================================================
//	@file	Title.cpp
//	@brief	タイトル
//	@autor	Takuya Ochi
//	@date	2018/1/6
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "Title.h"
#include "SoundEffect.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const float Title::EQUAL = 1.0f;				//	等倍
const float Title::SUB_SCALE_SPEED = 1.0f;		//	拡大率の速度

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
Title::Title()
{
	//	処理なし
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
Title::~Title()
{
	//	最終的な解放処理
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成処理
//-----------------------------------------------------------------------------
void Title::Create()
{
	//	タイトルロゴの初期化
	m_titleLogo.m_spriteHendle = LoadGraph("Data/Sprite/Title/TitleLogo.png");
	CommonDebug::Assert((m_titleLogo.m_spriteHendle <= -1), " [ Title.cpp ] : error : sprite loading failed.");
	m_titleLogo.m_pos = VGet(1090.0f, 150.0f, 0.0f);

	//	タイトルの初期化
	m_title.m_spriteHendle = LoadGraph("Data/Sprite/Title/Title.png");
	CommonDebug::Assert((m_title.m_spriteHendle <= -1), " [ Title.cpp ] : error : sprite loading failed.");
	m_title.m_pos = VGet(1430.0f, 425.0f, 0.0f);
	m_title.m_scale = 10.0f;
	m_title.m_angle = 0.0f;

	//	スタートの初期化
	m_start.m_spriteHendle = LoadGraph("Data/Sprite/Title/Start.png");
	CommonDebug::Assert((m_start.m_spriteHendle <= -1), " [ Title.cpp ] : error : sprite loading failed.");
	m_start.m_pos = VGet(550.0f, 925.0f, 0.0f);
	m_start.m_flashingTime = 0.0f;

	m_isSeStart = false;
}

//-----------------------------------------------------------------------------
//	@brief	解放処理
//-----------------------------------------------------------------------------
void Title::Release()
{
	//	スプライトのアンロード
	DeleteGraph(m_titleLogo.m_spriteHendle);
	DeleteGraph(m_title.m_spriteHendle);
	DeleteGraph(m_start.m_spriteHendle);
}

//-----------------------------------------------------------------------------
//	@brief	更新
//-----------------------------------------------------------------------------
void Title::Update(SoundEffect& _soundEffect)
{
	//	タイトルの拡大率
	_AskTitleScale(_soundEffect);
}

//-----------------------------------------------------------------------------
//	@brief	描画
//-----------------------------------------------------------------------------
void Title::Draw()
{
	DrawGraph((int)m_titleLogo.m_pos.x, (int)m_titleLogo.m_pos.y, m_titleLogo.m_spriteHendle, TRUE);
	DrawRotaGraph((int)m_title.m_pos.x, (int)m_title.m_pos.y, (double)m_title.m_scale, (double)m_title.m_angle, m_title.m_spriteHendle, TRUE);

	//	点滅描画
	_FlashingDraw(m_start);
}

//-----------------------------------------------------------------------------
//	@brief	タイトルの拡縮率を求める
//-----------------------------------------------------------------------------
void Title::_AskTitleScale(SoundEffect& _soundEffect)
{
	//	SEの開始
	if (!m_isSeStart)
	{
		//	拡大率を小さくする
		m_title.m_scale -= SUB_SCALE_SPEED;
		const bool isMinVal = m_title.m_scale <= EQUAL;
		if (isMinVal)
		{
			m_title.m_scale = EQUAL;
			_soundEffect.OnPlaySound(_soundEffect.ONE_SE_KIND::ONE_SE_TITLE);
			m_isSeStart = true;
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	点滅描画
//-----------------------------------------------------------------------------
void Title::_FlashingDraw(UI& _ui)
{
	const bool isDraw = _ui.m_flashingTime >= 0.0f && _ui.m_flashingTime <= 30.0f;
	const bool isReset = _ui.m_flashingTime >= 40.0f;

	if(isDraw) { DrawGraph((int)_ui.m_pos.x, (int)_ui.m_pos.y, _ui.m_spriteHendle, TRUE); }
	if (isReset) { _ui.m_flashingTime = 0.0f; }

	_ui.m_flashingTime++;
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放
//-----------------------------------------------------------------------------
void Title::_FinalRelease()
{
	//	スプライトのアンロード
	DeleteGraph(m_titleLogo.m_spriteHendle);
	DeleteGraph(m_title.m_spriteHendle);
	DeleteGraph(m_start.m_spriteHendle);
}
