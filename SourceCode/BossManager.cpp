
//=============================================================================
//	@file	BossManager.cpp
//	@brief	ボスマネージャー
//	@autor	Takuya Ochi
//	@date	2018/12/18
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "BossManager.h"
#include "MediumBoss.h"
#include "LastBoss.h"
#include "ShotManager.h"
#include "Common.h"
#include "SoundEffect.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const float BossManager::OUT_RANGE_MEDIUM_BOSS = -100.0f;			//	中ボスの範囲外
const float BossManager::OUT_RANGE_LAST_BOSS = -100.0f;				//	最終ボスの範囲外

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
BossManager::BossManager()
{
	//	まだどこも指していないので、NULL
	m_mediumBoss.m_pBuf = NULL;
	m_lastBoss.m_pBuf = NULL;
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
BossManager::~BossManager()
{
	//	最終的な解放処理
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成処理
//-----------------------------------------------------------------------------
void BossManager::Create()
{
	//	中ボス大本のモデルの読み込み
	m_mediumBoss.m_sourceModelHandle = MV1LoadModel("Data/Model/Boss/MediumBoss.mqo");
	CommonDebug::Assert((m_mediumBoss.m_sourceModelHandle <= -1), " [ BassManager.cpp ] : error : model loading failed.");

	//	最終ボス大本のモデルの読み込み
	m_lastBoss.m_sourceModelHandle = MV1LoadModel("Data/Model/Boss/BossBody.mqo");
	m_lastBoss.m_sideBarrelSourceModelHandle = MV1LoadModel("Data/Model/Boss/BossSideBarrel.mqo");
	m_lastBoss.m_armSourceModelHandle = MV1LoadModel("Data/Model/Boss/BossArm.mqo");
	m_lastBoss.m_coreSourceModelHandle = MV1LoadModel("Data/Model/Boss/BossCore.mqo");
	CommonDebug::Assert((m_lastBoss.m_sourceModelHandle <= -1), " [ BassManager.cpp ] : error : model loading failed.");
	CommonDebug::Assert((m_lastBoss.m_sideBarrelSourceModelHandle <= -1), " [ BassManager.cpp ] : error : model loading failed.");
	CommonDebug::Assert((m_lastBoss.m_armSourceModelHandle <= -1), " [ BassManager.cpp ] : error : model loading failed.");
	CommonDebug::Assert((m_lastBoss.m_coreSourceModelHandle <= -1), " [ BassManager.cpp ] : error : model loading failed.");

	//	中ボスの作成
	m_mediumBoss.m_pBuf = new MediumBoss(m_mediumBoss.m_sourceModelHandle);
	m_mediumBoss.m_pBuf->Create();
	m_mediumBoss.m_isEmerge = false;

	//	最終ボスの作成
	m_lastBoss.m_pBuf = new LastBoss(m_lastBoss.m_sourceModelHandle, m_lastBoss.m_sideBarrelSourceModelHandle, m_lastBoss.m_armSourceModelHandle, m_lastBoss.m_coreSourceModelHandle);
	m_lastBoss.m_pBuf->Create();
	m_lastBoss.m_isEmerge = false;
}

//-----------------------------------------------------------------------------
//	@brief	解放処理
//-----------------------------------------------------------------------------
void BossManager::Release()
{
	//	中ボスの解放
	CommonSafe::Release(m_mediumBoss.m_pBuf);

	//	最終ボスの解放
	CommonSafe::Release(m_lastBoss.m_pBuf);
}

//-----------------------------------------------------------------------------
//	@brief	初期処置
//-----------------------------------------------------------------------------
void BossManager::Initialize()
{
	//	中ボスの初期化
	if (m_mediumBoss.m_pBuf)
	{
		m_mediumBoss.m_pBuf->Initialize();
	}

	//	最終ボスの初期化
	if (m_lastBoss.m_pBuf)
	{
		m_lastBoss.m_pBuf->Initialize();
	}
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void BossManager::Update(ShotManager& _shot, SoundEffect& _soundEffect)
{
	const bool isNotClear = !PRODUCTION->GetIsClearProduction();
	if (isNotClear)
	{
		//	中ボスの更新
		if (m_mediumBoss.m_pBuf)
		{
			m_mediumBoss.m_pBuf->SetIsEmerge(m_mediumBoss.m_isEmerge);
			m_mediumBoss.m_pBuf->Update(_shot, _soundEffect);

			//	死んだら、メモリを解放する
			const bool isDead = m_mediumBoss.m_pBuf->GetIsDeleate() || m_mediumBoss.m_pBuf->GetPos().x <= OUT_RANGE_MEDIUM_BOSS;
			if (isDead) { CommonSafe::Release(m_mediumBoss.m_pBuf); }
		}

		//	最終ボスの更新
		if (m_lastBoss.m_pBuf)
		{
			m_lastBoss.m_pBuf->SetIsEmerge(m_lastBoss.m_isEmerge);
			m_lastBoss.m_pBuf->Update(_shot, _soundEffect);

			//	死んだら、メモリを解放する
			const bool isDead = !m_lastBoss.m_pBuf->GetIsAlive() || m_lastBoss.m_pBuf->GetPos().x <= OUT_RANGE_LAST_BOSS;
			if (isDead) { CommonSafe::Release(m_lastBoss.m_pBuf); }
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	描画処理
//-----------------------------------------------------------------------------
void BossManager::Draw()
{
	//	中ボスの描画
	if (m_mediumBoss.m_pBuf)
	{
		m_mediumBoss.m_pBuf->Draw();
	}

	//	最終ボスの描画
	if (m_lastBoss.m_pBuf)
	{
		m_lastBoss.m_pBuf->Draw();
	}
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放処理
//-----------------------------------------------------------------------------
void BossManager::_FinalRelease()
{
	//	中ボスの解放
	CommonSafe::Release(m_mediumBoss.m_pBuf);

	//	最終ボスの解放
	CommonSafe::Release(m_lastBoss.m_pBuf);
}
