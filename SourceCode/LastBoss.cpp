
//=============================================================================
//	@file	LastBoss.cpp
//	@brief	最終ボス
//	@autor	Takuya Ochi
//	@date	2018/12/21
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "LastBoss.h"
#include "Common.h"
#include "MoveHelper.h"
#include "AngleHelper.h"
#include "PlayerManager.h"
#include "ShotManager.h"
#include "EffekseerEmitter.h"
#include "SoundEffect.h"
#include "Pad.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const VECTOR	LastBoss::EMERGE_POS = VGet(206.0f, 50.0f, 0.0f);						//	出現時の座標
const VECTOR	LastBoss::EMERGE_RIGHT_ARM_POS = VGet(60.0f, 150.0f, 0.0f);				//	出現時の右アームの座標
const VECTOR	LastBoss::EMERGE_LEFT_ARM_POS = VGet(60.0f, -150.0f, 0.0f);				//	出現時の左アームの座標
const VECTOR	LastBoss::RELATIVE_CORE_POS = VGet(-9.5f, 5.0f, 0.0f);					//	コアの相対座標
const VECTOR	LastBoss::RELATIVE_SIDE_BARREL_POS = VGet(-10.0f, 8.0f, 0.0f);			//	サイドバレルの相対座標
const VECTOR	LastBoss::RELATIVE_ARM_POS = VGet(0.9f, 28.0f, 0.0f);					//	アームの相対座用
const VECTOR	LastBoss::OPEN_RELATIVE_CORE_POS = VGet(-9.5f, 8.0f, 0.0f);				//	コアの相対座標
const VECTOR	LastBoss::OPEN_RELATIVE_SIDE_BARREL_POS = VGet(-10.0f, 20.0f, 0.0f);	//	サイドバレルの相対座標
const VECTOR	LastBoss::OPEN_RELATIVE_ARM_POS = VGet(0.9f, 35.0f, 0.0f);				//	アームの相対座用
const VECTOR	LastBoss::START_POS = VGet(60.0f, 50.0f, 0.0f);							//	開始時の座標
const VECTOR	LastBoss::MOVE_PATTERN_1 = VGet(60.0f, 90.0f, 0.0f);					//	行動パターン１
const VECTOR	LastBoss::MOVE_PATTERN_2 = VGet(60.0f, 55.0f, 0.0f);					//	行動パターン２
const VECTOR	LastBoss::MOVE_PATTERN_3 = VGet(60.0f, 20.0f, 0.0f);					//	行動パターン３
const VECTOR	LastBoss::MOVE_PATTERN_4 = VGet(40.0f, 55.0f, 0.0f);					//	行動パターン４
const VECTOR	LastBoss::MOVE_PATTERN_5 = VGet(40.0f, 20.0f, 0.0f);					//	行動パターン５
const VECTOR	LastBoss::MOVE_PATTERN_6 = VGet(40.0f, 90.0f, 0.0f);					//	行動パターン６
const VECTOR	LastBoss::SHOT_START_POS_CORRECTION_1 = VGet(14.0f, 12.0f, 0.0f);		//	ショット開始位置座標の補正
const VECTOR	LastBoss::SHOT_START_POS_CORRECTION_2 = VGet(21.5f, 6.5f, 0.0f);		//	ショット開始位置座標の補正
const VECTOR	LastBoss::SHOT_START_POS_CORRECTION_3 = VGet(35.0f, 1.0f, 0.0f);		//	ショット開始位置座標の補正
const float		LastBoss::SHOT_SPEED = 1.5f;											//	ショットの速度
const float		LastBoss::MAX_START_WAIT_TIME = 35.0f;									//	ショットの開始時の遅延時間
const float		LastBoss::MAX_WAIT_TIME = 100.0f;										//	ショットの遅延時間
const float		LastBoss::LASER_LENGTH = -200.0f;										//	レーザーの長さ
const VECTOR	LastBoss::ADJUSTMENT_BODY_POS = VGet(15.0f, 0.0f, 0.0f);				//	当たり判定調整用のポジション ボディー
const VECTOR	LastBoss::ADJUSTMENT_SIDE_BARREL_POS = VGet(-10.0f, 1.0f, 0.0f);		//	当たり判定調整用のポジション サイドバレル
const VECTOR	LastBoss::ADJUSTMENT_ARM_CIRCLE_POS = VGet(-15.0f, 2.0f, 0.0f);			//	当たり判定調整用のポジション アーム
const VECTOR	LastBoss::ADJUSTMENT_ARM_RECT_POS = VGet(18.0f, 8.0f, 5.0f);			//	当たり判定調整用のポジション アーム
const VECTOR	LastBoss::ADJUSTMENT_LASER_POS = VGet(-25.0f, 4.0f, 5.0f);				//	当たり判定調整用のポイント レーザー
const VECTOR	LastBoss::ADJUSTMENT_CENTER_BARRE_POS = VGet(30.0f, 2.0f, 5.0f);		//	当たり判定調整用のポイント センターバレル
const VECTOR	LastBoss::ARM_CORRECTION = VGet(10.0f, 3.0f, 0.0f);						//	当たり判定用の補正 アーム
const VECTOR	LastBoss::CENTER_BARRE_CORRECTION = VGet(10.0f, 0.0f, 0.0f);			//	当たり判定用の補正 センターバレル
const float		LastBoss::LASER_EFFECT_CORRECTION = -28.0f;								//	エフェクトの補正 レーザー
const float		LastBoss::ROCKET_EFFECT_CORRECTION = 25.0f;								//	エフェクトの補正 ロケット
const float		LastBoss::HIT_BODY_RADIUS = 15.0f;										//	当たり判定用の半径 ボディー
const float		LastBoss::HIT_SIDE_BARREL_RADIUS = 7.0f;								//	当たり判定用の半径 サイドバレル
const float		LastBoss::HIT_CORE_RADIUS = 3.0f;										//	当たり判定用の半径 コア
const float		LastBoss::HIT_ARM_RADIUS = 10.0f;										//	当たり判定用の半径 アーム
const int		LastBoss::MAX_SIDE_BARREL_LIFE = 40;									//	ライフの最大 サイドバレル
const int		LastBoss::MAX_CORE_LIFE = 30;											//	ライフの最大 コア
const VECTOR	LastBoss::BOSS_RADIUS = VGet(20.0f, 15.0f, 0.0f);						//	ボスの半径
const int		LastBoss::LIFE_ZERO = 0;												//	ライフなし

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
LastBoss::LastBoss(const int _bodyModelHandle, const int _sideBarrelModelHandle, const int _armModelHandle, const int _coreModelHandle)
{
	//	各変数を初期化
	m_pos = CommonConstant::ORIGIN;
	m_dir = CommonConstant::ORIGIN;
	m_angle = CommonConstant::ORIGIN;

	//	モデルの複製
	m_modelHandle = MV1DuplicateModel(_bodyModelHandle);
	CommonDebug::Assert((m_modelHandle <= -1), " [ LastBoss.cpp ] : error : model loading failed.");

	for (int i = 0; i < BOSS_PARTS_INFO::BOSS_PARTS_NUM; i++)
	{
		m_bossPartsSideBarrel[i].m_modelHandle = MV1DuplicateModel(_sideBarrelModelHandle);
		m_bossPartsArm[i].m_modelHandle = MV1DuplicateModel(_armModelHandle);
		m_bossPartsCore[i].m_modelHandle = MV1DuplicateModel(_coreModelHandle);
		CommonDebug::Assert((m_bossPartsSideBarrel[i].m_modelHandle <= -1), " [ LastBoss.cpp ] : error : model loading failed.");
		CommonDebug::Assert((m_bossPartsArm[i].m_modelHandle <= -1), " [ LastBoss.cpp ] : error : model loading failed.");
		CommonDebug::Assert((m_bossPartsCore[i].m_modelHandle <= -1), " [ LastBoss.cpp ] : error : model loading failed.");

		//	まだどこも指していないので、NULLで初期化
		m_effectLaser[i] = NULL;
		m_effectInjection[i] = NULL;
	}

	//	まだどこも指していないので、NULLで初期化
	m_effectHit = NULL;

	for (int i = 0; i < CommonConstant::MAX_BOSS_EXPLOSION_NUM; i++)
	{
		m_effectExplosion[i] = NULL;
	}

	m_effectLastExplosion = NULL;
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
LastBoss::~LastBoss()
{
	//	最終的な解放処理
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成処理
//-----------------------------------------------------------------------------
void LastBoss::Create()
{
	//	エフェクトの読み込み
	m_effectHit = new EffekseerEmitter("Data/Effect/Hit/Hit.efk");
	m_effectHit->OnEndEffect();

	for (int i = 0; i < BOSS_PARTS_INFO::BOSS_PARTS_NUM; i++)
	{
		m_effectInjection[i] = new EffekseerEmitter("Data/Effect/Injection/Injection.efk");
		m_effectLaser[i] = new EffekseerEmitter("Data/Effect/Boss/Laser/Laser.efk");
		m_effectInjection[i]->OnEndEffect();
		m_effectLaser[i]->OnEndEffect();
	}

	for (int i = 0; i < CommonConstant::MAX_BOSS_EXPLOSION_NUM; i++)
	{
		m_effectExplosion[i] = new EffekseerEmitter("Data/Effect/Explosion/Explosion.efk");
		m_effectExplosion[i]->OnEndEffect();
	}

	m_effectLastExplosion = new EffekseerEmitter("Data/Effect/Explosion/Explosion.efk");
	m_effectLastExplosion->OnEndEffect();
}

//-----------------------------------------------------------------------------
//	@brief	解放処理
//-----------------------------------------------------------------------------
void LastBoss::Release()
{
	//	モデルのアンロード
	MV1DeleteModel(m_modelHandle);

	for (int i = 0; i < BOSS_PARTS_INFO::BOSS_PARTS_NUM; i++)
	{
		MV1DeleteModel(m_bossPartsSideBarrel[i].m_modelHandle);
		MV1DeleteModel(m_bossPartsArm[i].m_modelHandle);
		MV1DeleteModel(m_bossPartsCore[i].m_modelHandle);
	}

	//	エフェクトの削除
	CommonSafe::Delete(m_effectHit);

	for (int i = 0; i < BOSS_PARTS_INFO::BOSS_PARTS_NUM; i++)
	{
		CommonSafe::Delete(m_effectInjection[i]);
		CommonSafe::Delete(m_effectLaser[i]);
	}

	for (int i = 0; i < CommonConstant::MAX_BOSS_EXPLOSION_NUM; i++)
	{
		CommonSafe::Delete(m_effectExplosion[i]);
	}

	CommonSafe::Delete(m_effectLastExplosion);
}

//-----------------------------------------------------------------------------
//	@brief	初期処理
//-----------------------------------------------------------------------------
void LastBoss::Initialize()
{
	//	各変数を初期状態に設定
	m_pos = EMERGE_POS;
	m_dir = CommonConstant::ORIGIN;
	m_angle = CommonConstant::ORIGIN;
	m_emergeMoveTaim = 0.0f;
	m_behaviorTime = 0.0f;
	m_rocketTime = 0.0f;
	m_laserTime = 0.0f;
	m_moveSpeed = 1.0f;
	m_shotWaitTime = MAX_START_WAIT_TIME;
	m_isEmerge = false;
	m_isStarted = false;
	m_isRightCoreAlive = true;
	m_isLeftCoreAlive = true;
	m_isRightSideBarrelAlive = true;
	m_isLeftSideBarrelAlive = true;
	m_isStartRocket = false;
	m_isStartLaser = false;
	m_life = 1;
	m_behaviorKind = BEHAVIOR_KIND::BEHAVIOR_NONE;
	m_laser.m_vertexTop = CommonConstant::ORIGIN;
	m_laser.m_vertexUnder = CommonConstant::ORIGIN;
	m_isOffDraw = false;
	m_isAllDestruction = false;

	//	ライフに応じたモデルの色
	m_lifeNormalColorFYellow = CommonFunction::GetColor(0.0f, 0.0f, 0.8f, 1.0f);	//	青色
	m_lifeNormalColorFBlack = CommonFunction::GetColor(0.4f, 0.4f, 0.4f, 1.0f);		//	グレー
	m_lifeMiddleColorF = CommonFunction::GetColor(1.0f, 1.0f, 0.0f, 1.0f);			//	黄色
	m_lifeDangerColorF = CommonFunction::GetColor(1.0f, 0.0f, 0.8f, 1.0f);			//	赤

	//	色の割り当て
	MV1SetDifColorScale(m_modelHandle, m_lifeNormalColorFBlack.m_color);

	for (int i = 0; i < EXPLOSION_EFFECT_KIND::EXPLOSION_EFFECT_NUM; i++)
	{
		m_isUsedEffectExplosion[i] = false;
	}

	//	パーツの初期化
	for (int i = 0; i < BOSS_PARTS_INFO::BOSS_PARTS_NUM; i++)
	{
		//	コアの初期化
		_InitializeBossParts(m_bossPartsCore[i], (BOSS_PARTS_INFO)i, HIT_CORE_RADIUS, MAX_CORE_LIFE);

		//	サイドバレルの初期化
		_InitializeBossParts(m_bossPartsSideBarrel[i], (BOSS_PARTS_INFO)i, HIT_SIDE_BARREL_RADIUS, MAX_SIDE_BARREL_LIFE );

		//	アームの初期化
		_InitializeBossParts(m_bossPartsArm[i], (BOSS_PARTS_INFO)i, HIT_ARM_RADIUS);
		
		//	色の割り当て
		MV1SetDifColorScale(m_bossPartsArm[i].m_modelHandle, m_lifeNormalColorFBlack.m_color);

		//	レーザーの当たり判定の初期化
		m_laserHitRect[i].m_vertexTop = CommonConstant::ORIGIN;
		m_laserHitRect[i].m_vertexUnder = CommonConstant::ORIGIN;

		//	コアの当たり判定の初期化
		m_coreHitRect[i].m_vertexTop = CommonConstant::ORIGIN;
		m_coreHitRect[i].m_vertexUnder = CommonConstant::ORIGIN;

		//	サイドバレルの当たり判定の初期化
		m_sideBarrelHitRect[i].m_vertexTop = CommonConstant::ORIGIN;
		m_sideBarrelHitRect[i].m_vertexUnder = CommonConstant::ORIGIN;
	}

	//	角度を割り当てる
	MV1SetRotationXYZ(m_modelHandle, m_angle);
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void LastBoss::Update(ShotManager& _shot, SoundEffect& _soundEffect)
{
	//	必殺技のときは更新しない
	const bool isNotSpecialProduction = !PRODUCTION->GetIsSpecialProduction();
	if (isNotSpecialProduction)
	{
		//	出現時の演出
		_EmergeMove();

		//	開始していて、
		//	死んでいないとき
		const bool isActive = m_isStarted && (m_isLeftCoreAlive || m_isRightCoreAlive);
		if (isActive)
		{
			//	コアの状態
			_PartsSituation();

			//	相対座標の設定
			_AllocationRelativeCoordinates();

			const bool isNotSpecialProduction = !PRODUCTION->GetIsSpecialProduction();
			if (isNotSpecialProduction)
			{
				//	移動処理
				_Move();

				//	射撃処理
				_Shot(_shot);
			}

			//	ロケット処理
			_Rocket();

			//	レーザー処理
			_Laser();
		}

		//	モデルにポジションを割り当てる
		MV1SetPosition(m_modelHandle, m_pos);

		//	モデルに角度を割り当てる
		MV1SetRotationXYZ(m_modelHandle, m_angle);

		//	パーツのモデルにポジションと角度を割り当てる
		for (int i = 0; i < BOSS_PARTS_INFO::BOSS_PARTS_NUM; i++)
		{
			//	ポジションの割り当て
			MV1SetPosition(m_bossPartsCore[i].m_modelHandle, m_bossPartsCore[i].m_pos);					//	コア
			MV1SetPosition(m_bossPartsSideBarrel[i].m_modelHandle, m_bossPartsSideBarrel[i].m_pos);		//	サイドバレル
			MV1SetPosition(m_bossPartsArm[i].m_modelHandle, m_bossPartsArm[i].m_pos);					//	アーム

			//	角度の割り当て
			MV1SetRotationXYZ(m_bossPartsCore[i].m_modelHandle, m_bossPartsCore[i].m_angle);				//	コア
			MV1SetRotationXYZ(m_bossPartsSideBarrel[i].m_modelHandle, m_bossPartsSideBarrel[i].m_angle);	//	サイドバレル
			MV1SetRotationXYZ(m_bossPartsArm[i].m_modelHandle, m_bossPartsArm[i].m_angle);					//	アーム

			//	レーザーエフェクト
			_OnEffectLaser((*m_effectLaser[i]), _soundEffect, m_bossPartsArm[i].m_pos, (BOSS_PARTS_INFO)i);

			//	ロケットエフェクト
			_OnEffectRocket((*m_effectInjection[i]), _soundEffect, m_bossPartsArm[i].m_pos, (BOSS_PARTS_INFO)i);

			//	モデルの色変更
			_ChangeModelColor(m_bossPartsCore[i].m_modelHandle, m_bossPartsCore[i].m_life, MAX_CORE_LIFE, m_lifeNormalColorFYellow);
			_ChangeModelColor(m_bossPartsSideBarrel[i].m_modelHandle, m_bossPartsSideBarrel[i].m_life, MAX_SIDE_BARREL_LIFE, m_lifeNormalColorFBlack);
		}

		//	爆発エフェクト
		_OnEffectExplosion(_soundEffect);

		//	当たり判定の更新
		_UpdateHitJudgment();
	}

	//	パーツのモデルにポジションと角度を割り当てる
	for (int i = 0; i < BOSS_PARTS_INFO::BOSS_PARTS_NUM; i++)
	{
		//	ヒットエフェクト
		_OnHitEffect(m_bossPartsCore[i], _soundEffect, m_bossPartsCore[i].m_pos);
		_OnHitEffect(m_bossPartsSideBarrel[i], _soundEffect, m_bossPartsSideBarrel[i].m_pos);
	}
}

//-----------------------------------------------------------------------------
//	@brief	描画処理
//-----------------------------------------------------------------------------
void LastBoss::Draw()
{
	const bool isActive = m_isStart && !m_isOffDraw;
	if (isActive)
	{
		//	モデルの描画
		MV1DrawModel(m_modelHandle);

		//	アームの描画
		if (m_isLeftCoreAlive)
		{
			MV1DrawModel(m_bossPartsArm[BOSS_PARTS_INFO::BOSS_PARTS_LEFT].m_modelHandle);
		}
		if (m_isRightCoreAlive)
		{
			MV1DrawModel(m_bossPartsArm[BOSS_PARTS_INFO::BOSS_PARTS_RIGHT].m_modelHandle);
		}

		//	パーツのモデルの点滅描画
		if (m_isLeftCoreAlive) { _FlashingDraw(m_bossPartsCore[BOSS_PARTS_INFO::BOSS_PARTS_LEFT]); }
		if (m_isRightCoreAlive) { _FlashingDraw(m_bossPartsCore[BOSS_PARTS_INFO::BOSS_PARTS_RIGHT]); }
		if (m_isLeftSideBarrelAlive) { _FlashingDraw(m_bossPartsSideBarrel[BOSS_PARTS_INFO::BOSS_PARTS_LEFT]); }
		if (m_isRightSideBarrelAlive) { _FlashingDraw(m_bossPartsSideBarrel[BOSS_PARTS_INFO::BOSS_PARTS_RIGHT]); }
	}
}

//-----------------------------------------------------------------------------
//	@brief	コアのダメージ判定
//-----------------------------------------------------------------------------
void LastBoss::OnHitCoreDamage(const int _num)
{
	const bool isNotDamage = !m_bossPartsCore[_num].m_isDamage;
	if (isNotDamage)
	{
		m_bossPartsCore[_num].m_life--;
		m_bossPartsCore[_num].m_isDamage = true;
	}
}

//-----------------------------------------------------------------------------
//	@brief	サイドバレルのダメージ判定
//-----------------------------------------------------------------------------
void LastBoss::OnHitSideBarrelDamage(const int _num)
{
	const bool isNotDamage = !m_bossPartsSideBarrel[_num].m_isDamage;
	if (isNotDamage)
	{
		m_bossPartsSideBarrel[_num].m_life--;
		m_bossPartsSideBarrel[_num].m_isDamage = true;
	}
}

//-----------------------------------------------------------------------------
//	@brief	コアのダメージ判定
//-----------------------------------------------------------------------------
void LastBoss::OnHitCoreDamage(const int _num, const int _damage)
{
	const bool isNotDamage = !m_bossPartsCore[_num].m_isDamage;
	if (isNotDamage)
	{
		m_bossPartsCore[_num].m_life -= _damage;
		m_bossPartsCore[_num].m_isDamage = true;
	}
}

//-----------------------------------------------------------------------------
//	@brief	サイドバレルのダメージ判定
//-----------------------------------------------------------------------------
void LastBoss::OnHitSideBarrelDamage(const int _num, const int _damage)
{
	const bool isNotDamage = !m_bossPartsSideBarrel[_num].m_isDamage;
	if (isNotDamage)
	{
		m_bossPartsSideBarrel[_num].m_life -= _damage;
		m_bossPartsSideBarrel[_num].m_isDamage = true;
	}
}

//-----------------------------------------------------------------------------
//	@brief	破壊状況
//-----------------------------------------------------------------------------
void LastBoss::DestructionSituation()
{
	//	コアをすべて破壊した
	const bool isAllDestruction = !m_isLeftCoreAlive && !m_isRightCoreAlive;
	if (isAllDestruction) { m_isAllDestruction = true; m_isAppearance = false; }
}

//-----------------------------------------------------------------------------
//	@brief	パーツの当たり判定用の円形の getter
//-----------------------------------------------------------------------------
const Circle& LastBoss::GetCircle(const BOSS_PARTS_KIND _partsNum, const int _num) const
{
	//	当たり判定の取得
	switch (_partsNum)
	{
	case BOSS_PARTS_KIND::BOSS_PARTS_KIND_BODY:
		return m_hitCircle;
		break;

	case BOSS_PARTS_KIND::BOSS_PARTS_KIND_CORE:
		return m_bossPartsCore[_num].m_hitCircle;
		break;

	case BOSS_PARTS_KIND::BOSS_PARTS_KIND_SIDE_BARREL:
		return m_bossPartsSideBarrel[_num].m_hitCircle;
		break;

	case BOSS_PARTS_KIND::BOSS_PARTS_KIND_ARM:
		return m_bossPartsArm[_num].m_hitCircle;
		break;
	}

	return m_hitCircle;
}

//-----------------------------------------------------------------------------
//	@brief	当たり判定用のアームの長方形の　getter
//-----------------------------------------------------------------------------
const Rect& LastBoss::GetArmRect(const int _num) const
{
	return  m_bossPartsArm[_num].m_hitRect;
}

//-----------------------------------------------------------------------------
//	@brief	当たり判定用のコアの長方形の　getter
//-----------------------------------------------------------------------------
const Rect& LastBoss::GetCoreRect(const int _num) const
{
	return m_coreHitRect[_num];
}

//-----------------------------------------------------------------------------
//	@brief	当たり判定用のサイドバレルの長方形の　getter
//-----------------------------------------------------------------------------
const Rect& LastBoss::GetSideBarrelRect(const int _num) const
{
	return m_sideBarrelHitRect[_num];
}

//-----------------------------------------------------------------------------
//	@brief	当たり判定用のレーザーの長方形の　getter
//-----------------------------------------------------------------------------
const Rect& LastBoss::GetLaserRect(const int _num) const
{
	return m_laserHitRect[_num];
}

//-----------------------------------------------------------------------------
//	@brief	パーツの初期化
//-----------------------------------------------------------------------------
void LastBoss::_InitializeBossParts(BossParts& _bossParts, const BOSS_PARTS_INFO _bossPartsInfo, const float _hitRadius, const int _liffe)
{
	//	パーツの初期化
	_bossParts.m_pos = EMERGE_POS;											//	ポジション
	_bossParts.m_dir = CommonConstant::ORIGIN;								//	向き
	_bossParts.m_size = VGet(1.0f, 1.0f, 1.0f);								//	サイズ
	_bossParts.m_angle = CommonConstant::ORIGIN;							//	角度
	_bossParts.m_life = _liffe;												//	ライフ
	_bossParts.m_hitCircle.m_radius = _hitRadius;							//	当たり判定の半径
	_bossParts.m_hitCircle.m_centerPoint = CommonConstant::ORIGIN;			//	当たり判定の中心座標
	_bossParts.m_isDamage = false;

	//	パーツの位置が左なら角度を反転する
	const bool isLeft = _bossPartsInfo == BOSS_PARTS_INFO::BOSS_PARTS_LEFT;
	if (isLeft) { _bossParts.m_angle.x = CommonConstant::PI; }
}

//-----------------------------------------------------------------------------
//	@brief	開始時の移動
//-----------------------------------------------------------------------------
void LastBoss::_EmergeMove()
{
	//	出現時の演出
	if (m_isEmerge)
	{
		//	登場
		m_isAppearance = true;

		//	出現時の演出
		m_emergeMoveTaim++;
		const bool isEmergeMoveStart = m_emergeMoveTaim <= 5.0f;
		const bool isEmergeMove1 = m_emergeMoveTaim >= 5.0f && m_emergeMoveTaim <= 150.0f;
		const bool isEmergeMove2 = m_emergeMoveTaim >= 150.0f && m_emergeMoveTaim <= 210.0f;
		const bool isEmergeMove3 = m_emergeMoveTaim >= 210.0f && m_emergeMoveTaim <= 320.0f;
		const bool isEmergeMoveEnd = m_emergeMoveTaim == 320.0f;

		//	演出開始
		if (isEmergeMoveStart) 
		{
			m_pos = EMERGE_POS; 
			m_bossPartsArm[(int)BOSS_PARTS_INFO::BOSS_PARTS_RIGHT].m_pos = EMERGE_RIGHT_ARM_POS;	//	右アーム
			m_bossPartsArm[(int)BOSS_PARTS_INFO::BOSS_PARTS_LEFT].m_pos = EMERGE_LEFT_ARM_POS;		//	左アーム
		}

		//	本体の出現
		if (isEmergeMove1)
		{
			//	前進
			m_dir = VGet(-1.0f, 0.0f, 0.0f);
			VECTOR moving = MoveHelper::AskMoveAmount(m_dir, m_moveSpeed);
			m_pos = VAdd(m_pos, moving);
		}

		//	右アームの出現
		if (isEmergeMove2) 
		{
			const float lerpSpeed = 0.1f;
			VECTOR pos = m_bossPartsArm[(int)BOSS_PARTS_INFO::BOSS_PARTS_RIGHT].m_pos;
			VECTOR targetPos = _AskRelativeCoordinates(m_pos, RELATIVE_ARM_POS, BOSS_PARTS_INFO::BOSS_PARTS_RIGHT);
			pos = CommonFunction::Lerp(pos, targetPos, lerpSpeed);
			m_bossPartsArm[(int)BOSS_PARTS_INFO::BOSS_PARTS_RIGHT].m_pos = pos;
		}

		//	左アームの出現
		if (isEmergeMove3)
		{
			const float lerpSpeed = 0.1f;
			VECTOR pos = m_bossPartsArm[(int)BOSS_PARTS_INFO::BOSS_PARTS_LEFT].m_pos;
			VECTOR targetPos = _AskRelativeCoordinates(m_pos, RELATIVE_ARM_POS, BOSS_PARTS_INFO::BOSS_PARTS_LEFT);
			pos = CommonFunction::Lerp(pos, targetPos, lerpSpeed);
			m_bossPartsArm[(int)BOSS_PARTS_INFO::BOSS_PARTS_LEFT].m_pos = pos;
		}

		//	演出終了
		if (isEmergeMoveEnd)
		{
			m_pos = START_POS;
			m_angle = CommonConstant::ORIGIN;
			m_dir = CommonConstant::ORIGIN;

			for (int i = 0; i < BOSS_PARTS_INFO::BOSS_PARTS_NUM; i++)
			{
				//	アームの初期化
				m_bossPartsArm[i].m_pos = _AskRelativeCoordinates(m_pos, RELATIVE_ARM_POS, (BOSS_PARTS_INFO)i);
			}

			m_isStarted = true;
			m_isEmerge = false;
		}

		//	相対座標の設定
		for (int i = 0; i < BOSS_PARTS_INFO::BOSS_PARTS_NUM; i++)
		{
			m_bossPartsCore[i].m_pos = _AskRelativeCoordinates(m_pos, RELATIVE_CORE_POS, (BOSS_PARTS_INFO)i);					//	コア
			m_bossPartsSideBarrel[i].m_pos = _AskRelativeCoordinates(m_pos, RELATIVE_SIDE_BARREL_POS, (BOSS_PARTS_INFO)i);		//	サイドバレル
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	移動処理
//-----------------------------------------------------------------------------
void LastBoss::_Move()
{
	m_behaviorTime++;
	const bool isMovePattern1 = m_behaviorTime >= 0.0f && m_behaviorTime <= 100.0f;
	const bool isMovePattern2 = m_behaviorTime >= 200.0f && m_behaviorTime <= 300.0f;
	const bool isMovePattern3 = m_behaviorTime >= 400.0f && m_behaviorTime <= 500.0f;
	const bool isMovePattern4 = m_behaviorTime >= 600.0f && m_behaviorTime <= 700.0f;
	const bool isMovePattern5 = m_behaviorTime >= 800.0f && m_behaviorTime <= 900.0f;
	const bool isMovePattern6 = m_behaviorTime >= 1000.0f && m_behaviorTime <= 1400.0f;
	const bool isMovePattern7 = m_behaviorTime >= 1400.0f && m_behaviorTime <= 1500.0f;

	const bool isMovePattern8 = m_behaviorTime >= 1600.0f && m_behaviorTime <= 1700.0f;
	const bool isMovePattern9 = m_behaviorTime >= 1820.0f && m_behaviorTime <= 1950.0f;
	const bool isMovePattern10 = m_behaviorTime >= 1950.0f && m_behaviorTime <= 2100.0f;
	const bool isMovePattern11 = m_behaviorTime >= 2200.0f && m_behaviorTime <= 2300.0f;

	const bool isMovePattern12 = m_behaviorTime >= 2400.0f && m_behaviorTime <= 2500.0f;
	const bool isMovePattern13 = m_behaviorTime >= 2600.0f && m_behaviorTime <= 2700.0f;
	const bool isMovePattern14 = m_behaviorTime >= 2800.0f && m_behaviorTime <= 2900.0f;
	const bool isMovePattern15 = m_behaviorTime >= 3020.0f && m_behaviorTime <= 3150.0f;
	const bool isMovePattern16 = m_behaviorTime >= 3150.0f && m_behaviorTime <= 3300.0f;
	const bool isMovePattern17 = m_behaviorTime >= 3400.0f && m_behaviorTime <= 3500.0f;
	const bool isMovePattern18= m_behaviorTime >= 3600.0f && m_behaviorTime <= 3700.0f;
	if (m_behaviorTime >= 3800.0f) { m_behaviorTime = 0.0f;	m_behaviorKind = BEHAVIOR_KIND::BEHAVIOR_NONE; }

	//	ラープを使った移動処
	_LerpMove(isMovePattern1, MOVE_PATTERN_3, 0.05f);
	_LerpMove(isMovePattern2, MOVE_PATTERN_2, 0.05f);
	_LerpMove(isMovePattern3, MOVE_PATTERN_1, 0.05f);
	_LerpMove(isMovePattern4, MOVE_PATTERN_2, 0.05f);
	_LerpMove(isMovePattern5, MOVE_PATTERN_1, 0.05f);
	_LerpMove(isMovePattern6, MOVE_PATTERN_4, 0.05f);
	_LerpMove(isMovePattern7, MOVE_PATTERN_3, 0.05f);

	_LerpMove(isMovePattern8, MOVE_PATTERN_5, 0.05f);
	_LerpMove(isMovePattern11, MOVE_PATTERN_3, 0.05f);

	_LerpMove(isMovePattern12, MOVE_PATTERN_2, 0.05f);
	_LerpMove(isMovePattern13, MOVE_PATTERN_1, 0.05f);
	_LerpMove(isMovePattern14, MOVE_PATTERN_6, 0.05f);

	_LerpMove(isMovePattern17, MOVE_PATTERN_1, 0.05f);
	_LerpMove(isMovePattern18, MOVE_PATTERN_2, 0.05f);

	//	ラープを使わない移動処理
	if (isMovePattern9)
	{
		m_pos = VAdd(m_pos, VGet(0.0f, 0.6f, 0.0f));

		const bool isOutRange = m_pos.y >= MOVE_PATTERN_6.y;
		if(isOutRange) { m_pos.y = MOVE_PATTERN_6.y; }
	}
	if (isMovePattern10)
	{
		m_pos = VAdd(m_pos, VGet(0.0f, -0.6f, 0.0f));

		const bool isOutRange = m_pos.y <= MOVE_PATTERN_5.y;
		if (isOutRange) { m_pos.y = MOVE_PATTERN_5.y; }
	}
	if (isMovePattern15)
	{
		m_pos = VAdd(m_pos, VGet(0.0f, -0.6f, 0.0f));

		const bool isOutRange = m_pos.y <= MOVE_PATTERN_5.y;
		if (isOutRange) { m_pos.y = MOVE_PATTERN_5.y; }
	}
	if (isMovePattern16)
	{
		m_pos = VAdd(m_pos, VGet(0.0f, 0.6f, 0.0f));

		const bool isOutRange = m_pos.y >= MOVE_PATTERN_6.y;
		if (isOutRange) { m_pos.y = MOVE_PATTERN_6.y; }
	}
}

//-----------------------------------------------------------------------------
//	@brief	射撃処理
//-----------------------------------------------------------------------------
void LastBoss::_Shot(ShotManager& _shot)
{
	//	行動時間の範囲内で、ショット遅延がなくなったら、
	//	射撃処理をする
	const bool isShot1 = (m_behaviorTime == 110.0f) || (m_behaviorTime == 120.0f) || (m_behaviorTime == 130.0f);
	const bool isShot2 = (m_behaviorTime == 310.0f) || (m_behaviorTime == 320.0f) || (m_behaviorTime == 330.0f);
	const bool isShot3 = (m_behaviorTime == 510.0f) || (m_behaviorTime == 520.0f) || (m_behaviorTime == 530.0f);
	const bool isShot4 = (m_behaviorTime == 710.0f) || (m_behaviorTime == 720.0f) || (m_behaviorTime == 730.0f);
	const bool isShot5 = (m_behaviorTime == 910.0f) || (m_behaviorTime == 920.0f) || (m_behaviorTime == 930.0f);
	const bool isShot6 = (m_behaviorTime == 1140.0f) || (m_behaviorTime == 1160.0f) || (m_behaviorTime == 1180.0f) ||
						 (m_behaviorTime == 1240.0f) || (m_behaviorTime == 1260.0f) || (m_behaviorTime == 1280.0f) ||
						 (m_behaviorTime == 1340.0f) || (m_behaviorTime == 1360.0f) || (m_behaviorTime == 1380.0f);
	const bool isShot7 = (m_behaviorTime == 1510.0f) || (m_behaviorTime == 1520.0f) || (m_behaviorTime == 1530.0f);
	const bool isShot8 = (m_behaviorTime == 2110.0f) || (m_behaviorTime == 2120.0f) || (m_behaviorTime == 2130.0f);
	const bool isShot9 = (m_behaviorTime == 2310.0f) || (m_behaviorTime == 2320.0f) || (m_behaviorTime == 2330.0f);
	const bool isShot10 = (m_behaviorTime == 2510.0f) || (m_behaviorTime == 2520.0f) || (m_behaviorTime == 2530.0f);
	const bool isShot11 = (m_behaviorTime == 2710.0f) || (m_behaviorTime == 2720.0f) || (m_behaviorTime == 2730.0f);
	const bool isShot12 = (m_behaviorTime == 3310.0f) || (m_behaviorTime == 3320.0f) || (m_behaviorTime == 3330.0f);
	const bool isShot13 = (m_behaviorTime == 3510.0f) || (m_behaviorTime == 3520.0f) || (m_behaviorTime == 3530.0f);
	const bool isShot14 = (m_behaviorTime == 3710.0f) || (m_behaviorTime == 3720.0f) || (m_behaviorTime == 3730.0f);
	const bool isActiveShot = isShot1 || isShot2 || isShot3 || isShot4 || isShot5 || isShot6 || isShot7 || 
							  isShot8 || isShot9 || isShot10 || isShot11 || isShot12 || isShot13 || isShot14;
	if (isActiveShot)
	{
		//	状態がロケットのときは無視
		const bool isIgnore = m_behaviorKind == BEHAVIOR_KIND::BEHAVIOR_ROCKET;
		if (!isIgnore)
		{
			//	状態をショットにする
			m_behaviorKind = BEHAVIOR_KIND::BEHAVIOR_SHOT;
		}

		const VECTOR shotDir = VGet(-1.0f, 0.0f, 0.0f);

		//	右バレルが生きているなら、
		//	弾を生成する
		const bool isRightBarrelAlive = m_bossPartsSideBarrel[(int)BOSS_PARTS_INFO::BOSS_PARTS_RIGHT].m_life > LIFE_ZERO;
		if (isRightBarrelAlive)
		{
			const VECTOR shotPos1 = VGet(m_pos.x - SHOT_START_POS_CORRECTION_1.x, m_pos.y + SHOT_START_POS_CORRECTION_1.y, m_pos.z);
			_shot.RegisterOnList(_shot.SHOT_KIND::ENEMY_SHOT, shotPos1, shotDir, SHOT_SPEED);

			const VECTOR shotPos2 = VGet(m_pos.x - SHOT_START_POS_CORRECTION_2.x, m_pos.y + SHOT_START_POS_CORRECTION_2.y, m_pos.z);
			_shot.RegisterOnList(_shot.SHOT_KIND::ENEMY_SHOT, shotPos2, shotDir, SHOT_SPEED);
		}

		//	左バレルが生きているなら、
		//	弾を生成する
		const bool isLeftBarrelAlive = m_bossPartsSideBarrel[(int)BOSS_PARTS_INFO::BOSS_PARTS_LEFT].m_life > LIFE_ZERO;
		if (isLeftBarrelAlive)
		{
			const VECTOR shotPos3 = VGet(m_pos.x - SHOT_START_POS_CORRECTION_2.x, m_pos.y - SHOT_START_POS_CORRECTION_2.y, m_pos.z);
			_shot.RegisterOnList(_shot.SHOT_KIND::ENEMY_SHOT, shotPos3, shotDir, SHOT_SPEED);

			const VECTOR shotPos4 = VGet(m_pos.x - SHOT_START_POS_CORRECTION_1.x, m_pos.y - SHOT_START_POS_CORRECTION_1.y, m_pos.z);
			_shot.RegisterOnList(_shot.SHOT_KIND::ENEMY_SHOT, shotPos4, shotDir, SHOT_SPEED);
		}

		//	センターバレル
		const VECTOR shotPos5 = VGet(m_pos.x - SHOT_START_POS_CORRECTION_3.x, m_pos.y + SHOT_START_POS_CORRECTION_3.y, m_pos.z);
		_shot.RegisterOnList(_shot.SHOT_KIND::ENEMY_SHOT, shotPos5, shotDir, SHOT_SPEED);

		const VECTOR shotPos6 = VGet(m_pos.x - SHOT_START_POS_CORRECTION_3.x, m_pos.y - SHOT_START_POS_CORRECTION_3.y, m_pos.z);
		_shot.RegisterOnList(_shot.SHOT_KIND::ENEMY_SHOT, shotPos6, shotDir, SHOT_SPEED);
	}
}

//-----------------------------------------------------------------------------
//	@brief	ロケット処理
//-----------------------------------------------------------------------------
void LastBoss::_Rocket()
{
	const bool isRocketActive1 = m_behaviorTime == 1100.0f;
	if (isRocketActive1) { m_isStartRocket = true; }

	if(m_isStartRocket)
	{
		m_rocketTime++;
		const bool isRocketStart = m_rocketTime >= 0.0f && m_rocketTime <= 60.0f;
		const bool isRocket1 = m_rocketTime >= 160.0f && m_rocketTime <= 200.0f;
		const bool isRocket2 = m_rocketTime == 200.0f;
		const bool isRocket3 = m_rocketTime >= 220.0f && m_rocketTime <= 300.0f;
		const bool isRocketEnd = m_rocketTime == 300.0f;

		if (isRocketStart)
		{
			//	状態をロックオンにする
			m_behaviorKind = BEHAVIOR_KIND::BEHAVIOR_ROCKET;

			//	コアが生きているなら、アームを開く
			if (m_isLeftCoreAlive) { _LerpRelativeCoordinates(m_bossPartsArm[BOSS_PARTS_INFO::BOSS_PARTS_LEFT], BOSS_PARTS_INFO::BOSS_PARTS_LEFT, OPEN_RELATIVE_ARM_POS); }
			if (m_isRightCoreAlive) { _LerpRelativeCoordinates(m_bossPartsArm[BOSS_PARTS_INFO::BOSS_PARTS_RIGHT], BOSS_PARTS_INFO::BOSS_PARTS_RIGHT, OPEN_RELATIVE_ARM_POS); }

			//	パッドの振動機能
			const int vibrationPower = 500;
			PadInfo::Function::VibrationFunction(PadInfo::PAD_KIND::PAD_1, vibrationPower);
		}
		if (isRocket1)
		{
			//	パッドの振動機能の終了
			PadInfo::Function::EndVibrationFunction(PadInfo::PAD_KIND::PAD_1);
			
			//	移動処理
			const float speed = 5.0f;
			VECTOR velocity = VScale(VGet(-1.0f, 0.0f, 0.0f), speed);
			if (m_isLeftCoreAlive) { m_bossPartsArm[BOSS_PARTS_INFO::BOSS_PARTS_LEFT].m_pos = VAdd(m_bossPartsArm[BOSS_PARTS_INFO::BOSS_PARTS_LEFT].m_pos, velocity); }
			if (m_isRightCoreAlive) { m_bossPartsArm[BOSS_PARTS_INFO::BOSS_PARTS_RIGHT].m_pos = VAdd(m_bossPartsArm[BOSS_PARTS_INFO::BOSS_PARTS_RIGHT].m_pos, velocity); }

			//	パッドの振動機能
			PadInfo::Function::VibrationFunction(PadInfo::PAD_KIND::PAD_1);
		}
		if (isRocket2)
		{
			if (m_isLeftCoreAlive) { m_bossPartsArm[BOSS_PARTS_INFO::BOSS_PARTS_LEFT].m_pos = EMERGE_LEFT_ARM_POS;}
			if (m_isRightCoreAlive) { m_bossPartsArm[BOSS_PARTS_INFO::BOSS_PARTS_RIGHT].m_pos = EMERGE_RIGHT_ARM_POS;}
		}
		if (isRocket3)
		{
			//	ボスにくっつける
			_LerpRelativeCoordinates(m_bossPartsArm[BOSS_PARTS_INFO::BOSS_PARTS_LEFT], BOSS_PARTS_INFO::BOSS_PARTS_LEFT, RELATIVE_ARM_POS);
			_LerpRelativeCoordinates(m_bossPartsArm[BOSS_PARTS_INFO::BOSS_PARTS_RIGHT], BOSS_PARTS_INFO::BOSS_PARTS_RIGHT, RELATIVE_ARM_POS);

			//	パッドの振動機能の終了
			PadInfo::Function::EndVibrationFunction(PadInfo::PAD_KIND::PAD_1);
		}
		if (isRocketEnd)
		{
			//	初期化
			m_rocketTime = 0.0f;
			m_behaviorKind = BEHAVIOR_KIND::BEHAVIOR_NONE;
			m_isStartRocket = false;
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	レーザー処理
//-----------------------------------------------------------------------------
void LastBoss::_Laser()
{
	const bool isLaserActive1 = m_behaviorTime == 1700.0f;
	const bool isLaserActive2 = m_behaviorTime == 2900.0f;
	const bool isLaserActive = isLaserActive1 || isLaserActive2;
	if (isLaserActive) { m_isStartLaser = true; }

	if (m_isStartLaser)
	{
		m_laserTime++;
		const bool isLaserStart = m_laserTime >= 0.0f && m_laserTime <= 60.0f;
		const bool isLaser1 = m_laserTime == 130.0f;
		const bool isLaser2 = m_laserTime == 370.0f;
		const bool isLaser3 = m_laserTime >= 380.0f && m_laserTime <= 400.0f;
		const bool isLaserEnd = m_laserTime == 400.0f;

		if (isLaserStart)
		{
			//	状態をレーザーにする
			m_behaviorKind = BEHAVIOR_KIND::BEHAVIOR_LASER;
			m_laser.m_vertexTop = CommonConstant::ORIGIN;
			m_laser.m_vertexUnder = CommonConstant::ORIGIN;

			//	コアが生きているなら、アームを回転
			if (m_isLeftCoreAlive) { _ArmRota(m_bossPartsArm[BOSS_PARTS_INFO::BOSS_PARTS_LEFT], CommonConstant::PI); }
			if (m_isRightCoreAlive) { _ArmRota(m_bossPartsArm[BOSS_PARTS_INFO::BOSS_PARTS_RIGHT], CommonConstant::PI); }

			//	パッドの振動機能
			const int vibrationPower = 500;
			PadInfo::Function::VibrationFunction(PadInfo::PAD_KIND::PAD_1, vibrationPower);
		}
		if (isLaser1)
		{
			//	パッドの振動機能の終了
			PadInfo::Function::EndVibrationFunction(PadInfo::PAD_KIND::PAD_1);

			m_laser.m_vertexTop.x = LASER_LENGTH;

			//	パッドの振動機能
			PadInfo::Function::VibrationFunction(PadInfo::PAD_KIND::PAD_1);
		}
		if (isLaser2)
		{
			m_laser.m_vertexTop = CommonConstant::ORIGIN;

			//	パッドの振動機能の終了
			PadInfo::Function::EndVibrationFunction(PadInfo::PAD_KIND::PAD_1);

		}
		if (isLaser3)
		{
			//	アームを回転
			_ArmRota(m_bossPartsArm[BOSS_PARTS_INFO::BOSS_PARTS_LEFT], 0.0f);
			_ArmRota(m_bossPartsArm[BOSS_PARTS_INFO::BOSS_PARTS_RIGHT], 0.0f);
		}
		if (isLaserEnd)
		{
			m_behaviorKind = BEHAVIOR_KIND::BEHAVIOR_NONE;
			m_laserTime = 0.0f;
			m_isStartLaser = false;
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	パーツの状況
//-----------------------------------------------------------------------------
void LastBoss::_PartsSituation()
{
	//	コアが生きている間は、true
	m_isLeftCoreAlive = m_bossPartsCore[BOSS_PARTS_INFO::BOSS_PARTS_LEFT].m_life > LIFE_ZERO;
	m_isRightCoreAlive = m_bossPartsCore[BOSS_PARTS_INFO::BOSS_PARTS_RIGHT].m_life > LIFE_ZERO;

	//	サイドバレルが生きている間は、true
	m_isLeftSideBarrelAlive = m_bossPartsSideBarrel[BOSS_PARTS_INFO::BOSS_PARTS_LEFT].m_life > LIFE_ZERO;
	m_isRightSideBarrelAlive = m_bossPartsSideBarrel[BOSS_PARTS_INFO::BOSS_PARTS_RIGHT].m_life > LIFE_ZERO;
}

//-----------------------------------------------------------------------------
//	@brief	アームの回転
//-----------------------------------------------------------------------------
void LastBoss::_ArmRota(BossParts& _bossParts, const float _targetAngle)
{
	const float lerpSpeed = 0.2f;
	VECTOR angle = _bossParts.m_angle;
	angle.y = CommonFunction::Lerp(angle.y, _targetAngle, lerpSpeed);
	_bossParts.m_angle = angle;
}

//-----------------------------------------------------------------------------
//	@brief	ラープを使った相対座標
//-----------------------------------------------------------------------------
void LastBoss::_LerpRelativeCoordinates(BossParts& _bossParts, const BOSS_PARTS_INFO _bossInfo, const VECTOR _target)
{
	const float lerpSpeed = 0.1f;
	VECTOR pos = _bossParts.m_pos;
	VECTOR targetPos = _AskRelativeCoordinates(m_pos, _target, _bossInfo);
	pos = CommonFunction::Lerp(pos, targetPos, lerpSpeed);
	_bossParts.m_pos = pos;
}

//-----------------------------------------------------------------------------
//	@brief	線形保管を使った移動処理
//-----------------------------------------------------------------------------
void LastBoss::_LerpMove(bool _isConditions, VECTOR _targetPos, float _lerpSpeed)
{
	if (_isConditions) { m_pos = CommonFunction::Lerp(m_pos, _targetPos, _lerpSpeed); }
}

//-----------------------------------------------------------------------------
//	@brief	ヒットエフェクト
//-----------------------------------------------------------------------------
void LastBoss::_OnHitEffect(BossParts& _bossParts, SoundEffect& _soundEffect, const VECTOR _pos)
{
	//	ダメージが入ったとき
	if (_bossParts.m_isDamage)
	{
		const bool isHitStart = m_effectHit->GetPlayTime() == 0.0f;
		const bool isHitEnd = m_effectHit->GetPlayTime() == 18.0f;

		m_effectHit->SetPos(_pos);

		if (isHitStart)
		{
			m_effectHit->SetPlaySpeed(2.0f);

			if (PRODUCTION->GetIsSpecialProduction()) { m_effectHit->SetScale(VGet(30.0f, 30.0f, 30.0f)); }
			else { m_effectHit->SetScale(VGet(15.0f, 15.0f, 15.0f)); }

			m_effectHit->OnPlayEffect();
			_soundEffect.OnPlaySound(_soundEffect.SE_KIND::SE_HIT);
		}
		if (isHitEnd)
		{
			m_effectHit->OnEndEffect();
			_bossParts.m_isDamage = false;
			return;
		}

		m_effectHit->AddPlayTime();
		m_effectHit->Update();
	}
}

//-----------------------------------------------------------------------------
//	@brief	爆発エフェクト
//-----------------------------------------------------------------------------
void LastBoss::_OnEffectExplosion(SoundEffect& _soundEffect)
{
	const bool isLeftSideBarrelExplosion = !m_isLeftSideBarrelAlive && !m_isUsedEffectExplosion[EXPLOSION_EFFECT_KIND::EXPLOSION_EFFECT_LEFT_BARREL];
	const bool isRightSideBarrelExplosion = !m_isRightSideBarrelAlive && !m_isUsedEffectExplosion[EXPLOSION_EFFECT_KIND::EXPLOSION_EFFECT_RIGHT_BARREL];
	const bool isLeftCoreExplosion = !m_isLeftCoreAlive && !m_isUsedEffectExplosion[EXPLOSION_EFFECT_KIND::EXPLOSION_EFFECT_LEFT_CORE];
	const bool isRightCoreExplosion = !m_isRightCoreAlive && !m_isUsedEffectExplosion[EXPLOSION_EFFECT_KIND::EXPLOSION_EFFECT_RIGHT_CORE];
	const bool isLastExplosion = !m_isLeftCoreAlive && !m_isRightCoreAlive;

	const int right = 0;
	const int left = 1;
	_OneEffectExplosion(isLeftSideBarrelExplosion, (*m_effectExplosion[0]), _soundEffect, m_bossPartsSideBarrel[left].m_pos, m_isUsedEffectExplosion[EXPLOSION_EFFECT_KIND::EXPLOSION_EFFECT_LEFT_BARREL]);
	_OneEffectExplosion(isRightSideBarrelExplosion, (*m_effectExplosion[1]), _soundEffect, m_bossPartsSideBarrel[right].m_pos, m_isUsedEffectExplosion[EXPLOSION_EFFECT_KIND::EXPLOSION_EFFECT_RIGHT_BARREL]);
	_OneEffectExplosion(isLeftCoreExplosion, (*m_effectExplosion[2]), _soundEffect, m_bossPartsCore[left].m_pos, m_isUsedEffectExplosion[EXPLOSION_EFFECT_KIND::EXPLOSION_EFFECT_LEFT_CORE]);
	_OneEffectExplosion(isRightCoreExplosion, (*m_effectExplosion[3]), _soundEffect, m_bossPartsCore[right].m_pos, m_isUsedEffectExplosion[EXPLOSION_EFFECT_KIND::EXPLOSION_EFFECT_RIGHT_CORE]);
	_LastEffectExplosion(isLastExplosion, _soundEffect);
}

//-----------------------------------------------------------------------------
//	@brief	爆発エフェクト一つ分
//-----------------------------------------------------------------------------
void LastBoss::_OneEffectExplosion(const bool _isConditions, EffekseerEmitter& _effect, SoundEffect& _soundEffect, const VECTOR _pos, bool& _isUsedFlag)
{
	if (_isConditions)
	{
		const bool isExplosionStart = _effect.GetPlayTime() == 0.0f;
		const bool isExplosion1 = _effect.GetPlayTime() == 50.0f;
		const bool isExplosionEnd = _effect.GetPlayTime() == 100.0f;

		//	ポジションは常に追う
		_effect.SetPos(_pos);

		//	爆発エフェクトの開始
		if (isExplosionStart)
		{
			_effect.SetPlaySpeed(3.0f);
			_effect.SetScale(VGet(2.0f, 8.0f, 2.0f));
			_effect.OnPlayEffect();
		}

		if (isExplosion1)
		{
			_soundEffect.OnPlaySound(_soundEffect.SE_KIND::SE_EXPLOSION);
		}

		//	爆発エフェクトの終了
		if (isExplosionEnd)
		{
			_effect.OnEndEffect();
			_isUsedFlag = true;
			return;
		}

		_effect.AddPlayTime();
		_effect.Update();
	}
}

//-----------------------------------------------------------------------------
//	@brief	最終爆発エフェクト
//-----------------------------------------------------------------------------
void LastBoss::_LastEffectExplosion(const bool _isConditions, SoundEffect& _soundEffect)
{
	if (_isConditions)
	{
		const bool isRandamExplosion = m_effectLastExplosion->GetPlayTime() < 150.0f;
		if (isRandamExplosion)
		{
			//	乱数でエフェクトを出現させる
			const bool isActive = CommonFunction::GetRand(0, 5) == 0;
			if (isActive)
			{
				for (int i = 0; i < CommonConstant::MAX_BOSS_EXPLOSION_NUM; i++)
				{
					//	まだ再生していなければ
					const bool isNotPlay = !m_effectExplosion[i]->GetIsStartEffect();
					if (isNotPlay) { m_effectExplosion[i]->SetIsStartEffect(true); break; }
				}
			}

			//	ランダムな位置に爆破エフェクトを出す
			for (int i = 0; i < CommonConstant::MAX_BOSS_EXPLOSION_NUM; i++)
			{
				_RandamEffectExplosion((*m_effectExplosion[i]), _soundEffect);
			}
		}

		//	最終的な爆発エフェクト
		m_effectLastExplosion->SetPos(m_pos);

		//	最終的な爆発爆発エフェクトの開始
		const bool isLastExplosionStart = m_effectLastExplosion->GetPlayTime() == 150.0f;
		if (isLastExplosionStart)
		{
			m_effectLastExplosion->SetScale(VGet(5.0f, 10.0f, 5.0f));
			m_effectLastExplosion->SetPlaySpeed(1.5f);
			m_effectLastExplosion->OnPlayEffect();
		}

		const bool isLastExplosion1 = m_effectLastExplosion->GetPlayTime() == 230.0f;
		if (isLastExplosion1) { _soundEffect.OnPlaySound(_soundEffect.SE_KIND::SE_EXPLOSION); }

		//	最終的な爆発爆発エフェクトの終了
		const bool isLastExplosion2 = m_effectLastExplosion->GetPlayTime() == 250.0f;
		if(isLastExplosion2) { m_isOffDraw = true; }

		//	クリア時の演出開始
		const bool isClear = m_effectLastExplosion->GetPlayTime() == 300.0f;
		if (isClear) { PRODUCTION->SetIsClearProduction(true); }

		m_effectLastExplosion->AddPlayTime();
		m_effectLastExplosion->Update();
	}
}

//-----------------------------------------------------------------------------
//	@brief	ランダム爆発エフェクト
//-----------------------------------------------------------------------------
void LastBoss::_RandamEffectExplosion(EffekseerEmitter& _effect, SoundEffect& _soundEffect)
{
	const bool isStart = _effect.GetIsStartEffect();
	if (isStart)
	{
		const bool isExplosionStart = _effect.GetPlayTime() == 0.0f;
		const bool isExplosion = _effect.GetPlayTime() == 50.0f;
		const bool isExplosionEnd = _effect.GetPlayTime() == 100.0f;

		if (isExplosionStart)
		{
			VECTOR minPos = VSub(m_pos, BOSS_RADIUS);
			VECTOR maxPos = VAdd(m_pos, BOSS_RADIUS);
			VECTOR pos = CommonFunction::GetRand(minPos, maxPos);

			VECTOR minScale = VGet(2.0f, 4.0f, 2.0f);
			VECTOR maxScale = VGet(4.0f, 8.0f, 4.0f);
			VECTOR scale = CommonFunction::GetRand(minScale, maxScale);

			_effect.SetPos(pos);
			_effect.SetScale(scale);
			_effect.SetPlaySpeed(3.0f);
			_effect.OnPlayEffect();
		}

		if (isExplosion) { _soundEffect.OnPlaySound(_soundEffect.SE_KIND::SE_EXPLOSION); }

		//	爆発エフェクトの終了
		if (isExplosionEnd)
		{
			_effect.OnEndEffect();
			return;
		}

		_effect.AddPlayTime();
		_effect.Update();
	}
}

//-----------------------------------------------------------------------------
//	@brief	レーザーエフェクト
//-----------------------------------------------------------------------------
void LastBoss::_OnEffectLaser(EffekseerEmitter& _effect, SoundEffect& _soundEffect, const VECTOR _startPos, const BOSS_PARTS_INFO _partsInfo)
{
	//	レーザーエフェクト
	if(m_isStartLaser)
	{
		//	エフェクトの時間
		static const float startTime = 0.0f;
		static const float endTime = 400.0f;

		//	左のコアが破壊された
		const bool isLeftCoreAlive = !m_isLeftCoreAlive && _partsInfo == BOSS_PARTS_INFO::BOSS_PARTS_LEFT;
		if (isLeftCoreAlive) { _effect.SetPlayTime(endTime); }

		//	右のコアが破壊された
		const bool isRightCoreAlive = !m_isRightCoreAlive && _partsInfo == BOSS_PARTS_INFO::BOSS_PARTS_RIGHT;
		if (isRightCoreAlive) { _effect.SetPlayTime(endTime); }

		const bool isLaserStart = _effect.GetPlayTime() == startTime;
		const bool isLaserEnd = _effect.GetPlayTime() == 400.0f;
		
		//	ポジションの更新
		_effect.SetPos(VGet(_startPos.x + LASER_EFFECT_CORRECTION, _startPos.y, _startPos.z));

		//	レーザーエフェクトの開始
		if (isLaserStart)
		{
			_effect.SetPlaySpeed(1.2f);
			_effect.SetScale(VGet(3.0f, 3.0f, 10.0f));
			_effect.SetAngle(VGet(0.0f, 90.0f, 0.0f));
			_effect.OnPlayEffect();
			_soundEffect.OnPlaySound(_soundEffect.ONE_SE_KIND::ONE_SE_ENEMY_LASER);
		}

		//	レーザーエフェクトの終了
		if (isLaserEnd)
		{
			_effect.OnEndEffect();
			return;
		}

		_effect.AddPlayTime();
		_effect.Update();
	}
}

//-----------------------------------------------------------------------------
//	@brief	ロケットエフェクト
//-----------------------------------------------------------------------------
void LastBoss::_OnEffectRocket(EffekseerEmitter& _effect, SoundEffect& _soundEffect, const VECTOR _startPos, const BOSS_PARTS_INFO _partsInfo)
{
	//	エフェクトの開始
	const bool isRocketActive = m_behaviorTime == 1000.0f;
	if (isRocketActive) { _effect.SetIsStartEffect(true); }

	const bool isStart = _effect.GetIsStartEffect();
	if (isStart)
	{
		//	エフェクトの時間
		static const float startTime = 100.0f;
		static const float endTime = 300.0f;

		//	左のコアが破壊された
		const bool isLeftCoreAlive = !m_isLeftCoreAlive && _partsInfo == BOSS_PARTS_INFO::BOSS_PARTS_LEFT;
		if (isLeftCoreAlive) { _effect.SetPlayTime(endTime); }

		//	右のコアが破壊された
		const bool isRightCoreAlive = !m_isRightCoreAlive && _partsInfo == BOSS_PARTS_INFO::BOSS_PARTS_RIGHT;
		if (isRightCoreAlive) { _effect.SetPlayTime(endTime); }

		//	エフェクトの開始
		const bool isRocketStart = _effect.GetPlayTime() == startTime;
		const bool isRocketEnd = _effect.GetPlayTime() == endTime;

		//	ポジションの更新
		_effect.SetPos(VGet(_startPos.x + ROCKET_EFFECT_CORRECTION, _startPos.y, _startPos.z));

		//	レーザーエフェクトの開始
		if (isRocketStart)
		{
			_effect.SetScale(VGet(5.0f, 5.0f, 5.0f));
			_effect.OnPlayEffect();
			_soundEffect.OnPlaySound(_soundEffect.ONE_SE_KIND::ONE_SE_ROCKET);
		}

		//	レーザーエフェクトの終了
		if (isRocketEnd)
		{
			_effect.OnEndEffect();
			return;
		}

		_effect.AddPlayTime();
		_effect.Update();
	}
}

//-----------------------------------------------------------------------------
//	@brief	当たり判定用の点の更新
//-----------------------------------------------------------------------------
void LastBoss::_UpdateHitJudgment()
{
	//	アームの当たり判定用の更新
	_UpdateHitArm();

	//	コアの当たり判定用の更新
	_UpdateHitCore();

	//	サイドバレルの当たり判定用の更新
	_UpdateHitSideBarrel();

	//	レーザーの当たり判定用の更新
	_UpdateHitLaser();

	//	ボディーの当たり判定用の更新
	_UpdateHitBody();

	//	センターバレルの当たり判定用の更新
	_UpdateHitCenterBarrel();
}

//-----------------------------------------------------------------------------
//	@brief	アームの当たり判定用の更新
//-----------------------------------------------------------------------------
void LastBoss::_UpdateHitArm()
{
	const int left = BOSS_PARTS_INFO::BOSS_PARTS_LEFT;
	const int right = BOSS_PARTS_INFO::BOSS_PARTS_RIGHT;

	//	左アームの円の点の更新
	m_bossPartsArm[left].m_hitCircle.m_centerPoint =
	VGet(
			m_bossPartsArm[left].m_pos.x + ADJUSTMENT_ARM_CIRCLE_POS.x,
			m_bossPartsArm[left].m_pos.y + ADJUSTMENT_ARM_CIRCLE_POS.y,
			m_bossPartsArm[left].m_pos.z
		);

	//	左アームの円のサイズの更新
	m_bossPartsArm[left].m_hitCircle.m_radius = HIT_ARM_RADIUS;

	//	右アームの円の点の更新
	m_bossPartsArm[right].m_hitCircle.m_centerPoint =
	VGet(
		m_bossPartsArm[right].m_pos.x + ADJUSTMENT_ARM_CIRCLE_POS.x,
		m_bossPartsArm[right].m_pos.y - ADJUSTMENT_ARM_CIRCLE_POS.y,
		m_bossPartsArm[right].m_pos.z
	);

	//	左アームの円のサイズの更新
	m_bossPartsArm[right].m_hitCircle.m_radius = HIT_ARM_RADIUS;


//-----------------------------------------------------------------------------//

	//	左アームの長方形の頂点１の更新
	m_bossPartsArm[left].m_hitRect.m_vertexTop =
	VGet(
		m_bossPartsArm[left].m_pos.x - ADJUSTMENT_ARM_RECT_POS.x + ARM_CORRECTION.x,
		m_bossPartsArm[left].m_pos.y - ADJUSTMENT_ARM_RECT_POS.y + ARM_CORRECTION.y,
		m_bossPartsArm[left].m_pos.z - ADJUSTMENT_ARM_RECT_POS.z
	);

	//	左アームの長方形の頂点２の更新
	m_bossPartsArm[left].m_hitRect.m_vertexUnder =
	VGet(
		m_bossPartsArm[left].m_pos.x + ADJUSTMENT_ARM_RECT_POS.x + ARM_CORRECTION.x,
		m_bossPartsArm[left].m_pos.y + ADJUSTMENT_ARM_RECT_POS.y + ARM_CORRECTION.y,
		m_bossPartsArm[left].m_pos.z + ADJUSTMENT_ARM_RECT_POS.z
	);

	//	右アームの長方形の頂点１の更新
	m_bossPartsArm[right].m_hitRect.m_vertexTop =
	VGet(
		m_bossPartsArm[right].m_pos.x - ADJUSTMENT_ARM_RECT_POS.x + ARM_CORRECTION.x,
		m_bossPartsArm[right].m_pos.y - ADJUSTMENT_ARM_RECT_POS.y - ARM_CORRECTION.y,
		m_bossPartsArm[right].m_pos.z - ADJUSTMENT_ARM_RECT_POS.z
	);

	//	右アームの長方形の頂点２の更新
	m_bossPartsArm[right].m_hitRect.m_vertexUnder =
	VGet(
		m_bossPartsArm[right].m_pos.x + ADJUSTMENT_ARM_RECT_POS.x + ARM_CORRECTION.x,
		m_bossPartsArm[right].m_pos.y + ADJUSTMENT_ARM_RECT_POS.y - ARM_CORRECTION.y,
		m_bossPartsArm[right].m_pos.z + ADJUSTMENT_ARM_RECT_POS.z
	);

}

//-----------------------------------------------------------------------------
//	@brief	コアの当たり判定用の更新
//-----------------------------------------------------------------------------
void LastBoss::_UpdateHitCore()
{
	const int left = BOSS_PARTS_INFO::BOSS_PARTS_LEFT;
	const int right = BOSS_PARTS_INFO::BOSS_PARTS_RIGHT;

	//	左コアの円の点の更新
	m_bossPartsCore[left].m_hitCircle.m_centerPoint = m_bossPartsCore[left].m_pos;

	//	左コアの円のサイズの更新
	m_bossPartsCore[left].m_hitCircle.m_radius = HIT_CORE_RADIUS;

	//	右コアの円の点の更新
	m_bossPartsCore[right].m_hitCircle.m_centerPoint = m_bossPartsCore[right].m_pos;

	//	左コアの円のサイズの更新
	m_bossPartsCore[right].m_hitCircle.m_radius = HIT_CORE_RADIUS;


//-----------------------------------------------------------------------------//

	//	左コアの長方形の頂点１の更新
	m_coreHitRect[left].m_vertexTop =
	VGet(
		m_bossPartsSideBarrel[left].m_pos.x + HIT_CORE_RADIUS,
		m_bossPartsSideBarrel[left].m_pos.y + HIT_CORE_RADIUS,
		m_bossPartsSideBarrel[left].m_pos.z + HIT_CORE_RADIUS
	);

	//	左コアの長方形の頂点２の更新
	m_coreHitRect[left].m_vertexUnder = 
	VGet(
		m_bossPartsSideBarrel[left].m_pos.x - HIT_CORE_RADIUS,
		m_bossPartsSideBarrel[left].m_pos.y - HIT_CORE_RADIUS,
		m_bossPartsSideBarrel[left].m_pos.z - HIT_CORE_RADIUS
	);

	//	右コアの長方形の頂点１の更新
	m_coreHitRect[right].m_vertexTop = 
	VGet(
		m_bossPartsSideBarrel[right].m_pos.x + HIT_CORE_RADIUS,
		m_bossPartsSideBarrel[right].m_pos.y + HIT_CORE_RADIUS,
		m_bossPartsSideBarrel[right].m_pos.z + HIT_CORE_RADIUS
	);

	//	右コアの長方形の頂点２の更新
	m_coreHitRect[right].m_vertexUnder = 
	VGet(
		m_bossPartsSideBarrel[right].m_pos.x - HIT_CORE_RADIUS,
		m_bossPartsSideBarrel[right].m_pos.y - HIT_CORE_RADIUS,
		m_bossPartsSideBarrel[right].m_pos.z - HIT_CORE_RADIUS
	);
}

//-----------------------------------------------------------------------------
//	@brief	サイドバレルの当たり判定用の更新
//-----------------------------------------------------------------------------
void LastBoss::_UpdateHitSideBarrel()
{
	const int left = BOSS_PARTS_INFO::BOSS_PARTS_LEFT;
	const int right = BOSS_PARTS_INFO::BOSS_PARTS_RIGHT;

	//	左サイドバレルの円の点の更新
	m_bossPartsSideBarrel[left].m_hitCircle.m_centerPoint =
	VGet(
		m_bossPartsSideBarrel[left].m_pos.x + ADJUSTMENT_SIDE_BARREL_POS.x,
		m_bossPartsSideBarrel[left].m_pos.y + ADJUSTMENT_SIDE_BARREL_POS.y,
		m_bossPartsSideBarrel[left].m_pos.z
	);

	//	左サイドバレルの円のサイズの更新
	m_bossPartsSideBarrel[left].m_hitCircle.m_radius = HIT_SIDE_BARREL_RADIUS;

	//	右サイドバレルの円の点の更新
	m_bossPartsSideBarrel[right].m_hitCircle.m_centerPoint =
	VGet(
		m_bossPartsSideBarrel[right].m_pos.x + ADJUSTMENT_SIDE_BARREL_POS.x,
		m_bossPartsSideBarrel[right].m_pos.y - ADJUSTMENT_SIDE_BARREL_POS.y,
		m_bossPartsSideBarrel[right].m_pos.z
	);

	//	左サイドバレルの円のサイズの更新
	m_bossPartsSideBarrel[right].m_hitCircle.m_radius = HIT_SIDE_BARREL_RADIUS;

//-----------------------------------------------------------------------------//

	//	左サイドバレルの長方形の頂点１の更新
	m_sideBarrelHitRect[left].m_vertexTop =
	VGet(
		m_bossPartsSideBarrel[left].m_pos.x + ADJUSTMENT_SIDE_BARREL_POS.x + HIT_SIDE_BARREL_RADIUS,
		m_bossPartsSideBarrel[left].m_pos.y + ADJUSTMENT_SIDE_BARREL_POS.y + HIT_SIDE_BARREL_RADIUS,
		m_bossPartsSideBarrel[left].m_pos.z + HIT_SIDE_BARREL_RADIUS
	);

	//	左サイドバレルの長方形の頂点２の更新
	m_sideBarrelHitRect[left].m_vertexUnder = 
	VGet(
		m_bossPartsSideBarrel[left].m_pos.x + ADJUSTMENT_SIDE_BARREL_POS.x - HIT_SIDE_BARREL_RADIUS,
		m_bossPartsSideBarrel[left].m_pos.y - ADJUSTMENT_SIDE_BARREL_POS.y - HIT_SIDE_BARREL_RADIUS,
		m_bossPartsSideBarrel[left].m_pos.z - HIT_SIDE_BARREL_RADIUS
	);

	//	右サイドバレルの長方形の頂点１の更新
	m_sideBarrelHitRect[right].m_vertexTop = 
	VGet(
		m_bossPartsSideBarrel[right].m_pos.x + ADJUSTMENT_SIDE_BARREL_POS.x + HIT_SIDE_BARREL_RADIUS,
		m_bossPartsSideBarrel[right].m_pos.y + ADJUSTMENT_SIDE_BARREL_POS.y + HIT_SIDE_BARREL_RADIUS,
		m_bossPartsSideBarrel[right].m_pos.z + HIT_SIDE_BARREL_RADIUS
	);

	//	右サイドバレルの長方形の頂点２の更新
	m_sideBarrelHitRect[right].m_vertexUnder = 
	VGet(
		m_bossPartsSideBarrel[right].m_pos.x + ADJUSTMENT_SIDE_BARREL_POS.x - HIT_SIDE_BARREL_RADIUS,
		m_bossPartsSideBarrel[right].m_pos.y - ADJUSTMENT_SIDE_BARREL_POS.y - HIT_SIDE_BARREL_RADIUS,
		m_bossPartsSideBarrel[right].m_pos.z - HIT_SIDE_BARREL_RADIUS
	);
}

//-----------------------------------------------------------------------------
//	@brief	レーザーの当たり判定用の更新
//-----------------------------------------------------------------------------
void LastBoss::_UpdateHitLaser()
{
	const int left = BOSS_PARTS_INFO::BOSS_PARTS_LEFT;
	const int right = BOSS_PARTS_INFO::BOSS_PARTS_RIGHT;

	//	左レーザーの長方形の頂点１の更新
	m_laserHitRect[left].m_vertexTop =
	VGet(
		m_bossPartsArm[left].m_pos.x + ADJUSTMENT_LASER_POS.x + m_laser.m_vertexTop.x,
		m_bossPartsArm[left].m_pos.y - ADJUSTMENT_LASER_POS.y,
		m_bossPartsArm[left].m_pos.z - ADJUSTMENT_LASER_POS.z
	);

	//	左レーザーの長方形の頂点２の更新
	m_laserHitRect[left].m_vertexUnder =
	VGet(
		m_bossPartsArm[left].m_pos.x + ADJUSTMENT_LASER_POS.x + m_laser.m_vertexUnder.x,
		m_bossPartsArm[left].m_pos.y + ADJUSTMENT_LASER_POS.y,
		m_bossPartsArm[left].m_pos.z + ADJUSTMENT_LASER_POS.z
	);

	//	右レーザーの長方形の頂点１の更新
	m_laserHitRect[right].m_vertexTop =
	VGet(
		m_bossPartsArm[right].m_pos.x + ADJUSTMENT_LASER_POS.x + m_laser.m_vertexTop.x,
		m_bossPartsArm[right].m_pos.y - ADJUSTMENT_LASER_POS.y,
		m_bossPartsArm[right].m_pos.z - ADJUSTMENT_LASER_POS.z
	);

	//	右レーザーの長方形の頂点２の更新
	m_laserHitRect[right].m_vertexUnder =
	VGet(
		m_bossPartsArm[right].m_pos.x + ADJUSTMENT_LASER_POS.x + m_laser.m_vertexUnder.x,
		m_bossPartsArm[right].m_pos.y + ADJUSTMENT_LASER_POS.y,
		m_bossPartsArm[right].m_pos.z + ADJUSTMENT_LASER_POS.z
	);
}

//-----------------------------------------------------------------------------
//	@brief		ボディーの当たり判定用の更新
//-----------------------------------------------------------------------------
void LastBoss::_UpdateHitBody()
{
	//	ボディーの円の点の更新
	m_hitCircle.m_centerPoint = VGet(m_pos.x + ADJUSTMENT_BODY_POS.x, m_pos.y, m_pos.z);
	
	//	ボディーの円のサイズの更新
	m_hitCircle.m_radius = HIT_BODY_RADIUS;
}

//-----------------------------------------------------------------------------
//	@brief	センターバレルの当たり判定用の更新
//-----------------------------------------------------------------------------
void LastBoss::_UpdateHitCenterBarrel()
{
	//	センターバレルの長方形の頂点１の更新
	m_centerBarrel.m_vertexTop =
	VGet(
		m_pos.x - ADJUSTMENT_CENTER_BARRE_POS.x - CENTER_BARRE_CORRECTION.x,
		m_pos.y - ADJUSTMENT_CENTER_BARRE_POS.y,
		m_pos.z - ADJUSTMENT_CENTER_BARRE_POS.z
	);

	//	センターバレルの長方形の頂点２の更新
	m_centerBarrel.m_vertexUnder =
	VGet(
		m_pos.x + ADJUSTMENT_CENTER_BARRE_POS.x - CENTER_BARRE_CORRECTION.x,
		m_pos.y + ADJUSTMENT_CENTER_BARRE_POS.y,
		m_pos.z + ADJUSTMENT_CENTER_BARRE_POS.z
	);
}

//-----------------------------------------------------------------------------
//	@brief	相対座標を割り当てる
//-----------------------------------------------------------------------------
void LastBoss::_AllocationRelativeCoordinates()
{
	//	右バレルが生きているなら、
	//	ボスにくっつける
	if (m_isRightSideBarrelAlive) { m_bossPartsCore[BOSS_PARTS_INFO::BOSS_PARTS_RIGHT].m_pos = _AskRelativeCoordinates(m_pos, RELATIVE_CORE_POS, BOSS_PARTS_INFO::BOSS_PARTS_RIGHT); }
	else { m_bossPartsCore[BOSS_PARTS_INFO::BOSS_PARTS_RIGHT].m_pos = _AskRelativeCoordinates(m_pos, OPEN_RELATIVE_CORE_POS, BOSS_PARTS_INFO::BOSS_PARTS_RIGHT); }

	//	左バレルが生きているなら、
	//	ボスにくっつける
	if (m_isLeftSideBarrelAlive) { m_bossPartsCore[BOSS_PARTS_INFO::BOSS_PARTS_LEFT].m_pos = _AskRelativeCoordinates(m_pos, RELATIVE_CORE_POS, BOSS_PARTS_INFO::BOSS_PARTS_LEFT); }
	else { m_bossPartsCore[BOSS_PARTS_INFO::BOSS_PARTS_LEFT].m_pos = _AskRelativeCoordinates(m_pos, OPEN_RELATIVE_CORE_POS, BOSS_PARTS_INFO::BOSS_PARTS_LEFT); }

	m_bossPartsSideBarrel[BOSS_PARTS_INFO::BOSS_PARTS_RIGHT].m_pos = _AskRelativeCoordinates(m_pos, RELATIVE_SIDE_BARREL_POS, BOSS_PARTS_INFO::BOSS_PARTS_RIGHT);
	m_bossPartsSideBarrel[BOSS_PARTS_INFO::BOSS_PARTS_LEFT].m_pos = _AskRelativeCoordinates(m_pos, RELATIVE_SIDE_BARREL_POS, BOSS_PARTS_INFO::BOSS_PARTS_LEFT);

	const bool isActive = m_behaviorKind != BEHAVIOR_KIND::BEHAVIOR_ROCKET;
	if (isActive)
	{
		m_bossPartsArm[BOSS_PARTS_INFO::BOSS_PARTS_RIGHT].m_pos = _AskRelativeCoordinates(m_pos, RELATIVE_ARM_POS, BOSS_PARTS_INFO::BOSS_PARTS_RIGHT);
		m_bossPartsArm[BOSS_PARTS_INFO::BOSS_PARTS_LEFT].m_pos = _AskRelativeCoordinates(m_pos, RELATIVE_ARM_POS, BOSS_PARTS_INFO::BOSS_PARTS_LEFT);
	}
}

//-----------------------------------------------------------------------------
//	@brief	相対座標を割り当てる
//-----------------------------------------------------------------------------
VECTOR LastBoss::_AskRelativeCoordinates(const VECTOR _basePos, const VECTOR _relativePos, const BOSS_PARTS_INFO _bossPartsInfo)
{
	VECTOR tmpPos;

	tmpPos = VAdd(_basePos, _relativePos);

	//	パーツの位置が左ならｙ軸のみ反転する
	const bool isLeft = _bossPartsInfo == BOSS_PARTS_INFO::BOSS_PARTS_LEFT;
	if (isLeft) { tmpPos.y = _basePos.y - _relativePos.y; }

	return tmpPos;
}

//-----------------------------------------------------------------------------
//	@brief	モデルの色を変更する
//-----------------------------------------------------------------------------
void LastBoss::_ChangeModelColor(const int _modelHandle, const int _life, const int _maxLife, const ColorF _normalColor)
{
	const int midleLife = _maxLife / 2;
	const int dangerLife = _maxLife / 5;

	//	ライフは中間より大きいので、
	//	モデルの色はデフォルト色
	const bool isNormalColor = _life > midleLife;
	if(isNormalColor) { MV1SetDifColorScale(_modelHandle, _normalColor.m_color); }

	//	ライフは中間なので、
	//	モデルの色を黄色に変換
	const bool isLifeMidle = _life > dangerLife && _life <= midleLife;
	if (isLifeMidle) { MV1SetDifColorScale(_modelHandle, m_lifeMiddleColorF.m_color); }

	//	ライフは中間なので、
	//	モデルの色を赤色に変換
	const bool isLifeDanger = _life > LIFE_ZERO && _life <= dangerLife;
	if (isLifeDanger) { MV1SetDifColorScale(_modelHandle, m_lifeDangerColorF.m_color); }
}

//-----------------------------------------------------------------------------
//	@brief	点滅描画処理
//-----------------------------------------------------------------------------
void LastBoss::_FlashingDraw(BossParts& _bossParts)
{
	//	ダメージが入ったとき
	if (_bossParts.m_isDamage)
	{
		m_flashingTime++;

		//	点滅
		const bool isDraw = (m_flashingTime >= 0.0f && m_flashingTime <= 3.0f) ||
			(m_flashingTime >= 6.0f && m_flashingTime <= 9.0f) ||
			(m_flashingTime >= 12.0f && m_flashingTime <= 15.0f);
		if (isDraw) { MV1DrawModel(_bossParts.m_modelHandle); }

		const bool isReset = m_flashingTime >= 18.0f;
		if (isReset) { m_flashingTime = 0.0f;}
	}
	else
	{
		MV1DrawModel(_bossParts.m_modelHandle);
	}
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放処理
//-----------------------------------------------------------------------------
void LastBoss::_FinalRelease()
{
	//	モデルのアンロード
	MV1DeleteModel(m_modelHandle);

	for (int i = 0; i < BOSS_PARTS_INFO::BOSS_PARTS_NUM; i++)
	{
		MV1DeleteModel(m_bossPartsSideBarrel[i].m_modelHandle);
		MV1DeleteModel(m_bossPartsArm[i].m_modelHandle);
		MV1DeleteModel(m_bossPartsCore[i].m_modelHandle);
	}

	//	エフェクトの削除
	CommonSafe::Delete(m_effectHit);

	for (int i = 0; i < BOSS_PARTS_INFO::BOSS_PARTS_NUM; i++)
	{
		CommonSafe::Delete(m_effectInjection[i]);
		CommonSafe::Delete(m_effectLaser[i]);
	}

	for (int i = 0; i < CommonConstant::MAX_BOSS_EXPLOSION_NUM; i++)
	{
		CommonSafe::Delete(m_effectExplosion[i]);
	}

	CommonSafe::Delete(m_effectLastExplosion);
}

