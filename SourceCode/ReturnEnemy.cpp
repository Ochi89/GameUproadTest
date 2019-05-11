
//=============================================================================
//	@file	ReturnEnemy.cpp
//	@brief	カムバックエネミー
//	@autor	Takuya Ochi
//	@date	2018/11/16
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "ReturnEnemy.h"
#include "Common.h"
#include "MoveHelper.h"
#include "AngleHelper.h"
#include "PlayerManager.h"
#include "ShotManager.h"
#include "SoundEffect.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const float ReturnEnemy::ADD_RADIAN_SPEED = 0.1f;	//	ラジアン角の加算速度

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
ReturnEnemy::ReturnEnemy(const int _modelHandle)
	: EnemyBase(_modelHandle)
{
	//	各変数を初期化
	m_pos = CommonConstant::ORIGIN;
	m_dir = CommonConstant::ORIGIN;
	m_angle = CommonConstant::ORIGIN;
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
ReturnEnemy::~ReturnEnemy()
{
	//	最終的な解放処理
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成処理
//-----------------------------------------------------------------------------
void ReturnEnemy::Create()
{
	// 処理なし
}

//-----------------------------------------------------------------------------
//	@brief	解放処理
//-----------------------------------------------------------------------------
void ReturnEnemy::Release()
{
	// 処理なし
}

//-----------------------------------------------------------------------------
//	@brief	初期処理
//-----------------------------------------------------------------------------
void ReturnEnemy::Initialize()
{
	//	各変数を初期状態に設定
	m_pos = VGet(0.0f, 50.0f, 0.0f);
	m_dir = CommonConstant::ORIGIN;
	m_angle = VGet(0.0f, 5.0f, 0.0f);
	m_behaviorTime = 0.0f;
	m_moveSpeed = 0.0f;
	m_radian = 0.0f;
	m_shotWaitTime = 0.0f;
	m_aliveTime = 0.0f;
	m_formationID = 0;
	m_entryID = 0;
	m_entryKind = 0;
	m_isHavingItems = false;
	m_isAttenuation = false;
	m_isHit = false;
	m_isOffDraw = false;
	m_isDeleate = false;

	//	当たり判定用の構造体の初期化
	m_hitCircle.m_radius = HIT_RADIUS;
	m_hitCircle.m_centerPoint = CommonConstant::ORIGIN;

	//	角度を割り当てる
	MV1SetRotationXYZ(m_modelHandle, m_angle);
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void ReturnEnemy::Update(PlayerManager& _playerManager, ShotManager& _shotManager, SoundEffect& _soundEffect)
{
	//	ヒットしていないときか、
	//	必殺技以外のとき
	const bool isActive = !m_isHit && !PRODUCTION->GetIsSpecialProduction();
	if (isActive)
	{
		//	向きの切り替え
		_ChangeDir();

		//	移動処理
		VECTOR moving = MoveHelper::AskMoveAmount(m_dir, m_moveSpeed);

		//	ショットを使用する
		if (m_isUseShot)
		{
			//	ショットの登録
			_ShotEntry(_playerManager, _shotManager);
		}

		//	ポジションの更新
		m_pos = VAdd(m_pos, moving);

		//	奥行きの最小値
		if (m_pos.z <= 0.0f) { m_pos.z = 0.0f; }

		//	モデルにポジションを割り当てる
		MV1SetPosition(m_modelHandle, m_pos);

		//	モデルに角度を割り当てる
		MV1SetRotationXYZ(m_modelHandle, m_angle);
	}

	//	モデルの色の割り当て
	_ChangeColor();

	//	爆発エフェクト
	_OnEffectExplosion(_soundEffect);

	//	当たり判定がずれるので、
	//	モデルにポジションを割り当てた後に、
	//	当たり判定用の座標の更新する
	_UpdateHitPoint();
}

//-----------------------------------------------------------------------------
//	@brief	向きの切り替え
//-----------------------------------------------------------------------------
void ReturnEnemy::_ChangeDir()
{
	m_behaviorTime++;

	bool isPattern1 = m_behaviorTime >= 0.0f && m_behaviorTime <= 60.0f;
	bool isPattern2 = m_behaviorTime >= 60.0f && m_behaviorTime <= 90.0f;
	bool isPattern3 = m_behaviorTime >= 90.0f && m_behaviorTime <= 1000.0f;

	if (isPattern1)
	{
		const float lerpSpeed = 0.2f;
		VECTOR tergetDir = VGet(1.0f, 0.0f, 0.0f);
		m_dir = CommonFunction::Lerp(m_dir, tergetDir, lerpSpeed);
		m_moveSpeed = 3.0f;
		m_isAttenuation = true;
	}
	if (isPattern2)
	{
		const float lerpSpeed = 0.2f;
		VECTOR tergetDir = VGet(0.0f, 0.0f, -1.0f);
		m_dir = CommonFunction::Lerp(m_dir, tergetDir, lerpSpeed);
		m_moveSpeed = 3.0f;
		m_isAttenuation = false;
	}
	if (isPattern3)
	{
		const float lerpSpeed = 0.5f;

		//	ラジアン角の加算
		m_radian += ADD_RADIAN_SPEED;
		VECTOR tergetDir = VGet(-1.0f, sinf(m_radian), 0.0f);
		m_dir = CommonFunction::Lerp(m_dir, tergetDir, lerpSpeed);
		m_moveSpeed = 1.0f;
	}
}

//-----------------------------------------------------------------------------
//	@brief	当たり判定用の点の更新
//-----------------------------------------------------------------------------
void ReturnEnemy::_UpdateHitPoint()
{
	//	当たり判定用の中心座標の更新
	m_hitCircle.m_centerPoint = VGet(m_pos.x, m_pos.y - CENTER_CORRECTION, m_pos.z);

	//	当たり判定用の座標の更新
	m_hitRect.m_vertexTop = VGet(m_pos.x + RECT_CORRECTION.x, m_pos.y + RECT_CORRECTION.y, m_pos.z + RECT_CORRECTION.z);
	m_hitRect.m_vertexUnder = VGet(m_pos.x - RECT_CORRECTION.x, m_pos.y - RECT_CORRECTION.y, m_pos.z - RECT_CORRECTION.z);
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放処理
//-----------------------------------------------------------------------------
void ReturnEnemy::_FinalRelease()
{
	// 処理なし
}

