
//=============================================================================
//	@file	SceneManager.cpp
//	@brief	シーンマネージャー
//	@autor	Takuya Ochi
//	@date	2018/9/28
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "SceneManager.h"
#include "SceneBase.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneResult.h"
#include "SceneDemo.h"
#include "Common.h"
#include "Pad.h"
#include "BackGround.h"
#include "Camera.h"
#include "Sound.h"
#include "SoundEffect.h"

//#define CREATE_GRID 0

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
SceneManager::SceneManager()
{
	//	まだどこも指していないので NULL
	m_pScene = NULL;
	m_pPad1 = NULL;
	m_pPad2 = NULL;
	m_pPad3 = NULL;
	m_pPad4 = NULL;
	m_pCamera = NULL;
	m_pBackGround = NULL;
	m_pSoundEffect = NULL;
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
SceneManager::~SceneManager()
{
	//	最終確認
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成処理
//-----------------------------------------------------------------------------
void SceneManager::Create()
{
	//	カメラの作成
	m_pCamera = new Camera;

	//	背景の作成
	m_pBackGround = new BackGround();
	m_pBackGround->Create();

	//	パッドの読み込み
	m_pPad1 = new Pad();
	m_pPad2 = new Pad();
	m_pPad3 = new Pad();
	m_pPad4 = new Pad();

	//	SEの作成
	m_pSoundEffect = new SoundEffect();
	m_pSoundEffect->Create();
}

//-----------------------------------------------------------------------------
//	@brief	解放処理
//-----------------------------------------------------------------------------
void SceneManager::Release()
{
	//	パッドの解放
	_PadRelease();

	//	カメラの解放
	CommonSafe::Delete(m_pCamera);

	//	背景の解放
	CommonSafe::Release(m_pBackGround);

	//	SEの解放
	CommonSafe::Release(m_pSoundEffect);

	//	シーンの解放
	CommonSafe::Release(m_pScene);
}

//-----------------------------------------------------------------------------
//	@brief	初期処理
//-----------------------------------------------------------------------------
void SceneManager::Initialize()
{
	//	ライトの調整
	int lightHandle = CreateDirLightHandle(VGet(1.0f, -1.0f, 1.0f));
	SetLightDirectionHandle(lightHandle, VGet(-0.5f, 0.0f, 0.0f));
}

//-----------------------------------------------------------------------------
//	@brief	シーンの切り替え
//-----------------------------------------------------------------------------
void SceneManager::ChangeScene(SCENE_KIND _sceneKind)
{
	//	シーンが存在し切り替えを行ったときに、解放する
	if (m_pScene) { CommonSafe::Release(m_pScene); }

	//	シーンの種類で切り替える
	switch (_sceneKind)
	{

		//	タイトルシーン
	case SCENE_KIND::TITLE_SCENE:
		m_pScene = new SceneTitle();
        m_pScene->Create();
		m_pScene->Initialize(*m_pPad1, *m_pCamera);
		break;

		//	ゲームシーン
	case SCENE_KIND::GAME_SCENE:
		m_pScene = new SceneGame();
        m_pScene->Create();
		m_pScene->Initialize(*m_pPad1, *m_pCamera);
		break;

		//	リザルトシーン
	case SCENE_KIND::RESULT_SCENE:
		m_pScene = new SceneResult();
        m_pScene->Create();
		m_pScene->Initialize(*m_pPad1, *m_pCamera);
		break;

		//	デモシーン
	case SCENE_KIND::DEMO_SCENE:
		m_pScene = new SceneDemo();
        m_pScene->Create();
		m_pScene->Initialize(*m_pPad1, *m_pCamera);
		break;
	}
}

//-----------------------------------------------------------------------------
//	@brief	メインループ
//-----------------------------------------------------------------------------
void SceneManager::Ran()
{
	//	エスケープキーが押されるか、ウインドウが閉じられるまでループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		//	更新
		_Update();

		//	描画
		_Draw();
	}
}

//-----------------------------------------------------------------------------
//	@brief	更新
//-----------------------------------------------------------------------------
void SceneManager::_Update()
{
	//	パッドの入力を取る
	_PadInput();

	//	更新処理
    //  引数は毎回生成するとゲームが重くなるので、
    //  各シーンで使いまわしするものを渡している
	m_pScene->Update(*m_pPad1, *m_pCamera, *m_pBackGround, *m_pSoundEffect, this);

	//	画面のクリア
	ClearDrawScreen();
}

//-----------------------------------------------------------------------------
//	@brief	描画
//-----------------------------------------------------------------------------
void SceneManager::_Draw()
{
#ifdef CREATE_GRID
	
	//	グリッドの描画
	CommonFunction::CreateGrid();
	
#endif // CREATE_GRID

	//	描画処理
	m_pScene->Draw(*m_pBackGround);

	//	画面の更新
	ScreenFlip();
}

//-----------------------------------------------------------------------------
//	@brief	パッドの入力を取る
//-----------------------------------------------------------------------------
void SceneManager::_PadInput()
{
	//	パッドの入力を取る
	m_pPad1->InputPadNum(PadInfo::PAD_KIND::PAD_1);
	m_pPad2->InputPadNum(PadInfo::PAD_KIND::PAD_2);
	m_pPad3->InputPadNum(PadInfo::PAD_KIND::PAD_3);
	m_pPad4->InputPadNum(PadInfo::PAD_KIND::PAD_4);
}

//-----------------------------------------------------------------------------
//	@brief	パッドを解放する
//-----------------------------------------------------------------------------
void SceneManager::_PadRelease()
{
	//	パッド１の解放
	CommonSafe::Delete(m_pPad1);

	//	パッド２の解放
	CommonSafe::Delete(m_pPad2);

	//	パッド３の解放
	CommonSafe::Delete(m_pPad3);

	//	パッド４の解放
	CommonSafe::Delete(m_pPad4);
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放
//-----------------------------------------------------------------------------
void SceneManager::_FinalRelease()
{
	//	パッドの解放
	_PadRelease();

	//	カメラの解放
	CommonSafe::Delete(m_pCamera);

	//	背景の解放
	CommonSafe::Release(m_pBackGround);

	//	SEの解放
	CommonSafe::Release(m_pSoundEffect);

	//	シーンの解放
    CommonSafe::Release(m_pScene);
}
