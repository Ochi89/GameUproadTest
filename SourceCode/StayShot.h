
//=============================================================================
//	@file	StayShot.h
//	@brief	滞在弾ひとつ分の処理
//	@autor	Takuya Ochi
//	@date	2018/12/25
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
struct Circle;
struct Rect;
class EffekseerEmitter;
class SoundEffect;

//-----------------------------------------------------------------------------
//	@brief	ステイショットクラス
//-----------------------------------------------------------------------------
class StayShot final
{
public:

//===================== 関数 ======================//

	//	コンストラクタ / デストラクタ
	StayShot();
	~StayShot();

	//	初期処理
	void Initialize();

	//	更新処理
	void Update(SoundEffect& _soundEffect);

//==================== setter =====================//

	//	開始位置の setter
	void SetStartPos(const VECTOR _set) { m_pos = _set; }

	//	一度のみの setter
	void SetIsOneTime(const bool _set) { m_isOneTime = _set; }

	//	滞在に切り替えるフラグの setter
	void SetIsChangeStay(const bool _set) { m_isChangeStay = _set; }

//==================== getter =====================//

	//	ポジションの getter
	const VECTOR& GetPos() const { return m_pos; }

	//	一度のみの getter
	const bool& GetIsOneTime() const { return m_isOneTime; }

	//	当たり判定用の円形の getter
	const Circle& GetCircle() const { return m_hitCircle; }

	//	当たり判定用の長方形の getter
	const Rect& GetRect() const { return m_hitRect; }

	//	ショットの終了の getter
	const bool& GetIsShotEnd() const { return m_isShotEnd; }

private:

//================== 内部処理関数 ==================//

	//	移動処理
	void _Move(SoundEffect& _soundEffect);

	//	滞在処理
	void _Stay(SoundEffect& _soundEffect);

	//	当たり判定用の点の更新
	void _UpdateHitPoint();

	//	最終的な解放処理
	void _FinalRelease();

//=================== メンバ変数 ===================//

	VECTOR		m_pos;					//	位置
	bool		m_isOneTime;			//	一度のみ処理フラグ
	bool		m_isChangeStay;			//	モードを滞在に切り替えるフラグ
	float		m_stayTime;				//	滞在時間
	bool		m_isShotEnd;			//	ショットの終了
	bool		m_isMoveShotSeStart;	//	移動ショットSE開始
	bool		m_isStayShotSeStart;	//	滞在ショットSE開始

	Circle		m_hitCircle;			//	当たり判定用の円形
	Rect		m_hitRect;				//	当たり判定用の長方形

	//	エフェクト関係
	EffekseerEmitter*	m_effectMove;	//	移動エフェクト
	EffekseerEmitter*	m_effectStay;	//	滞在エフェクト
	float				m_effectScale;	//	エフェクトの拡大率

//===================== 静的定数 ===================//

	static const VECTOR		MOVE_DIR;			//	移動向き
	static const float		MOVE_SPEED;			//	移動速度
	static const float		MOVE_HIT_RADIUS;	//	移動中の当たり判定用の半径
	static const float		STAY_HIT_RADIUS;	//	滞在中の当たり判定用の半径
	static const float		MIN_HIT_RADIUS;		//	当たり判定用の最小半径
	static const float		MAX_STAY_TIME;		//	滞在中の時間の最大
	static const float		EFFECT_SCALE;		//	エフェクトの拡大速度
	static const float		MAX_X_POS;			//	X座標の最大
	static const VECTOR		ADJUSTMENT_RECT;	//	当たり判定調整用

};