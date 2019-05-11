
//=============================================================================
//	@file	UIGauge.h
//	@brief	ゲージUI
//	@autor	Takuya Ochi
//	@date	2018/12/23
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
struct Color;
struct UI;
class SoundEffect;

//-----------------------------------------------------------------------------
//	@brief	UIGaugeクラス
//-----------------------------------------------------------------------------
class UIGauge final
{
public:

//==================== 列挙体 =====================//

	//	ゲームの加算種類
	enum ADD_GAUGE_KIND
	{
		ENEMY,
		BOSS,
		ITEM,
	};

//===================== 関数 ======================//

	//	コンストラクタ/デストラクタ
	UIGauge();
	~UIGauge();

	//	作成処理
	void Create();

	//	解放処理
	void Release();

	//	更新処理
	void Update(SoundEffect& _soundEffect);

	//	描画処理
	void Draw();

	//	ゲージの加算
	void AddGauge(const ADD_GAUGE_KIND _addGaugeKind);

	//	ゲージの初期化
	void ResetGauge();

//==================== getter =====================//

	//	ゲージの最大フラグの　getter
	const bool GetIsGaugeMax() const { return m_isGaugeMax; }

private:

//================== 内部処理関数 ==================//

	//	最終的な解放処理
	void _FinalRelease();

//=================== メンバ変数 ===================//

	UI		m_buttonUI;				//	ボタンUI
	Rect	m_gaugeFlame;			//	ゲージ枠
	Rect	m_gauge;				//	ゲージ
	Color	m_gaugeFlameColor;		//	ゲージ枠の色
	Color	m_gaugeColor;			//	ゲージの色
	float	m_targetGauge;			//	ゲージの目的地
	bool	m_isGaugeMax;			//	ゲージの最大フラグ
	bool	m_isChangeButtonScale;	//	ボタンサイズ切り替え

//===================== 静的定数 ===================//

	static const VECTOR		START_POS;			//	開始位置
	static const VECTOR		END_POS;			//	終了位置
	static const float		FLAME_INTERVAL;		//	フレーム間隔
	static const float		ADD_GAUGE_ENEMY;	//	エネミー討伐時の加算量
	static const float		ADD_GAUGE_BOSS;		//	ボス討伐時の加算量
	static const float		ADD_GAUGE_ITEM;		//	アイテム取得時の加算量
	static const float		MIN_BUTTON_SCALE;	//	ボタンの拡大率の最小
	static const float		MAX_BUTTON_SCALE;	//	ボタンの拡大率の最大
	static const float		BUTTON_SCALE_SPEED;	//	ボタンの拡大率の速度
	static const int		MAX_COLOR;			//	色の最大
	static const int		MIN_COLOR;			//	色の最小
	static const int		COLOR_SPEED;		//	色の速度
};