
//=============================================================================
//	@file	SceneGame.h
//	@brief	ゲームシーン
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
class EnemyManager;
class BossManager;
class ShotManager;
class StayShotManager;
class ItemManager;
class Camera;
class UILife;
class UIGauge;
class UIStayShot;
class UIGameOver;
class UIBackPixel;
class UICongratulation;
class BackGround;
class EffekseerManager;
class Warning;
class PlayCounter;

//-----------------------------------------------------------------------------
//	@brief	タイトルシーン派生クラス
//-----------------------------------------------------------------------------
class SceneGame : public SceneBase
{
public:

//===================== 関数 ======================//

	//	コンストラクタ/ /デストラクタ
	SceneGame();
	~SceneGame();

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

	//	ヒットチェック
	void _AllHitChecker(SoundEffect& _soundEffect);

	//	ヒットチェックの描画
	void _AllHitCheckerOfDraw();

	//	デバッグ
	void _Debug();

	//	シーン切り替え
	void _ChangeScene(Pad& _pad, SoundEffect& _soundEffect, SceneManager* _sceneManager) override;

	//	最終的な解放
	void _FinalRelease() override;

//=================== メンバ変数 ===================//

    PlayerManager*		m_pPlayerManager;	    //	プレイヤー
	EnemyManager*		m_pEnemyManager;		//	エネミー
	BossManager*		m_pBossManager;			//	ボス
	ShotManager*		m_pShotManager;			//	ショット
	StayShotManager*	m_pStayShotManager;		//	滞在ショット
	ItemManager*		m_pItemManager;			//	アイテム
	UILife*				m_pUILife;				//	ライフUI
	UIGauge*			m_pUIGauge;				//	ゲージUI
	UIStayShot*			m_pUIStayShot;			//	ステイショットUI
	UIGameOver*			m_pUIGameOver;			//	ゲームオーバーUI
	UIBackPixel*		m_pUIBackPixel;			//	背景ピクセルUI
	UICongratulation*	m_pUICongratulation;	//	クリアUI
	EffekseerManager*	m_pEffekseerManager;	//	エフェクト
	Warning*			m_pWarning;				//	警告
	PlayCounter*		m_pPlayCounter;			//	プレイ回数
	bool				m_isDebug;				//	デバッグ
	bool				m_isDebugDraw;			//	デバッグ描画

//===================== 静的定数 ===================//

	static const float MAX_CHANGE_LOSE_SCENE_WAIT_TIME;		//	シーン切り替えの遅延の最大
	static const float MAX_CHANGE_WIN_SCENE_WAIT_TIME;		//	シーン切り替えの遅延の最大
	static const float MAX_CHANGE_TITLE_SCENE_WAIT_TIME;	//	シーン切り替えの遅延の最大
	static const float MAX_CHANGE_BGM_WAIT_TIME;			//	BGM切り替えの遅延の最大
};
