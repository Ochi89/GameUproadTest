
//=============================================================================
//	@file	UIGauge.cpp
//	@brief	ゲージ
//	@autor	Takuya Ochi
//	@date	2018/12/23
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "UIGauge.h"
#include "SoundEffect.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const VECTOR	UIGauge::START_POS = VGet(700.0f, 50.0f, 0.0f);		//	開始位置
const VECTOR	UIGauge::END_POS = VGet(1400.0f, 75.0f, 0.0f);		//	終了位置
const float		UIGauge::FLAME_INTERVAL = 3.0f;						//	フレーム間隔
const float		UIGauge::ADD_GAUGE_ENEMY = 8.0f;					//	エネミー討伐時の加算量
const float		UIGauge::ADD_GAUGE_BOSS = 8.0f;						//	ボス討伐時の加算量
const float		UIGauge::ADD_GAUGE_ITEM = 28.0f;					//	アイテム取得時の加算量
const float		UIGauge::MIN_BUTTON_SCALE = 1.0f;					//	ボタンの拡大率の最小
const float		UIGauge::MAX_BUTTON_SCALE = 1.2f;					//	ボタンの拡大率の最大
const float		UIGauge::BUTTON_SCALE_SPEED = 0.03f;				//	ボタンの拡大率の速度
const int		UIGauge::MAX_COLOR = 255;							//	色の最大
const int		UIGauge::MIN_COLOR = 180;							//	色の最小
const int		UIGauge::COLOR_SPEED = 5;							//	色の速度

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
UIGauge::UIGauge()
{
	//	処置なし
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
UIGauge::~UIGauge()
{
	//	最終的な解放
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成処理
//-----------------------------------------------------------------------------
void UIGauge::Create()
{
	//	各変数の初期化
	m_gaugeFlame.m_vertexTop = START_POS;
	m_gaugeFlame.m_vertexUnder = END_POS;
	m_gauge.m_vertexTop = START_POS;
	m_gauge.m_vertexUnder = VGet(START_POS.x, END_POS.y, START_POS.z);

	//	フレーム色
	m_gaugeFlameColor.m_red = 128;
	m_gaugeFlameColor.m_green = 128;
	m_gaugeFlameColor.m_blue = 128;
	m_gaugeFlameColor.m_color = CommonFunction::GetColor(m_gaugeFlameColor);

	//	ゲージ色
	m_gaugeColor.m_red = 255;
	m_gaugeColor.m_green = 255;
	m_gaugeColor.m_blue = 255;
	m_gaugeColor.m_color = CommonFunction::GetColor(m_gaugeColor);

	m_targetGauge = START_POS.x;
	m_isGaugeMax = false;

	//	ボタンUI
	m_buttonUI.m_spriteHendle = LoadGraph("Data/Sprite/Gauge/ButtonX.png");
	CommonDebug::Assert((m_buttonUI.m_spriteHendle <= -1), " [ UIGauge.cpp ] : error : sprite loading failed.");
	m_buttonUI.m_pos = VGet(1050.0f, 65.0f, 0.0f);
	m_buttonUI.m_scale = MIN_BUTTON_SCALE;
	m_isChangeButtonScale = false;
}

//-----------------------------------------------------------------------------
//	@brief	解放処理
//-----------------------------------------------------------------------------
void UIGauge::Release()
{
	//	スプライトのアンロード
	DeleteGraph(m_buttonUI.m_spriteHendle);
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void UIGauge::Update(SoundEffect& _soundEffect)
{
	//	ゲージの加算
	m_gauge.m_vertexUnder.x = m_targetGauge;

	//	ゲージがたまったら、
	//	カウントを加算し、ゲージを初期値にする
	const bool isOutRange = m_gauge.m_vertexUnder.x > END_POS.x;
	if (isOutRange)
	{
		//	ゲージがたまった瞬間のみSE再生
		const bool isOnSe = !m_isGaugeMax;
		if (isOnSe) { _soundEffect.OnPlaySound(_soundEffect.ONE_SE_KIND::ONE_SE_GAUGE_MAX); }

		m_isGaugeMax = true;

		//	大きさの変更
		m_gauge.m_vertexTop = VGet(START_POS.x - FLAME_INTERVAL, START_POS.y - FLAME_INTERVAL, START_POS.z);
		m_gauge.m_vertexUnder = VGet(END_POS.x + FLAME_INTERVAL, END_POS.y + FLAME_INTERVAL, END_POS.z);

		//	色の変更
		m_gaugeColor.m_red -= COLOR_SPEED;
		m_gaugeColor.m_green -= COLOR_SPEED;
		m_gaugeColor.m_blue -= COLOR_SPEED;
		
		//	最小処理
		const bool isMinColor = m_gaugeColor.m_red <= MIN_COLOR;
		if(isMinColor)
		{
			m_gaugeColor.m_red = MAX_COLOR;
			m_gaugeColor.m_green = MAX_COLOR;
			m_gaugeColor.m_blue = MAX_COLOR;
		}

		//	ボタンの拡大率の変更
		if(m_isChangeButtonScale) { m_buttonUI.m_scale -= BUTTON_SCALE_SPEED; }
		else { m_buttonUI.m_scale += BUTTON_SCALE_SPEED; }

		//	最大最小処理
		const bool isMaxVal = m_buttonUI.m_scale >= MAX_BUTTON_SCALE;
		const bool isMinVal = m_buttonUI.m_scale <= MIN_BUTTON_SCALE;
		if (isMaxVal) { m_buttonUI.m_scale = MAX_BUTTON_SCALE; m_isChangeButtonScale = true; }
		if (isMinVal) { m_buttonUI.m_scale = MIN_BUTTON_SCALE; m_isChangeButtonScale = false; }
	}

	//	色を求める	
	m_gaugeColor.m_color = CommonFunction::GetColor(m_gaugeColor);
}

//-----------------------------------------------------------------------------
//	@brief	描画処理
//-----------------------------------------------------------------------------
void UIGauge::Draw()
{
	//	ゲージの描画
	DrawBox((int)m_gaugeFlame.m_vertexTop.x, (int)m_gaugeFlame.m_vertexTop.y, (int)m_gaugeFlame.m_vertexUnder.x, (int)m_gaugeFlame.m_vertexUnder.y, m_gaugeFlameColor.m_color, TRUE);
	DrawBox((int)m_gauge.m_vertexTop.x, (int)m_gauge.m_vertexTop.y, (int)m_gauge.m_vertexUnder.x, (int)m_gauge.m_vertexUnder.y, m_gaugeColor.m_color, TRUE);

	//	ボタンの描画
	if(m_isGaugeMax) { DrawRotaGraph((int)m_buttonUI.m_pos.x, (int)m_buttonUI.m_pos.y, (double)m_buttonUI.m_scale, 0, m_buttonUI.m_spriteHendle, TRUE); }
}

//-----------------------------------------------------------------------------
//	@brief	ゲージの加算
//-----------------------------------------------------------------------------
void UIGauge::AddGauge(const ADD_GAUGE_KIND _addGaugeKind)
{
	switch (_addGaugeKind)
	{

	case ADD_GAUGE_KIND::ENEMY:
		m_targetGauge += ADD_GAUGE_ENEMY;
		break;

	case ADD_GAUGE_KIND::BOSS:
		m_targetGauge += ADD_GAUGE_BOSS;
		break;

	case ADD_GAUGE_KIND::ITEM:
		m_targetGauge += ADD_GAUGE_ITEM;
		break;
	}
}

//-----------------------------------------------------------------------------
//	@brief	ゲージの初期化
//-----------------------------------------------------------------------------
void UIGauge::ResetGauge()
{
	//	初期化
	m_targetGauge = START_POS.x;
	m_isGaugeMax = false;

	//	位置の初期化
	m_gauge.m_vertexTop = START_POS;
	m_gauge.m_vertexUnder = VGet(START_POS.x, END_POS.y, START_POS.z);

	//	ゲージ色
	m_gaugeColor.m_red = 255;
	m_gaugeColor.m_green = 255;
	m_gaugeColor.m_blue = 255;
	m_gaugeColor.m_color = CommonFunction::GetColor(m_gaugeColor);

	//	ボタンのサイズ
	m_buttonUI.m_scale = MIN_BUTTON_SCALE;
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放
//-----------------------------------------------------------------------------
void UIGauge::_FinalRelease()
{
	//	スプライトのアンロード
	DeleteGraph(m_buttonUI.m_spriteHendle);
}

