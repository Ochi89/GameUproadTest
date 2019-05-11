
//=============================================================================
//	@file	Opening.h
//	@brief	オープニング
//	@autor	Takuya Ochi
//	@date	2018/1/18
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "Opening.h"
#include "Pad.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const int		Opening::MIN_ALPHA = 0;				//	アルファ値の最小
const int		Opening::MAX_ALPHA = 255;			//	アルファ値の最大
const int		Opening::ALPHA_DEFAULT_SPEED = 3;	//	アルファ値のデフォルト速度
const int		Opening::ALPHA_ACCELE_SPEED = 9;	//	アルファ値のアクセル速度

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
Opening::Opening()
{
	//	オープニングの読み込み
	_LoadUI(m_OpeningUI1, "Data/Sprite/Opening/Opening01.png");
	_LoadUI(m_OpeningUI2, "Data/Sprite/Opening/Opening02.png");
	_LoadUI(m_OpeningUI3, "Data/Sprite/Opening/Opening03.png");
	_LoadUI(m_OpeningUI4, "Data/Sprite/Opening/Opening04.png");
	_LoadUI(m_OpeningUI5, "Data/Sprite/Opening/Opening05.png");
	_LoadUI(m_OpeningUI6, "Data/Sprite/Opening/Opening06.png");

	//	背景の設定
	m_backGroundRect.m_vertexTop = VGet(0.0f, 0.0f, 0.0f);
	m_backGroundRect.m_vertexUnder = VGet((float)CommonConstant::WINDOW_WIDTH, (float)CommonConstant::WINDOW_HEIGHT, 0.0f);
	m_backGroundColor.m_red = 0;
	m_backGroundColor.m_green = 0;
	m_backGroundColor.m_blue = 0;
	m_backGroundColor.m_color = CommonFunction::GetColor(m_backGroundColor);

	//	そのほかの初期化
	m_openingCount = OPENING_KIND::OPENING_NONE;
	m_alpha = MIN_ALPHA;
	m_isChangeAlpha = false;
	m_isStart = false;
	m_isEndOpening = false;
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
Opening::~Opening()
{
	//	最終的な解放処理
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	インスタンス生成
//-----------------------------------------------------------------------------
Opening* Opening::GetInstance()
{
	return &m_instance;
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void Opening::Update(Pad& _pad)
{
	//	オープニングが終了していないとき
	if (!m_isEndOpening)
	{
		//	アルファ値の変更
		_ChangeAlpha();

		//	アルファ値の加速
		_AcceleAlpha(_pad);

		//	オープニングのスキップ
		_OpeningSkip(_pad);

		//	オープニングの終了
		_EndOpening();
	}
}

//-----------------------------------------------------------------------------
//	@brief	描画処理
//-----------------------------------------------------------------------------
void Opening::Draw()
{
	//	背景の描画
	DrawBox(
		(int)m_backGroundRect.m_vertexTop.x, 
		(int)m_backGroundRect.m_vertexTop.y, 
		(int)m_backGroundRect.m_vertexUnder.x,
		(int)m_backGroundRect.m_vertexUnder.y,
		m_backGroundColor.m_color,
		TRUE
	);

	//	オープニングの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);
	switch (m_openingCount)
	{
	case OPENING_KIND::OPENING_1:
		_DrawUI(m_OpeningUI1);
		break;

	case OPENING_KIND::OPENING_2:
		_DrawUI(m_OpeningUI2);
		break;

	case OPENING_KIND::OPENING_3:
		_DrawUI(m_OpeningUI3);
		break;

	case OPENING_KIND::OPENING_4:
		_DrawUI(m_OpeningUI4);
		break;

	case OPENING_KIND::OPENING_5:
		_DrawUI(m_OpeningUI5);
		break;

	case OPENING_KIND::OPENING_6:
		_DrawUI(m_OpeningUI6);
		break;
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, MIN_ALPHA);
}

//-----------------------------------------------------------------------------
//	@brief	UIの読み込み
//-----------------------------------------------------------------------------
void Opening::_LoadUI(UI& _ui, const char* _fileName)
{
	_ui.m_spriteHendle = LoadGraph(_fileName);
	CommonDebug::Assert((_ui.m_spriteHendle <= -1), " [ Opening.cpp ] : error : sprite loading failed.");
	_ui.m_pos = VGet(0.0f, 0.0f, 0.0f);
}

//-----------------------------------------------------------------------------
//	@brief	UIの描画
//-----------------------------------------------------------------------------
void Opening::_DrawUI(UI _ui)
{
	DrawGraph((int)_ui.m_pos.x, (int)_ui.m_pos.y, _ui.m_spriteHendle, TRUE);
}

//-----------------------------------------------------------------------------
//	@brief	アルファ値の変更
//-----------------------------------------------------------------------------
void Opening::_ChangeAlpha()
{
	if (!m_isStart)
	{
		m_alphaSpeed = ALPHA_DEFAULT_SPEED;
		m_openingCount++;
		m_alpha = MIN_ALPHA;
		m_isStart = true;
	}

	//	アルファ値の加算減算の切り替え
	if (m_isChangeAlpha) { m_alpha -= m_alphaSpeed; }
	else { m_alpha += m_alphaSpeed; }

	//	最大最小処理
	const bool isMaxVal = m_alpha >= MAX_ALPHA;
	const bool isMinVal = m_alpha <= MIN_ALPHA;
	if (isMaxVal) { m_alpha = MAX_ALPHA; m_isChangeAlpha = true; }
	if (isMinVal) { m_alpha = MIN_ALPHA; m_isChangeAlpha = false; m_isStart = false; }
}

//-----------------------------------------------------------------------------
//	@brief	アルファ値の加速
//-----------------------------------------------------------------------------
void Opening::_AcceleAlpha(Pad& _pad)
{
	const bool isKeyActive = KEY->Press(KEY_INPUT_SPACE) || KEY->Press(KEY_INPUT_X) || KEY->Press(KEY_INPUT_K) || KEY->Press(KEY_INPUT_A);
	const bool isNextActive = isKeyActive || _pad.GetXInputButton(PadInfo::PAD_BUTTON_KIND::BUTTON_ALL);
	if (isNextActive)
	{
		m_alphaSpeed = ALPHA_ACCELE_SPEED;
	}
}

//-----------------------------------------------------------------------------
//	@brief	オープニングのスキップ
//-----------------------------------------------------------------------------
void Opening::_OpeningSkip(Pad& _pad)
{
	const bool isNextActive = KEY->Press(KEY_INPUT_RETURN) || _pad.GetXInputButton(PadInfo::PAD_BUTTON_KIND::BUTTON_START);
	if (isNextActive)
	{
		m_isEndOpening = true;
	}
}

//-----------------------------------------------------------------------------
//	@brief	終了処理
//-----------------------------------------------------------------------------
void Opening::_EndOpening()
{
	const bool isOpeningEnd = m_openingCount >= OPENING_KIND::OPENING_NUM;
	if (isOpeningEnd)
	{
		m_isEndOpening = true;
	}
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放
//-----------------------------------------------------------------------------
void Opening::_FinalRelease()
{
	//	スプライトのアンロード
	DeleteGraph(m_OpeningUI1.m_spriteHendle);
	DeleteGraph(m_OpeningUI2.m_spriteHendle);
	DeleteGraph(m_OpeningUI3.m_spriteHendle);
	DeleteGraph(m_OpeningUI4.m_spriteHendle);
	DeleteGraph(m_OpeningUI5.m_spriteHendle);
	DeleteGraph(m_OpeningUI6.m_spriteHendle);
}
