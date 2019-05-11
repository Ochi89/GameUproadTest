
//=============================================================================
//	@file	Player.cpp
//	@brief	プレイヤー
//	@autor	Takuya Ochi
//	@date	2018/11/14
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "TitlePlayer.h"
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
TitlePlayer::TitlePlayer(const int _modelHandle)
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
TitlePlayer::~TitlePlayer()
{
    //	最終的な解放処理
    _FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成処理
//-----------------------------------------------------------------------------
void TitlePlayer::Create()
{
    //  処理なし
}

//-----------------------------------------------------------------------------
//	@brief	解放処理
//-----------------------------------------------------------------------------
void TitlePlayer::Release()
{
    //  処理なし
}

//-----------------------------------------------------------------------------
//	@brief	初期処理
//-----------------------------------------------------------------------------
void TitlePlayer::Initialize()
{
    //	各変数を初期状態に設定
    m_pos = TITLE_POS;
    m_dir = CommonConstant::ORIGIN;
    m_angle = VGet(0.0f, 1.4f, 0.0f);
    m_size = VGet(1.0f, 1.0f, 1.0f);
    m_life = START_LIFE;
    m_invincibleTime = 0.0f;
    m_isInvincible = false;
    m_isStopMove = false;
    m_offDraw = false;

    //	角度とサイズを割り当てる
    MV1SetRotationXYZ(m_modelHandle, m_angle);
    MV1SetScale(m_modelHandle, m_size);
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void TitlePlayer::Update()
{
    //  開始時の演出
    _StartProdoction();

    //  浮遊演出
    _FloatingProdoction();
}

//-----------------------------------------------------------------------------
//	@brief	開始時の演出
//-----------------------------------------------------------------------------
void TitlePlayer::_StartProdoction()
{
    //  開始していなかったら、処理をスキップ
    bool isNotStart = !PRODUCTION->GetIsTitleProduction();
    if (isNotStart) { return; }

    //  真っ直ぐに進む
    VECTOR tmpDir = VGet(1.0f, 0.0f, 0.0f);
    const float tmpSpeed = 10.0f;
    VECTOR moving = MoveHelper::AskMoveAmount(tmpDir, tmpSpeed);
    m_pos = VAdd(m_pos, moving);

    //	モデルにポジションを割り当てる
    MV1SetPosition(m_modelHandle, m_pos);
}

//-----------------------------------------------------------------------------
//	@brief	浮遊演出
//-----------------------------------------------------------------------------
void TitlePlayer::_FloatingProdoction()
{
    //  開始したら、処理をスキップ
    bool isStart = PRODUCTION->GetIsTitleProduction();
    if (isStart) { return; }

    //  sinカーブによる向きを求める
    _AskSinCurveDirection();
    
    //	浮遊
    const float tmpSpeed = 0.1f;
    VECTOR velocity = VScale(m_dir, tmpSpeed);
    m_pos = VAdd(m_pos, velocity);

    //	モデルにポジションを割り当てる
    MV1SetPosition(m_modelHandle, m_pos);
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放処理
//-----------------------------------------------------------------------------
void TitlePlayer::_FinalRelease()
{
    //  処理なし
}
