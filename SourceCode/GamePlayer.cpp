
//=============================================================================
//	@file	Player.cpp
//	@brief	プレイヤー
//	@autor	Takuya Ochi
//	@date	2018/11/14
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "GamePlayer.h"
#include "Common.h"
#include "Pad.h"
#include "MoveHelper.h"
#include "AngleHelper.h"
#include "ShotManager.h"
#include "StayShotManager.h"
#include "EffekseerEmitter.h"
#include "UIGauge.h"
#include "SoundEffect.h"
#include "EnemyManager.h"
#include "EnemyBase.h"

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
GamePlayer::GamePlayer(const int _modelHandle)
	: PlayerBase(_modelHandle)
{
	//	各変数を初期化
	m_pos = CommonConstant::ORIGIN;
	m_dir = CommonConstant::ORIGIN;
	m_angle = CommonConstant::ORIGIN;
	m_size = CommonConstant::ORIGIN;
	m_life = START_LIFE;
	m_speed = 0.0f;
	m_emergeMoveTaim = 0.0f;
	m_invincibleTime = 0.0f;
	m_isInvincible = false;
	m_hitCircle.m_radius = HIT_RADIUS;
	m_hitCircle.m_centerPoint = CommonConstant::ORIGIN;

	//	まだどこも指していないので、NULLで初期化
	m_effectLaser = NULL;
	m_effectHit = NULL;
	m_effectExplosion = NULL;
	m_effectSpeedUp = NULL;
	m_effectGetItem = NULL;
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
GamePlayer::~GamePlayer()
{
	//	最終的な解放処理
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成処理
//-----------------------------------------------------------------------------
void GamePlayer::Create()
{
	//	エフェクトの読み込み
	m_effectLaser = new EffekseerEmitter("Data/Effect/Player/Laser/Laser.efk");
	m_effectHit = new EffekseerEmitter("Data/Effect/Hit/Hit.efk");
	m_effectExplosion = new EffekseerEmitter("Data/Effect/Explosion/Explosion.efk");
	m_effectSpeedUp = new EffekseerEmitter("Data/Effect/Player/SpeedUp/SpeedUp.efk");
	m_effectGetItem = new EffekseerEmitter("Data/Effect/Player/GetItem/GetItem.efk");
}

//-----------------------------------------------------------------------------
//	@brief	解放処理
//-----------------------------------------------------------------------------
void GamePlayer::Release()
{
	//	エフェクトの削除
	CommonSafe::Delete(m_effectLaser);
	CommonSafe::Delete(m_effectHit);
	CommonSafe::Delete(m_effectExplosion);
	CommonSafe::Delete(m_effectSpeedUp);
	CommonSafe::Delete(m_effectGetItem);
}

//-----------------------------------------------------------------------------
//	@brief	初期処理
//-----------------------------------------------------------------------------
void GamePlayer::Initialize()
{
	//	各変数を初期状態に設定
	m_pos = EMERGE_POS;
	m_dir = CommonConstant::ORIGIN;
	m_angle = VGet(0.0f, 1.4f, 0.0f);
	m_size = VGet(0.4f, 0.4f, 0.4f);
	m_life = START_LIFE;
	m_speed = MOVE_SPEED;
	m_emergeMoveTaim = 0.0f;
	m_isEmerge = true;
	m_invincibleTime = 0.0f;
	m_isInvincible = false;
	m_isInvincibleAfterLaser = false;
	m_isMove = false;
	m_shotWaitTime = 0.0f;
	m_isStopMove = false;
	m_isSceneProduction = false;
	m_isClearProduction = false;
	m_laserTime = 0.0f;
	m_isStartLaser = false;
	m_stayShotWaitTime = 0.0f;
	m_radian = 0.0f;
	m_isUseStayShot = true;
	m_laserHitRect.m_vertexTop = CommonConstant::ORIGIN;
	m_laserHitRect.m_vertexUnder = CommonConstant::ORIGIN;
	m_itemHitRect.m_vertexTop = CommonConstant::ORIGIN;
	m_itemHitRect.m_vertexUnder = CommonConstant::ORIGIN;
	m_offDraw = false;
	m_isStartEffectGetItem = false;

	//	デモ用
	m_demoDirChangeTime = 0.0f;
	m_demoTargetPos = CommonConstant::ORIGIN;

	//	当たり判定用の円形構造体の初期化
	m_hitCircle.m_radius = HIT_RADIUS;
	m_hitCircle.m_centerPoint = CommonConstant::ORIGIN;

	//	当たり判定用の長方形構造体の初期化
	m_hitRect.m_vertexTop = CommonConstant::ORIGIN;
	m_hitRect.m_vertexUnder = CommonConstant::ORIGIN;

	//	角度とサイズを割り当てる
	MV1SetRotationXYZ(m_modelHandle, m_angle);
	MV1SetScale(m_modelHandle, m_size);
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void GamePlayer::Update(Pad& _pad, ShotManager& _shot, StayShotManager& _stayShot, SoundEffect& _soundEffect, UIGauge& _gaugeUI)
{
	//	残機があり、移動可能のときのみ更新する
	const int notLife = 0;
	const bool isAlive = m_life > notLife;
	const bool isActive = !m_isStopMove && isAlive && !m_isDamage;
	if (isActive)
	{
		//	開始時の移動
		_EmergeMove();

		//	無敵時間の計測
		_MeasuringInvincibleTime();

		//	出現が終わっていたら、
		//	動けるようにする
		if (!m_isEmerge)
		{
			//	移動処理
			//	moving のノルムが0以外のときは、移動中
			VECTOR moving = MoveHelper::AskMoveAmount(m_dir, _pad, m_speed, true, true);
			m_isMove = VSize(moving) != 0;

			//	ポジションの更新
			m_pos = VAdd(m_pos, moving);

			//	範囲外指定
			_OutOfRange();

			//	射撃処理
			_Shot(_pad, _shot, _stayShot, _soundEffect);

			//	必殺技処理
			_SpecialAttack(_pad, _gaugeUI);

			//	角度処理
			AngleHelper::AskAngle(m_angle, _pad);
		}
	}

	//	シーン演出の開始
	if (m_isSceneProduction)
	{
		VECTOR tmpDir = VGet(1.0f, 0.0f, 0.0f);
		const float tmpSpeed = 5.0f;
		VECTOR moving = MoveHelper::AskMoveAmount(tmpDir, tmpSpeed);

		//	ポジションの更新
		m_pos = VAdd(m_pos, moving);
	}

	//	クリア演出の開始
	if (m_isClearProduction)
	{
		VECTOR tmpDir = VGet(1.0f, 0.0f, 0.0f);
		const float tmpSpeed = 10.0f;
		VECTOR moving = MoveHelper::AskMoveAmount(tmpDir, tmpSpeed);

		//	ポジションの更新
		m_pos = VAdd(m_pos, moving);
	}

	//	レーザー処理
	_Laser();

	//	レーザーエフェクト
	_LaseEffect(_soundEffect);

	//	ヒットエフェクト
	_OnHitEffect(_soundEffect);

	//	爆発エフェクト
	_OnEffectExplosion(_soundEffect);

	//	速度アップ
	_OnEffectSpeedUp(_soundEffect);

	//	アイテム取得エフェクト
	_OnEffectGetItem();

	//	モデルにポジションを割り当てる
	MV1SetPosition(m_modelHandle, m_pos);

	//	モデルに角度を割り当てる
	MV1SetRotationXYZ(m_modelHandle, m_angle);

	//	当たり判定がずれるので、
	//	モデルにポジションを割り当てた後に、
	//	当たり判定用の座標の更新する
	_UpdateHitPoint();

	//	残機をリザルトへ引き渡す
	HAND_OVER_RESULT->SetLife(m_life);
}

//-----------------------------------------------------------------------------
//	@brief	弾の処理
//-----------------------------------------------------------------------------
void GamePlayer::_Shot(Pad& _pad, ShotManager& _shot, StayShotManager& _stayShot, SoundEffect& _soundEffect)
{
	//	キーが押されているとき、弾を生成する
	const bool isInputActiveShot = KEY->Press(KEY_INPUT_K) || KEY->Press(KEY_INPUT_X) || _pad.GetXInputButton(PadInfo::PAD_BUTTON_KIND::BUTTON_A);
	const bool isActiveShot = isInputActiveShot && m_shotWaitTime-- <= 0.0f;
	if (isActiveShot)
	{
		//	弾リストに追加する
		_shot.RegisterOnList(ShotManager::SHOT_KIND::PLAYER_SHOT, m_pos, VGet(1.0f, 0.0f, 0.0f), SHOT_SPEED, _soundEffect);
		m_shotWaitTime = MAX_SHOT_WAIT_TIME;
	}

	//	入力がない時は、遅延を消す
	if (!isInputActiveShot) { m_shotWaitTime = 0.0f; }

	//	滞在ショットの使用状況
	m_stayShotWaitTime--;
	m_isUseStayShot = (m_stayShotWaitTime <= 0.0f);

	//	キーが押されていて、遅延がない時は
	//	滞在弾を生成する
	const bool isInputActiveStayShot = KEY->Press(KEY_INPUT_L) || KEY->Press(KEY_INPUT_C) || _pad.GetXInputButton(PadInfo::PAD_BUTTON_KIND::BUTTON_B);
	const bool isActiveStayShot = isInputActiveStayShot && m_stayShotWaitTime <= 0.0f;
	if (isActiveStayShot)
	{
		_stayShot.RegisterOnList(m_pos);
		m_stayShotWaitTime = MAX_STAY_SHOT_WAIT_TIME;
	}
}

//-----------------------------------------------------------------------------
//	@brief	必殺技処理
//-----------------------------------------------------------------------------
void GamePlayer::_SpecialAttack(Pad& _pad, UIGauge& _gaugeUI)
{
	//	キーが押されていて、ゲージが最大のときは、必殺発動
	const bool isInputActive = KEY->Press(KEY_INPUT_J) || KEY->Press(KEY_INPUT_Z) || _pad.GetXInputButton(PadInfo::PAD_BUTTON_KIND::BUTTON_X);
	const bool isActive = isInputActive && _gaugeUI.GetIsGaugeMax() && !PRODUCTION->GetIsSceneProduction();
	if (isActive)
	{
		//	必殺技を発動し、
		//	ゲージの初期化
		PRODUCTION->SetIsSpecialProduction(true);
		_gaugeUI.ResetGauge();
		m_isStartLaser = true;
	}

}

//-----------------------------------------------------------------------------
//	@brief	レーザー
//-----------------------------------------------------------------------------
void GamePlayer::_Laser()
{
	//	レーザー開始
	if (m_isStartLaser)
	{
		m_laserTime++;

		const bool isStart = m_laserTime <= 100.0f;
		const bool isLaser1 = m_laserTime >= 100.0f && m_laserTime <= 300.0f;
		const bool isReset = m_laserTime >= 300.0f;
		const bool isEnd = m_laserTime >= 360.0f;
		if(isStart) 
		{
			m_laserHitRect.m_vertexTop = VGet(m_pos.x, m_pos.y - LASER_SIZE, m_pos.z - LASER_SIZE_DEPTH);
			m_laserHitRect.m_vertexUnder = VGet(m_pos.x, m_pos.y + LASER_SIZE, m_pos.z + LASER_SIZE_DEPTH);

			//	パッドの振動機能
			const int vibrationPower = 500;
			PadInfo::Function::VibrationFunction(PadInfo::PAD_KIND::PAD_1, vibrationPower);
		}
		if (isLaser1)
		{
			//	パッドの振動機能の終了
			PadInfo::Function::EndVibrationFunction(PadInfo::PAD_KIND::PAD_1);

			m_laserHitRect.m_vertexUnder.x += LASER_SPEED;
			
			//	パッドの振動機能
			PadInfo::Function::VibrationFunction(PadInfo::PAD_KIND::PAD_1);
		}
		if (isReset)
		{
			m_laserHitRect.m_vertexTop = CommonConstant::ORIGIN;
			m_laserHitRect.m_vertexUnder = CommonConstant::ORIGIN;

			//	パッドの振動機能の終了
			PadInfo::Function::EndVibrationFunction(PadInfo::PAD_KIND::PAD_1);
		}
		if (isEnd)
		{
			m_laserTime = 0.0f;
			m_isStartLaser = false;

			//	パッドの振動機能の終了
			PadInfo::Function::EndVibrationFunction(PadInfo::PAD_KIND::PAD_1);
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	範囲外処理
//-----------------------------------------------------------------------------
void GamePlayer::_OutOfRange()
{
	//	画面外に行かないようにする
	const bool isOverX1 = m_pos.x <= MIN_POS_X;
	if (isOverX1) { m_pos.x = MIN_POS_X; }

	const bool isOverX2 = m_pos.x >= MAX_POS_X;
	if (isOverX2) { m_pos.x = MAX_POS_X; }

	const bool isOverY1 = m_pos.y <= MIN_POS_Y;
	if (isOverY1) { m_pos.y = MIN_POS_Y; }

	const bool isOverY2 = m_pos.y >= MAX_POS_Y;
	if (isOverY2) { m_pos.y = MAX_POS_Y; }
}

//-----------------------------------------------------------------------------
//	@brief	開始時の移動
//-----------------------------------------------------------------------------
void GamePlayer::_EmergeMove()
{
	//	出現時の演出
	if (m_isEmerge)
	{
		//	時間による演出の切り替え
		m_emergeMoveTaim++;
		const bool isEmergeMoveStart = m_emergeMoveTaim <= 30.0f;
		const bool isEmergeMove1 = m_emergeMoveTaim >= 30.0f && m_emergeMoveTaim <= 45.0f;
		const bool isEmergeMove2 = m_emergeMoveTaim >= 45.0f && m_emergeMoveTaim <= 70.0f;
		const bool isEmergeMoveEnd = m_emergeMoveTaim >= 70.0f;
		
		//	開始開始
		if (isEmergeMoveStart)
		{
			m_angle = VGet(0.0f, 1.4f, 0.0f);
			m_pos = EMERGE_POS;
			m_offDraw = false;
		}

		//	開始移動１
		if (isEmergeMove1)
		{
			//	先進
			m_dir = VGet(1.0f, 0.0f, 0.0f);
			VECTOR moving = MoveHelper::AskMoveAmount(m_dir, EMERGE_MOVE_SPEED);
			m_pos = VAdd(m_pos, moving);

			//	回転処理
			m_angle.x++;
			m_angle.z++;
		}

		//	開始移動２
		if (isEmergeMove2)
		{
			//	開始位置まで線形保管で移動
			float lerpSpeed = 0.1f;
			m_pos = CommonFunction::Lerp(m_pos, START_POS, lerpSpeed);

			//	回転処理
			m_angle.x++;
			m_angle.z++;
		}

		//	開始時の初期化
		if(isEmergeMoveEnd)
		{
			m_dir = CommonConstant::ORIGIN;
			m_angle = VGet(0.0f, 1.4f, 0.0f);
			m_pos = START_POS;
			m_emergeMoveTaim = 0.0f;
			m_isEmerge = false;
		}
	}

}

//-----------------------------------------------------------------------------
//	@brief	当たり判定用の点の更新
//-----------------------------------------------------------------------------
void GamePlayer::_UpdateHitPoint()
{
	//	当たり判定用の中心座標の更新
	m_hitCircle.m_centerPoint = VGet(m_pos.x - CENTER_CORRECTION, m_pos.y, m_pos.z);

	//	当たり判定用の座標の更新
	m_hitRect.m_vertexTop = VGet(m_pos.x + RECT_CORRECTION.x - CENTER_CORRECTION, m_pos.y + RECT_CORRECTION.y, m_pos.z + RECT_CORRECTION.z);
	m_hitRect.m_vertexUnder = VGet(m_pos.x - RECT_CORRECTION.x - CENTER_CORRECTION, m_pos.y - RECT_CORRECTION.y, m_pos.z - RECT_CORRECTION.z);

	//	アイテム用の当たり判定座標の更新
	//	当たり判定用の座標の更新
	m_itemHitRect.m_vertexTop = VSub(m_pos, ITEM_RECT_CORRECTION);
	m_itemHitRect.m_vertexUnder = VAdd(m_pos, ITEM_RECT_CORRECTION);
	m_itemHitRect.m_vertexTop.x -= CENTER_CORRECTION;
	m_itemHitRect.m_vertexUnder.x -= CENTER_CORRECTION;
}

//-----------------------------------------------------------------------------
//	@brief	レーザーエフェクト
//-----------------------------------------------------------------------------
void GamePlayer::_LaseEffect(SoundEffect& _soundEffect)
{
	//	レーザー開始
	if (m_isStartLaser)
	{
		const bool isLaseStart = m_effectLaser->GetPlayTime() == 0.0f;
		const bool isLaseStop = m_effectLaser->GetPlayTime() == 360.0f;

		//	レーザーエフェクトの開始
		if (isLaseStart)
		{
			m_effectLaser->SetPos(VGet(m_pos.x + CENTER_CORRECTION, m_pos.y, m_pos.z));
			m_effectLaser->SetPlaySpeed(1.5f);
			m_effectLaser->SetScale(VGet(25.0f, 25.0f, 25.0f));
			m_effectLaser->SetAngle(VGet(0.0f, -90.0f, 0.0f));
			m_effectLaser->OnPlayEffect();
			_soundEffect.OnPlaySound(_soundEffect.ONE_SE_KIND::ONE_SE_PLAYER_LASER);
		}

		//	レーザーエフェクトの終了
		if (isLaseStop)
		{
			m_effectLaser->OnEndEffect();
			return;
		}

		m_effectLaser->AddPlayTime();
		m_effectLaser->Update();
	}
}

//-----------------------------------------------------------------------------
//	@brief	ヒットエフェクト
//-----------------------------------------------------------------------------
void GamePlayer::_OnHitEffect(SoundEffect& _soundEffect)
{
	//	ダメージが入ったとき
	if (m_isDamage)
	{
		const bool isHitStart = m_effectHit->GetPlayTime() == 0.0f;
		const bool isHitEnd = m_effectHit->GetPlayTime() == 100.0f;

		//	エフェクトの開始処理
		if(isHitStart) 
		{
			m_effectHit->SetPos(VGet(m_pos.x - CENTER_CORRECTION, m_pos.y, m_pos.z));
			m_effectHit->SetScale(VGet(3.0f, 3.0f, 3.0f));
			m_effectHit->OnPlayEffect();
			_soundEffect.OnPlaySound(_soundEffect.SE_KIND::SE_HIT);
		}

		//	エフェクトの終了処理
		if (isHitEnd)
		{
			m_effectHit->OnEndEffect();
			return;
		}

		//	エフェクトの更新
		m_effectHit->AddPlayTime();
		m_effectHit->Update();
	}
}

//-----------------------------------------------------------------------------
//	@brief	爆発エフェクト
//-----------------------------------------------------------------------------
void GamePlayer::_OnEffectExplosion(SoundEffect& _soundEffect)
{
	//	ダメージが入ったとき
	if (m_isDamage)
	{
		const bool isExplosionStart = m_effectExplosion->GetPlayTime() == 0.0f;
		const bool isExplosion1 = m_effectExplosion->GetPlayTime() == 50.0f;
		const bool isExplosionEnd = m_effectExplosion->GetPlayTime() == 100.0f;

		//	爆発エフェクトの開始
		if (isExplosionStart)
		{
			m_effectExplosion->SetPos(m_pos);
			m_effectExplosion->SetPlaySpeed(3.0f);
			m_effectExplosion->SetScale(VGet(2.0f, 2.0f, 2.0f));
			m_effectExplosion->OnPlayEffect();

			//	パッドの振動機能
			PadInfo::Function::VibrationFunction(PadInfo::PAD_KIND::PAD_1);
		}

		//	プレイヤーモデルの非表示と
		//	SEの再生
		if (isExplosion1) 
		{
			m_offDraw = true;
			_soundEffect.OnPlaySound(_soundEffect.SE_KIND::SE_EXPLOSION);

			//	パッドの振動機能の終了
			PadInfo::Function::EndVibrationFunction(PadInfo::PAD_KIND::PAD_1);
		}

		//	爆発エフェクトの終了
		if (isExplosionEnd)
		{
			m_effectExplosion->OnEndEffect();
			m_isInvincible = true;
			m_isEmerge = true;
			m_isDamage = false;

			return;
		}
		
		//	エフェクトの更新
		m_effectExplosion->AddPlayTime();
		m_effectExplosion->Update();
	}
}

//-----------------------------------------------------------------------------
//	@brief	速度アップエフェクト
//-----------------------------------------------------------------------------
void GamePlayer::_OnEffectSpeedUp(SoundEffect& _soundEffect)
{
	//	シーン演出時
	if (m_isSceneProduction)
	{
		const bool isSpeedUpStart = m_effectSpeedUp->GetPlayTime() == 0.0f;
		const bool isSpeedUpEnd = m_effectSpeedUp->GetPlayTime() == 120.0f;

		//	エフェクトの開始処理
		if (isSpeedUpStart)
		{
			const float effectPosX = -50.0f;
			m_effectSpeedUp->SetPos(VGet(effectPosX, m_pos.y, m_pos.z));
			m_effectSpeedUp->SetScale(VGet(3.0f, 3.0f, 3.0f));
			m_effectSpeedUp->SetAngle(VGet(20.0f, 0.0f, 0.0f));
			m_effectSpeedUp->OnPlayEffect();
			_soundEffect.OnPlaySound(_soundEffect.ONE_SE_KIND::ONE_SE_WIND);
		}

		//	エフェクトの終了処理
		if (isSpeedUpEnd)
		{
			m_effectSpeedUp->OnEndEffect();
			return;
		}

		//	エフェクトの更新
		m_effectSpeedUp->AddPlayTime();
		m_effectSpeedUp->Update();
	}
}

//-----------------------------------------------------------------------------
//	@brief	アイテム取得時のエフェクト
//-----------------------------------------------------------------------------
void GamePlayer::_OnEffectGetItem()
{
	//	アイテム取得時
	if (m_isStartEffectGetItem)
	{
		const bool isGetItemStart = m_effectGetItem->GetPlayTime() == 0.0f;
		const bool isGetItemEnd = m_effectGetItem->GetPlayTime() == 110.0f;
		
		//	常にプレイヤーを追う
		m_effectGetItem->SetPos(VGet(m_pos.x - CENTER_CORRECTION, m_pos.y, m_pos.z));
		
		//	エフェクトの開始処理
		if (isGetItemStart)
		{
			m_effectGetItem->SetScale(VGet(7.0f, 5.0f, 7.0f));
			m_effectGetItem->SetPlaySpeed(2.0f);
			m_effectGetItem->OnPlayEffect();
		}

		//	エフェクトの終了処理
		if (isGetItemEnd)
		{
			m_effectGetItem->OnEndEffect();
			m_isStartEffectGetItem = false;
			return;
		}

		//	エフェクトの更新
		m_effectGetItem->AddPlayTime();
		m_effectGetItem->Update();
	}
}

//-----------------------------------------------------------------------------
//	@brief	無敵時間の計測
//-----------------------------------------------------------------------------
void GamePlayer::_MeasuringInvincibleTime()
{
	//	無敵状態のとき
	const bool isInvincible = m_isInvincible && !m_isEmerge;
	if (isInvincible)
	{
		//	無敵時間の計測し、
		//	最大になったら、無敵を解除する
		m_invincibleTime++;
		const bool isMaxInvincibleTime = m_invincibleTime >= MAX_INVINCIBLE_TIME_1;
		if (isMaxInvincibleTime)
		{
			//	無敵状態を解除する
			m_invincibleTime = 0.0f;
			m_isInvincible = false;
		}
	}

	//	レーザー使用後の無敵処理
	if (m_isInvincibleAfterLaser)
	{
		//	無敵時間の計測し、
		//	最大になったら、無敵を解除する
		m_invincibleTime++;
		const bool isMaxInvincibleTime = m_invincibleTime >= MAX_INVINCIBLE_TIME_2;
		if (isMaxInvincibleTime)
		{
			//	無敵状態を解除する
			m_invincibleTime = 0.0f;
			m_isInvincibleAfterLaser = false;
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放処理
//-----------------------------------------------------------------------------
void GamePlayer::_FinalRelease()
{
	//	エフェクトの削除
	CommonSafe::Delete(m_effectLaser);
	CommonSafe::Delete(m_effectHit);
	CommonSafe::Delete(m_effectExplosion);
	CommonSafe::Delete(m_effectSpeedUp);
	CommonSafe::Delete(m_effectGetItem);
}
