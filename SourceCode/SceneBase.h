
//=============================================================================
//	@file	SceneBase.h
//	@brief	シーンベース
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

//-----------------------------------------------------------------------------
//	@brief	前方宣言
//-----------------------------------------------------------------------------
class SceneManager;
class Pad;
class BackGround;
class Camera;
class Sound;
class SoundEffect;

//-----------------------------------------------------------------------------
//	@brief	シーン基底クラス
//-----------------------------------------------------------------------------
class SceneBase
{
public:

//===================== 関数 ======================//

	//	コンストラクタ / デストラクタ
	SceneBase() {};
	virtual ~SceneBase() {};

	//	作成処理
	virtual void Create() = 0;

	//	解放処理
	virtual void Release()	= 0;

	//	初期処理
	virtual void Initialize(Pad& _pad, Camera& _camera) = 0;

	//	更新処理
	virtual void Update(Pad& _pad, Camera& _camera, BackGround& _backGround, SoundEffect& _soundEffect, SceneManager* _sceneManager) = 0;

	//	描画処理
	virtual void Draw(BackGround& _backGround) = 0;

protected:

//================== 内部処理関数 ==================//

	//	シーン切り替え
	virtual void _ChangeScene(Pad& _pad, SoundEffect& _soundEffect, SceneManager* _sceneManager) {};

	//	最終的な解放
	virtual void _FinalRelease() = 0;

//=================== メンバ変数 ===================//

	Sound*				m_pBackgroundMusic;		    //	BGM
	float				m_changeSceneWaitTime;	    //	シーン切り替えの遅延

};