
//=============================================================================
//	@file	Player.cpp
//	@brief	プレイヤー
//	@autor	Takuya Ochi
//	@date	2018/11/14
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "DemoPlayer.h"
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
DemoPlayer::DemoPlayer(const int _modelHandle)
    : GamePlayer(_modelHandle)
{
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
DemoPlayer::~DemoPlayer()
{
    //	最終的な解放処理
    _FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成処理
//-----------------------------------------------------------------------------
void DemoPlayer::Create()
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
void DemoPlayer::Release()
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
void DemoPlayer::Initialize()
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
void DemoPlayer::Update(ShotManager& _shot, StayShotManager& _stayShot, SoundEffect& _soundEffect, UIGauge& _gaugeUI, EnemyManager& _enemyManager)
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
            //	向きの切り替え
            _ChangeDir(_enemyManager);

            //	移動処理
            //	moving のノルムが0以外のときは、移動中
            VECTOR moving = MoveHelper::AskMoveAmount(m_dir, m_speed);
            m_isMove = VSize(moving) != 0;

            //	ポジションの更新
            m_pos = VAdd(m_pos, moving);

            //	範囲外指定
            _OutOfRange();

            //	射撃処理
            _Shot(_shot, _stayShot, _soundEffect);

            //	角度処理
            AngleHelper::AskAngle(m_angle, m_dir);
        }
    }

    //	ヒットエフェクト
    _OnHitEffect(_soundEffect);

    //	爆発エフェクト
    _OnEffectExplosion(_soundEffect);

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
}

//-----------------------------------------------------------------------------
//	@brief	移動向きの切り替え
//-----------------------------------------------------------------------------
void DemoPlayer::_ChangeDir(EnemyManager& _enemyManager)
{
    //	向きの切り替え
    const float startTime = 0.0f;
    const bool isStart = m_demoDirChangeTime == startTime;
    if (isStart)
    {
        //	敵リストの先頭を取得
        const int beginningOfEnemyList = 0;
        EnemyBase* enemy = _enemyManager.GetEnemyPtr(beginningOfEnemyList);

        if (enemy)
        {
            //	目的地を求める
            m_demoTargetPos = enemy->GetPos();
            m_demoTargetPos.x -= MAX_DEMO_DISTANCE_TO_ENEMY;
        }

        //	向きを求める
        m_dir = VSub(m_demoTargetPos, m_pos);

        //	正規化
        m_dir = VNorm(m_dir);
    }

    //	向きの初期化
    const float endTime = 60.0f;
    const bool isInitDir = m_demoDirChangeTime == endTime;
    if (isInitDir) { m_dir = CommonConstant::ORIGIN; }

    m_demoDirChangeTime++;

    //	最大処理
    const float resetTime = 100.0f;
    const bool isMaxVal = m_demoDirChangeTime > resetTime;
    if (isMaxVal) { m_demoDirChangeTime = 0.0f; }
}

//-----------------------------------------------------------------------------
//	@brief	弾の処理
//-----------------------------------------------------------------------------
void DemoPlayer::_Shot(ShotManager& _shot, StayShotManager& _stayShot, SoundEffect& _soundEffect)
{
    //	遅延時間がなくなったら、弾を生成する
    m_shotWaitTime--;
    const bool isActiveShot = m_shotWaitTime <= MAX_DEMO_SHOT_FIRING_TIME;
    if (isActiveShot)
    {
        //	弾リストに追加する
        _shot.RegisterOnList(ShotManager::SHOT_KIND::PLAYER_SHOT, m_pos, VGet(1.0f, 0.0f, 0.0f), SHOT_SPEED, _soundEffect);
        m_shotWaitTime = MAX_SHOT_WAIT_TIME;
    }

    //	滞在ショットの使用状況
    m_stayShotWaitTime--;
    m_isUseStayShot = (m_stayShotWaitTime <= 0.0f);

    //	発射遅延時間が発射時間より小さくなったら、滞在弾を生成する
    const bool isActiveStayShot = m_stayShotWaitTime <= MAX_DEMO_STAY_SHOT_FIRING_TIME;
    if (isActiveStayShot)
    {
        _stayShot.RegisterOnList(m_pos);
        m_stayShotWaitTime = MAX_STAY_SHOT_WAIT_TIME;
    }
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放処理
//-----------------------------------------------------------------------------
void DemoPlayer::_FinalRelease()
{
    //	エフェクトの削除
    CommonSafe::Delete(m_effectLaser);
    CommonSafe::Delete(m_effectHit);
    CommonSafe::Delete(m_effectExplosion);
    CommonSafe::Delete(m_effectSpeedUp);
    CommonSafe::Delete(m_effectGetItem);
}
