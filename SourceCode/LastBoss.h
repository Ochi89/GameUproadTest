
//=============================================================================
//	@file	MediumBoss.h
//	@brief	最終ボス
//	@autor	Takuya Ochi
//	@date	2018/12/21
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
struct ColorF;
class ShotManager;
class EffekseerEmitter;
class SoundEffect;

//-----------------------------------------------------------------------------
//	@brief	最終ボスクラス
//-----------------------------------------------------------------------------
class LastBoss : public CharacterBase
{
public:

//===================== 列挙隊 =====================//

	//	パーツの種類
	enum BOSS_PARTS_KIND
	{
		BOSS_PARTS_KIND_BODY,				//	ボディー
		BOSS_PARTS_KIND_CORE,				//	コア
		BOSS_PARTS_KIND_SIDE_BARREL,		//	サイドバレット
		BOSS_PARTS_KIND_ARM,				//	アーム
	};

//===================== 関数 ======================//
	
	//	コンストラクタ / デストラクタ
	LastBoss(const int _bodyModelHandle, const int _sideBarrelModelHandle, const int _armModelHandle, const int _coreModelHandle);
	~LastBoss();

	//	作成処理
	void Create() override final;

	//	解放処理
	void Release() override final;

	//	初期処理
	void Initialize() override final;

	//	更新処理
	void Update(ShotManager& _shot, SoundEffect& _soundEffect);

	//	描画処理
	void Draw() override final;

	//	コアのダメージ判定
	void OnHitCoreDamage(const int _num);

	//	コアのダメージ判定
	void OnHitCoreDamage(const int _num, const int _damage);

	//	サイドバレルのダメージ判定
	void OnHitSideBarrelDamage(const int _num);

	//	サイドバレルのダメージ判定
	void OnHitSideBarrelDamage(const int _num, const int _damage);

	//	破壊状況
	void DestructionSituation();


//==================== getter =====================//

	//	ライフの getter
	const int& GetLife() const { return m_life; }

	//	生きているかの getter
	const bool GetIsAlive() const { return m_life > 0; }

	//	出現フラグの getter
	const bool& GetIsEmerge() const { return m_isEmerge; }

	//	登場フラグの getter
	const bool& GetIsAppearance() const { return m_isAppearance; }

	//	行動開始フラグの getter
	const bool& GetIsStarted() const { return m_isStarted; }

	//	当たり判定用の円形の　getter
	const Circle& GetCircle(const BOSS_PARTS_KIND _partsNum, const int _num = 0) const;

	//	当たり判定用のアームの長方形の　getter
	const Rect& GetArmRect(const int _num = 0) const;

	//	当たり判定用のコアの長方形の　getter
	const Rect& GetCoreRect(const int _num = 0) const;

	//	当たり判定用のサイドバレルの長方形の　getter
	const Rect& GetSideBarrelRect(const int _num = 0) const;

	//	当たり判定用のレーザーの長方形の　getter
	const Rect& GetLaserRect(const int _num = 0) const;

	//	当たり判定用のセンターバレルの長方形の　getter
	const Rect& GetCenterBarrelRect() { return m_centerBarrel; }

	//	右コアが生きているかの getter
	const bool& GetIsRightCoreAlive() const { return m_isRightCoreAlive; }

	//	左コアが生きているかの getter
	const bool& GetIsLeftCoreAlive() const { return m_isLeftCoreAlive; }

	//	右サイドバレルが生きているかの getter
	const bool& GetIsRightSideBarrelAlive() const { return m_isRightSideBarrelAlive; }

	//	左サイドバレルが生きているかの getter
	const bool& GetIsLeftSideBarrelAlive() const { return m_isLeftSideBarrelAlive; }

	//	すべて破壊したかの getter
	const bool& GetIsAllDestruction() const { return m_isAllDestruction; }

	//	コアのダメージ判定の getter
	const bool& GetIsCoreDamage(const int _num) { return m_bossPartsCore[_num].m_isDamage; }

	//	サイドバレルのダメージ判定の getter
	const bool& GetIsSideBarrelDamage(const int _num) { return m_bossPartsSideBarrel[_num].m_isDamage; }

	//	ボス死亡フラグの getter
	const bool GetIsDeath() { return (!m_isLeftCoreAlive && !m_isRightCoreAlive); }

//==================== setter =====================//

