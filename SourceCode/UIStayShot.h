
//=============================================================================
//	@file	UIStayShot.h
//	@brief	ステイショットUI
//	@autor	Takuya Ochi
//	@date	2018/1/10
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	プリプロセッサ
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "Common.h"

//-----------------------------------------------------------------------------
//	@brief	前方宣言
//-----------------------------------------------------------------------------
struct Rect;
struct UI;
struct Color;
struct Animation;
class SoundEffect;
class PlayerManager;

//-----------------------------------------------------------------------------
//	@brief	UIStayShotクラス
//-----------------------------------------------------------------------------
class UIStayShot final
{
public:

//===================== 関数 ======================//

	//	コンストラクタ/デストラクタ
	UIStayShot();
	~UIStayShot();

	//	作成処理
	void Create();

	//	解放処理
	void Release();

	//	更新処理
	void Update(PlayerManager& _playerManager, SoundEffect& _soundEffect);

	//	描画処理
	void Draw(PlayerManager& _playerManager);

private:

//================== 内部処理関数 ==================//

	//	ボタンの拡大率の切り替え
	void _ChangeButtonScale();

	//	リセット
	void _Reset();

	//	ゲージの加算
	void _AddGauge();

	//	最終的な解放処理
	void _FinalRelease();

//=================== メンバ変数 ===================//

	int			m_stayShotSpriteHendle[CommonConstant::STAY_SHOT_UI_ANIM_NUM];	//	ステイショットUIのスプライトハンドル
	VECTOR		m_stayShotPos;													//	ステイショットUIのポジション
	Animation	m_stayShotAnimation;											//	ステイショットUIのアニメーション
	float		m_stayShotScale;												//	ステイショットUIの拡大率
	UI			m_buttonUI;														//	ボタンUI
	bool		m_isChangeButtonScale;											//	ボタンサイズ切り替え
	Rect		m_gauge;														//	ゲージ
	int			m_gaugeAlpha;													//	ゲージのアルファ値
	Color		m_gaugeColor;													//	ゲージの色
	bool		m_isOneTimeReset;												//	リセットフラグ
	bool		m_isOneTimeSE;													//	SEフラグ

//===================== 静的定数 ===================//

	static const VECTOR		START_POS;					//	開始位置
	static const VECTOR		END_POS;					//	終了位置
	static const float		MIN_BUTTON_SCALE;			//	ボタンの拡大率の最小
	static const float		MAX_BUTTON_SCALE;			//	ボタンの拡大率の最大
	static const float		BUTTON_SCALE_SPEED;			//	ボタンの拡大率の速度

};