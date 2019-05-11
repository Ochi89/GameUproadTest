
//=============================================================================
//	@file	Main.h
//	@brief	メイン
//	@autor	Takuya Ochi
//	@date	2018/9/28
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "EffekseerForDXLib.h"
#include "Common.h"
#include "SceneManager.h"
#include "SceneBase.h"

//-----------------------------------------------------------------------------
//	@brief	前方宣言
//-----------------------------------------------------------------------------
bool _isInitializeDxLib();              //  DxLibの初期化処理
bool _isInitializeEffekseer();          //  Effekseerの初期化処理

//-----------------------------------------------------------------------------
//	@brief	メイン処理
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//	DxLibの初期化処理
	if (_isInitializeDxLib()) { DxLib_End(); return 0; }

	//	Effekseerの初期化処理
	if (_isInitializeEffekseer()) { DxLib_End(); return 0; }

	//	シーンマネージャーの作成
    SceneManager* pSceneManager = new SceneManager();
    if (CommonDebug::Assert((pSceneManager == NULL), " [ Main.cpp ] : error : create scene manager failed.")) { DxLib_End(); return 0; }

	//	タイトルシーンとして作成し、初期化する
    pSceneManager->Create();
    pSceneManager->ChangeScene(pSceneManager->SCENE_KIND::TITLE_SCENE);
    pSceneManager->Initialize();
	
	//	メインループ
	pSceneManager->Ran();

	//	シーン管理クラスの解放
    CommonSafe::Release(pSceneManager);

	//	ＤＸライブラリの終了処理
	DxLib_End();

	//	ソフトの終了 
	return 0;
}

//-----------------------------------------------------------------------------
//	@brief	DxLibの初期化処理
//-----------------------------------------------------------------------------
bool _isInitializeDxLib()
{
	//	描画先を裏画面に変更する。
	SetDrawScreen(DX_SCREEN_BACK);

	//	ＤＸライブラリ初期化処理
	//	エラーが起きたら直ちに終了
	if (DxLib_Init() == -1) { return true; }

	//	画面モードのセット
	SetGraphMode(CommonConstant::WINDOW_WIDTH, CommonConstant::WINDOW_HEIGHT, CommonConstant::COLOR_BIT);
	ChangeWindowMode(FALSE);

	//	マウスの表示無効
	SetMouseDispFlag(FALSE);

	return false;
}

//-----------------------------------------------------------------------------
//	@brief	Effekseerの初期化処理
//-----------------------------------------------------------------------------
bool _isInitializeEffekseer()
{
	//	フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	//	DXライブラリのデバイスロストした時のコールバック
	//	ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	return false;
}
