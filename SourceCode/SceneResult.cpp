
//=============================================================================
//	@file	SceneResult.cpp
//	@brief	リザルトシーン
//	@autor	Takuya Ochi
//	@date	2018/9/28
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "SceneResult.h"
#include "PlayerManager.h"
#include "SceneManager.h"
#include "Common.h"
#include "Pad.h"
#include "Ranking.h"
#include "Camera.h"
#include "BackGround.h"
#include "UIBackPixel.h"
#include "SoundEffect.h"
#include "Sound.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const float SceneResult::MAX_CHANGE_SCENE_WAIT_TIME = 60.0f;		//	シーン切り替えの遅延の最大

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
SceneResult::SceneResult()
{
	//	まだどこも指していないので、NULLで初期化
    m_pPlayerManager = NULL;
	m_pRanking = NULL;
	m_pUIBackPixel = NULL;
	m_pBackgroundMusic = NULL;
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
SceneResult::~SceneResult()
{
	// 最終確認
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成処理
//-----------------------------------------------------------------------------
void SceneResult::Create()
{
    //  プレイヤーの作成
    m_pPlayerManager = new PlayerManager();
    m_pPlayerManager->Create(m_pPlayerManager->PLAYER_INFO::PLAYER_RESULT);

	//	ランキングの作成
	m_pRanking = new Ranking();
    m_pRanking->Initialize();

	//	背景ピクセルUIの作成
	m_pUIBackPixel = new UIBackPixel();

	//	サウンドの作成
	m_pBackgroundMusic = new Sound("Data/Sound/BGM/Result.mp3");
	m_pBackgroundMusic->OnPlay(DX_PLAYTYPE_LOOP);
}

//-----------------------------------------------------------------------------
//	@brief	解放
//-----------------------------------------------------------------------------
void SceneResult::Release()
{
    //	プレイヤーの解放
    CommonSafe::Release(m_pPlayerManager);

	//	ランキングの解放
	CommonSafe::Delete(m_pRanking);

	//	背景ピクセルUIの解放
	CommonSafe::Delete(m_pUIBackPixel);

	//	BGMの解放
	CommonSafe::Delete(m_pBackgroundMusic);
}

//-----------------------------------------------------------------------------
//	@brief	初期化
//-----------------------------------------------------------------------------
void SceneResult::Initialize(Pad& _pad, Camera& _camera)
{
	//	初期化
    m_pRanking->Initialize();
	_camera.Initialize();

	//	シーン遷移遅延時間の初期化
	m_changeSceneWaitTime = 0.0f;

	//	キーの入力を使用中にする
	KEY->AllInUse();

	//	パッドの入力を使用中にする
	_pad.AllInUse();

	//	シーン遷移フラグの初期化
	m_isChangeScene = false;

	//	パッドの振動機能の終了
	PadInfo::Function::EndVibrationFunction(PadInfo::PAD_KIND::PAD_1);
}

//-----------------------------------------------------------------------------
//	@brief	更新
//-----------------------------------------------------------------------------
void SceneResult::Update(Pad& _pad, Camera& _camera, BackGround& _backGround, SoundEffect& _soundEffect, SceneManager* _sceneManager)
{
	//	ランキングの更新処理
	m_pRanking->Update(_pad, _soundEffect);

	//	カメラの更新処理
	_camera.UpdateForResult(*m_pPlayerManager);

	//	プレイヤーの更新処理
    m_pPlayerManager->Update();

	//	背景の更新処理
	_backGround.Update();

	//	背景ピクセルの更新処理
	m_pUIBackPixel->Update();

	//	SEの更新
	_soundEffect.Update();

	//シーンの変更
	_ChangeScene(_pad, _soundEffect, _sceneManager);
}

//-----------------------------------------------------------------------------
//	@brief	描画
//-----------------------------------------------------------------------------
void SceneResult::Draw(BackGround& _backGround)
{
	//	背景の描画処理
	_backGround.Draw();
	
	//	背景ピクセルUIの描画処理
	m_pUIBackPixel->Draw();
	
	//	プレイヤーの描画処理
    m_pPlayerManager->Draw();

	//	ランキングの描画処理
	m_pRanking->Draw();
}

//-----------------------------------------------------------------------------
//	@brief	シーンの切り替え
//-----------------------------------------------------------------------------
void SceneResult::_ChangeScene(Pad& _pad, SoundEffect& _soundEffect, SceneManager* _sceneManager)
{
	//	ランキングが開始したら
	const bool isRankingStart = m_pRanking->GetIsRankingStart();
	if (isRankingStart)
	{
		//	入力があったら次へ
		const bool isKeyActive = KEY->Push(KEY_INPUT_RETURN) || KEY->Push(KEY_INPUT_SPACE) || KEY->Push(KEY_INPUT_X) || KEY->Push(KEY_INPUT_K) || KEY->Push(KEY_INPUT_A);
		const bool isNextActive = isKeyActive || _pad.GetXInputPushButton(PadInfo::PAD_BUTTON_KIND::BUTTON_A);
		const bool isActive = isNextActive && !m_isChangeScene;
		if (isActive) { m_isChangeScene = true; _soundEffect.OnPlaySound(_soundEffect.ONE_SE_KIND::ONE_SE_MENU_DECISION); }

		//	タイトルシーンへ
		//	シーンの切り替え
		if (m_isChangeScene)
		{
			m_changeSceneWaitTime++;
			const bool isChangeScene = m_changeSceneWaitTime >= MAX_CHANGE_SCENE_WAIT_TIME;
			if (isChangeScene) { _sceneManager->ChangeScene(SceneManager::SCENE_KIND::TITLE_SCENE); }
			return;
		}
	}

	//	時間経過による、シーン切り替え
	const bool isSceneChange = m_pRanking->GetIsSceneChange();
	if (isSceneChange) { _sceneManager->ChangeScene(SceneManager::SCENE_KIND::TITLE_SCENE); }
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放
//-----------------------------------------------------------------------------
void SceneResult::_FinalRelease()
{
    //	プレイヤーの解放
    CommonSafe::Release(m_pPlayerManager);

	//	ランキングの解放
	CommonSafe::Delete(m_pRanking);

	//	背景ピクセルUIの解放
	CommonSafe::Delete(m_pUIBackPixel);

	//	BGMの解放
	CommonSafe::Delete(m_pBackgroundMusic);
}
