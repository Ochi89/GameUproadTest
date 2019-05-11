
//=============================================================================
//	@file	TurningEnemy.h
//	@brief	旋回エネミー
//	@autor	Takuya Ochi
//	@date	2018/12/20
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	プリプロセッサ
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "EnemyBase.h"

//-----------------------------------------------------------------------------
//	@brief	前方宣言
//-----------------------------------------------------------------------------
class PlayerManager;
class ShotManager;
class SoundEffect;

//-----------------------------------------------------------------------------
//	@brief	旋回エネミークラス
//-----------------------------------------------------------------------------
class TurningEnemy : public EnemyBase
{
public:

//===================== 関数 ======================//

	//	コンストラクタ / デストラクタ
	TurningEnemy(const int _modelHandle);
	~TurningEnemy();

	//	作成処理
	void Create() override final;

	//	解放処理
	void Release() override final;

	//	初期処理
	void Initialize() override final;

	//	更新処理
	void Update(PlayerManager& _playerManager, ShotManager& _shotManager, SoundEffect& _soundEffect) override final;

private:

//================== 内部処理関数 ==================//

	//	向きの切り替え処理
	void _ChangeDir() override final;

	//	角度から向きを求める
	void _ChangeMoveAngle(const float _moveAngleSpeed, const bool _isScalingSize, const float _ScalingSize);

	//	当たり判定用の点の更新
	void _UpdateHitPoint() override final;

	//	最終的な解放処理
	void _FinalRelease() override final;

//=================== メンバ変数 ===================//

	float	m_moveAngle;			//	移動角度
	float	m_moveAngleWaitTime;	//	移動角度の待機時間
	bool	m_isOnReverse;			//	反転処理フラグ
	bool	m_isOneTime;			//	一度のみフラグ

//===================== 静的定数 ===================//

	static const float MOVE_ANGLE_SPEED;			//	回転角度の回転速度
	static const float MAX_MOVE_ANGLE_WAIT_TIME;	//	回転速度の待機時間の最大
	static const float MAX_MOVE_ANGLE;				//	回転角度の最大
	static const float MIN_MOVE_ANGLE;				//	回転角度の最小

};