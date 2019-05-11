
//=============================================================================
//	@file	PlayerBase.h
//	@brief	プレイヤーベース
//	@autor	Takuya Ochi
//	@date	2018/11/14
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	プリプロセッサ
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "CharacterBase.h"

//-----------------------------------------------------------------------------
//	@brief	前方宣言
//-----------------------------------------------------------------------------
struct Circle;
struct Rect;
class Pad;
class ShotManager;
class StayShotManager;
class UIGauge;
class EffekseerEmitter;
class SoundEffect;
class EnemyManager;

//-----------------------------------------------------------------------------
//	@brief	プレイヤー基底クラス
//-----------------------------------------------------------------------------
class PlayerBase : public CharacterBase
{
public:
	
	//	コンストラクタ / デストラクタ
	PlayerBase(const int _modelHandle);
	virtual ~PlayerBase();

	//	作成処理
	virtual void Create() override = 0;

	//	解放処理
	virtual void Release() override = 0;

	//	初期処理
	virtual void Initialize() override = 0;

	//	更新処理
    virtual void Update() {};
    virtual void Update(Pad& _pad, ShotManager& _shot, StayShotManager& _stayShot, SoundEffect& _soundEffect, UIGauge& _gaugeUI) {};
    virtual void Update(ShotManager& _shot, StayShotManager& _stayShot, SoundEffect& _soundEffect, UIGauge& _gaugeUI, EnemyManager& _enemyManager) {};

	//	描画処理
	virtual void Draw() override final;

	//	ダメージ判定
	void OnHitDamage() { m_life--; }

//==================== getter =====================//

	//	残りライフの getter
	const int& GetLife() const { return m_life; }

	//	出現フラグの getter
	const bool& GetIsEmerge() const { return m_isEmerge; }

	//	ダメージフラグの getter
	const bool& GetIsDamage() const { return m_isDamage; }

	//	無敵フラグの getter
	const bool& GetIsInvincible() const { return m_isInvincible; }

	//	レーザー使用後の無敵フラグの getter
	const bool& GetIsInvincibleAfterLaser() const { return m_isInvincibleAfterLaser; }

	//	残機があるかの getter
	const bool GetIsAlive() const { return (m_life > 0); }

	//	レーザーのポジションの getter
	const Rect& GetLaserHitRect() const { return m_laserHitRect; }

	//	滞在弾の遅延時間の getter
	const float& GetStayShotWaitTime() const { return m_stayShotWaitTime; }

	//	初期位置の getter
	const VECTOR GetStartPos() const;

	//	滞在ショットの使用状況の getter
	const bool& GetIsUseStayShot() const { return m_isUseStayShot; }

	//	アイテム用の当たり判定の getter
	const Rect& GetItemHitRect() const { return m_itemHitRect; }

	//	アイテム取得時のエフェクトの開始フラグの getter
	const bool& GetIsStartEffectGetItem() const { return m_isStartEffectGetItem; }

//==================== setter =====================//
		
	//	出現フラグの setter
	void SetIsEmerge(const bool _set) { m_isEmerge = _set; }

	//	ダメージフラグの setter
	void SetIsDamage(const bool _set) { m_isDamage = _set; }

	//	無敵フラグの setter
	void SetIsInvincible(const bool _set) { m_isInvincible = _set; }

	//	レーザー使用後の無敵フラグの setter
	void SetIsInvincibleAfterLaser(const bool _set) { m_isInvincibleAfterLaser = _set; }

	//	移動停止フラグの setter
	void SetIsStopMove(const bool _set) { m_isStopMove = _set; }

	//	シーン演出フラグの setter
	void SetIsSceneProduction(const bool _set) { m_isSceneProduction = _set; }

	//	クリア演出フラグの setter
	void SetIsClearProduction(const bool _set) { m_isClearProduction = _set; }

	//	アイテム取得時のエフェクトの開始フラグの setter
	void SetIsStartEffectGetItem(const bool _set) { m_isStartEffectGetItem = _set; }

protected:

//================== 内部処理関数 ==================//

    //  sinカーブによる向きを求める
    void _AskSinCurveDirection();

	//	最終的な解放処理
	virtual void _FinalRelease() = 0;

//=================== メンバ変数 ===================//

