
//=============================================================================
//	@file	HitChecker.cpp
//	@brief	ヒットチェッカー
//	@autor	Takuya Ochi
//	@date	2018/12/13
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "HitChecker.h"
#include "Common.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "BossManager.h"
#include "ShotManager.h"
#include "StayShotManager.h"
#include "ItemManager.h"
#include "PlayerBase.h"
#include "EnemyBase.h"
#include "MediumBoss.h"
#include "LastBoss.h"
#include "Shot.h"
#include "StayShot.h"
#include "Item.h"
#include "UILife.h"
#include "UIGauge.h"
#include "UIScore.h"
#include "SoundEffect.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const float HitChecker::ACTIVE_LENGHT = 15.0f;			//	当たり判定有効距離

//-----------------------------------------------------------------------------
//	@brief	プレイヤーとエネミーの当たり判定
//-----------------------------------------------------------------------------
void HitChecker::PlayerToEnemy(PlayerManager& _playerManager, EnemyManager& _enemyManager, ItemManager& _itemManager)
{
	//	プレイヤーのポインタの取得
	PlayerBase* player = _playerManager.GetPlayerPtr();

	for (int i = 0; i < _enemyManager.GetListSize(); i++)
	{
		//	エネミーのポインタの取得
		EnemyBase* enemy = _enemyManager.GetEnemyPtr(i);

		//	ポインタが存在するなら
		if (player && enemy)
		{
			//	オブジェクト間の距離を測る
			//	true のときのみ、当たり判定の計算を行う
			const bool isActive = _AskObjectLenght(player->GetPos(), enemy->GetPos());
			if (isActive)
			{
				//	まだダメージを受けていないとき、
				//	当たり判定を受け取る
				const bool isNotInvincible = !player->GetIsInvincible() && !player->GetIsInvincibleAfterLaser();
				const bool isNotDamage = isNotInvincible && !player->GetIsDamage() && !enemy->GetIsHIt();
				if (isNotDamage)
				{
					//	当たり判定
					const bool isHit = _CircleHitCheck(player->GetCircle(), enemy->GetCircle());
					if (isHit)
					{
						//	ヒットしたので、
						//	プレイヤーの残機を減らす

						//	プレイヤーの残機を減らす
						player->OnHitDamage();
						player->SetIsDamage(true);

						//	隊列から削除
						const int formationID = enemy->GetFormationID();
						_enemyManager.SubEnemyCount(formationID);

						//	アイテムを持っているときか、
						//	隊列をすべて倒したら、
						//	アイテムを生成
						const bool isHavingItems = enemy->GetIsHavingItems();
						const bool isEnemyExistence = _enemyManager.GetIsEnemyExistence(formationID);
						const bool isSetItem = ((formationID != 0) && !isEnemyExistence) || isHavingItems;
						if (isSetItem) { _itemManager.RegisterOnList(enemy->GetPos()); }

						//	エネミーにもヒットした
						enemy->SetIsHit(true);

						//	スコアの加算
						SCORE->AddScore(SCORE->ADD_SCORE::ADD_SCORE_ENEMY);
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	プレイヤーと中ボスの当たり判定
//-----------------------------------------------------------------------------
void HitChecker::PlayerToMediumBoss(PlayerManager& _playerManager, BossManager& _bossManager)
{
	//	プレイヤーのポインタの取得
	PlayerBase* player = _playerManager.GetPlayerPtr();

	//	中ボスのポインタの取得
	MediumBoss* mediumBoss = _bossManager.GetMediumBoss();

	//	ポインタが存在するなら
	if (player && mediumBoss)
	{
		//	オブジェクト間の距離を測る
		//	true のときのみ、当たり判定の計算を行う
		const bool isActive = _AskObjectLenght(player->GetPos(), mediumBoss->GetPos());
		if (isActive)
		{
			//	まだダメージを受けていないとき、
			//	当たり判定を受け取る
			const bool isNotInvincible = !player->GetIsInvincible() && !player->GetIsInvincibleAfterLaser();
			const bool isNotDamage = isNotInvincible && !player->GetIsDamage();
			if (isNotDamage)
			{
				//	当たり判定
				const bool isHit = _CircleHitCheck(player->GetCircle(), mediumBoss->GetCircle());
				if (isHit)
				{
					//	ヒットしたので、
					//	プレイヤーの残機を減らす

					//	プレイヤーの残機を減らす
					player->OnHitDamage();
					player->SetIsDamage(true);
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	プレイヤーと最終ボスの当たり判定
//-----------------------------------------------------------------------------
void HitChecker::PlayerToLastBoss(PlayerManager& _playerManager, BossManager& _bossManager)
{
	static const int right = 0;
	static const int left = 1;

	//	プレイヤーのポインタの取得
	PlayerBase* player = _playerManager.GetPlayerPtr();

	//	中ボスのポインタの取得
	LastBoss* lastBoss = _bossManager.GetLastBossPtr();

	//	ポインタが存在するなら
	if (player && lastBoss)
	{
		//	まだダメージを受けていないとき、
		//	当たり判定を受け取る
		const bool isNotInvincible = !player->GetIsInvincible() && !player->GetIsInvincibleAfterLaser();
		const bool isNotDamage = isNotInvincible && !player->GetIsDamage();
		if (isNotDamage)
		{
			//	当たり判定
			const bool isCoreActive = lastBoss->GetIsRightCoreAlive() || lastBoss->GetIsRightCoreAlive();
			const bool isHitBody = _CircleHitCheck(player->GetCircle(), lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_BODY)) && isCoreActive;
			const bool isHitArmRight = _CircleHitCheck(player->GetCircle(), lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_ARM, right)) && lastBoss->GetIsRightCoreAlive();
			const bool isHitArmLeft = _CircleHitCheck(player->GetCircle(), lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_ARM, left)) && lastBoss->GetIsLeftCoreAlive();
			const bool isHitArmBodyRight = _BoxHitCheck(player->GetRect(), lastBoss->GetArmRect(right)) && lastBoss->GetIsRightCoreAlive();
			const bool isHitArmBodyLeft = _BoxHitCheck(player->GetRect(), lastBoss->GetArmRect(left)) && lastBoss->GetIsLeftCoreAlive();
			const bool isHitLaserRight = _BoxHitCheck(player->GetRect(), lastBoss->GetLaserRect(right)) && lastBoss->GetIsRightCoreAlive();
			const bool isHitLaserLeft = _BoxHitCheck(player->GetRect(), lastBoss->GetLaserRect(left)) && lastBoss->GetIsLeftCoreAlive();
			const bool isHitCenterBarrel = _BoxHitCheck(player->GetRect(), lastBoss->GetCenterBarrelRect()) && isCoreActive;
			const bool isHitOnly = isHitBody || isHitArmRight || isHitArmLeft || isHitArmBodyRight || isHitArmBodyLeft || isHitLaserRight || isHitLaserLeft || isHitCenterBarrel;
			if (isHitOnly)
			{
				//	プレイヤーの残機を減らす
				player->OnHitDamage();
				player->SetIsDamage(true);
			}

			const bool isHitCoreRight = _CircleHitCheck(player->GetCircle(), lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_CORE, right)) && lastBoss->GetIsRightCoreAlive();
			if (isHitCoreRight)
			{
				//	ボスのパーツにダメージを与える
				lastBoss->OnHitCoreDamage(right);

				//	プレイヤーの残機を減らす
				player->OnHitDamage();
				player->SetIsDamage(true);
			}

			const bool isHitCoreLeft = _CircleHitCheck(player->GetCircle(), lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_CORE, left)) && lastBoss->GetIsLeftCoreAlive();
			if (isHitCoreLeft)
			{
				//	ボスのパーツにダメージを与える
				lastBoss->OnHitCoreDamage(left);

				//	プレイヤーの残機を減らす
				player->OnHitDamage();
				player->SetIsDamage(true);
			}

			const bool isHitSideBarrelRight = _CircleHitCheck(player->GetCircle(), lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_SIDE_BARREL, right)) && lastBoss->GetIsRightSideBarrelAlive();
			if (isHitSideBarrelRight)
			{
				//	ボスのパーツにダメージを与える
				lastBoss->OnHitSideBarrelDamage(right);

				//	プレイヤーの残機を減らす
				player->OnHitDamage();
				player->SetIsDamage(true);
			}

			const bool isHitSideBarrelLeft = _CircleHitCheck(player->GetCircle(), lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_SIDE_BARREL, left)) && lastBoss->GetIsLeftSideBarrelAlive();
			if (isHitSideBarrelLeft)
			{
				//	ボスのパーツにダメージを与える
				lastBoss->OnHitSideBarrelDamage(left);

				//	プレイヤーの残機を減らす
				player->OnHitDamage();
				player->SetIsDamage(true);
			}

			//	ボスの破壊状況
			lastBoss->DestructionSituation();

			//	すべて破壊
			const bool isAllDestruction = lastBoss->GetIsAllDestruction();
			if (isAllDestruction) { _playerManager.SetIsClear(true); }
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	プレイヤーとエネミーの弾の当たり判定
//-----------------------------------------------------------------------------
void HitChecker::PlayerToEnemyShot(PlayerManager& _playerManager, ShotManager& _shotManager)
{
	//	プレイヤーのポインタの取得
	PlayerBase* player = _playerManager.GetPlayerPtr();

	for (int i = 0; i < _shotManager.GetListSize(ShotManager::SHOT_KIND::ENEMY_SHOT); i++)
	{
		//	ショットのポインタの取得
		Shot* shot = _shotManager.GetShotPtr(ShotManager::SHOT_KIND::ENEMY_SHOT, i);

		//	ポインタが存在するなら
		if (player && shot)
		{
			//	オブジェクト間の距離を測る
			//	true のときのみ、当たり判定の計算を行う
			const bool isActive = _AskObjectLenght(player->GetPos(), shot->GetPos());
			if (isActive)
			{
				//	まだダメージを受けていないとき、
				//	当たり判定を受け取る
				const bool isNotInvincible = !player->GetIsInvincible() && !player->GetIsInvincibleAfterLaser();
				const bool isNotDamage = isNotInvincible && !player->GetIsDamage();
				if (isNotDamage)
				{
					//	当たり判定
					const bool isHit = _CircleHitCheck(player->GetCircle(), shot->GetCircle());
					if (isHit)
					{
						//	ヒットしたので、
						//	プレイヤーの残機を減らす

						//	プレイヤーの残機を減らす
						player->OnHitDamage();
						player->SetIsDamage(true);

						//	エネミーの弾をリストから削除する
						_shotManager.RemoveFromList(ShotManager::SHOT_KIND::ENEMY_SHOT, shot);
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	プレイヤーの弾とエネミーの当たり判定
//-----------------------------------------------------------------------------
void HitChecker::PlayerShotToEnemy(ShotManager& _shotManager, EnemyManager& _enemyManager, ItemManager& _itemManager, UIGauge& _gaugeUI)
{
	for (int i = 0; i < _shotManager.GetListSize(ShotManager::SHOT_KIND::PLAYER_SHOT); i++)
	{
		//	ショットのポインタの取得
		Shot* shot = _shotManager.GetShotPtr(ShotManager::SHOT_KIND::PLAYER_SHOT, i);

		for (int j = 0; j < _enemyManager.GetListSize(); j++)
		{
			//	エネミーのポインタの取得
			EnemyBase* enemy = _enemyManager.GetEnemyPtr(j);

			//	ポインタが存在するなら
			if (shot && enemy)
			{
				//	オブジェクト間の距離を測る
				//	true のときのみ、当たり判定の計算を行う
				const bool isActive = _AskObjectLenght(shot->GetPos(), enemy->GetPos());
				if (isActive)
				{
					//	エネミーがヒットしていないとき
					const bool isNotDamage = !enemy->GetIsHIt();
					if (isNotDamage)
					{

						//	当たり判定
						const bool isHit = _CircleHitCheck(shot->GetCircle(), enemy->GetCircle());
						if (isHit)
						{
							//	ヒットしたので、
							//	エネミーとプレイヤーの弾を、
							//	リストから削除する

							//	エネミーをリストから削除する
							//	隊列から削除
							const int formationID = enemy->GetFormationID();
							_enemyManager.SubEnemyCount(formationID);

							//	アイテムを持っているときか、
							//	隊列をすべて倒したら、
							//	アイテムを生成
							const bool isHavingItems = enemy->GetIsHavingItems();
							const bool isEnemyExistence = _enemyManager.GetIsEnemyExistence(formationID);
							const bool isSetItem = ((formationID != 0) && !isEnemyExistence) || isHavingItems;
							if (isSetItem) { _itemManager.RegisterOnList(enemy->GetPos()); }

							//	エネミーにもヒットした
							enemy->SetIsHit(true);

							//	プレイヤーの弾をリストから削除する
							_shotManager.RemoveFromList(ShotManager::SHOT_KIND::PLAYER_SHOT, shot);

							//	ゲージの加算
							_gaugeUI.AddGauge(_gaugeUI.ADD_GAUGE_KIND::ENEMY);

							//	スコアの加算
							SCORE->AddScore(SCORE->ADD_SCORE::ADD_SCORE_ENEMY);
						}
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	プレイヤーの弾とボスの当たり判定
//-----------------------------------------------------------------------------
void HitChecker::PlayerShotToMediumBoss(ShotManager& _shotManager, BossManager& _bossManager, UIGauge& _gaugeUI)
{
	for (int i = 0; i < _shotManager.GetListSize(ShotManager::SHOT_KIND::PLAYER_SHOT); i++)
	{
		//	ショットのポインタの取得
		Shot* shot = _shotManager.GetShotPtr(ShotManager::SHOT_KIND::PLAYER_SHOT, i);

		//	中ボスのポインタの取得
		MediumBoss* mediumBoss = _bossManager.GetMediumBoss();

		//	ポインタが存在するなら
		if (shot && mediumBoss)
		{
			//	オブジェクト間の距離を測る
			//	true のときのみ、当たり判定の計算を行う
			const bool isActive = _AskObjectLenght(shot->GetPos(), mediumBoss->GetPos());
			if (isActive)
			{
				//	当たり判定
				const bool isHit = _CircleHitCheck(shot->GetCircle(), mediumBoss->GetCircle());
				if (isHit)
				{
					//	ヒットしたので、
					//	プレイヤーの弾をリストから削除し、
					//	ボスにダメージを与える

					//	ボスがダメージを受けた場合、
					//	連続して入らないように制限
					if (!mediumBoss->GetIsDamage())
					{
						//	ボスにダメージを与え、
						//	ボスを点滅させる
						mediumBoss->OnHitDamage();
					}

					//	プレイヤーの弾をリストから削除する
					_shotManager.RemoveFromList(ShotManager::SHOT_KIND::PLAYER_SHOT, shot);

					//	ゲージの加算
					const bool isDamageMoment = !mediumBoss->GetIsDamage();
					if (isDamageMoment) { _gaugeUI.AddGauge(_gaugeUI.ADD_GAUGE_KIND::BOSS); }
				}

			}
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	プレイヤー弾と最終ボスの当たり判定
//-----------------------------------------------------------------------------
void HitChecker::PlayerShotToLastBoss(PlayerManager& _playerManager, ShotManager& _shotManager, BossManager& _bossManager, UIGauge& _gaugeUI)
{
	static const int right = 0;
	static const int left = 1;

	for (int i = 0; i < _shotManager.GetListSize(ShotManager::SHOT_KIND::PLAYER_SHOT); i++)
	{
		//	ショットのポインタの取得
		Shot* shot = _shotManager.GetShotPtr(ShotManager::SHOT_KIND::PLAYER_SHOT, i);

		//	中ボスのポインタの取得
		LastBoss* lastBoss = _bossManager.GetLastBossPtr();

		//	ポインタが存在するなら
		if (shot && lastBoss)
		{
			//	当たり判定
			const bool isCoreActive = lastBoss->GetIsRightCoreAlive() || lastBoss->GetIsRightCoreAlive();
			const bool isHitBody = _CircleHitCheck(shot->GetCircle(), lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_BODY)) && isCoreActive;
			const bool isHitArmRight = _CircleHitCheck(shot->GetCircle(), lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_ARM, right)) && lastBoss->GetIsRightCoreAlive();
			const bool isHitArmLeft = _CircleHitCheck(shot->GetCircle(), lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_ARM, left)) && lastBoss->GetIsLeftCoreAlive();
			const bool isHitCenterBarrel = _BoxHitCheck(shot->GetRect(), lastBoss->GetCenterBarrelRect()) && isCoreActive;
			const bool isHitOnly = isHitBody || isHitArmRight || isHitArmLeft || isHitCenterBarrel;
			if (isHitOnly)
			{
				//	プレイヤーの弾をリストから削除する
				_shotManager.RemoveFromList(ShotManager::SHOT_KIND::PLAYER_SHOT, shot);
			}

			const bool isHitCoreRight = _CircleHitCheck(shot->GetCircle(), lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_CORE, right)) && lastBoss->GetIsRightCoreAlive();
			if (isHitCoreRight)
			{
				//	ゲージの加算
				const bool isDamageMoment = !lastBoss->GetIsCoreDamage(right);
				if (isDamageMoment) { _gaugeUI.AddGauge(_gaugeUI.ADD_GAUGE_KIND::BOSS); }

				//	ボスのパーツにダメージを与える
				lastBoss->OnHitCoreDamage(right);

				//	プレイヤーの弾をリストから削除する
				_shotManager.RemoveFromList(ShotManager::SHOT_KIND::PLAYER_SHOT, shot);
			}

			const bool isHitCoreLeft = _CircleHitCheck(shot->GetCircle(), lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_CORE, left)) && lastBoss->GetIsLeftCoreAlive();
			if (isHitCoreLeft)
			{
				//	ゲージの加算
				const bool isDamageMoment = !lastBoss->GetIsCoreDamage(left);
				if (isDamageMoment) { _gaugeUI.AddGauge(_gaugeUI.ADD_GAUGE_KIND::BOSS); }

				//	ボスのパーツにダメージを与える
				lastBoss->OnHitCoreDamage(left);

				//	プレイヤーの弾をリストから削除する
				_shotManager.RemoveFromList(ShotManager::SHOT_KIND::PLAYER_SHOT, shot);
			}

			const bool isHitSideBarrelRight = _CircleHitCheck(shot->GetCircle(), lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_SIDE_BARREL, right)) && lastBoss->GetIsRightSideBarrelAlive();
			if (isHitSideBarrelRight)
			{
				//	ゲージの加算
				const bool isDamageMoment = !lastBoss->GetIsSideBarrelDamage(right);
				if (isDamageMoment) { _gaugeUI.AddGauge(_gaugeUI.ADD_GAUGE_KIND::BOSS); }

				//	ボスのパーツにダメージを与える
				lastBoss->OnHitSideBarrelDamage(right);

				//	プレイヤーの弾をリストから削除する
				_shotManager.RemoveFromList(ShotManager::SHOT_KIND::PLAYER_SHOT, shot);
			}

			const bool isHitSideBarrelLeft = _CircleHitCheck(shot->GetCircle(), lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_SIDE_BARREL, left)) && lastBoss->GetIsLeftSideBarrelAlive();
			if (isHitSideBarrelLeft)
			{
				//	ゲージの加算
				const bool isDamageMoment = !lastBoss->GetIsSideBarrelDamage(left);
				if (isDamageMoment) { _gaugeUI.AddGauge(_gaugeUI.ADD_GAUGE_KIND::BOSS); }

				//	ボスのパーツにダメージを与える
				lastBoss->OnHitSideBarrelDamage(left);

				//	プレイヤーの弾をリストから削除する
				_shotManager.RemoveFromList(ShotManager::SHOT_KIND::PLAYER_SHOT, shot);
			}

			//	ボスの破壊状況
			lastBoss->DestructionSituation();

			//	すべて破壊
			const bool isAllDestruction = lastBoss->GetIsAllDestruction();
			if (isAllDestruction) { _playerManager.SetIsClear(true); }
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	プレイヤー滞在弾とエネミーの当たり判定
//-----------------------------------------------------------------------------
void HitChecker::PlayerStayShotToEnemy(StayShotManager& _stayShotManager, EnemyManager& _enemyManager, ItemManager& _itemManager, UIGauge& _gaugeUI)
{
	for (int i = 0; i < _stayShotManager.GetListSize(); i++)
	{
		//	ショットのポインタの取得
		StayShot* stayShot = _stayShotManager.GetStayShotPtr(i);

		for (int j = 0; j < _enemyManager.GetListSize(); j++)
		{
			//	エネミーのポインタの取得
			EnemyBase* enemy = _enemyManager.GetEnemyPtr(j);

			//	ポインタが存在するなら
			if (stayShot && enemy)
			{
				//	オブジェクト間の距離を測る
				//	true のときのみ、当たり判定の計算を行う
				const bool isActive = _AskObjectLenght(stayShot->GetPos(), enemy->GetPos());
				if (isActive)
				{
					//	エネミーがヒットしていないとき
					const bool isNotDamage = !enemy->GetIsHIt();
					if (isNotDamage)
					{
						//	当たり判定
						const bool isHit = _CircleHitCheck(stayShot->GetCircle(), enemy->GetCircle());
						if (isHit)
						{
							//	ヒットしたので、
							//	エネミーとプレイヤーの弾を、
							//	リストから削除する

							//	エネミーをリストから削除する
							//	隊列から削除
							const int formationID = enemy->GetFormationID();
							_enemyManager.SubEnemyCount(formationID);

							//	アイテムを持っているときか、
							//	隊列をすべて倒したら、
							//	アイテムを生成
							const bool isHavingItems = enemy->GetIsHavingItems();
							const bool isEnemyExistence = _enemyManager.GetIsEnemyExistence(formationID);
							const bool isSetItem = ((formationID != 0) && !isEnemyExistence) || isHavingItems;
							if (isSetItem) { _itemManager.RegisterOnList(enemy->GetPos()); }

							//	エネミーにもヒットした
							enemy->SetIsHit(true);

							//	ヒットしたので、
							//	ショットのモードを変更
							stayShot->SetIsChangeStay(true);

							//	ゲージの加算
							_gaugeUI.AddGauge(_gaugeUI.ADD_GAUGE_KIND::ENEMY);

							//	スコアの加算
							SCORE->AddScore(SCORE->ADD_SCORE::ADD_SCORE_ENEMY);
						}
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	プレイヤー滞在弾と中ボスの当たり判定
//-----------------------------------------------------------------------------
void HitChecker::PlayerStayShotToMediumBoss(StayShotManager& _stayShotManager, BossManager& _bossManager, UIGauge& _gaugeUI)
{
	for (int i = 0; i < _stayShotManager.GetListSize(); i++)
	{
		//	ショットのポインタの取得
		StayShot* stayShot = _stayShotManager.GetStayShotPtr(i);

		//	中ボスのポインタの取得
		MediumBoss* mediumBoss = _bossManager.GetMediumBoss();

		//	ポインタが存在するなら
		if (stayShot && mediumBoss)
		{
			//	オブジェクト間の距離を測る
			//	true のときのみ、当たり判定の計算を行う
			const bool isActive = _AskObjectLenght(stayShot->GetPos(), mediumBoss->GetPos());
			if (isActive)
			{
				//	当たり判定
				const bool isHit = _CircleHitCheck(stayShot->GetCircle(), mediumBoss->GetCircle());
				if (isHit)
				{
					//	ヒットしたので、
					//	プレイヤーの弾をリストから削除し、
					//	ボスにダメージを与える

					//	ボスがダメージを受けた場合、
					//	連続して入らないように制限
					if (!mediumBoss->GetIsDamage())
					{
						//	ボスにダメージを与え、
						//	ボスを点滅させる
						mediumBoss->OnHitDamage();
					}

					//	ヒットしたので、
					//	ショットのモードを変更
					stayShot->SetIsChangeStay(true);

					//	ゲージの加算
					const bool isDamageMoment = !mediumBoss->GetIsDamage();
					if (isDamageMoment) { _gaugeUI.AddGauge(_gaugeUI.ADD_GAUGE_KIND::BOSS); }
				}

			}
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	プレイヤー滞在弾と最終ボスの当たり判定
//-----------------------------------------------------------------------------
void HitChecker::PlayerStayShotToLastBoss(PlayerManager& _playerManager, StayShotManager& _stayShotManager, BossManager& _bossManager, UIGauge& _gaugeUI)
{
	static const int right = 0;
	static const int left = 1;

	for (int i = 0; i < _stayShotManager.GetListSize(); i++)
	{
		//	ショットのポインタの取得
		StayShot* stayShot = _stayShotManager.GetStayShotPtr(i);

		//	最終ボスのポインタの取得
		LastBoss* lastBoss = _bossManager.GetLastBossPtr();

		//	ポインタが存在するなら
		if (stayShot && lastBoss)
		{
			//	当たり判定
			const bool isCoreActive = lastBoss->GetIsRightCoreAlive() || lastBoss->GetIsRightCoreAlive();
			const bool isHitBody = _CircleHitCheck(stayShot->GetCircle(), lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_BODY)) && isCoreActive;
			const bool isHitArmRight = _CircleHitCheck(stayShot->GetCircle(), lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_ARM, right)) && lastBoss->GetIsRightCoreAlive();
			const bool isHitArmLeft = _CircleHitCheck(stayShot->GetCircle(), lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_ARM, left)) && lastBoss->GetIsLeftCoreAlive();
			const bool isHitCenterBarrel = _BoxHitCheck(stayShot->GetRect(), lastBoss->GetCenterBarrelRect()) && isCoreActive;
			const bool isHitOnly = isHitBody || isHitArmRight || isHitArmLeft || isHitCenterBarrel;
			if (isHitOnly)
			{
				//	ヒットしたので、
				//	ショットのモードを変更
				stayShot->SetIsChangeStay(true);
			}

			const bool isHitCoreRight = _CircleHitCheck(stayShot->GetCircle(), lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_CORE, right)) && lastBoss->GetIsRightCoreAlive();
			if (isHitCoreRight)
			{
				//	ゲージの加算
				const bool isDamageMoment = !lastBoss->GetIsCoreDamage(right);
				if (isDamageMoment) { _gaugeUI.AddGauge(_gaugeUI.ADD_GAUGE_KIND::BOSS); }

				//	ボスのパーツにダメージを与える
				lastBoss->OnHitCoreDamage(right);

				//	ヒットしたので、
				//	ショットのモードを変更
				stayShot->SetIsChangeStay(true);
			}

			const bool isHitCoreLeft = _CircleHitCheck(stayShot->GetCircle(), lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_CORE, left)) && lastBoss->GetIsLeftCoreAlive();
			if (isHitCoreLeft)
			{
				//	ゲージの加算
				const bool isDamageMoment = !lastBoss->GetIsCoreDamage(left);
				if (isDamageMoment) { _gaugeUI.AddGauge(_gaugeUI.ADD_GAUGE_KIND::BOSS); }

				//	ボスのパーツにダメージを与える
				lastBoss->OnHitCoreDamage(left);

				//	ヒットしたので、
				//	ショットのモードを変更
				stayShot->SetIsChangeStay(true);
			}

			const bool isHitSideBarrelRight = _CircleHitCheck(stayShot->GetCircle(), lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_SIDE_BARREL, right)) && lastBoss->GetIsRightSideBarrelAlive();
			if (isHitSideBarrelRight)
			{
				//	ゲージの加算
				const bool isDamageMoment = !lastBoss->GetIsSideBarrelDamage(right);
				if (isDamageMoment) { _gaugeUI.AddGauge(_gaugeUI.ADD_GAUGE_KIND::BOSS); }

				//	ボスのパーツにダメージを与える
				lastBoss->OnHitSideBarrelDamage(right);

				//	ヒットしたので、
				//	ショットのモードを変更
				stayShot->SetIsChangeStay(true);

			}

			const bool isHitSideBarrelLeft = _CircleHitCheck(stayShot->GetCircle(), lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_SIDE_BARREL, left)) && lastBoss->GetIsLeftSideBarrelAlive();
			if (isHitSideBarrelLeft)
			{
				//	ゲージの加算
				const bool isDamageMoment = !lastBoss->GetIsSideBarrelDamage(left);
				if (isDamageMoment) { _gaugeUI.AddGauge(_gaugeUI.ADD_GAUGE_KIND::BOSS); }

				//	ボスのパーツにダメージを与える
				lastBoss->OnHitSideBarrelDamage(left);

				//	ヒットしたので、
				//	ショットのモードを変更
				stayShot->SetIsChangeStay(true);
			}

			//	ボスの破壊状況
			lastBoss->DestructionSituation();

			//	すべて破壊
			const bool isAllDestruction = lastBoss->GetIsAllDestruction();
			if (isAllDestruction) { _playerManager.SetIsClear(true); }
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	プレイヤーとアイテムの当たり判定
//-----------------------------------------------------------------------------
void HitChecker::PlayerToItem(PlayerManager& _playerManager, ItemManager& _itemManager, SoundEffect& _soundEffect, UIGauge& _gaugeUI)
{
	//	プレイヤーのポインタの取得
	PlayerBase* player = _playerManager.GetPlayerPtr();

	for (int i = 0; i < _itemManager.GetListSize(); i++)
	{
		//	アイテムのポインタの取得
		Item* item = _itemManager.GetItemPtr(i);

		//	ポインタが存在するなら
		if (player && item)
		{
			//	オブジェクト間の距離を測る
			//	true のときのみ、当たり判定の計算を行う
			const bool isActive = _AskObjectLenght(player->GetPos(), item->GetPos());
			if (isActive)
			{
				//	当たり判定
				const bool isHit = _BoxHitCheck(player->GetItemHitRect(), item->GetRect());
				if (isHit)
				{
					//	ヒットしたので、
					//	アイテムカウンタを増やし、
					//	アイテムはリストから削除する

					//	アイテム取得エフェクトが開始していないときは
					//	エフェクトを再生する
					const bool isNotStartEffect = !player->GetIsStartEffectGetItem();
					if(isNotStartEffect) { player->SetIsStartEffectGetItem(true); }

					//	アイテムをリストから削除する
					_itemManager.RemoveFromList(item);

					//	SE
					_soundEffect.OnPlaySound(_soundEffect.SE_KIND::SE_ITEM);

					//	ゲージの加算
					_gaugeUI.AddGauge(_gaugeUI.ADD_GAUGE_KIND::ITEM);

					//	スコアの加算
					SCORE->AddScore(SCORE->ADD_SCORE::ADD_SCORE_ITEM);
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	プレイヤーの必殺技とすべての当たり判定
//-----------------------------------------------------------------------------
void HitChecker::PlayerSpecialAttackToAllObject(PlayerManager& _playerManager, EnemyManager& _enemyManager, BossManager& _bossManager, ShotManager& _shotManager, ItemManager& _itemManager)
{
	//	必殺技のとき
	const bool isSpecialProductionHitActive = PRODUCTION->GetIsSpecialProduction();
	if (isSpecialProductionHitActive)
	{
		//	プレイヤーの取得
		PlayerBase* player = _playerManager.GetPlayerPtr();

		const int size = _enemyManager.GetListSize();
		for (int i = 0; i < size; i++)
		{
			//	エネミーのポインタの取得
			EnemyBase* enemy = _enemyManager.GetEnemyPtr(i);
			if (player && enemy)
			{
				//	エネミーがヒットしていないとき
				const bool isNotDamage = !enemy->GetIsHIt();
				if (isNotDamage)
				{
					//	当たり判定
					const bool isHit = _BoxHitCheck(player->GetLaserHitRect(), enemy->GetRect());
					if (isHit)
					{
						//	隊列から削除
						const int formationID = enemy->GetFormationID();
						_enemyManager.SubEnemyCount(formationID);

						//	アイテムを持っているときか、
						//	隊列をすべて倒したら、
						//	アイテムを生成
						const bool isHavingItems = enemy->GetIsHavingItems();
						const bool isEnemyExistence = _enemyManager.GetIsEnemyExistence(formationID);
						const bool isSetItem = ((formationID != 0) && !isEnemyExistence) || isHavingItems;
						if (isSetItem) { _itemManager.RegisterOnList(enemy->GetPos()); }

						//	エネミーにもヒットした
						enemy->SetIsHit(true);

						//	スコアの加算
						SCORE->AddScore(SCORE->ADD_SCORE::ADD_SCORE_ENEMY);
					}
				}
			}
		}

		//	中ボスのポインタの取得
		MediumBoss* mediumBoss = _bossManager.GetMediumBoss();
		if (player && mediumBoss)
		{
			//	ボスが出現しているなら
			const bool isMediumBossEmerge = mediumBoss->GetIsEmerge() || mediumBoss->GetIsStarted();
			if (isMediumBossEmerge)
			{
				//	当たり判定
				const bool isHit = _BoxHitCheck(player->GetLaserHitRect(), mediumBoss->GetRect());
				if (isHit)
				{
					//	ダメージを与える
					mediumBoss->OnHitDamage();
				}
			}
		}

		//	最終ボスのポインタの取得
		LastBoss* lastBoss = _bossManager.GetLastBossPtr();
		if (player && lastBoss)
		{
			const int right = 0;
			const int left = 1;

			const bool isHitCoreRight = _BoxHitCheck(player->GetLaserHitRect(), lastBoss->GetCoreRect(right)) && lastBoss->GetIsRightCoreAlive();
			if (isHitCoreRight)
			{
				//	ボスのパーツにダメージを与える
				lastBoss->OnHitCoreDamage(right);
			}

			const bool isHitCoreLeft = _BoxHitCheck(player->GetLaserHitRect(), lastBoss->GetCoreRect(left)) && lastBoss->GetIsLeftCoreAlive();
			if (isHitCoreLeft)
			{
				//	ボスのパーツにダメージを与える
				lastBoss->OnHitCoreDamage(left);
			}

			const bool isHitSideBarrelRight = _BoxHitCheck(player->GetLaserHitRect(), lastBoss->GetSideBarrelRect(right)) && lastBoss->GetIsRightSideBarrelAlive();
			if (isHitSideBarrelRight)
			{
				//	ボスのパーツにダメージを与える
				lastBoss->OnHitSideBarrelDamage(right);
			}

			const bool isHitSideBarrelLeft = _BoxHitCheck(player->GetLaserHitRect(), lastBoss->GetSideBarrelRect(left)) && lastBoss->GetIsLeftSideBarrelAlive();
			if (isHitSideBarrelLeft)
			{
				//	ボスのパーツにダメージを与える
				lastBoss->OnHitSideBarrelDamage(left);
			}
		}

		//	敵の弾をすべて消す
		for (int i = 0; i < _shotManager.GetListSize(ShotManager::SHOT_KIND::ENEMY_SHOT); i++)
		{
			//	ショットのポインタの取得
			Shot* shot = _shotManager.GetShotPtr(ShotManager::SHOT_KIND::ENEMY_SHOT, i);

			//	ポインタが存在するなら
			if (shot)
			{
				//	エネミーの弾をリストから削除する
				_shotManager.RemoveFromList(ShotManager::SHOT_KIND::ENEMY_SHOT, shot);
			}
		}
	}

	//	最終ボスのポインタの取得
	LastBoss* lastBoss = _bossManager.GetLastBossPtr();
	if (lastBoss)
	{
		//	ボスの破壊状況
		lastBoss->DestructionSituation();

		//	すべて破壊
		const bool isAllDestruction = lastBoss->GetIsAllDestruction();
		if (isAllDestruction) { _playerManager.SetIsClear(true); }
	}
}


//=============================================================================
//	@brief	以下、デバッグ用の当たり判定の描画関数
//=============================================================================


//-----------------------------------------------------------------------------
//	@brief	プレイヤーとエネミーの当たり判定の描画
//-----------------------------------------------------------------------------
void HitChecker::PlayerToEnemyOfDraw(PlayerManager& _playerManager, EnemyManager& _enemyManager)
{
	//	プレイヤーのポインタの取得
	PlayerBase* player = _playerManager.GetPlayerPtr();

	for (int i = 0; i < _enemyManager.GetListSize(); i++)
	{
		//	エネミーのポインタの取得
		EnemyBase* enemy = _enemyManager.GetEnemyPtr(i);

		//	ポインタが存在するなら
		if (player && enemy)
		{
			//	当たり判定の描画
			_CircleHitCheckOfDraw(player->GetCircle(), enemy->GetCircle());
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	プレイヤーと中ボスの当たり判定の描画
//-----------------------------------------------------------------------------
void HitChecker::PlayerToMediumBossOfDraw(PlayerManager& _playerManager, BossManager& _bossManager)
{
	//	プレイヤーのポインタの取得
	PlayerBase* player = _playerManager.GetPlayerPtr();

	//	中ボスのポインタの取得
	MediumBoss* mediumBoss = _bossManager.GetMediumBoss();

	//	ポインタが存在するなら
	if (player && mediumBoss)
	{
		//	当たり判定の描画
		_CircleHitCheckOfDraw(player->GetCircle(), mediumBoss->GetCircle());
	}
}

//-----------------------------------------------------------------------------
//	@brief	プレイヤーと最終ボスの当たり判定の描画
//-----------------------------------------------------------------------------
void HitChecker::PlayerToLastBossOfDraw(PlayerManager& _playerManager, BossManager& _bossManager)
{
	//	プレイヤーのポインタの取得
	PlayerBase* player = _playerManager.GetPlayerPtr();

	//	最終ボスのポインタの取得
	LastBoss* lastBoss = _bossManager.GetLastBossPtr();

	//	ポインタが存在するなら
	if (player && lastBoss)
	{
		if (lastBoss->GetIsAppearance())
		{
			//	当たり判定の描画
			_BoxHitCheckOfDraw(player->GetRect(), lastBoss->GetCenterBarrelRect());
			_CircleHitCheckOfDraw(player->GetCircle(), lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_BODY));

			if (lastBoss->GetIsLeftCoreAlive())
			{
				DrawSphere3D(lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_CORE, 1).m_centerPoint, lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_CORE, 1).m_radius, 5, 0x00ffff, 0x00ffff, false);
				DrawSphere3D(lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_ARM, 1).m_centerPoint, lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_ARM, 1).m_radius, 5, 0x00ffff, 0x00ffff, false);
				_BoxHitCheckOfDraw(lastBoss->GetLaserRect(1), lastBoss->GetArmRect(1));
			}

			if (lastBoss->GetIsRightCoreAlive())
			{
				DrawSphere3D(lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_CORE, 0).m_centerPoint, lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_CORE, 0).m_radius, 5, 0x00ffff, 0x00ffff, false);
				DrawSphere3D(lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_ARM, 0).m_centerPoint, lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_ARM, 0).m_radius, 5, 0x00ffff, 0x00ffff, false);
				_BoxHitCheckOfDraw(lastBoss->GetLaserRect(0), lastBoss->GetArmRect(0));
			}

			if (lastBoss->GetIsRightSideBarrelAlive())
			{
				DrawSphere3D(lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_SIDE_BARREL, 0).m_centerPoint, lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_SIDE_BARREL, 0).m_radius, 5, 0x00ffff, 0x00ffff, false);
			}

			if (lastBoss->GetIsLeftSideBarrelAlive())
			{
				DrawSphere3D(lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_SIDE_BARREL, 1).m_centerPoint, lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_SIDE_BARREL, 1).m_radius, 5, 0x00ffff, 0x00ffff, false);
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	プレイヤーとエネミーの弾の当たり判定の描画
//-----------------------------------------------------------------------------
void HitChecker::PlayerToEnemyShotOfDraw(PlayerManager& _playerManager, ShotManager& _shotManager)
{
	//	プレイヤーのポインタの取得
	PlayerBase* player = _playerManager.GetPlayerPtr();

	for (int i = 0; i < _shotManager.GetListSize(ShotManager::SHOT_KIND::ENEMY_SHOT); i++)
	{
		//	ショットのポインタの取得
		Shot* shot = _shotManager.GetShotPtr(ShotManager::SHOT_KIND::ENEMY_SHOT, i);

		//	ポインタが存在するなら
		if (player && shot)
		{
			//	当たり判定の描画
			_CircleHitCheckOfDraw(player->GetCircle(), shot->GetCircle());
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	プレイヤーの弾とエネミーの当たり判定の描画
//-----------------------------------------------------------------------------
void HitChecker::PlayerShotToEnemyOfDraw(ShotManager& _shotManager, EnemyManager& _enemyManager)
{
	for (int i = 0; i < _shotManager.GetListSize(ShotManager::SHOT_KIND::PLAYER_SHOT); i++)
	{
		//	ショットのポインタの取得
		Shot* shot = _shotManager.GetShotPtr(ShotManager::SHOT_KIND::PLAYER_SHOT, i);

		for (int j = 0; j < _enemyManager.GetListSize(); j++)
		{
			//	エネミーのポインタの取得
			EnemyBase* enemy = _enemyManager.GetEnemyPtr(j);

			//	ポインタが存在するなら
			if (shot && enemy)
			{
				//	当たり判定の描画
				_CircleHitCheckOfDraw(shot->GetCircle(), enemy->GetCircle());
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	プレイヤーの弾と中ボスの当たり判定の描画
//-----------------------------------------------------------------------------
void HitChecker::PlayerShotToMediumBossOfDraw(ShotManager& _shotManager, BossManager& _bossManager)
{
	for (int i = 0; i < _shotManager.GetListSize(ShotManager::SHOT_KIND::PLAYER_SHOT); i++)
	{
		//	ショットのポインタの取得
		Shot* shot = _shotManager.GetShotPtr(ShotManager::SHOT_KIND::PLAYER_SHOT, i);

		//	中ボスのポインタの取得
		MediumBoss* mediumBoss = _bossManager.GetMediumBoss();

		//	ポインタが存在するなら
		if (shot && mediumBoss)
		{
			//	当たり判定の描画
			_CircleHitCheckOfDraw(shot->GetCircle(), mediumBoss->GetCircle());
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	プレイヤーの弾と最終ボスの当たり判定の描画
//-----------------------------------------------------------------------------
void HitChecker::PlayerShotToLastBossOfDraw(ShotManager& _shotManager, BossManager& _bossManager)
{
	for (int i = 0; i < _shotManager.GetListSize(ShotManager::SHOT_KIND::PLAYER_SHOT); i++)
	{
		//	ショットのポインタの取得
		Shot* shot = _shotManager.GetShotPtr(ShotManager::SHOT_KIND::PLAYER_SHOT, i);

		//	最終ボスのポインタの取得
		LastBoss* lastBoss = _bossManager.GetLastBossPtr();

		//	ポインタが存在するなら
		if (shot && lastBoss)
		{
			if (lastBoss->GetIsAppearance())
			{
				//	当たり判定の描画
				DrawCube3D(shot->GetRect().m_vertexTop, shot->GetRect().m_vertexUnder, 5, GetColor(255, 255, 255), false);
				_CircleHitCheckOfDraw(shot->GetCircle(), lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_BODY));

				if (lastBoss->GetIsLeftCoreAlive())
				{
					DrawSphere3D(lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_CORE, 1).m_centerPoint, lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_CORE, 1).m_radius, 5, 0x00ffff, 0x00ffff, false);
					DrawSphere3D(lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_ARM, 1).m_centerPoint, lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_ARM, 1).m_radius, 5, 0x00ffff, 0x00ffff, false);
					_BoxHitCheckOfDraw(lastBoss->GetLaserRect(1), lastBoss->GetArmRect(1));
				}

				if (lastBoss->GetIsRightCoreAlive())
				{
					DrawSphere3D(lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_CORE, 0).m_centerPoint, lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_CORE, 0).m_radius, 5, 0x00ffff, 0x00ffff, false);
					DrawSphere3D(lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_ARM, 0).m_centerPoint, lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_ARM, 0).m_radius, 5, 0x00ffff, 0x00ffff, false);
					_BoxHitCheckOfDraw(lastBoss->GetLaserRect(0), lastBoss->GetArmRect(0));
				}

				if (lastBoss->GetIsRightSideBarrelAlive())
				{
					DrawSphere3D(lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_SIDE_BARREL, 0).m_centerPoint, lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_SIDE_BARREL, 0).m_radius, 5, 0x00ffff, 0x00ffff, false);
				}

				if (lastBoss->GetIsLeftSideBarrelAlive())
				{
					DrawSphere3D(lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_SIDE_BARREL, 1).m_centerPoint, lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_SIDE_BARREL, 1).m_radius, 5, 0x00ffff, 0x00ffff, false);
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	プレイヤー滞在弾とエネミーの当たり判定
//-----------------------------------------------------------------------------
void HitChecker::PlayerStayShotToEnemyOfDraw(StayShotManager& _stayShotManager, EnemyManager& _enemyManager)
{
	for (int i = 0; i < _stayShotManager.GetListSize(); i++)
	{
		//	ショットのポインタの取得
		StayShot* stayShot = _stayShotManager.GetStayShotPtr(i);

		for (int j = 0; j < _enemyManager.GetListSize(); j++)
		{
			//	エネミーのポインタの取得
			EnemyBase* enemy = _enemyManager.GetEnemyPtr(j);

			//	ポインタが存在するなら
			if (stayShot && enemy)
			{
				//	当たり判定の描画
				_CircleHitCheckOfDraw(stayShot->GetCircle(), enemy->GetCircle());
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	プレイヤー滞在弾と中ボスの当たり判定
//-----------------------------------------------------------------------------
void HitChecker::PlayerStayShotToMediumBossOfDraw(StayShotManager& _stayShotManager, BossManager& _bossManager)
{
	for (int i = 0; i < _stayShotManager.GetListSize(); i++)
	{
		//	ショットのポインタの取得
		StayShot* stayShot = _stayShotManager.GetStayShotPtr(i);

		//	中ボスのポインタの取得
		MediumBoss* mediumBoss = _bossManager.GetMediumBoss();

		//	ポインタが存在するなら
		if (stayShot && mediumBoss)
		{
			//	当たり判定の描画
			_CircleHitCheckOfDraw(stayShot->GetCircle(), mediumBoss->GetCircle());
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	プレイヤー滞在弾と最終ボスの当たり判定の描画
//-----------------------------------------------------------------------------
void HitChecker::PlayerStayShotToLastBossOfDraw(StayShotManager& _stayShotManager, BossManager& _bossManager)
{
	for (int i = 0; i < _stayShotManager.GetListSize(); i++)
	{
		//	ショットのポインタの取得
		StayShot* stayShot = _stayShotManager.GetStayShotPtr(i);

		//	最終ボスのポインタの取得
		LastBoss* lastBoss = _bossManager.GetLastBossPtr();

		//	ポインタが存在するなら
		if (stayShot && lastBoss)
		{
			if (lastBoss->GetIsAppearance())
			{
				//	当たり判定の描画
				DrawCube3D(stayShot->GetRect().m_vertexTop, stayShot->GetRect().m_vertexUnder, 5, GetColor(255, 255, 255), false);
				_CircleHitCheckOfDraw(stayShot->GetCircle(), lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_BODY));

				if (lastBoss->GetIsLeftCoreAlive())
				{
					DrawSphere3D(lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_CORE, 1).m_centerPoint, lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_CORE, 1).m_radius, 5, 0x00ffff, 0x00ffff, false);
					DrawSphere3D(lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_ARM, 1).m_centerPoint, lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_ARM, 1).m_radius, 5, 0x00ffff, 0x00ffff, false);
					_BoxHitCheckOfDraw(lastBoss->GetLaserRect(1), lastBoss->GetArmRect(1));
				}

				if (lastBoss->GetIsRightCoreAlive())
				{
					DrawSphere3D(lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_CORE, 0).m_centerPoint, lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_CORE, 0).m_radius, 5, 0x00ffff, 0x00ffff, false);
					DrawSphere3D(lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_ARM, 0).m_centerPoint, lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_ARM, 0).m_radius, 5, 0x00ffff, 0x00ffff, false);
					_BoxHitCheckOfDraw(lastBoss->GetLaserRect(0), lastBoss->GetArmRect(0));
				}

				if (lastBoss->GetIsRightSideBarrelAlive())
				{
					DrawSphere3D(lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_SIDE_BARREL, 0).m_centerPoint, lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_SIDE_BARREL, 0).m_radius, 5, 0x00ffff, 0x00ffff, false);
				}

				if (lastBoss->GetIsLeftSideBarrelAlive())
				{
					DrawSphere3D(lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_SIDE_BARREL, 1).m_centerPoint, lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_SIDE_BARREL, 1).m_radius, 5, 0x00ffff, 0x00ffff, false);
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	プレイヤーとアイテムの当たり判定の描画
//-----------------------------------------------------------------------------
void HitChecker::PlayerToItemOfDraw(PlayerManager& _playerManager, ItemManager& _item)
{
	//	プレイヤーのポインタの取得
	PlayerBase* player = _playerManager.GetPlayerPtr();

	for (int i = 0; i < _item.GetListSize(); i++)
	{
		//	アイテムのポインタの取得
		Item* item = _item.GetItemPtr(i);

		//	ポインタが存在するなら
		if (player && item)
		{
			//	当たり判定の描画
			_BoxHitCheckOfDraw(player->GetItemHitRect(), item->GetRect());
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	プレイヤーの必殺技とすべての当たり判定の描画
//-----------------------------------------------------------------------------
void HitChecker::PlayerSpecialAttackToAllObjectOfDraw(PlayerManager& _playerManager, EnemyManager& _enemyManager, BossManager& _bossManager)
{
	//	プレイヤーのポインタの取得
	PlayerBase* player = _playerManager.GetPlayerPtr();

	const int size = _enemyManager.GetListSize();
	for (int i = 0; i < size; i++)
	{
		//	エネミーのポインタの取得
		EnemyBase* enemy = _enemyManager.GetEnemyPtr(i);

		//	ポインタが存在するなら
		if (player && enemy)
		{
			//	当たり判定の描画
			_BoxHitCheckOfDraw(player->GetLaserHitRect(), enemy->GetRect());
		}
	}

	//	中ボスのポインタの取得
	MediumBoss* mediumBoss = _bossManager.GetMediumBoss();

	//	ポインタが存在するなら
	if (player && mediumBoss)
	{
		//	当たり判定の描画
		_BoxHitCheckOfDraw(player->GetLaserHitRect(), mediumBoss->GetRect());
	}

	LastBoss* lastBoss = _bossManager.GetLastBossPtr();
	if (player && lastBoss)
	{
		if (lastBoss->GetIsAppearance())
		{
			//	当たり判定の描画
			//_CircleHitCheckOfDraw(player->GetLaserCircle(), lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_BODY));

			if (lastBoss->GetIsLeftCoreAlive())
			{
				DrawSphere3D(lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_CORE, 1).m_centerPoint, lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_CORE, 1).m_radius, 5, 0x00ffff, 0x00ffff, false);
				DrawSphere3D(lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_ARM, 1).m_centerPoint, lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_ARM, 1).m_radius, 5, 0x00ffff, 0x00ffff, false);
				_BoxHitCheckOfDraw(lastBoss->GetLaserRect(1), lastBoss->GetArmRect(1));
			}

			if (lastBoss->GetIsRightCoreAlive())
			{
				DrawSphere3D(lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_CORE, 0).m_centerPoint, lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_CORE, 0).m_radius, 5, 0x00ffff, 0x00ffff, false);
				DrawSphere3D(lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_ARM, 0).m_centerPoint, lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_ARM, 0).m_radius, 5, 0x00ffff, 0x00ffff, false);
				_BoxHitCheckOfDraw(lastBoss->GetLaserRect(0), lastBoss->GetArmRect(0));
			}

			if (lastBoss->GetIsRightSideBarrelAlive())
			{
				DrawSphere3D(lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_SIDE_BARREL, 0).m_centerPoint, lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_SIDE_BARREL, 0).m_radius, 5, 0x00ffff, 0x00ffff, false);
			}

			if (lastBoss->GetIsLeftSideBarrelAlive())
			{
				DrawSphere3D(lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_SIDE_BARREL, 1).m_centerPoint, lastBoss->GetCircle(lastBoss->BOSS_PARTS_KIND::BOSS_PARTS_KIND_SIDE_BARREL, 1).m_radius, 5, 0x00ffff, 0x00ffff, false);
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	オブジェクト間の距離を測る
//-----------------------------------------------------------------------------
bool HitChecker::_AskObjectLenght(const VECTOR _pos1, const VECTOR _pos2)
{
	//	ターゲットまでのベクトルを求める
	VECTOR targetVec = VSub(_pos1, _pos2);
	float targetLen = VSize(targetVec);

	//	ターゲット間の距離が一定より小さくなったら、
	//	当たり判定を行う
	return (targetLen <= ACTIVE_LENGHT);
}

//-----------------------------------------------------------------------------
//	@brief	円形同士の当たり判定
//-----------------------------------------------------------------------------
bool HitChecker::_CircleHitCheck(Circle _target1, Circle _target2)
{
	//	距離の計算
	VECTOR distance = VSub(_target1.m_centerPoint, _target2.m_centerPoint);

	//	距離が半径より小さいときは当たり
	if (VSize(distance) < (_target1.m_radius + _target2.m_radius)) { return true; }
	return false;
}

//-----------------------------------------------------------------------------
//	@brief	矩形同士の当たり判定
//-----------------------------------------------------------------------------
bool HitChecker::_BoxHitCheck(Rect _target1, Rect _target2)
{
	//	XY軸で当たり判定
	const bool isHitAxisXY = ((_target1.m_vertexTop.x <= _target2.m_vertexUnder.x) &&
		(_target1.m_vertexTop.y <= _target2.m_vertexUnder.y) &&
		(_target1.m_vertexUnder.x >= _target2.m_vertexTop.x) &&
		(_target1.m_vertexUnder.y >= _target2.m_vertexTop.y));

	//	XZ軸で当たり判定
	const bool isHitAxisXZ = ((_target1.m_vertexTop.x <= _target2.m_vertexUnder.x) &&
		(_target1.m_vertexTop.z <= _target2.m_vertexUnder.z) &&
		(_target1.m_vertexUnder.x >= _target2.m_vertexTop.x) &&
		(_target1.m_vertexUnder.z >= _target2.m_vertexTop.z));

	//	地面との接触判定
	return  isHitAxisXY && isHitAxisXZ;
}

//-----------------------------------------------------------------------------
//	@brief	円形同士の当たり判定の描画
//-----------------------------------------------------------------------------
void HitChecker::_CircleHitCheckOfDraw(Circle _target1, Circle _target2)
{
	//	ターゲット１の描画
	DrawSphere3D(_target1.m_centerPoint, _target1.m_radius, 5, 0x00ffff, 0x00ffff, false);

	//	ターゲット２の描画
	DrawSphere3D(_target2.m_centerPoint, _target2.m_radius, 5, 0x00ffff, 0x00ffff, false);
}

//-----------------------------------------------------------------------------
//	@brief	矩形同士の当たり判定の描画
//-----------------------------------------------------------------------------
void HitChecker::_BoxHitCheckOfDraw(Rect _target1, Rect _target2)
{
	//	ターゲット１の描画
	DrawCube3D(_target1.m_vertexTop, _target1.m_vertexUnder, 5, GetColor(255, 255, 255), false);

	//	ターゲット２の描画
	DrawCube3D(_target2.m_vertexTop, _target2.m_vertexUnder, 5, GetColor(255, 255, 255), false);
}
