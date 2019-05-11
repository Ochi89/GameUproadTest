
//=============================================================================
//	@file	UpDownEnemy.cpp
//	@brief	旋回エネミー
//	@autor	Takuya Ochi
//	@date	2018/12/20
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "UpDownEnemy.h"
#include "Common.h"
#include "MoveHelper.h"
#include "AngleHelper.h"
#include "PlayerManager.h"
#include "ShotManager.h"
#include "SoundEffect.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const float UpDownEnemy::MOVE_ANGLE_SPEED = 20.0f;				//	回転角度の回転速度
const float UpDownEnemy::MAX_MOVE_ANGLE_WAIT_TIME = 6.5f;		//	回転速度の待機時間の最大
const float UpDownEnemy::MAX_MOVE_ANGLE = 150.0f;				//	回転角度の最大
const float UpDownEnemy::MIN_MOVE_ANGLE = -150.0f;				//	回転角度の最小

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
UpDownEnemy::UpDownEnemy(const int _modelHandle)
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
UpDownEnemy::~UpDownEnemy()
{
	//	最終的な解放処理
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成処理
//-----------------------------------------------------------------------------
void UpDownEnemy::Create()
{
	// 処理なし
}

//-----------------------------------------------------------------------------
//	@brief	解放処理
//-----------------------------------------------------------------------------
void UpDownEnemy::Release()
{
	// 処理なし
}

//-----------------------------------------------------------------------------
//	@brief	初期処理
//-----------------------------------------------------------------------------
void UpDownEnemy::Initialize()
{
	//	各変数を初期状態に設定
	m_pos = VGet(0.0f, 50.0f, 0.0f);
	m_dir = CommonConstant::ORIGIN;
	m_angle = VGet(0.0f, 5.0f, 0.0f);
	m_behaviorTime = 0.0f;
	m_moveSpeed = 1.5f;
	m_moveAngle = 180.0f;
	m_moveAngleWaitTime = 0.0f;
	m_shotWaitTime = 0.0f;
	m_aliveTime = 0.0f;
	m_formationID = 0;
	m_entryID = 0;
	m_entryKind = 0;
	m_isOnReverse = false;
	m_isOneTime = false;
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
void UpDownEnemy::Update(PlayerManager& _playerManager, ShotManager& _shotManager, SoundEffect& _soundEffect)
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
void UpDownEnemy::_ChangeDir()
{
	m_behaviorTime++;

	//	時間りよるパターンの切り替え
	bool isPattern1 = m_behaviorTime >= 0.0f && m_behaviorTime <= 90.0f;
	bool isPattern2 = m_behaviorTime >= 90.0f && m_behaviorTime <= 180.0f;
	bool isPattern3 = m_behaviorTime >= 180.0f && m_behaviorTime <= 270.0f;

	//	反転処理
	if (!m_isOneTime)
	{
		//	y軸が一定より低かったら、
		//	移動向きを反転させる
		bool isActiveReverse = m_pos.y <= 0.0f;
		if (isActiveReverse) { m_isOnReverse = true; }
		m_isOneTime = true;
	}

	//	向きを求める
	if (m_isOnReverse)
	{
		//	下から上に
		if (isPattern1) { m_dir = VGet(0.0f, 1.0f, 0.0f); 	m_isAttenuation = true; }
		if (isPattern2)
		{
			const float lerpSpeed = 0.2f;
			VECTOR tergetDir = VGet(0.0f, 0.0f, -1.0f);
			m_dir = CommonFunction::Lerp(m_dir, tergetDir, lerpSpeed);
			m_isAttenuation = false;
		}
		if (isPattern3)
		{
			const float lerpSpeed = 0.2f;
			VECTOR tergetDir = VGet(0.0f, -1.0f, 0.0f);
			m_dir = CommonFunction::Lerp(m_dir, tergetDir, lerpSpeed);

		}
	}
	else
	{
		//	上から下に
		if (isPattern1) { m_dir = VGet(0.0f, -1.0f, 0.0f); 	m_isAttenuation = true; }
		if (isPattern2)
		{
			const float lerpSpeed = 0.2f;
			VECTOR tergetDir = VGet(0.0f, 0.0f, -1.0f);
			m_dir = CommonFunction::Lerp(m_dir, tergetDir, lerpSpeed);
			m_isAttenuation = false;
		}
		if (isPattern3)
		{
			const float lerpSpeed = 0.2f;
			VECTOR tergetDir = VGet(0.0f, 1.0f, 0.0f);
			m_dir = CommonFunction::Lerp(m_dir, tergetDir, lerpSpeed);

		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	当たり判定用の点の更新
//-----------------------------------------------------------------------------
void UpDownEnemy::_UpdateHitPoint()
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
void UpDownEnemy::_FinalRelease()
{
	// 処理なし
}

