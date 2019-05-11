
//=============================================================================
//	@file	Player.cpp
//	@brief	プレイヤー
//	@autor	Takuya Ochi
//	@date	2018/11/14
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	プリプロセッサ
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "GamePlayer.h"

//-----------------------------------------------------------------------------
//	@brief	前方宣言
//-----------------------------------------------------------------------------
class ShotManager;
class StayShotManager;
class UIGauge;
class SoundEffect;
class EnemyManager;

//-----------------------------------------------------------------------------
//	@brief	プレイヤークラス
//-----------------------------------------------------------------------------
class DemoPlayer : public GamePlayer
{
public:

    //===================== 関数 ======================//

    //	コンストラクタ / デストラクタ
    DemoPlayer(const int _modelHandle);
    ~DemoPlayer();

    //	作成処理
    void Create() override final;

    //	解放処理
    void Release() override final;

    //	初期処理
    void Initialize() override final;

    //	更新処理
    void Update(ShotManager& _shot, StayShotManager& _stayShot, SoundEffect& _soundEffect, UIGauge& _gaugeUI, EnemyManager& _enemyManager) override final;

private:

//================== 内部処理関数 ==================//

    //	移動向きの切り替え
    void _ChangeDir(EnemyManager& _enemyManager);

    //	弾の処理
    void _Shot(ShotManager& _shot, StayShotManager& _stayShot, SoundEffect& _soundEffect);

    //	最終的な解放処理
    void _FinalRelease() override final;
};