
//=============================================================================
//	@file	PlayerBase.cpp
//	@brief	プレイヤーベース
//	@autor	Takuya Ochi
//	@date	2018/11/14
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "PlayerBase.h"
#include "ShotManager.h"
#include "StayShotManager.h"
#include "EffekseerEmitter.h"
#include "UIGauge.h"
#include "Pad.h"
#include "Common.h"
#include "SoundEffect.h"
#include "EnemyManager.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const VECTOR	PlayerBase::TITLE_POS = VGet(10.0f, 25.0f, 40.0f);			//	タイトル時の座標
const VECTOR	PlayerBase::RESULT_POS = VGet(-50.0f, 50.0f, 0.0f);			//	リザルト時の座標
const VECTOR	PlayerBase::EMERGE_POS = VGet(-70.0f, 50.0f, -40.0f);		//	出現時の座標
const VECTOR	PlayerBase::START_POS = VGet(-30.0f, 50.0f, 0.0f);			//	開始時の座標
const VECTOR	PlayerBase::RECT_CORRECTION = VGet(0.5f, 0.5f, 1.0f);		//	座標の補正
const VECTOR	PlayerBase::ITEM_RECT_CORRECTION = VGet(7.0f, 2.0f, 1.0f);	//	アイテム座標の補正
const float		PlayerBase::EMERGE_MOVE_SPEED = 4.0f;						//	出現時の移動速度
const float		PlayerBase::SHOT_SPEED = 7.0f;								//	ショット速度
const float		PlayerBase::MAX_SHOT_WAIT_TIME = 8.0f;						//	ショットの遅延時間
const float		PlayerBase::MAX_STAY_SHOT_WAIT_TIME = 70.0f;				//	滞在ショットの遅延時間
const float		PlayerBase::CENTER_CORRECTION = 5.0f;						//	中心座標の補正
const float		PlayerBase::HIT_RADIUS = 0.5f;								//	当たり判定用の半径
const float		PlayerBase::MAX_INVINCIBLE_TIME_1 = 120.0f;					//	当たり判定の無効時間の最大
const float		PlayerBase::MAX_INVINCIBLE_TIME_2 = 30.0f;					//	当たり判定の無効時間の最大
const float		PlayerBase::MOVE_SPEED = 1.2f;								//	移動速度
const float		PlayerBase::MIN_POS_X = -90.0f;								//	ｘ座標の最小
const float		PlayerBase::MAX_POS_X = 95.0f;								//	ｘ座標の最大
const float		PlayerBase::MIN_POS_Y = 5.0f;								//	ｙ座標の最小
const float		PlayerBase::MAX_POS_Y = 105.0f;								//	ｙ座標の最大
const float		PlayerBase::LASER_SIZE = 40.0f;								//	レーザーサイズ
const float		PlayerBase::LASER_SIZE_DEPTH = 60.0f;						//	レーザーサイズの奥行き
const float		PlayerBase::LASER_SPEED = 20.0f;							//	レーザー速度
const float		PlayerBase::ADD_RADIAN_SPEED = 0.1f;						//	ラジアン角の加算速度
const float		PlayerBase::MAX_DEMO_SHOT_FIRING_TIME = -3.0f;				//	ショットの発射時間
const float		PlayerBase::MAX_DEMO_STAY_SHOT_FIRING_TIME = -10.0f;		//	滞在ショットの発射時間
const float		PlayerBase::MAX_DEMO_CHANGE_DIR_WAIT_TIME = 180.0f;			//	デモ用の向き切り替え遅延時間
const float		PlayerBase::MAX_DEMO_DISTANCE_TO_ENEMY = 80.0f;				//	デモ用の敵との距離の間隔
const int		PlayerBase::START_LIFE = 5;									//	開始時の残機
const int		PlayerBase::ZERO_LIFE = 0;									//	残機なし

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
PlayerBase::PlayerBase(const int _modelHandle)
{
	//	モデルの複製
	m_modelHandle = MV1DuplicateModel(_modelHandle);
	CommonDebug::Assert((m_modelHandle <= -1), " [ PlayerBase.cpp ] : error : missing duplicat model.");
	m_flashingTime = 0.0f;
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
PlayerBase::~PlayerBase()
{
	//	モデルのアンロード
	MV1DeleteModel(m_modelHandle);
}

//-----------------------------------------------------------------------------
//	@brief	描画
//-----------------------------------------------------------------------------
void PlayerBase::Draw()
{
	//	無敵状態のとき
	const bool isInvincible = m_isInvincible && !m_isEmerge;
	if (isInvincible)
	{
		m_flashingTime++;
		const bool isFlashingEnd = m_flashingTime > 10.0f;
		if (isFlashingEnd) { m_flashingTime = 0.0f; }

		const bool isFlashing = m_flashingTime >= 5.0f && m_flashingTime <= 10.0f;
		if (isFlashing) { return; }
	}

	//	描画無視しないときは、
	//	モデルを描画
	const bool isDraw = !m_offDraw && m_life > ZERO_LIFE;
	if (isDraw)
	{
		//	モデルの描画
		MV1DrawModel(m_modelHandle);
	}
}

//-----------------------------------------------------------------------------
//	@brief	初期位置の getter
//-----------------------------------------------------------------------------
const VECTOR PlayerBase::GetStartPos() const
{
	return START_POS;
}

//-----------------------------------------------------------------------------
//	@brief	sinカーブによる向きを求める
//-----------------------------------------------------------------------------
void PlayerBase::_AskSinCurveDirection()
{
    //	ラジアン角の加算
    m_radian += ADD_RADIAN_SPEED;

    //	向きを求める
    //	sinカーブを使い、浮遊するようなカーブ
    m_dir.y = sinf(m_radian);
}
