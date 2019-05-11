
//=============================================================================
//	@file	SceneTitle.h
//	@brief	タイトルシーン
//	@autor	Takuya Ochi
//	@date	2018/9/28
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	プリプロセッサ
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "SceneBase.h"

//-----------------------------------------------------------------------------
//	@brief	前方宣言
//-----------------------------------------------------------------------------
class SceneManager;
class PlayerManager;
class UIBackPixel;
class Pad;
class Title;
class Opening;

//-----------------------------------------------------------------------------
//	@brief	タイトルシーン派生クラス
//-----------------------------------------------------------------------------
class SceneTitle : public SceneBase
{
public:

//===================== 関数 ======================//

	//	コンストラクタ/ /デストラクタ
	SceneTitle();
	~SceneTitle();

	//	作成処理
	void Create() override;

	//	解放処理
	void Release() override;

	//	初期処理
	void Initialize(Pad& _pad, Camera& _camera) override;

	//	更新処理
	void Update(Pad& _pad, Camera& _camera, BackGround& _backGround, SoundEffect& _soundEffect, SceneManager* _sceneManager) override;

	//	描画処理
	void Draw(BackGround& _backGround) override;

private:

//================== 内部処理関数 ==================//

	//	シーン切り替え
	void _ChangeScene(Pad& _pad, SoundEffect& _soundEffect, SceneManager* _sceneManager) override;

	//	最終的な解放
	void _FinalRelease() override;

//=================== メンバ変数 ===================//

    PlayerManager*		m_pPlayerManager;	    //	プレイヤー
	Title*				m_pTitle;				//	タイトル
	UIBackPixel*		m_pUIBackPixel;			//	背景ピクセルUI
	float				m_demoTime;				//	デモの時間

//===================== 静的定数 ===================//

	static const float MAX_CHANGE_SCENE_WAIT_TIME;		//	シーン切り替えの遅延の最大
	static const float START_DEMO_TIME;					//	デモ開始時間

};
