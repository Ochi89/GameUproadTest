
//=============================================================================
//	@file	ReturnEnemy.h
//	@brief	カムバックエネミー
//	@autor	Takuya Ochi
//	@date	2018/11/16
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
//	@brief	カムバックエネミークラス
//-----------------------------------------------------------------------------
class ReturnEnemy : public EnemyBase
{
public:

//===================== 関数 ======================//

	//	コンストラクタ / デストラクタ
	ReturnEnemy(const int _modelHandle);
	~ReturnEnemy();

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

	//	当たり判定用の点の更新
	void _UpdateHitPoint() override final;

	//	最終的な解放処理
	void _FinalRelease() override final;

//=================== メンバ変数 ===================//

	float m_radian;		//	ラジアン角

//===================== 静的定数 ===================//

	static const float ADD_RADIAN_SPEED;	//	ラジアン角の加算速度

};