	//	出現フラグの setter
	void SetIsEmerge(const bool _set) { m_isEmerge = _set; m_isStart = _set; }

	//	ライフの setter
	void SetLife(const int _set) { m_life = _set; };

private:

//===================== 列挙隊 =====================//

	//	パーツの情報
	enum BOSS_PARTS_INFO
	{
		BOSS_PARTS_RIGHT,
		BOSS_PARTS_LEFT,
		BOSS_PARTS_NUM,
	};

	//	ボスの行動種類
	enum BEHAVIOR_KIND
	{
		BEHAVIOR_NONE,		//	なし
		BEHAVIOR_SHOT,		//	ショット
		BEHAVIOR_ROCKET,	//	ロケット
		BEHAVIOR_LASER		//	レーザー
	};

	//	爆発エフェクトの種類
	enum EXPLOSION_EFFECT_KIND
	{
		EXPLOSION_EFFECT_LEFT_BARREL,		//	左のサイドバレル
		EXPLOSION_EFFECT_RIGHT_BARREL,		//	右のサイドバレル
		EXPLOSION_EFFECT_LEFT_CORE,			//	左のコア
		EXPLOSION_EFFECT_RIGHT_CORE,		//	右のコア
		EXPLOSION_EFFECT_LAST,				//	最後
		EXPLOSION_EFFECT_NUM,				//	数
	};

//===================== 構造体 =====================//

	//	パーツの構造体
	struct BossParts
	{
		int			m_modelHandle;		//	モデルハンドル
		VECTOR		m_pos;				//	ポジション
		VECTOR		m_dir;				//	ディレクション
		VECTOR		m_angle;			//	アングル
		VECTOR		m_size;				//	サイズ
		int			m_life;				//	ライフ
		bool		m_isDamage;			//	ダメージフラグ
		Circle		m_hitCircle;		//	当たり判定用の円の構造体
		Rect		m_hitRect;			//	当たり判定用の長方形の構造体
	};

//================== 内部処理関数 ==================//

	//	パーツの初期化
	void _InitializeBossParts(BossParts& _bossParts, const BOSS_PARTS_INFO _bossPartsInfo, const float _hitRadius = 0.0f, const int _liffe = 1);

	//	開始時の移動
	void _EmergeMove();

	//	移動処理
	void _Move();

	//	射撃処理
	void _Shot(ShotManager& _shot);
	
	//	ロケット処理
	void _Rocket();

	//	レーザー
	void _Laser();

	//	パーツの状況
	void _PartsSituation();

	//	アームの回転
	void _ArmRota(BossParts& _bossParts, const float _targetAngle);

	//	ラープを使った相対座標
	void _LerpRelativeCoordinates(BossParts& _bossParts, const BOSS_PARTS_INFO _bossInfo, const VECTOR _target);

	//	線形保管を使用した移動
	void _LerpMove(bool _isConditions, VECTOR _targetPos, float _lerpSpeed);

	//	ヒットエフェクト
	void _OnHitEffect(BossParts& _bossParts, SoundEffect& _soundEffect, const VECTOR _pos);

	//	爆発エフェクト
	void _OnEffectExplosion(SoundEffect& _soundEffect);

	//	爆発エフェクト一つ分
	void _OneEffectExplosion(const  bool _isConditions, EffekseerEmitter& _effect, SoundEffect& _soundEffect, const VECTOR _pos, bool& _isUsedFlag);

	//	最終爆発エフェクト
	void _LastEffectExplosion(const  bool _isConditions, SoundEffect& _soundEffect);

	//	ランダム爆発エフェクト
	void _RandamEffectExplosion(EffekseerEmitter& _effect, SoundEffect& _soundEffect);

	//	レーザーエフェクト
	void _OnEffectLaser(EffekseerEmitter& _effect, SoundEffect& _soundEffect, const VECTOR _startPos, const BOSS_PARTS_INFO _partsInfo);

	//	ロケットエフェクト
	void _OnEffectRocket(EffekseerEmitter& _effect, SoundEffect& _soundEffect, const VECTOR _startPos, const BOSS_PARTS_INFO _partsInfo);

	//	当たり判定用の更新
	void _UpdateHitJudgment();

