
//=============================================================================
//	@file	HitChecker.h
//	@brief	ヒットチェッカー
//	@autor	Takuya Ochi
//	@date	2018/12/13
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	プリプロセッサ
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "Common.h"

//-----------------------------------------------------------------------------
//	@brief	前方宣言
//-----------------------------------------------------------------------------
class PlayerManager;
class EnemyManager;
class BossManager;
class ShotManager;
class StayShotManager;
class ItemManager;
class PlayerBase;
class EnemyBase;
class Shot;
class UILife;
class UIGauge;
class SoundEffect;

//-----------------------------------------------------------------------------
//	@brief	ヒットチェッカークラス
//-----------------------------------------------------------------------------
class HitChecker final
{
public:

//==================== 列挙体 =====================//

	//	当たり判定の種類
	enum HIT_CHECKER_KIND
	{
		CIRCLE,
		RECT,
	};

//===================== 関数 ======================//

	//	プレイヤーとエネミーの当たり判定
	static void PlayerToEnemy(PlayerManager& _playerManager, EnemyManager& _enemyManager, ItemManager& _itemManager);

	//	プレイヤーと中ボスの当たり判定
	static void PlayerToMediumBoss(PlayerManager& _playerManager, BossManager& _bossManager);

	//	プレイヤーと最終ボスの当たり判定
	static void PlayerToLastBoss(PlayerManager& _playerManager, BossManager& _bossManager);


	//	プレイヤーとエネミーの弾の当たり判定
	static void PlayerToEnemyShot(PlayerManager& _playerManager, ShotManager& _shotManager);


	//	プレイヤー弾とエネミーの当たり判定
	static void PlayerShotToEnemy(ShotManager& _shotManager, EnemyManager& _enemyManager, ItemManager& _itemManager, UIGauge& _gaugeUI);

	//	プレイヤー弾と中ボスの当たり判定
	static void PlayerShotToMediumBoss(ShotManager& _shotManager, BossManager& _bossManager, UIGauge& _gaugeUI);

	//	プレイヤー弾と最終ボスの当たり判定
	static void PlayerShotToLastBoss(PlayerManager& _playerManager, ShotManager& _shotManager, BossManager& _bossManager, UIGauge& _gaugeUI);
	

	//	プレイヤー滞在弾とエネミーの当たり判定
	static void PlayerStayShotToEnemy(StayShotManager& _stayShotManager, EnemyManager& _enemyManager, ItemManager& _itemManager, UIGauge& _gaugeUI);

	//	プレイヤー滞在弾と中ボスの当たり判定
	static void PlayerStayShotToMediumBoss(StayShotManager& _stayShotManager, BossManager& _bossManager, UIGauge& _gaugeUI);

	//	プレイヤー滞在弾と最終ボスの当たり判定
	static void PlayerStayShotToLastBoss(PlayerManager& _playerManager, StayShotManager& _stayShotManager, BossManager& _bossManager, UIGauge& _gaugeUI);


	//	プレイヤーとアイテムの当たり判定
	static void PlayerToItem(PlayerManager& _playerManager, ItemManager& _itemManager, SoundEffect& _soundEffect, UIGauge& _gaugeUI);


	//	プレイヤーの必殺技とすべての当たり判定
	static void PlayerSpecialAttackToAllObject(PlayerManager& _playerManager, EnemyManager& _enemyManager, BossManager& _bossManager, ShotManager& _shotManager, ItemManager& _itemManager);


	//=============================================================================
	//	@brief	以下、デバッグ用の当たり判定の描画関数
	//=============================================================================


	//	プレイヤーとエネミーの当たり判定の描画
	static void PlayerToEnemyOfDraw(PlayerManager& _playerManager, EnemyManager& _enemyManager);

	//	プレイヤーと中ボスの当たり判定の描画
	static void PlayerToMediumBossOfDraw(PlayerManager& _playerManager, BossManager& _bossManager);

	//	プレイヤーと最終ボスの当たり判定の描画
	static void PlayerToLastBossOfDraw(PlayerManager& _playerManager, BossManager& _bossManager);


	//	プレイヤーとエネミーの弾の当たり判定の描画
	static void PlayerToEnemyShotOfDraw(PlayerManager& _playerManager, ShotManager& _shotManager);


	//	プレイヤーの弾とエネミーの当たり判定の描画
	static void PlayerShotToEnemyOfDraw(ShotManager& _shotManager, EnemyManager& _enemyManager);

	//	プレイヤーの弾と中ボスの当たり判定の描画
	static void PlayerShotToMediumBossOfDraw(ShotManager& _shotManager, BossManager& _bossManager);

	//	プレイヤー弾と最終ボスの当たり判定の描画
	static void PlayerShotToLastBossOfDraw(ShotManager& _shotManager, BossManager& _bossManager);


	//	プレイヤー滞在弾とエネミーの当たり判定の描画
	static void PlayerStayShotToEnemyOfDraw(StayShotManager& _stayShotManager, EnemyManager& _enemyManager);

	//	プレイヤー滞在弾と中ボスの当たり判定の描画
	static void PlayerStayShotToMediumBossOfDraw(StayShotManager& _stayShotManager, BossManager& _bossManager);

	//	プレイヤー滞在弾と最終ボスの当たり判定の描画
	static void PlayerStayShotToLastBossOfDraw(StayShotManager& _stayShotManager, BossManager& _bossManager);


	//	プレイヤーとアイテムの当たり判定の描画
	static void PlayerToItemOfDraw(PlayerManager& _playerManager, ItemManager& _item);


	//	プレイヤーの必殺技とすべての当たり判定の描画
	static void PlayerSpecialAttackToAllObjectOfDraw(PlayerManager& _playerManager, EnemyManager& _enemyManager, BossManager& _bossManager);

private:

//================== 内部処理関数 ==================//

	//	オブジェクト間の距離を測る
	static bool _AskObjectLenght(const VECTOR _pos1, const VECTOR _pos2);

	//	円形の当たり判定
	static bool _CircleHitCheck(Circle _target1, Circle _target2);

	//	矩形の当たり判定
	static bool _BoxHitCheck(Rect _target1, Rect _target2);

	//	円形の当たり判定の描画
	static void _CircleHitCheckOfDraw(Circle _target1, Circle _target2);

	//	矩形の当たり判定の描画
	static void _BoxHitCheckOfDraw(Rect _target1, Rect _target2);

//===================== 静的定数 ===================//

	static const float ACTIVE_LENGHT;			//	当たり判定の有効距離

};
