
//=============================================================================
//	@file	MediumBoss.h
//	@brief	中ボス
//	@autor	Takuya Ochi
//	@date	2018/12/18
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
class ShotManager;
class EffekseerEmitter;
class SoundEffect;

//-----------------------------------------------------------------------------
//	@brief	中ボスクラス
//-----------------------------------------------------------------------------
class MediumBoss : public CharacterBase
{
public:

//===================== 関数 ======================//

	//	コンストラクタ / デストラクタ
	MediumBoss(const int _modelHandle);
	~MediumBoss();

	//	作成処理
	void Create();

	//	解放処理
	void Release();

	//	初期処理
	void Initialize();

	//	更新処理
	void Update(ShotManager& _shot, SoundEffect& _soundEffect);

	//	描画処理
	virtual void Draw() override final;

	//	ダメージ判定
	void OnHitDamage()
	{
		if (!m_isDamage)
		{
			m_life--;
			m_isDamage = true;
		}
	}

	//	ダメージ判定
	void OnHitDamage(const int _damage)
	{
		if (!m_isDamage)
		{
			m_life -= _damage;
			m_isDamage = true;
		}
	}

//==================== getter =====================//

	//	ライフの getter
	const int& GetLife() const { return m_life; }

	//	生きているかの getter
	const bool GetIsAlive() const { return m_life > 0; }

	//	ダメージ判定の getter
	const bool& GetIsDamage() const { return m_isDamage; }

	//	出現フラグの getter
	const bool& GetIsEmerge() const { return m_isEmerge; }

	//	行動開始フラグの getter
	const bool& GetIsStarted() const { return m_isStarted; }

	//	削除フラグの getter
	const bool& GetIsDeleate() const { return m_isDeleate; }

//==================== setter =====================//

	//	出現フラグの setter
	void SetIsEmerge(const bool _set) { m_isEmerge = _set; m_isStart = _set; }

	//	ライフの setter
	void SetLife(const int _set) { m_life = _set; };

private:

//================== 内部処理関数 ==================//

	//	開始時の移動
	void _EmergeMove();

	//	移動処理
	void _Move();

	//	線形保管を使用した移動
	void _LerpMove(bool _isConditions, VECTOR _targetPos, float _lerpSpeed);

	//	射撃処理
	void _Shot(ShotManager& _shot);

	//	ヒットエフェクト
	void _OnHitEffect(SoundEffect& _soundEffect);

	//	爆発エフェクト
	void _OnEffectExplosion(SoundEffect& _soundEffect);

	//	当たり判定用の点の更新
	void _UpdateHitPoint();

	//	最終的な解放処理
	void _FinalRelease();

//=================== メンバ変数 ===================//

	int		m_life;				//	ライフ
	float	m_emergeMoveTaim;	//	演出時間
	float	m_behaviorTime;		//	行動時間
	float	m_moveSpeed;		//	移動速度
	bool	m_isEmerge;			//	出現フラグ
	bool	m_isStart;			//	開始フラグ
	bool	m_isDamage;			//	ダメージフラグ
	bool	m_isDeleate;		//	削除フラグ
	bool	m_isStarted;		//	行動開始フラグ
	bool	m_isOffDraw;		//	描画オフフラグ
	float	m_flashingTime;		//	点滅時間

	//	エフェクト関係
	EffekseerEmitter*	m_effectHit;			//	ヒットエフェクト
	EffekseerEmitter*	m_effectExplosion;		//	爆発エフェクト

//===================== 静的定数 ===================//

	static const VECTOR		EMERGE_POS;						//	出現時の座標
	static const VECTOR		END_POS;						//	退却時の座標
	static const VECTOR		START_POS;						//	開始時の座標
	static const VECTOR		MOVE_PATTERN_1;					//	行動パターン１
	static const VECTOR		MOVE_PATTERN_2;					//	行動パターン２
	static const VECTOR		MOVE_PATTERN_3;					//	行動パターン３
	static const VECTOR		MOVE_PATTERN_4;					//	行動パターン４
	static const VECTOR		SHOT_START_POS_CORRECTION_1;	//	ショット開始位置座標の補正
	static const VECTOR		SHOT_START_POS_CORRECTION_2;	//	ショット開始位置座標の補正
	static const VECTOR		SHOT_START_POS_CORRECTION_3;	//	ショット開始位置座標の補正
	static const VECTOR		SHOT_START_POS_CORRECTION_4;	//	ショット開始位置座標の補正
	static const VECTOR		RECT_CORRECTION;				//	座標の補正
	static const float		SHOT_SPEED;						//	ショット速度
	static const float		MAX_START_WAIT_TIME;			//	ショットの開始時の遅延時間
	static const float		MAX_WAIT_TIME;					//	ショットの遅延時間
	static const float		HIT_RADIUS;						//	当たり判定用の半径
	static const int		MAX_LIFE;						//	ライフの最大
	static const int		MIN_LIFE;						//	ライフの最小

};
