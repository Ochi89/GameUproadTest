
//=============================================================================
//	@file	SceneDemo.cpp
//	@brief	デモシーン
//	@autor	Takuya Ochi
//	@date	2018/1/13
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "SceneDemo.h"
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
#include "UIDemoPlay.h"
#include "UIBackPixel.h"
#include "BackGround.h"
#include "EffekseerManager.h"
#include "SoundEffect.h"
#include "Sound.h"
#include "Warning.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const float		SceneDemo::END_DEMO_TIME = 1600.0f;						//	デモの終了
const float		SceneDemo::MAX_CHANGE_LOSE_SCENE_WAIT_TIME = 60.0f;		//	シーン切り替えの遅延の最大

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
SceneDemo::SceneDemo()
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
	m_pUIDemoPlay = NULL;
	m_pUIBackPixel = NULL;
	m_pEffekseerManager = NULL;
	m_pBackgroundMusic = NULL;
	m_pWarning = NULL;
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
SceneDemo::~SceneDemo()
{
	//	最終確認
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成処理
//-----------------------------------------------------------------------------
void SceneDemo::Create()
{
	//	ロードの描画
	LOADING->Draw();

	//	エフェクトの作成
	m_pEffekseerManager = new EffekseerManager();

    //  プレイヤーの作成
    m_pPlayerManager = new PlayerManager();
    m_pPlayerManager->Create(m_pPlayerManager->PLAYER_INFO::PLAYER_DEMO);

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

	//	ステイショットUI
	m_pUIStayShot = new UIStayShot();
    m_pUIStayShot->Create();

	//	デモプレイUI1の作成
	m_pUIDemoPlay = new UIDemoPlay();
    m_pUIDemoPlay->Create();

	//	背景ピクセルUIの作成
	m_pUIBackPixel = new UIBackPixel();

	//	フォントの作成処理
	FONT->Create();

	//	サウンドの作成
	m_pBackgroundMusic = new Sound("Data/Sound/BGM/Game.mp3");
	m_pBackgroundMusic->OnPlay(DX_PLAYTYPE_LOOP);

	//	警告の作成
	m_pWarning = new Warning();
}

//-----------------------------------------------------------------------------
//	@brief	解放
//-----------------------------------------------------------------------------
void SceneDemo::Release()
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

	//	デモプレイUIの解放
	CommonSafe::Release(m_pUIDemoPlay);

	//	背景ピクセルUIの解放
	CommonSafe::Delete(m_pUIBackPixel);

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
void SceneDemo::Initialize(Pad& _pad, Camera& _camera)
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

	//	デモ時間の初期化
	m_demoTime = 0.0f;

}

//-----------------------------------------------------------------------------
//	@brief	更新
//-----------------------------------------------------------------------------
void SceneDemo::Update(Pad& _pad, Camera& _camera, BackGround& _backGround, SoundEffect& _soundEffect, SceneManager* _sceneManager)
{
	//	カメラの更新処理
	_camera.Update(*m_pPlayerManager);

	//	プレイヤーの更新処理
    m_pPlayerManager->Update(*m_pShotManager, *m_pStayShotManager, _soundEffect, *m_pUIGauge, *m_pEnemyManager);

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

	//	背景ピクセルの更新処理
	m_pUIBackPixel->Update();

	//	ゲージUIの更新処理
	m_pUIGauge->Update(_soundEffect);

	//	ステイショットUIの更新処理
	m_pUIStayShot->Update(*m_pPlayerManager, _soundEffect);

	//	デモプレイUIの更新処理
	m_pUIDemoPlay->Update();

	//	演出時のゲージの更新処理
	PRODUCTION->Update();

	//	スコアの更新処理
	SCORE->Update();

	//	エフェクトの更新処理
	m_pEffekseerManager->Update();

	//	警告
	m_pWarning->Update();

	//	すべての当たり判定
	_AllHitChecker(_soundEffect);

	//	SEの更新
	_soundEffect.Update();

	//	シーンの変更
	_ChangeScene(_pad, _soundEffect, _sceneManager);
}

//-----------------------------------------------------------------------------
//	@brief	描画
//-----------------------------------------------------------------------------
void SceneDemo::Draw(BackGround& _backGround)
{
	//	背景の描画処理
	_backGround.Draw();

	//	背景ピクセルの描画処理
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

	//	スコアの描画処理
	SCORE->Draw();

	//	演出時のゲージの描画処理
	PRODUCTION->Draw();

	//	デモプレイUIの描画処理
	m_pUIDemoPlay->Draw();

	//	エフェクトの描画処理
	m_pEffekseerManager->Draw();

	//	メニューの描画処理
	MENU->Draw();
}

//-----------------------------------------------------------------------------
//	@brief	すべての当たり判定
//-----------------------------------------------------------------------------
void SceneDemo::_AllHitChecker(SoundEffect& _soundEffect)
{
	//	演出中は当たり判定を無視
	const bool isProduction = PRODUCTION->GetIsSceneProduction() || PRODUCTION->GetIsSpecialProduction() || PRODUCTION->GetIsClearProduction();
	if (!isProduction)
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
	HitChecker::PlayerSpecialAttackToAllObject(*m_pPlayerManager, *m_pEnemyManager, *m_pBossManager, *m_pShotManager, *m_pItemManager);
}

//-----------------------------------------------------------------------------
//	@brief	シーンの切り替え
//-----------------------------------------------------------------------------
void SceneDemo::_ChangeScene(Pad& _pad, SoundEffect& _soundEffect, SceneManager* _sceneManager)
{
	//	プレイヤーの死亡時
	PlayerBase* player = m_pPlayerManager->GetPlayerPtr();
	const bool isLoseActive = !player->GetIsAlive();
	if (isLoseActive)
	{
		m_changeSceneWaitTime++;
		const bool isChangeScene = m_changeSceneWaitTime >= MAX_CHANGE_LOSE_SCENE_WAIT_TIME;
		if (isChangeScene) { _sceneManager->ChangeScene(SceneManager::SCENE_KIND::TITLE_SCENE); }
		return;
	}

	const bool isKeyActive = KEY->Push(KEY_INPUT_RETURN) || KEY->Push(KEY_INPUT_SPACE) || KEY->Push(KEY_INPUT_X) || KEY->Push(KEY_INPUT_K) || KEY->Push(KEY_INPUT_A);
	const bool isNextActive = isKeyActive || _pad.GetXInputPushButton(PadInfo::PAD_BUTTON_KIND::BUTTON_ALL);
	if (isNextActive) { _soundEffect.OnPlaySound(_soundEffect.ONE_SE_KIND::ONE_SE_MENU_DECISION); }

	//	デモ終了か、キー入力があるとき
	m_demoTime++;
	const bool isDemoEnd = m_demoTime >= END_DEMO_TIME;
	const bool isActive = isDemoEnd || isNextActive;
	
	//	タイトルシーンへ
	if (isActive) { _sceneManager->ChangeScene(SceneManager::SCENE_KIND::TITLE_SCENE); }
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放
//-----------------------------------------------------------------------------
void SceneDemo::_FinalRelease()
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

	//	デモプレイUIの解放
	CommonSafe::Release(m_pUIDemoPlay);

	//	背景ピクセルUIの解放
	CommonSafe::Delete(m_pUIBackPixel);

	//	エフェクトの解放
	CommonSafe::Delete(m_pEffekseerManager);

	//	BGMの解放
	CommonSafe::Delete(m_pBackgroundMusic);

	//	警告の解放
	CommonSafe::Delete(m_pWarning);
}