	float	m_emergeMoveTaim;				//	出現時の移動時間
	bool	m_isEmerge;						//	出現フラグ
	float	m_speed;						//	速度
	int		m_life;							//	残機
	bool	m_isDamage;						//	ダメージフラグ
	bool	m_offDraw;						//	描画しないフラグ
	float	m_invincibleTime;				//	当たり判定の無効時間
	bool	m_isInvincible;					//	無敵フラグ
	bool	m_isInvincibleAfterLaser;		//	レーザー使用後の無敵フラグ
	bool	m_isMove;						//	移動中かのフラグ
	bool	m_isStopMove;					//	移動停止フラグ
	bool	m_isSceneProduction;			//	シーン演出フラグ
	bool	m_isClearProduction;			//	クリア演出フラグ
	Rect	m_laserHitRect;					//	レーザーの当たり判定
	float	m_laserTime;					//	レーザーの出現時間
	bool	m_isStartLaser;					//	レーザー開始フラグ
	float	m_stayShotWaitTime;				//	滞在弾の遅延時間
	float	m_radian;						//	ラジアン
	bool	m_isUseStayShot;				//	滞在ショットの使用状況
	Rect	m_itemHitRect;					//	アイテム用の当たり判定
	bool	m_isStartEffectGetItem;			//	アイテム取得時のエフェクトの開始フラグ

	//	デモ用
	float	m_demoDirChangeTime;			//	デモ用の向き切り替え遅延時間
	VECTOR	m_demoTargetPos;				//	デモ用の目的の位置

	//	エフェクト関係
	EffekseerEmitter*	m_effectLaser;		//	レーザーエフェクト
	EffekseerEmitter*	m_effectHit;		//	ヒットエフェクト
	EffekseerEmitter*	m_effectExplosion;	//	爆発エフェクト
	EffekseerEmitter*	m_effectSpeedUp;	//	速度アップエフェクト
	EffekseerEmitter*	m_effectGetItem;	//	アイテム取得エフェクト

//===================== 静的定数 ===================//
	
	static const VECTOR		TITLE_POS;							//	タイトル時の座標
	static const VECTOR		RESULT_POS;							//	リザルト時の座標
	static const VECTOR		EMERGE_POS;							//	出現時の座標
	static const VECTOR		START_POS;							//	開始時の座標
	static const VECTOR		RECT_CORRECTION;					//	座標の補正
	static const VECTOR		ITEM_RECT_CORRECTION;				//	アイテム座標の補正
	static const float		EMERGE_MOVE_SPEED;					//	出現時の移動速度
	static const float		SHOT_SPEED;							//	ショット速度
	static const float		MAX_SHOT_WAIT_TIME;					//	ショットの遅延時間
	static const float		MAX_STAY_SHOT_WAIT_TIME;			//	滞在ショットの遅延時間
	static const float		CENTER_CORRECTION;					//	中心座標の補正
	static const float		HIT_RADIUS;							//	当たり判定用の半径
	static const float		MAX_INVINCIBLE_TIME_1;				//	当たり判定の無効時間の最大
	static const float		MAX_INVINCIBLE_TIME_2;				//	当たり判定の無効時間の最大
	static const float		MOVE_SPEED;							//	移動速度
	static const float		MIN_POS_X;							//	ｘ座標の最小
	static const float		MAX_POS_X;							//	ｘ座標の最大
	static const float		MIN_POS_Y;							//	ｙ座標の最小
	static const float		MAX_POS_Y;							//	ｙ座標の最大
	static const float		LASER_SIZE;							//	レーザーサイズ
	static const float		LASER_SIZE_DEPTH;					//	レーザーサイズの奥行き
	static const float		LASER_SPEED;						//	レーザー速度
	static const float		ADD_RADIAN_SPEED;					//	ラジアン角の加算速度
	static const float		MAX_DEMO_SHOT_FIRING_TIME;			//	ショットの遅延時間
	static const float		MAX_DEMO_STAY_SHOT_FIRING_TIME;		//	滞在ショットの発射時間
	static const float		MAX_DEMO_CHANGE_DIR_WAIT_TIME;		//	デモ用の向き切り替え遅延時間
	static const float		MAX_DEMO_DISTANCE_TO_ENEMY;			//	デモ用の敵との距離の間隔
	static const int		START_LIFE;							//	開始時の残機
	static const int		ZERO_LIFE;							//	残機なし

private:

//=================== メンバ変数 ===================//

	float m_flashingTime;	//	点滅時間

};