
//=============================================================================
//	@file	PlayerManager.cpp
//	@brief	プレイヤーマネージャー
//	@autor	Takuya Ochi
//	@date	2018/11/14
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	プリプロセッサ
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "PlayerBase.h"

//-----------------------------------------------------------------------------
//	@brief	前方宣言
//-----------------------------------------------------------------------------
class Pad;
class PlayerBase;
class ShotManager;
class StayShotManager;
class UIGauge;
class SoundEffect;
class EnemyManager;

//-----------------------------------------------------------------------------
//	@brief	プレイヤー管理クラス
//-----------------------------------------------------------------------------
class PlayerManager final
{
public:

//==================== 列挙体 =====================//

    //	シーンの種類
    enum PLAYER_INFO
    {
        PLAYER_TITLE,		//	タイトル
        PLAYER_GAME,		//	ゲーム
        PLAYER_RESULT,		//	リザルト
        PLAYER_DEMO,		//	デモ
    };

//===================== 関数 ======================//

	//	コンストラクタ / デストラクタ
	PlayerManager();
	~PlayerManager();

	//	作成処理
	void Create(PLAYER_INFO _playerInfo);

	//	解放処理
	void Release();	

	//	更新処理
    void Update();
    void Update(Pad& _pad, ShotManager& _shot, StayShotManager& _stayShot, SoundEffect& _soundEffect, UIGauge& _gauge);
    void Update(ShotManager& _shot, StayShotManager& _stayShot, SoundEffect& _soundEffect, UIGauge& _gauge, EnemyManager& _enemyManager);

	//	描画処理
	void Draw();

//==================== getter =====================//

	//	プレイヤーへのポインタの getter
	PlayerBase* GetPlayerPtr() { return m_pPlayer; }

	//	クリアしたかの getter
	const bool GetIsClear() const { return m_isClear; }

//==================== setter =====================//

	//	クリアしたかの setter
	void SetIsClear(const bool _set) { m_isClear = _set; }

	//	演出中フラグの setter
	void SetIsProduction(const bool _set) { m_isProduction = _set; }

private:

//================== 内部処理関数 ==================//

	//	最終的な解放処理
	void _FinalRelease();

//=================== メンバ変数 ===================//

	int				m_sourceModelHandle;	//	大本ハンドル
	PlayerBase*		m_pPlayer;				//	プレイヤー
	bool			m_isClear;				//	クリアフラグ
	bool			m_isProduction;			//	演出中フラグ
};