	//	アームの当たり判定用の更新
	void _UpdateHitArm();

	//	コアの当たり判定用の更新
	void _UpdateHitCore();

	//	サイドバレルの当たり判定用の更新
	void _UpdateHitSideBarrel();

	//	レーザーの当たり判定用の更新
	void _UpdateHitLaser();

	//	ボディーの当たり判定用の更新
	void _UpdateHitBody();

	//	センターバレルの当たり判定用の更新
	void _UpdateHitCenterBarrel();

	//	相対座標を割り当てる
	void _AllocationRelativeCoordinates();

	//	相対座標を求める
	VECTOR _AskRelativeCoordinates(const VECTOR _basePos, const VECTOR _relativePos, const BOSS_PARTS_INFO _bossPartsInfo);

	//	モデルの色を変更する
	void _ChangeModelColor(const int _modelHandle, const int _life, const int _maxLife, const ColorF _normalColor);

	//	点滅描画処理
	void _FlashingDraw(BossParts& _bossParts);

	//	最終的な解放処理
	void _FinalRelease() override final;

//=================== メンバ変数 ===================//

	int				m_life;																	//	ライフ
	float			m_emergeMoveTaim;														//	演出時間
	float			m_behaviorTime;															//	行動時間
	float			m_rocketTime;															//	ロケット時間
	float			m_laserTime;															//	レーザー時間
	float			m_moveSpeed;															//	移動速度
	bool			m_isEmerge;																//	出現フラグ
	bool			m_isAppearance;															//	登場フラグ
	bool			m_isStart;																//	開始フラグ
	bool			m_isStarted;															//	行動開始フラグ
	bool			m_isRightCoreAlive;														//	右コアが生きている
	bool			m_isLeftCoreAlive;														//	左コアが生きている
	bool			m_isRightSideBarrelAlive;												//	右サイドバレルが生きている
	bool			m_isLeftSideBarrelAlive;												//	左サイドバレルが生きている
	bool			m_isAllDestruction;														//	すべて破壊した
	float			m_flashingTime;															//	点滅時間
	bool			m_isUsedEffectExplosion[EXPLOSION_EFFECT_KIND::EXPLOSION_EFFECT_NUM];	//	爆発エフェクトの使用状況
	bool			m_isOffDraw;															//	描画オフ
	bool			m_isStartRocket;														//	ロケットの開始
	bool			m_isStartLaser;															//	レーザーの開始
	Circle			m_hitCircle;															//	当たり判定用の円の構造体
	Rect			m_laser;																//	レーザー
	Rect			m_centerBarrel;															//	センターバレル
	Rect			m_laserHitRect[BOSS_PARTS_INFO::BOSS_PARTS_NUM];						//	レーザーの当たり判定
	Rect			m_coreHitRect[BOSS_PARTS_INFO::BOSS_PARTS_NUM];							//	コアの当たり判定
	Rect			m_sideBarrelHitRect[BOSS_PARTS_INFO::BOSS_PARTS_NUM];					//	サイドバレルの当たり判定
	BEHAVIOR_KIND	m_behaviorKind;															//	ボスの行動種類

	BossParts		m_bossPartsCore[BOSS_PARTS_INFO::BOSS_PARTS_NUM];						//	コア
	BossParts		m_bossPartsSideBarrel[BOSS_PARTS_INFO::BOSS_PARTS_NUM];					//	サイドバレル
	BossParts		m_bossPartsArm[BOSS_PARTS_INFO::BOSS_PARTS_NUM];						//	アーム

	ColorF			m_lifeNormalColorFYellow;												//	ライフデフォルト色 黄色
	ColorF			m_lifeNormalColorFBlack;												//	ライフデフォルト色 黒
	ColorF			m_lifeMiddleColorF;														//	ライフ中間色
	ColorF			m_lifeDangerColorF;														//	ライフ危険色

	//	エフェクト関係
	EffekseerEmitter*	m_effectHit;												//	ヒットエフェクト
	EffekseerEmitter*	m_effectExplosion[CommonConstant::MAX_BOSS_EXPLOSION_NUM];	//	爆発エフェクト
	EffekseerEmitter*	m_effectLastExplosion;										//	最終爆発エフェクト
	EffekseerEmitter*	m_effectLaser[BOSS_PARTS_INFO::BOSS_PARTS_NUM];				//	レーザーエフェクト
	EffekseerEmitter*	m_effectInjection[BOSS_PARTS_INFO::BOSS_PARTS_NUM];			//	噴射エフェクト

//===================== 静的定数 ===================//

