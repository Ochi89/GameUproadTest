
//=============================================================================
//	@file	SceneGame.cpp
//	@brief	ゲームシーン
//	@autor	Takuya Ochi
//	@date	2018/9/28
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "SceneGame.h"
#include "SceneManager.h"
#include "Common.h"
#include "Pad.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "BossManager.h"
#include "ShotManager.h"
#include "StayShotManager.h"
#include "ItemManager.h"
#include "Camera.h"
#include "HitChecker.h"
#include "UILife.h"
#include "UIGauge.h"
#include "UIScore.h"
#include "UIStayShot.h"
#include "UIGameOver.h"
#include "UIBackPixel.h"
#include "UICongratulation.h"
#include "BackGround.h"
#include "EffekseerManager.h"
#include "SoundEffect.h"
#include "Sound.h"
#include "Warning.h"
#include "PlayCounter.h"
#include "LastBoss.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const float SceneGame::MAX_CHANGE_LOSE_SCENE_WAIT_TIME = 180.0f;	//	シーン切り替えの遅延の最大
const float SceneGame::MAX_CHANGE_WIN_SCENE_WAIT_TIME = 600.0f;		//	シーン切り替えの遅延の最大
const float SceneGame::MAX_CHANGE_TITLE_SCENE_WAIT_TIME = 30.0f;	//	シーン切り替えの遅延の最大
const float SceneGame::MAX_CHANGE_BGM_WAIT_TIME = 240.0f;			//	BGM切り替えの遅延の最大

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
SceneGame::SceneGame()
{
	//	まだどこも指していないので、NULLで初期化
    m_pPlayerManager = NULL;
	m_pEnemyManager = NULL;
	m_pBossManager = NULL;
	m_pShotManager = NULL;
	m_pStayShotManager = NULL;
	m_pItemManager = NULL;
	m_pUILife = NULL;
	m_pUIGauge = NULL;
	m_pUIStayShot = NULL;
	m_pUIGameOver = NULL;
	m_pUIBackPixel = NULL;
	m_pUICongratulation = NULL;
	m_pEffekseerManager = NULL;
	m_pBackgroundMusic = NULL;
	m_pWarning = NULL;
	m_pPlayCounter = NULL;
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
SceneGame::~SceneGame()
{
	//	最終確認
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成処理
//-----------------------------------------------------------------------------
void SceneGame::Create()
{
	//	ロードの描画
	LOADING->Draw();

	//	エフェクトの作成
	m_pEffekseerManager = new EffekseerManager();

    //  プレイヤーの作成
    m_pPlayerManager = new PlayerManager();
    m_pPlayerManager->Create(m_pPlayerManager->PLAYER_INFO::PLAYER_GAME);
	
	//	エネミーの作成
	m_pEnemyManager = new EnemyManager();
    m_pEnemyManager->Create();
    m_pEnemyManager->Initialize();

	//	ボスの作成
	m_pBossManager = new BossManager();
    m_pBossManager->Create();
    m_pBossManager->Initialize();

	//	ショットの作成
	m_pShotManager = new ShotManager();
    m_pShotManager->Create();

	//	滞在ショットの作成
	m_pStayShotManager = new StayShotManager();
    m_pStayShotManager->Create();
    m_pStayShotManager->Initialize();

	//	アイテムの作成
	m_pItemManager = new ItemManager();
    m_pItemManager->Create();

	//	ライフUIの作成
	m_pUILife = new UILife();
    m_pUILife->Create();

	//	ゲージUIの作成
	m_pUIGauge = new UIGauge();
    m_pUIGauge->Create();

	//	ステイショットUIの作成
	m_pUIStayShot = new UIStayShot();
    m_pUIStayShot->Create();

	//	クリアUI
	m_pUICongratulation = new UICongratulation();
    m_pUICongratulation->Create();

	//	背景ピクセルUIの作成
	m_pUIBackPixel = new UIBackPixel();

	//	ゲームオーバーUIの作成
	m_pUIGameOver = new UIGameOver();
    m_pUIGameOver->Create();

	//	フォントの作成処理
	FONT->Create();

	//	サウンドの作成
	m_pBackgroundMusic = new Sound("Data/Sound/BGM/Game.mp3", "Data/Sound/BGM/Game2.mp3");
	m_pBackgroundMusic->OnPlay(DX_PLAYTYPE_LOOP);
	
	//	警告の作成
	m_pWarning = new Warning();

	//	プレイ回数の作成、加算し
	//	これ以上使用しないので、削除する
	m_pPlayCounter = new PlayCounter();
	m_pPlayCounter->AddPlayCounter();
	CommonSafe::Delete(m_pPlayCounter);

	m_isDebug = false;
	m_isDebugDraw = false;
}

//-----------------------------------------------------------------------------
//	@brief	解放
//-----------------------------------------------------------------------------
void SceneGame::Release()
{
    //	プレイヤーの解放
    CommonSafe::Release(m_pPlayerManager);

	//	エネミーの解放
	CommonSafe::Release(m_pEnemyManager);

	//	ボスの解放
	CommonSafe::Release(m_pBossManager);

	//	ショットの解放
	CommonSafe::Release(m_pShotManager);

	//	滞在ショットの解放
	CommonSafe::Release(m_pStayShotManager);

	//	アイテムの解放
	CommonSafe::Release(m_pItemManager);

	//	ライフUIの解放
	CommonSafe::Release(m_pUILife);

	//	ゲージUIの解放
	CommonSafe::Release(m_pUIGauge);

	//	ステイショットUIの解放
	CommonSafe::Release(m_pUIStayShot);

	//	ゲームオーバーUIの解放
	CommonSafe::Release(m_pUIGameOver);

	//	背景ピクセルUIの解放
	CommonSafe::Delete(m_pUIBackPixel);

	//	クリアUI
	CommonSafe::Release(m_pUICongratulation);

	//	エフェクトの解放
	CommonSafe::Delete(m_pEffekseerManager);

	//	BGMの解放
	CommonSafe::Delete(m_pBackgroundMusic);

	//	警告の解放
	CommonSafe::Delete(m_pWarning);
}

//-----------------------------------------------------------------------------
//	@brief	初期化
//-----------------------------------------------------------------------------
void SceneGame::Initialize(Pad& _pad, Camera& _camera)
{
	//	各クラスの初期処理
	_camera.Initialize();
    m_pEnemyManager->Initialize();
    m_pBossManager->Initialize();
    m_pStayShotManager->Initialize();

	//	キーの入力を使用中にする
	KEY->AllInUse();

	//	パッドの入力を使用中にする
	_pad.AllInUse();

	//	シーン遷移遅延時間の初期化
	m_changeSceneWaitTime = 0.0f;
}

//-----------------------------------------------------------------------------
//	@brief	更新
//-----------------------------------------------------------------------------
void SceneGame::Update(Pad& _pad, Camera& _camera, BackGround& _backGround, SoundEffect& _soundEffect, SceneManager* _sceneManager)
{
	//	メニュー中ではければ、更新する
	const bool isNotMenu = !MENU->GetIsSkipUpdate();
	if (isNotMenu)
	{
		//	カメラの更新処理
		_camera.Update(*m_pPlayerManager);

		//	プレイヤーの更新処理
        m_pPlayerManager->Update(_pad, *m_pShotManager, *m_pStayShotManager, _soundEffect, *m_pUIGauge);

		//	エネミーの更新処理
		m_pEnemyManager->Update(*m_pPlayerManager, *m_pShotManager, *m_pBossManager, *m_pItemManager, *m_pBackgroundMusic, _soundEffect, *m_pWarning);

		//	ボスの更新処理
		m_pBossManager->Update(*m_pShotManager, _soundEffect);

		//	ショットの更新処理
		m_pShotManager->Update();

		//	滞在ショットの更新処理
		m_pStayShotManager->Update(_soundEffect);

		//	アイテムの更新処理
		m_pItemManager->Update();

		//	背景の更新処理
		_backGround.Update();

		//	ゲージUIの更新処理
		m_pUIGauge->Update(_soundEffect);

		//	ステイショットUIの更新処理
		m_pUIStayShot->Update(*m_pPlayerManager,_soundEffect);

		//	ゲームオーバーUIの更新処理
		m_pUIGameOver->Update(*m_pPlayerManager);

		//	背景ピクセルUIの更新処理
		m_pUIBackPixel->Update();

		//	クリアUIの更新処理
		m_pUICongratulation->Update(*m_pPlayerManager);

		//	演出時のゲージの更新処理
		PRODUCTION->Update();

		//	スコアの更新処理
		SCORE->Update();

		//	エフェクトの更新処理
		m_pEffekseerManager->Update();

		//	警告
		m_pWarning->Update();

		//	BGMの切り替え
		m_pBackgroundMusic->ChangeSound(DX_PLAYTYPE_LOOP, MAX_CHANGE_BGM_WAIT_TIME);

		//	すべての当たり判定
		_AllHitChecker(_soundEffect);
	}

	//	メニューの更新処理
	MENU->Update(_pad, _soundEffect);

	//	SEの更新
	_soundEffect.Update();

	//	デバッグ
	_Debug();

	//	シーンの変更
	_ChangeScene(_pad, _soundEffect, _sceneManager);
}

//-----------------------------------------------------------------------------
//	@brief	描画
//-----------------------------------------------------------------------------
void SceneGame::Draw(BackGround& _backGround)
{
	//	背景の描画処理
	_backGround.Draw();

	//	背景ピクセルUIの描画処理
	m_pUIBackPixel->Draw();

	//	エネミーの描画処理
	m_pEnemyManager->Draw();

	//	ボスの描画処理
	m_pBossManager->Draw();

	//	ショットの描画処理
	m_pShotManager->Draw();

	//	プレイヤーの描画処理
    m_pPlayerManager->Draw();

	//	アイテムの描画処理
	m_pItemManager->Draw();

	//	ライフUIの描画処理
	m_pUILife->Draw(*m_pPlayerManager);

	//	ゲージUIの描画処理
	m_pUIGauge->Draw();

	//	ステイショットUIの描画処理
	m_pUIStayShot->Draw(*m_pPlayerManager);

	//	ゲームオーバーUIの描画処理
	m_pUIGameOver->Draw();

	//	クリアUIの描画処理
	m_pUICongratulation->Draw();

	//	警告の描画
	m_pWarning->Draw();

	//	スコアの描画処理
	SCORE->Draw();

	//	エフェクトの描画処理
	m_pEffekseerManager->Draw();

	//	メニューの描画処理
	MENU->Draw();

	//	演出時のゲージの描画処理
	PRODUCTION->Draw();

	//	すべての当たり判定の描画
	_AllHitCheckerOfDraw();
}

//-----------------------------------------------------------------------------
//	@brief	すべての当たり判定
//-----------------------------------------------------------------------------
void SceneGame::_AllHitChecker(SoundEffect& _soundEffect)
{
	//	演出中は当たり判定を無視
	const bool isProduction = PRODUCTION->GetIsSceneProduction() || PRODUCTION->GetIsSpecialProduction() || PRODUCTION->GetIsClearProduction();
	if (!isProduction)
	{
		
		//	ボスが生きている間と、デバッグ時以外は、
		//	プレイヤーの機体の当たり判定を取る
		LastBoss* lastBoss = m_pBossManager->GetLastBossPtr();
		const bool isNntDeath = !lastBoss->GetIsDeath();
		const bool isNotDebug = !m_isDebug;
		const bool isActive = isNntDeath && isNotDebug;
		if (isActive)
		{
			//	プレイヤーとエネミーの当たり判定
			//	エネミーの死亡時にアイテムの生成も行う
			HitChecker::PlayerToEnemy(*m_pPlayerManager, *m_pEnemyManager, *m_pItemManager);

			//	プレイヤーと中ボスの当たり判定
			HitChecker::PlayerToMediumBoss(*m_pPlayerManager, *m_pBossManager);

			//	プレイヤーと最終ボスの当たり判定
			HitChecker::PlayerToLastBoss(*m_pPlayerManager, *m_pBossManager);

			//	プレイヤーとエネミーの弾の当たり判定
			HitChecker::PlayerToEnemyShot(*m_pPlayerManager, *m_pShotManager);
		}

		//	プレイヤーの弾とエネミーの当たり判定
		//	エネミーの死亡時にアイテムの生成も行う
		HitChecker::PlayerShotToEnemy(*m_pShotManager, *m_pEnemyManager, *m_pItemManager, *m_pUIGauge);

		//	プレイヤーの弾と中ボスの当たり判定
		HitChecker::PlayerShotToMediumBoss(*m_pShotManager, *m_pBossManager, *m_pUIGauge);

		//	プレイヤーの弾と最終ボスの当たり判定
		HitChecker::PlayerShotToLastBoss(*m_pPlayerManager, *m_pShotManager, *m_pBossManager, *m_pUIGauge);

		//	プレイヤー滞在弾とエネミーの当たり判定
		HitChecker::PlayerStayShotToEnemy(*m_pStayShotManager, *m_pEnemyManager, *m_pItemManager, *m_pUIGauge);

		//	プレイヤー滞在弾と中ボスの当たり判定
		HitChecker::PlayerStayShotToMediumBoss(*m_pStayShotManager, *m_pBossManager, *m_pUIGauge);

		//	プレイヤー滞在弾と最終ボスの当たり判定
		HitChecker::PlayerStayShotToLastBoss(*m_pPlayerManager, *m_pStayShotManager, *m_pBossManager, *m_pUIGauge);
	}

	//	プレイヤーとアイテムの当たり判定
	HitChecker::PlayerToItem(*m_pPlayerManager, *m_pItemManager, _soundEffect, *m_pUIGauge);

	//	必殺技の当たり判定
	HitChecker::PlayerSpecialAttackToAllObject(*m_pPlayerManager, *m_pEnemyManager,  *m_pBossManager, *m_pShotManager, *m_pItemManager);
}

//-----------------------------------------------------------------------------
//	@brief	すべての当たり判定の描画
//-----------------------------------------------------------------------------
void SceneGame::_AllHitCheckerOfDraw()
{
	//	デバッグ中は描画
	const bool isDebug = m_isDebug || m_isDebugDraw;
	if (isDebug)
	{
		//	プレイヤーとエネミーの当たり判定の描画
		HitChecker::PlayerToEnemyOfDraw(*m_pPlayerManager, *m_pEnemyManager);

		//	プレイヤーと中ボスの当たり判定の描画
		HitChecker::PlayerToMediumBossOfDraw(*m_pPlayerManager, *m_pBossManager);

		//	プレイヤーと最終ボスの当たり判定の描画
		HitChecker::PlayerToLastBossOfDraw(*m_pPlayerManager, *m_pBossManager);

		// プレイヤーとエネミーの弾の当たり判定の描画
		HitChecker::PlayerToEnemyShotOfDraw(*m_pPlayerManager, *m_pShotManager);

		// プレイヤーの弾とエネミーの当たり判定の描画
		HitChecker::PlayerShotToEnemyOfDraw(*m_pShotManager, *m_pEnemyManager);

		//	プレイヤーの弾と中ボスの当たり判定の描画
		HitChecker::PlayerShotToMediumBossOfDraw(*m_pShotManager, *m_pBossManager);

		//	プレイヤー弾と最終ボスの当たり判定の描画
		HitChecker::PlayerShotToLastBossOfDraw(*m_pShotManager, *m_pBossManager);

		//	プレイヤー滞在弾とエネミーの当たり判定
		HitChecker::PlayerStayShotToEnemyOfDraw(*m_pStayShotManager, *m_pEnemyManager);

		//	プレイヤーの滞在弾と中ボスの当たり判定の描画
		HitChecker::PlayerStayShotToMediumBossOfDraw(*m_pStayShotManager, *m_pBossManager);

		//	プレイヤー滞在弾と最終ボスの当たり判定の描画
		HitChecker::PlayerStayShotToLastBossOfDraw(*m_pStayShotManager, *m_pBossManager);

		//	プレイヤーとアイテムの当たり判定の描画
		HitChecker::PlayerToItemOfDraw(*m_pPlayerManager, *m_pItemManager);

		//	必殺技の当たり判定
		HitChecker::PlayerSpecialAttackToAllObjectOfDraw(*m_pPlayerManager, *m_pEnemyManager, *m_pBossManager);
	}
}

//-----------------------------------------------------------------------------
//	@brief	デバッグ
//-----------------------------------------------------------------------------
void SceneGame::_Debug()
{
	//	デバッグの切り替え
	const bool isDebug = KEY->Push(KEY_INPUT_F4);
	if (isDebug) { m_isDebug = !m_isDebug; }

	//	デバッグ描画の切り替え
	const bool isDebugDraw = KEY->Push(KEY_INPUT_F3);
	if (isDebugDraw) { m_isDebugDraw = !m_isDebugDraw; }
}

//-----------------------------------------------------------------------------
//	@brief	シーンの切り替え
//-----------------------------------------------------------------------------
void SceneGame::_ChangeScene(Pad& _pad, SoundEffect& _soundEffect, SceneManager* _sceneManager)
{
	//	リザルトシーンへ
	PlayerBase* player = m_pPlayerManager->GetPlayerPtr();
	const bool isLoseActive = !player->GetIsAlive();
	if (isLoseActive)
	{
		m_changeSceneWaitTime++;
		const bool isChangeScene = m_changeSceneWaitTime >= MAX_CHANGE_LOSE_SCENE_WAIT_TIME;
		if (isChangeScene) { _sceneManager->ChangeScene(SceneManager::SCENE_KIND::RESULT_SCENE); }
		return;
	}

	//	リザルトシーンへ
	const bool isWinActive = m_pPlayerManager->GetIsClear();
	if (isWinActive)
	{
		m_changeSceneWaitTime++;
		const bool isChangeScene = m_changeSceneWaitTime >= MAX_CHANGE_WIN_SCENE_WAIT_TIME;
		if (isChangeScene) { _sceneManager->ChangeScene(SceneManager::SCENE_KIND::RESULT_SCENE); }
		return;
	}

	//	タイトルへ
	const bool isTitle = MENU->GetIsGoToTitle();
	if (isTitle)
	{
		m_changeSceneWaitTime++;
		const bool isChangeScene = m_changeSceneWaitTime >= MAX_CHANGE_TITLE_SCENE_WAIT_TIME;
		if (isChangeScene) { _sceneManager->ChangeScene(SceneManager::SCENE_KIND::TITLE_SCENE); }
		return;
	}

}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放
//-----------------------------------------------------------------------------
void SceneGame::_FinalRelease()
{
    //	プレイヤーの解放
    CommonSafe::Release(m_pPlayerManager);

	//	エネミーの解放
	CommonSafe::Release(m_pEnemyManager);

	//	ボスの解放
	CommonSafe::Release(m_pBossManager);

	//	ショットの解放
	CommonSafe::Release(m_pShotManager);

	//	滞在ショットの解放
	CommonSafe::Release(m_pStayShotManager);

	//	アイテムの解放
	CommonSafe::Release(m_pItemManager);

	//	ライフUIの解放
	CommonSafe::Release(m_pUILife);

	//	ゲージUIの解放
	CommonSafe::Release(m_pUIGauge);

	//	ステイショットUIの解放
	CommonSafe::Release(m_pUIStayShot);

	//	ゲームオーバーUIの解放
	CommonSafe::Release(m_pUIGameOver);

	//	背景ピクセルUIの解放
	CommonSafe::Delete(m_pUIBackPixel);

	//	クリアUI
	CommonSafe::Release(m_pUICongratulation);

	//	エフェクトの解放
	CommonSafe::Delete(m_pEffekseerManager);

	//	BGMの解放
	CommonSafe::Delete(m_pBackgroundMusic);

	//	警告の解放
	CommonSafe::Delete(m_pWarning);
}
