﻿
//=============================================================================
//	@file	Player.cpp
//	@brief	プレイヤー
//	@autor	Takuya Ochi
//	@date	2018/11/14
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "ResultPlayer.h"
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
ResultPlayer::ResultPlayer(const int _modelHandle)
    : PlayerBase(_modelHandle)
{
    //	各変数を初期化
    m_offDraw = HAND_OVER_RESULT->GetIsClear();
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

    //	まだどこも指していないので、nullptr で初期化
    m_effectLaser = nullptr ;
    m_effectHit = nullptr ;
    m_effectExplosion = nullptr ;
    m_effectSpeedUp = nullptr ;
    m_effectGetItem = nullptr ;
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
ResultPlayer::~ResultPlayer()
{
    //	最終的な解放処理
    _FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成処理
//-----------------------------------------------------------------------------
void ResultPlayer::Create()
{
    //  処理なし
}

//-----------------------------------------------------------------------------
//	@brief	解放処理
//-----------------------------------------------------------------------------
void ResultPlayer::Release()
{
    //  処理なし
}

//-----------------------------------------------------------------------------
//	@brief	初期処理
//-----------------------------------------------------------------------------
void ResultPlayer::Initialize()
{
    //	各変数を初期状態に設定
    m_pos = RESULT_POS;
    m_dir = CommonConstant::ORIGIN;
    m_angle = VGet(0.0f, 1.4f, 0.0f);
    m_size = VGet(0.4f, 0.4f, 0.4f);
    m_isInvincible = false;
    m_isStopMove = false;
    m_offDraw = (bool)(m_life <= ZERO_LIFE);

    //	角度とサイズを割り当てる
    MV1SetRotationXYZ(m_modelHandle, m_angle);
    MV1SetScale(m_modelHandle, m_size);
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void ResultPlayer::Update()
{
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
void ResultPlayer::_FinalRelease()
{
    //  処理なし
}