	static const VECTOR		EMERGE_POS;						//	出現時の座標
	static const VECTOR		EMERGE_RIGHT_ARM_POS;			//	出現時の右アームの座標
	static const VECTOR		EMERGE_LEFT_ARM_POS;			//	出現時の左アームの座標
	static const VECTOR		RELATIVE_CORE_POS;				//	コアの相対座標
	static const VECTOR		RELATIVE_SIDE_BARREL_POS;		//	サイドバレルの相対座標
	static const VECTOR		RELATIVE_ARM_POS;				//	アームの相対座用
	static const VECTOR		OPEN_RELATIVE_CORE_POS;			//	コアの相対座標
	static const VECTOR		OPEN_RELATIVE_SIDE_BARREL_POS;	//	サイドバレルの相対座標
	static const VECTOR		OPEN_RELATIVE_ARM_POS;			//	アームの相対座用
	static const VECTOR		START_POS;						//	開始時の座標

	static const VECTOR		MOVE_PATTERN_1;					//	行動パターン１
	static const VECTOR		MOVE_PATTERN_2;					//	行動パターン２
	static const VECTOR		MOVE_PATTERN_3;					//	行動パターン３
	static const VECTOR		MOVE_PATTERN_4;					//	行動パターン４
	static const VECTOR		MOVE_PATTERN_5;					//	行動パターン５
	static const VECTOR		MOVE_PATTERN_6;					//	行動パターン６

	static const VECTOR		SHOT_START_POS_CORRECTION_1;	//	ショット開始位置座標の補正
	static const VECTOR		SHOT_START_POS_CORRECTION_2;	//	ショット開始位置座標の補正
	static const VECTOR		SHOT_START_POS_CORRECTION_3;	//	ショット開始位置座標の補正
	static const float		SHOT_SPEED;						//	ショット速度
	static const float		MAX_START_WAIT_TIME;			//	ショットの開始時の遅延時間
	static const float		MAX_WAIT_TIME;					//	ショットの遅延時間
	static const float		LASER_LENGTH;					//	レーザーの長さ

	static const VECTOR		ADJUSTMENT_BODY_POS;			//	当たり判定調整用のポイント ボディー
	static const VECTOR		ADJUSTMENT_SIDE_BARREL_POS;		//	当たり判定調整用のポイント サイドバレル
	static const VECTOR		ADJUSTMENT_ARM_CIRCLE_POS;		//	当たり判定調整用のポイント アーム
	static const VECTOR		ADJUSTMENT_ARM_RECT_POS;		//	当たり判定調整用のポイント アーム
	static const VECTOR		ADJUSTMENT_LASER_POS;			//	当たり判定調整用のポイント レーザー
	static const VECTOR		ADJUSTMENT_CENTER_BARRE_POS;	//	当たり判定調整用のポイント センターバレル
	static const VECTOR		ARM_CORRECTION;					//	当たり判定用の補正 アーム
	static const VECTOR		CENTER_BARRE_CORRECTION;		//	当たり判定用の補正 センターバレル
	static const float		LASER_EFFECT_CORRECTION;		//	エフェクトの補正 レーザー
	static const float		ROCKET_EFFECT_CORRECTION;		//	エフェクトの補正 ロケット
	static const float		HIT_BODY_RADIUS;				//	当たり判定用の半径 ボディー
	static const float		HIT_SIDE_BARREL_RADIUS;			//	当たり判定用の半径 サイドバレル
	static const float		HIT_CORE_RADIUS;				//	当たり判定用の半径 コア
	static const float		HIT_ARM_RADIUS;					//	当たり判定用の半径 アーム
	static const int		MAX_SIDE_BARREL_LIFE;			//	ライフの最大 サイドバレル
	static const int		MAX_CORE_LIFE;					//	ライフの最大 コア
	static const VECTOR		BOSS_RADIUS;					//	ボスの半径
	static const int		LIFE_ZERO;						//	ライフなし
};
