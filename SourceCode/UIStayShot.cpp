
//=============================================================================
//	@file	UIStayShot.h
//	@brief	ステイショットUI
//	@autor	Takuya Ochi
//	@date	2018/1/10
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "UIStayShot.h"
#include "PlayerManager.h"
#include "PlayerBase.h"
#include "SoundEffect.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const VECTOR	UIStayShot::START_POS = VGet(475.0f, 11.0f, 0.0f);	//	開始位置
const VECTOR	UIStayShot::END_POS = VGet(674.0f, 109.0f, 0.0f);	//	終了位置
const float		UIStayShot::MIN_BUTTON_SCALE = 1.0f;				//	ボタンの拡大率の最小
const float		UIStayShot::MAX_BUTTON_SCALE = 1.2f;				//	ボタンの拡大率の最大
const float		UIStayShot::BUTTON_SCALE_SPEED = 0.03f;				//	ボタンの拡大率の速度

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
UIStayShot::UIStayShot()
{
	//	処理なし
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
UIStayShot::~UIStayShot()
{
	//	最終的な解放
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成処理
//-----------------------------------------------------------------------------
void UIStayShot::Create()
{
	//	初期化
	LoadDivGraph("Data/Sprite/StayShot/StayShot.png", 10, 10, 1, 300, 150, m_stayShotSpriteHendle);
	CommonDebug::Assert((m_stayShotSpriteHendle[0] <= -1), " [ UIStayShot.cpp ] : error : sprite loading failed.");
	m_stayShotPos = VGet(575.0f, 60.0f, 0.0f);
	m_stayShotAnimation.m_count = 0;
	m_stayShotAnimation.m_time = 0.0f;
	m_stayShotAnimation.m_maxTime = 5.0f;
	m_stayShotAnimation.m_maxCount = CommonConstant::STAY_SHOT_UI_ANIM_NUM;
	m_stayShotScale = 0.7f;

	//	ボタンUI
	m_buttonUI.m_spriteHendle = LoadGraph("Data/Sprite/StayShot/ButtonB.png");
	CommonDebug::Assert((m_buttonUI.m_spriteHendle <= -1), " [ UIStayShot.cpp ] : error : sprite loading failed.");
	m_buttonUI.m_pos = VGet(575.0f, 60.0f, 0.0f);
	m_buttonUI.m_scale = MIN_BUTTON_SCALE;
	m_isChangeButtonScale = false;

	//	ゲージ
	m_gauge.m_vertexTop = START_POS;
	m_gauge.m_vertexUnder = END_POS;
	m_gaugeAlpha = 180;

	//	色
	m_gaugeColor.m_red = 50;
	m_gaugeColor.m_green = 50;
	m_gaugeColor.m_blue = 50;
	m_gaugeColor.m_color = CommonFunction::GetColor(m_gaugeColor);

	m_isOneTimeReset = false;
	m_isOneTimeSE = true;
}

//-----------------------------------------------------------------------------
//	@brief	解放処理
//-----------------------------------------------------------------------------
void UIStayShot::Release()
{
	//	スプライトのアンロード
	for (int i = 0; i < CommonConstant::STAY_SHOT_UI_ANIM_NUM; i++)
	{
		DeleteGraph(m_stayShotSpriteHendle[i]);
	}

	DeleteGraph(m_buttonUI.m_spriteHendle);
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void UIStayShot::Update(PlayerManager& _playerManager, SoundEffect& _soundEffect)
{
	PlayerBase* player = _playerManager.GetPlayerPtr();
	if (player)
	{
		//	滞在ショットは使用可能
		const bool isUseStayShot = player->GetIsUseStayShot();
		if (isUseStayShot)
		{
			if (!m_isOneTimeSE) { _soundEffect.OnPlaySound(_soundEffect.ONE_SE_KIND::ONE_SE_STAY_SHOT_GAUGE_MAX); m_isOneTimeSE = true; }

			//	アニメーションを求める
			CommonFunction::AskAnimationCount(m_stayShotAnimation);

			//	ボタンの拡大率の切り替え
			_ChangeButtonScale();

			m_isOneTimeReset = false;
		}
		else
		{
			//	ダメージを受けていないとき
			const bool isNotDamage = !player->GetIsDamage();
			if (isNotDamage)
			{
				//	リセット
				_Reset();

				//	ゲージの加算
				_AddGauge();
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	描画処理
//-----------------------------------------------------------------------------
void UIStayShot::Draw(PlayerManager& _playerManager)
{
	DrawRotaGraph((int)m_stayShotPos.x, (int)m_stayShotPos.y, (double)m_stayShotScale, 0, m_stayShotSpriteHendle[m_stayShotAnimation.m_count], TRUE);
	
	PlayerBase* player = _playerManager.GetPlayerPtr();
	if (player)
	{
		//	滞在ショットは使用可能
		const bool isUseStayShot = player->GetIsUseStayShot();
		if (isUseStayShot)
		{
			DrawRotaGraph((int)m_buttonUI.m_pos.x, (int)m_buttonUI.m_pos.y, (double)m_buttonUI.m_scale, 0, m_buttonUI.m_spriteHendle, TRUE);
		}
		else
		{
			//	ゲージの描画
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_gaugeAlpha);
			DrawBox((int)m_gauge.m_vertexTop.x, (int)m_gauge.m_vertexTop.y, (int)m_gauge.m_vertexUnder.x, (int)m_gauge.m_vertexUnder.y, m_gaugeColor.m_color, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	ボタンの拡大率の切り替え
//-----------------------------------------------------------------------------
void UIStayShot::_ChangeButtonScale()
{
	//	ボタンの拡大率の変更
	if (m_isChangeButtonScale) { m_buttonUI.m_scale -= BUTTON_SCALE_SPEED; }
	else { m_buttonUI.m_scale += BUTTON_SCALE_SPEED; }

	//	最大最小処理
	const bool isMaxVal = m_buttonUI.m_scale >= MAX_BUTTON_SCALE;
	const bool isMinVal = m_buttonUI.m_scale <= MIN_BUTTON_SCALE;
	if (isMaxVal) { m_buttonUI.m_scale = MAX_BUTTON_SCALE; m_isChangeButtonScale = true; }
	if (isMinVal) { m_buttonUI.m_scale = MIN_BUTTON_SCALE; m_isChangeButtonScale = false; }
}

//-----------------------------------------------------------------------------
//	@brief	リセット
//-----------------------------------------------------------------------------
void UIStayShot::_Reset()
{
	if (!m_isOneTimeReset)
	{
		//	ゲージの初期化
		m_gauge.m_vertexTop = START_POS;
		m_gauge.m_vertexUnder = END_POS;

		//	アニメーションのリセット
		m_stayShotAnimation.m_time = 0.0f;

		//	ボタン初期化
		m_buttonUI.m_scale = MIN_BUTTON_SCALE;
		m_isChangeButtonScale = false;

		m_isOneTimeReset = true;
		m_isOneTimeSE = false;
	}
}

//-----------------------------------------------------------------------------
//	@brief	ゲージの加算
//-----------------------------------------------------------------------------
void UIStayShot::_AddGauge()
{
	//	ゲージの加算
	m_gauge.m_vertexTop.x += 3.0f;

	//	ゲージの最大
	const bool isMaxVal = m_gauge.m_vertexTop.x >= END_POS.x;
	if (isMaxVal) { m_gauge.m_vertexTop.x = END_POS.x; }
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放
//-----------------------------------------------------------------------------
void UIStayShot::_FinalRelease()
{
	//	スプライトのアンロード
	for (int i = 0; i < CommonConstant::STAY_SHOT_UI_ANIM_NUM; i++)
	{
		DeleteGraph(m_stayShotSpriteHendle[i]);
	}

	DeleteGraph(m_buttonUI.m_spriteHendle);
}
