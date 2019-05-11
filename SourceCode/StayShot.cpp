
//=============================================================================
//	@file	StayShot.cpp
//	@brief	滞在弾ひとつ分の処理
//	@autor	Takuya Ochi
//	@date	2018/12/25
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "StayShot.h"
#include "Common.h"
#include "EffekseerEmitter.h"
#include "SoundEffect.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const VECTOR	StayShot::MOVE_DIR = VGet(1.0f, 0.0f, 0.0f);			//	移動向き
const float		StayShot::MOVE_SPEED = 0.5f;							//	移動速度
const float		StayShot::MOVE_HIT_RADIUS = 2.0f;						//	移動中の当たり判定用の半径
const float		StayShot::STAY_HIT_RADIUS = 5.0f;						//	滞在中の当たり判定用の半径
const float		StayShot::MIN_HIT_RADIUS = 0.1f;						//	当たり判定用の最小半径
const float		StayShot::MAX_STAY_TIME = 80.0f;						//	滞在中の時間の最大
const float		StayShot::EFFECT_SCALE = 0.01f;							//	エフェクトの拡大速度
const float		StayShot::MAX_X_POS = 110.0f;							//	X座標の最大
const VECTOR	StayShot::ADJUSTMENT_RECT = VGet(2.0f, 2.0f, 2.0f);		//	当たり判定調整用

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
StayShot::StayShot()
{
	//	各変数の初期化
	m_pos = CommonConstant::ORIGIN;
	m_isOneTime = false;
	m_isChangeStay = false;
	m_stayTime = 0.0f;
	m_isShotEnd = false;
	m_isMoveShotSeStart = false;
	m_isStayShotSeStart = false;

	//	当たり判定用の構造体の初期化
	m_hitCircle.m_radius = MOVE_HIT_RADIUS;
	m_hitCircle.m_centerPoint = CommonConstant::ORIGIN;

	m_hitRect.m_vertexTop = CommonConstant::ORIGIN;
	m_hitRect.m_vertexUnder = CommonConstant::ORIGIN;

	//	まだどこも指していないので、NULLで初期化
	m_effectMove = NULL;
	m_effectStay = NULL;
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
StayShot::~StayShot()
{
	//	最終的な解放処理
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	初期処理
//-----------------------------------------------------------------------------
void StayShot::Initialize()
{
	//	各変数の初期化
	m_pos = CommonConstant::ORIGIN;
	m_isOneTime = false;
	m_isChangeStay = false;
	m_stayTime = 0.0f;
	m_isShotEnd = false;
	m_isMoveShotSeStart = false;
	m_isStayShotSeStart = false;

	//	当たり判定用の構造体の初期化
	m_hitCircle.m_radius = MOVE_HIT_RADIUS;
	m_hitCircle.m_centerPoint = CommonConstant::ORIGIN;

	m_hitRect.m_vertexTop = CommonConstant::ORIGIN;
	m_hitRect.m_vertexUnder = CommonConstant::ORIGIN;

	//	エフェクトの読み込み
	m_effectMove = new EffekseerEmitter("Data/Effect/Player/StayShot/Move/Move.efk");
	m_effectStay = new EffekseerEmitter("Data/Effect/Player/StayShot/Stay/Stay.efk");
	m_effectScale = 1.0f;
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void StayShot::Update(SoundEffect& _soundEffect)
{
	if (m_isChangeStay)
	{
		//	滞在処理
		_Stay(_soundEffect);
	}
	else
	{
		//	移動処理
		_Move(_soundEffect);
	}

	//	当たり判定用の座標の更新する
	_UpdateHitPoint();
}

//-----------------------------------------------------------------------------
//	@brief	移動処理
//-----------------------------------------------------------------------------
void StayShot::_Move(SoundEffect& _soundEffect)
{
	//	SE
	if (!m_isMoveShotSeStart) { _soundEffect.OnPlaySound(_soundEffect.SE_KIND::SE_MOVE_SHOT); m_isMoveShotSeStart = true; }

	//	移動
	VECTOR velocity = VScale(MOVE_DIR, MOVE_SPEED);
	m_pos = VAdd(m_pos, velocity);

	m_effectMove->SetPos(m_pos);
	m_effectMove->SetPlaySpeed(2.0f);
	m_effectMove->OnPlayEffect();

	//	範囲外に出たら、エフェクトを消す
	const bool isArrivalRight = m_pos.x >= MAX_X_POS;
	if (isArrivalRight) { m_effectMove->OnEndEffect(); }

	m_effectMove->Update();
}

//-----------------------------------------------------------------------------
//	@brief	滞在処理
//-----------------------------------------------------------------------------
void StayShot::_Stay(SoundEffect& _soundEffect)
{
	if (!m_isShotEnd)
	{
		//	SE
		if (!m_isStayShotSeStart)
		{
			_soundEffect.OnPlaySound(_soundEffect.SE_KIND::SE_STAY_SHOT);
			m_isStayShotSeStart = true;
		}

		m_stayTime++;

		//	その場に滞在する
		const bool isStay = m_stayTime <= MAX_STAY_TIME;
		if (isStay)
		{
			//	当たり判定を大きくする
			const float lerpSpeed = 0.5f;
			m_hitCircle.m_radius = CommonFunction::Lerp(m_hitCircle.m_radius, STAY_HIT_RADIUS, lerpSpeed);

			//	エフェクト
			m_effectScale += EFFECT_SCALE;
			m_effectStay->SetPos(m_pos);
			m_effectStay->SetPlaySpeed(1.5f);
			m_effectStay->SetScale(VGet(m_effectScale, m_effectScale, m_effectScale));
			m_effectStay->OnPlayEffect();
			m_effectMove->OnEndEffect();
		}

		//	当たり判定をだんだん小さくする
		const bool isStayEnd = m_stayTime >= MAX_STAY_TIME;
		if (isStayEnd)
		{
			//	当たり判定を小さくする
			const float lerpSpeed = 0.1f;
			m_hitCircle.m_radius = CommonFunction::Lerp(m_hitCircle.m_radius, 0.0f, lerpSpeed);

			//	エフェクト
			m_effectScale -= EFFECT_SCALE;
			m_effectStay->SetScale(VGet(m_effectScale, m_effectScale, m_effectScale));
		}

		//	当たり判定が一定数より小さくなったら、
		//	ショットを終了させる
		const bool isShotEnd = m_hitCircle.m_radius <= MIN_HIT_RADIUS;
		if (isShotEnd) { m_isShotEnd = true; m_effectStay->OnEndEffect(); }

		m_effectStay->Update();
	}
}

//-----------------------------------------------------------------------------
//	@brief	当たり判定用の点の更新
//-----------------------------------------------------------------------------
void StayShot::_UpdateHitPoint()
{
	//	当たり判定用の中心座標の更新
	m_hitCircle.m_centerPoint = m_pos;

	m_hitRect.m_vertexTop = VSub(m_pos, ADJUSTMENT_RECT);
	m_hitRect.m_vertexUnder = VAdd(m_pos, ADJUSTMENT_RECT);
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放処理
//-----------------------------------------------------------------------------
void StayShot::_FinalRelease()
{
	//	エフェクトの削除
	CommonSafe::Delete(m_effectMove);
	CommonSafe::Delete(m_effectStay);
}
