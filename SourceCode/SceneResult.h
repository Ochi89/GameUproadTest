
//=============================================================================
//	@file	SceneResult.h
//	@brief	リザルトシーン
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
class PlayerManager;
class SceneManager;
class UIBackPixel;
class Pad;
class Ranking;

//-----------------------------------------------------------------------------
//	@brief	リザルトシーンクラス
//-----------------------------------------------------------------------------
class SceneResult : public SceneBase
{
public:

//===================== 関数 ======================//

	//	コンストラクタ/ /デストラクタ
	SceneResult();
	~SceneResult();

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
	Ranking*			m_pRanking;				//	ランキング
	UIBackPixel*		m_pUIBackPixel;			//	背景ピクセルUI
	bool				m_isChangeScene;		//	シーン切り替え

//===================== 静的定数 ===================//

	static const float MAX_CHANGE_SCENE_WAIT_TIME;		//	シーン切り替えの遅延の最大

};
