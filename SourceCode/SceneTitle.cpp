
//=============================================================================
//	@file	SceneTitle.cpp
//	@brief	タイトルシーン
//	@autor	Takuya Ochi
//	@date	2018/9/28
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "SceneTitle.h"
#include "PlayerManager.h"
#include "SceneManager.h"
#include "Common.h"
#include "Pad.h"
#include "Camera.h"
#include "BackGround.h"
#include "Production.h"
#include "UIScore.h"
#include "UIBackPixel.h"
#include "SoundEffect.h"
#include "Sound.h"
#include "Title.h"
#include "Opening.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const float		SceneTitle::MAX_CHANGE_SCENE_WAIT_TIME = 80.0f;		//	シーン切り替えの遅延の最大
const float		SceneTitle::START_DEMO_TIME = 600.0f;				//	デモ開始時間

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
SceneTitle::SceneTitle()
{
	//	まだどこも指していないので、NULLで初期化
    m_pPlayerManager = NULL;
    m_pBackgroundMusic = NULL;
	m_pUIBackPixel = NULL;
	m_pTitle = NULL;
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
SceneTitle::~SceneTitle()
{
	//	最終確認
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成処理
//-----------------------------------------------------------------------------
void SceneTitle::Create()
{
    //  プレイヤーの作成
    m_pPlayerManager = new PlayerManager();
    m_pPlayerManager->Create(m_pPlayerManager->PLAYER_INFO::PLAYER_TITLE);

	//	タイトルの作成
	m_pTitle = new Title();
    m_pTitle->Create();

	//	背景ピクセルUIの作成
	m_pUIBackPixel = new UIBackPixel();

	//	サウンドの作成
	m_pBackgroundMusic = new Sound("Data/Sound/BGM/Title.mp3");
}

//-----------------------------------------------------------------------------
//	@brief	解放
//-----------------------------------------------------------------------------   
void SceneTitle::Release()
{
    //	プレイヤーの解放
    CommonSafe::Release(m_pPlayerManager);

	//	タイトルの解放
	CommonSafe::Release(m_pTitle);

	//	背景ピクセルUIの解放
	CommonSafe::Delete(m_pUIBackPixel);

	//	BGMの解放
	CommonSafe::Delete(m_pBackgroundMusic);
}

//-----------------------------------------------------------------------------
//	@brief	初期化
//-----------------------------------------------------------------------------
void SceneTitle::Initialize(Pad& _pad, Camera& _camera)
{
	//	各クラスの初期処理
	_camera.InitializeForTitle();

	//	シーン遷移遅延時間の初期化
	m_changeSceneWaitTime = 0.0f;

	//	演出の初期化
	PRODUCTION->AllReset();

	//	スコアの初期化
	SCORE->ScoreReset();

	//	メニューの初期化
	MENU->Initialize();

	//	リセット
	HAND_OVER_RESULT->Reset();

	//	キーの入力を使用中にする
	KEY->AllInUse();

	//	パッドの入力を使用中にする
	_pad.AllInUse();

	//	デモ開始時間の初期化
	m_demoTime = 0.0f;
}

//-----------------------------------------------------------------------------
//	@brief	更新
//-----------------------------------------------------------------------------
void SceneTitle::Update(Pad& _pad, Camera& _camera, BackGround& _backGround, SoundEffect& _soundEffect, SceneManager* _sceneManager)
{
	//	オープニングが終了したら
	const bool isOpeningEnd = OPENING->GetIsEndOpening();
	if (isOpeningEnd)
	{
		//	BGMの再生
		m_pBackgroundMusic->OnPlay(DX_PLAYTYPE_LOOP);

		//	カメラの更新処理
		_camera.UpdateForTitle(*m_pPlayerManager);

		//	プレイヤーの更新処理
        m_pPlayerManager->Update();

		//	背景の更新処理
		_backGround.Update();

		//	背景ピクセルの更新処理
		m_pUIBackPixel->Update();

		//	タイトルの更新処理
		m_pTitle->Update(_soundEffect);

		//	SEの更新
		_soundEffect.Update();

		//	シーンの変更
		_ChangeScene(_pad, _soundEffect, _sceneManager);

		//	デモ開始時間
		if (!PRODUCTION->GetIsTitleProduction()) { m_demoTime++; }
	}
	else
	{
		//	オープニングの更新処理
		OPENING->Update(_pad);
	}
}

//-----------------------------------------------------------------------------
//	@brief	描画
//-----------------------------------------------------------------------------
void SceneTitle::Draw(BackGround& _backGround)
{
	//	オープニングが終了したら
	const bool isOpeningEnd = OPENING->GetIsEndOpening();
	if (isOpeningEnd)
	{
		//	背景の描画処理
		_backGround.Draw();

		//	背景ピクセルUIの描画処理
		m_pUIBackPixel->Draw();

		//	プレイヤーの描画処理
        m_pPlayerManager->Draw();

		//	タイトルの描画処理
		m_pTitle->Draw();
	}
	else
	{
		//	オープニングの描画処理
		OPENING->Draw();
	}
}

//-----------------------------------------------------------------------------
//	@brief	シーンの切り替え
//-----------------------------------------------------------------------------
void SceneTitle::_ChangeScene(Pad& _pad, SoundEffect& _soundEffect, SceneManager* _sceneManager)
{
	//	演習開始
	const bool isKeyActive = KEY->Push(KEY_INPUT_RETURN) || KEY->Push(KEY_INPUT_SPACE) || KEY->Push(KEY_INPUT_X) || KEY->Push(KEY_INPUT_K) || KEY->Push(KEY_INPUT_A);
	const bool isNextActive = isKeyActive || _pad.GetXInputPushButton(PadInfo::PAD_BUTTON_KIND::BUTTON_A);
	const bool isActive = isNextActive && !PRODUCTION->GetIsTitleProduction();
	if (isActive) { PRODUCTION->SetIsTitleProduction(true); _soundEffect.OnPlaySound(_soundEffect.ONE_SE_KIND::ONE_SE_SELECT); }

	//	ゲームシーンの切り替え
	if (PRODUCTION->GetIsTitleProduction())
	{
		m_changeSceneWaitTime++;
		const bool isChangeScene = m_changeSceneWaitTime >= MAX_CHANGE_SCENE_WAIT_TIME;
		if (isChangeScene) { _sceneManager->ChangeScene(SceneManager::SCENE_KIND::GAME_SCENE); }
		return;
	}

	//	デモシーンの切り替え
	const bool isDemoStart = m_demoTime >= START_DEMO_TIME;
	if (isDemoStart)
	{
		_sceneManager->ChangeScene(SceneManager::SCENE_KIND::DEMO_SCENE);
		return;
	}
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放
//-----------------------------------------------------------------------------
void SceneTitle::_FinalRelease()
{
    //	プレイヤーの解放
    CommonSafe::Release(m_pPlayerManager);

	//	タイトルの解放
	CommonSafe::Release(m_pTitle);

	//	背景ピクセルUIの解放
	CommonSafe::Delete(m_pUIBackPixel);

	//	BGMの解放
	CommonSafe::Delete(m_pBackgroundMusic);
}
