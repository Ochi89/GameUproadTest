
//=============================================================================
//	@file	PlayerManager.cpp
//	@brief	プレイヤーマネージャー
//	@autor	Takuya Ochi
//	@date	2018/11/14
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "PlayerManager.h"
#include "PlayerBase.h"
#include "TitlePlayer.h"
#include "GamePlayer.h"
#include "DemoPlayer.h"
#include "ResultPlayer.h"
#include "Common.h"
#include "ShotManager.h"
#include "StayShotManager.h"
#include "SoundEffect.h"
#include "EnemyManager.h"
#include "EnemyBase.h"
#include "SceneManager.h"

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
PlayerManager::PlayerManager()
	: m_sourceModelHandle(-1)
{
	//	まだどこも指していないので、NULL
	m_pPlayer = NULL;
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
PlayerManager::~PlayerManager()
{
	//	最終的な解放処理
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成処理
//-----------------------------------------------------------------------------
void PlayerManager::Create(PLAYER_INFO _playerInfo)
{
    //	大本のモデルの読み込み
    m_sourceModelHandle = MV1LoadModel("Data/Model/Player/Player.pmx");
    CommonDebug::Assert((m_sourceModelHandle <= -1), " [ PlayerManager.cpp ] : error : model loading failed.");

    //	プレイヤーの切り替え
    switch (_playerInfo)
    {
        //	タイトルシーン
    case PLAYER_INFO::PLAYER_TITLE:
        m_pPlayer = new TitlePlayer(m_sourceModelHandle);
        m_pPlayer->Create();
        m_pPlayer->Initialize();
        break;

        //	ゲームシーン
    case PLAYER_INFO::PLAYER_GAME:
        m_isClear = false;
        m_isProduction = false;
        m_pPlayer = new GamePlayer(m_sourceModelHandle);
        m_pPlayer->Create();
        m_pPlayer->Initialize();
        break;

        //	リザルトシーン
    case PLAYER_INFO::PLAYER_RESULT:
        m_pPlayer = new ResultPlayer(m_sourceModelHandle);
        m_pPlayer->Create();
        m_pPlayer->Initialize();
        break;

        //	デモシーン
    case PLAYER_INFO::PLAYER_DEMO:
        m_pPlayer = new DemoPlayer(m_sourceModelHandle);
        m_pPlayer->Create();
        m_pPlayer->Initialize();
        break;
    }
}

//-----------------------------------------------------------------------------
//	@brief	解放処理
//-----------------------------------------------------------------------------
void PlayerManager::Release()
{
	//	プレイヤーの解放
	CommonSafe::Release(m_pPlayer);
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void PlayerManager::Update()
{
    //	プレイヤーの更新
    m_pPlayer->Update();
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void PlayerManager::Update(Pad& _pad, ShotManager& _shot, StayShotManager& _stayShot, SoundEffect& _soundEffect, UIGauge& _gauge)
{
    //	クリアしたときは移動停止
    if (m_isClear)
    {
        m_pPlayer->SetIsStopMove(m_isClear);
    }

    //	クリア状況をリザルトへ引き渡す
    HAND_OVER_RESULT->SetIsClear(m_isClear);

    //	プレイヤーの更新
    m_pPlayer->Update(_pad, _shot, _stayShot, _soundEffect, _gauge);
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void PlayerManager::Update(ShotManager& _shot, StayShotManager& _stayShot, SoundEffect& _soundEffect, UIGauge& _gauge, EnemyManager& _enemyManager)
{
    //	プレイヤーの更新
    m_pPlayer->Update(_shot, _stayShot, _soundEffect, _gauge, _enemyManager);
}

//-----------------------------------------------------------------------------
//	@brief	描画処理
//-----------------------------------------------------------------------------
void PlayerManager::Draw()
{
	//	プレイヤーの描画
	m_pPlayer->Draw();
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放処理
//-----------------------------------------------------------------------------
void PlayerManager::_FinalRelease()
{
	//	プレイヤーの解放
	CommonSafe::Release(m_pPlayer);
}
