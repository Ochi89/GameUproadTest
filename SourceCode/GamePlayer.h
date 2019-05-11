
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
#include "PlayerBase.h"

//-----------------------------------------------------------------------------
//	@brief	前方宣言
//-----------------------------------------------------------------------------
class Pad;
class ShotManager;
class StayShotManager;
class UIGauge;
class SoundEffect;
class EnemyManager;

//-----------------------------------------------------------------------------
//	@brief	プレイヤークラス
//-----------------------------------------------------------------------------
class GamePlayer : public PlayerBase
{
public:

//===================== 関数 ======================//

	//	コンストラクタ / デストラクタ
    GamePlayer(const int _modelHandle);
	~GamePlayer();

	//	作成処理
    virtual void Create() override;

	//	解放処理
    virtual void Release() override;

	//	初期処理
    virtual void Initialize() override;

	//	更新処理
    virtual void Update(Pad& _pad, ShotManager& _shot, StayShotManager& _stayShot, SoundEffect& _soundEffect, UIGauge& _gaugeUI) override;

protected:

//================== 内部処理関数 ==================//

	//	弾の処理
	void _Shot(Pad& _pad, ShotManager& _shot, StayShotManager& _stayShot, SoundEffect& _soundEffect);

	//	必殺技委
	void _SpecialAttack(Pad& _pad, UIGauge& _gaugeUI);

	//	レーザー
	void _Laser();

	//	範囲外処理
	void _OutOfRange();

	//	開始時の移動
	void _EmergeMove();

	//	当たり判定用の点の更新
	void _UpdateHitPoint();

	//	レーザーエフェクト
	void _LaseEffect(SoundEffect& _soundEffect);

	//	ヒットエフェクト
	void _OnHitEffect(SoundEffect& _soundEffect);

	//	爆発エフェクト
	void _OnEffectExplosion(SoundEffect& _soundEffect);

	//	速度アップエフェクト
	void _OnEffectSpeedUp(SoundEffect& _soundEffect);

	//	アイテム取得時のエフェクト
	void _OnEffectGetItem();

	//	無敵時間の計測
	void _MeasuringInvincibleTime();

	//	最終的な解放処理
    virtual void _FinalRelease() override;
};