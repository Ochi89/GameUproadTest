
//=============================================================================
//	@file	MediumBoss.cpp
//	@brief	中ボス
//	@autor	Takuya Ochi
//	@date	2018/12/18
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "MediumBoss.h"
#include "Common.h"
#include "MoveHelper.h"
#include "AngleHelper.h"
#include "PlayerManager.h"
#include "ShotManager.h"
#include "EffekseerEmitter.h"
#include "SoundEffect.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const VECTOR	MediumBoss::EMERGE_POS = VGet(40.0f, -100.0f, 100.0f);					//	出現時の座標
const VECTOR	MediumBoss::START_POS = VGet(65.0f, 55.0f, 0.0f);						//	開始時の座標
const VECTOR	MediumBoss::END_POS = VGet(-300.0f, 55.0f, 0.0f);						//	退却時の座標
const VECTOR	MediumBoss::MOVE_PATTERN_1 = VGet(65.0f, 55.0f, 0.0f);					//	行動パターン１
const VECTOR	MediumBoss::MOVE_PATTERN_2 = VGet(20.0f, 55.0f, 0.0f);					//	行動パターン２
const VECTOR	MediumBoss::MOVE_PATTERN_3 = VGet(65.0f, 30.0f, 0.0f);					//	行動パターン３
const VECTOR	MediumBoss::MOVE_PATTERN_4 = VGet(20.0f, 30.0f, 0.0f);					//	行動パターン４
const VECTOR	MediumBoss::SHOT_START_POS_CORRECTION_1 = VGet(5.0f, 4.0f, 0.0f);		//	ショット開始位置座標の補正
const VECTOR	MediumBoss::SHOT_START_POS_CORRECTION_2 = VGet(10.0f, 2.0f, 0.0f);		//	ショット開始位置座標の補正
const VECTOR	MediumBoss::SHOT_START_POS_CORRECTION_3 = VGet(10.0f, 2.0f, 0.0f);		//	ショット開始位置座標の補正
const VECTOR	MediumBoss::SHOT_START_POS_CORRECTION_4 = VGet(5.0f, 4.0f, 0.0f);		//	ショット開始位置座標の補正
const VECTOR	MediumBoss::RECT_CORRECTION = VGet(12.0f, 12.0f, 5.0f);					//	座標の補正
const float		MediumBoss::SHOT_SPEED = 1.0f;											//	ショットの速度
const float		MediumBoss::MAX_START_WAIT_TIME = 35.0f;								//	ショットの開始時の遅延時間
const float		MediumBoss::MAX_WAIT_TIME = 100.0f;										//	ショットの遅延時間
const float		MediumBoss::HIT_RADIUS = 12.0f;											//	当たり判定用の半径
const int		MediumBoss::MAX_LIFE = 15;												//	ライフの最大
const int		MediumBoss::MIN_LIFE = 0;												//	ライフの最小

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
MediumBoss::MediumBoss(const int _modelHandle)
{
	//	モデルの複製
	m_modelHandle = MV1DuplicateModel(_modelHandle);
	CommonDebug::Assert((m_modelHandle <= -1), " [ MediumBoss.cpp ] : error : missing duplicat model.");

	//	各変数を初期化
	m_pos = CommonConstant::ORIGIN;
	m_dir = CommonConstant::ORIGIN;
	m_angle = CommonConstant::ORIGIN;

	//	まだどこも指していないので、NULLで初期化
	m_effectHit = NULL;
	m_effectExplosion = NULL;
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
MediumBoss::~MediumBoss()
{
	//	最終的な解放処理
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成処理
//-----------------------------------------------------------------------------
void MediumBoss::Create()
{
	//	エフェクトの読み込み
	m_effectHit = new EffekseerEmitter("Data/Effect/Hit/Hit.efk");
	m_effectExplosion = new EffekseerEmitter("Data/Effect/Explosion/Explosion.efk");
}

//-----------------------------------------------------------------------------
//	@brief	解放処理
//-----------------------------------------------------------------------------
void MediumBoss::Release()
{
	//	モデルのアンロード
	MV1DeleteModel(m_modelHandle);
}

//-----------------------------------------------------------------------------
//	@brief	初期処理
//-----------------------------------------------------------------------------
void MediumBoss::Initialize()
{
	//	各変数を初期状態に設定
	m_pos = EMERGE_POS;
	m_dir = CommonConstant::ORIGIN;
	m_angle = CommonConstant::ORIGIN;
	m_emergeMoveTaim = 0.0f;
	m_behaviorTime = 0.0f;
	m_moveSpeed = 1.0f;
	m_shotWaitTime = 0.0f;
	m_isEmerge = false;
	m_isStart = false;
	m_isStarted = false;
	m_isDamage = false;
	m_isDeleate = false;
	m_isOffDraw = false;
	m_life = MAX_LIFE;

	//	当たり判定用の構造体の初期化
	m_hitCircle.m_radius = HIT_RADIUS;
	m_hitCircle.m_centerPoint = CommonConstant::ORIGIN;

	//	角度を割り当てる
	MV1SetRotationXYZ(m_modelHandle, m_angle);
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void MediumBoss::Update(ShotManager& _shot, SoundEffect& _soundEffect)
{
	//	出現時の演出
	_EmergeMove();

	//	始動していて、
	//	ライフが残っているととき
	const bool isActive = m_isStarted && m_life > MIN_LIFE && !PRODUCTION->GetIsSpecialProduction();
	if (isActive)
	{
		//	移動処理
		_Move();

		//	射撃処理
		_Shot(_shot);
	}

	//	モデルにポジションを割り当てる
	MV1SetPosition(m_modelHandle, m_pos);

	//	モデルに角度を割り当てる
	MV1SetRotationXYZ(m_modelHandle, m_angle);

	//	ヒットエフェクト
	_OnHitEffect(_soundEffect);

	//	爆発エフェクト
	_OnEffectExplosion(_soundEffect);

	//	当たり判定がずれるので、
	//	モデルにポジションを割り当てた後に、
	//	当たり判定用の座標の更新する
	_UpdateHitPoint();
}

//-----------------------------------------------------------------------------
//	@brief	描画処理
//-----------------------------------------------------------------------------
void MediumBoss::Draw()
{
	if (m_isStart)
	{
		if (!m_isOffDraw)
		{
			//	点滅
			if (m_isDamage)
			{
				m_flashingTime++;
				const bool isFlashingEnd = m_flashingTime > 6.0f;
				if (isFlashingEnd) { m_flashingTime = 0.0f; }

				//	点滅
				const bool isFlashing = m_flashingTime >= 0.0f && m_flashingTime <= 3.0f;
				if (isFlashing) { MV1DrawModel(m_modelHandle); }
			}
			else
			{
				//	モデルの描画
				MV1DrawModel(m_modelHandle);
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	開始時の移動
//-----------------------------------------------------------------------------
void MediumBoss::_EmergeMove()
{
	//	出現時の演出
	if (m_isEmerge)
	{
		//	出現時の演出
		m_emergeMoveTaim++;
		const bool isEmergeMoveStart = m_emergeMoveTaim <= 5.0f;
		const bool isEmergeMove1 = m_emergeMoveTaim >= 5.0f && m_emergeMoveTaim <= 120.0f;
		const bool isEmergeMove2 = m_emergeMoveTaim >= 120.0f && m_emergeMoveTaim <= 162.0f;
		const bool isEmergeRota = m_emergeMoveTaim >= 100.0f && m_emergeMoveTaim <= 162.0f;
		const bool isEmergeMoveEnd = m_emergeMoveTaim == 162.0f;

		//	演出開始
		if (isEmergeMoveStart) { m_pos = EMERGE_POS; }

		if (isEmergeMove1)
		{
			//	浮上
			m_dir = VGet(0.0f, 1.5f, 0.0f);
			VECTOR moving = MoveHelper::AskMoveAmount(m_dir, m_moveSpeed);
			m_pos = VAdd(m_pos, moving);
		}

		//	ラープを使った移動処理
		_LerpMove(isEmergeMove2, START_POS, 0.1f);

		//	モデルの回転
		if (isEmergeRota) { m_angle.x -= 0.1f; }

		//	演出終了
		if (isEmergeMoveEnd)
		{
			m_pos = START_POS;
			m_angle = CommonConstant::ORIGIN;
			m_dir = CommonConstant::ORIGIN;
			m_isStarted = true;
			m_isEmerge = false;
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	移動処理
//-----------------------------------------------------------------------------
void MediumBoss::_Move()
{
	m_behaviorTime++;
	const bool isMovePattern1 = m_behaviorTime >= 100.0f && m_behaviorTime <= 200.0f;
	const bool isMovePattern2 = m_behaviorTime >= 200.0f && m_behaviorTime <= 300.0f;
	const bool isMovePattern3 = m_behaviorTime >= 300.0f && m_behaviorTime <= 400.0f;
	const bool isMovePattern4 = m_behaviorTime >= 400.0f && m_behaviorTime <= 500.0f;
	const bool isMovePattern5 = m_behaviorTime >= 500.0f && m_behaviorTime <= 600.0f;
	const bool isMovePattern6 = m_behaviorTime >= 600.0f && m_behaviorTime <= 700.0f;
	const bool isMovePattern7 = m_behaviorTime >= 700.0f && m_behaviorTime <= 800.0f;
	const bool isMovePattern8 = m_behaviorTime >= 800.0f && m_behaviorTime <= 900.0f;
	const bool isMovePattern9 = m_behaviorTime >= 900.0f && m_behaviorTime <= 1000.0f;
	const bool isMovePattern10 = m_behaviorTime >= 1000.0f && m_behaviorTime <= 1100.0f;
	const bool isMovePattern11 = m_behaviorTime >= 1100.0f;
	
	//	ラープを使った移動処理
	_LerpMove(isMovePattern1, MOVE_PATTERN_3, 0.05f);
	_LerpMove(isMovePattern2, MOVE_PATTERN_1, 0.05f);
	_LerpMove(isMovePattern3, MOVE_PATTERN_2, 0.05f);
	_LerpMove(isMovePattern4, MOVE_PATTERN_1, 0.05f);
	_LerpMove(isMovePattern5, MOVE_PATTERN_3, 0.05f);
	_LerpMove(isMovePattern6, MOVE_PATTERN_1, 0.05f);
	_LerpMove(isMovePattern7, MOVE_PATTERN_3, 0.05f);
	_LerpMove(isMovePattern8, MOVE_PATTERN_4, 0.05f);
	_LerpMove(isMovePattern9, MOVE_PATTERN_3, 0.05f);
	_LerpMove(isMovePattern10, MOVE_PATTERN_1, 0.05f);
	_LerpMove(isMovePattern11, END_POS, 0.01f);
}

//-----------------------------------------------------------------------------
//	@brief	線形保管を使った移動処理
//-----------------------------------------------------------------------------
void MediumBoss::_LerpMove(bool _isConditions, VECTOR _targetPos, float _lerpSpeed)
{
	if (_isConditions) { m_pos = CommonFunction::Lerp(m_pos, _targetPos, _lerpSpeed); }
}

//-----------------------------------------------------------------------------
//	@brief	射撃処理
//-----------------------------------------------------------------------------
void MediumBoss::_Shot(ShotManager& _shot)
{
	m_shotWaitTime++;
	const bool isActiveShot = m_shotWaitTime == 35.0f || m_shotWaitTime == 40.0f || m_shotWaitTime == 45.0f;
	if (m_shotWaitTime >= MAX_WAIT_TIME) { m_shotWaitTime = 0.0f; }

	if (isActiveShot)
	{
		const VECTOR shotDir = VGet(-1.0f, 0.0f, 0.0f);

		const VECTOR shotPos1 = VGet(m_pos.x - SHOT_START_POS_CORRECTION_1.x, m_pos.y + SHOT_START_POS_CORRECTION_1.y, m_pos.z);
		_shot.RegisterOnList(_shot.SHOT_KIND::ENEMY_SHOT, shotPos1, shotDir, SHOT_SPEED);

		const VECTOR shotPos2 = VGet(m_pos.x - SHOT_START_POS_CORRECTION_2.x, m_pos.y + SHOT_START_POS_CORRECTION_2.y, m_pos.z);
		_shot.RegisterOnList(_shot.SHOT_KIND::ENEMY_SHOT, shotPos2, shotDir, SHOT_SPEED);

		const VECTOR shotPos3 = VGet(m_pos.x - SHOT_START_POS_CORRECTION_2.x, m_pos.y - SHOT_START_POS_CORRECTION_2.y, m_pos.z);
		_shot.RegisterOnList(_shot.SHOT_KIND::ENEMY_SHOT, shotPos3, shotDir, SHOT_SPEED);

		const VECTOR shotPos4 = VGet(m_pos.x - SHOT_START_POS_CORRECTION_4.x, m_pos.y - SHOT_START_POS_CORRECTION_4.y, m_pos.z);
		_shot.RegisterOnList(_shot.SHOT_KIND::ENEMY_SHOT, shotPos4, shotDir, SHOT_SPEED);
	}
}

//-----------------------------------------------------------------------------
//	@brief	ヒットエフェクト
//-----------------------------------------------------------------------------
void MediumBoss::_OnHitEffect(SoundEffect& _soundEffect)
{
	//	ダメージが入ったとき
	if (m_isDamage)
	{
		const bool isHitStart = m_effectHit->GetPlayTime() == 0.0f;
		const bool isHitEnd = m_effectHit->GetPlayTime() == 24.0f;

		m_effectHit->SetPos(m_pos);
		
		if (isHitStart)
		{
			m_effectHit->SetPlaySpeed(2.0f);

			if (PRODUCTION->GetIsSpecialProduction()) { m_effectHit->SetScale(VGet(30.0f, 30.0f, 30.0f)); }
			else { m_effectHit->SetScale(VGet(10.0f, 10.0f, 10.0f)); }

			m_effectHit->OnPlayEffect();
			_soundEffect.OnPlaySound(_soundEffect.SE_KIND::SE_HIT);
		}
		if (isHitEnd)
		{
			m_effectHit->OnEndEffect();
			m_isDamage = false;
			return;
		}

		m_effectHit->AddPlayTime();
		m_effectHit->Update();
	}
}

//-----------------------------------------------------------------------------
//	@brief	爆発エフェクト
//-----------------------------------------------------------------------------
void MediumBoss::_OnEffectExplosion(SoundEffect& _soundEffect)
{
	//	ライフがなくなったとき
	const bool isNotLife = m_life <= MIN_LIFE && !m_isDeleate;
	if (isNotLife)
	{
		const bool isExplosionStart = m_effectExplosion->GetPlayTime() == 0.0f;
		const bool isExplosion1 = m_effectExplosion->GetPlayTime() == 60.0f;
		const bool isExplosionEnd = m_effectExplosion->GetPlayTime() == 100.0f;

		//	爆発エフェクトの開始
		if (isExplosionStart)
		{
			m_effectExplosion->SetPos(m_pos);
			m_effectExplosion->SetPlaySpeed(2.0f);
			m_effectExplosion->SetScale(VGet(4.0f, 4.0f, 4.0f));
			m_effectExplosion->OnPlayEffect();
		}

		if(isExplosion1) { m_isOffDraw = true; _soundEffect.OnPlaySound(_soundEffect.SE_KIND::SE_EXPLOSION); }

		//	爆発エフェクトの終了
		if (isExplosionEnd)
		{
			m_effectExplosion->OnEndEffect();
			m_isDeleate = true;
			return;
		}

		m_effectExplosion->AddPlayTime();
		m_effectExplosion->Update();
	}
}

//-----------------------------------------------------------------------------
//	@brief	当たり判定用の点の更新
//-----------------------------------------------------------------------------
void MediumBoss::_UpdateHitPoint()
{
	//	当たり判定用の中心座標の更新
	m_hitCircle.m_centerPoint = VGet(m_pos.x, m_pos.y, m_pos.z);

	//	当たり判定用の座標の更新
	m_hitRect.m_vertexTop = VGet(m_pos.x + RECT_CORRECTION.x, m_pos.y + RECT_CORRECTION.y, m_pos.z + RECT_CORRECTION.z);
	m_hitRect.m_vertexUnder = VGet(m_pos.x - RECT_CORRECTION.x, m_pos.y - RECT_CORRECTION.y, m_pos.z - RECT_CORRECTION.z);
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放処理
//-----------------------------------------------------------------------------
void MediumBoss::_FinalRelease()
{
	//	モデルのアンロード
	MV1DeleteModel(m_modelHandle);
}

