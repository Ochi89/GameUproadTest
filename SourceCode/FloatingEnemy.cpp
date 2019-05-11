
//=============================================================================
//	@file	FloatingEnemy.cpp
//	@brief	浮遊エネミー
//	@autor	Takuya Ochi
//	@date	2018/11/16
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "FloatingEnemy.h"
#include "Common.h"
#include "MoveHelper.h"
#include "AngleHelper.h"
#include "PlayerManager.h"
#include "ShotManager.h"
#include "SoundEffect.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const float FloatingEnemy::ADD_RADIAN_SPEED = 0.1f;	//	ラジアン角の加算速度

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
FloatingEnemy::FloatingEnemy(const int _modelHandle)
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
FloatingEnemy::~FloatingEnemy()
{
	//	最終的な解放処理
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成処理
//-----------------------------------------------------------------------------
void FloatingEnemy::Create()
{
	// 処理なし
}

//-----------------------------------------------------------------------------
//	@brief	解放処理
//-----------------------------------------------------------------------------
void FloatingEnemy::Release()
{
	// 処理なし
}

//-----------------------------------------------------------------------------
//	@brief	初期処理
//-----------------------------------------------------------------------------
void FloatingEnemy::Initialize()
{
	//	各変数を初期状態に設定
	m_pos = VGet(0.0f, 50.0f, 0.0f);
	m_dir = CommonConstant::ORIGIN;
	m_angle = VGet(0.0f, 5.0f, 0.0f);
	m_behaviorTime = 0.0f;
	m_moveSpeed = 1.0f;
	m_radian = 0.0f;
	m_shotWaitTime = 0.0f;
	m_aliveTime = 0.0f;
	m_formationID = 0;
	m_entryID = 0;
	m_entryKind = 0;
	m_isHavingItems = false;
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
void FloatingEnemy::Update(PlayerManager& _playerManager, ShotManager& _shotManager, SoundEffect& _soundEffect)
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
void FloatingEnemy::_ChangeDir()
{
	//	ラジアン角の加算
	m_radian += ADD_RADIAN_SPEED;

	//	向きを求める
	m_dir.x = -1.0f;
	m_dir.y = sinf(m_radian);		//	sinカーブを使い、浮遊するようなカーブ

	//	正規化
	if (VSquareSize(m_dir) != 0) { m_dir = VNorm(m_dir); }
}

//-----------------------------------------------------------------------------
//	@brief	当たり判定用の点の更新
//-----------------------------------------------------------------------------
void FloatingEnemy::_UpdateHitPoint()
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
void FloatingEnemy::_FinalRelease()
{
	// 処理なし
}

