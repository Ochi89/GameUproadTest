
//=============================================================================
//	@file	EnemyManager.cpp
//	@brief	エネミーマネージャー
//	@autor	Takuya Ochi
//	@date	2018/11/16
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "EnemyManager.h"
#include "EnemyBase.h"
#include "BossManager.h"
#include "CurveEnemy.h"
#include "ReturnEnemy.h"
#include "FloatingEnemy.h"
#include "LineEnemy.h"
#include "TurningEnemy.h"
#include "UpDownEnemy.h"
#include "PlayerManager.h"
#include "ShotManager.h"
#include "ItemManager.h"
#include "Key.h"
#include "HitChecker.h"
#include "SoundEffect.h"
#include "Sound.h"
#include "Warning.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const float EnemyManager::MAX_EMERGNCE_INTERVAL_TIME = 100.0f;	//	出撃タイマーの最大間隔
const float	EnemyManager::MAX_WAIT_TIME = 20.0f;				//	待機時間の最大
const float	EnemyManager::PART_INTERAL = 250.0f;				//	パートの間隔
const float	EnemyManager::MIN_ALIVE_TIME = 80.0f;				//	生存時間の最小
const float	EnemyManager::MEDIUM_BOSS_EMERGE_TIME = 1700.0f;	//	中ボスの出現時の時間
const float	EnemyManager::LAST_BOSS_EMERGE_TIME = 10500.0f;		//	最終ボスの出現時の時間
const float	EnemyManager::WARNING_SE_TIME = 10430.0f;			//	警告音の時間
const float	EnemyManager::PRODUCTION_TIME = 5100.0f;			//	演出の時間
const float	EnemyManager::MAX_FLAME = 14625.0f;					//	フレームの最大
const float	EnemyManager::RESET_FLAME = 10825.0f;				//	フレームのリセット値
const float EnemyManager::MAX_X_POS = 130.0f;					//	X座標の最大
const float EnemyManager::MIN_X_POS = -130.0f;					//	X座標の最小
const float EnemyManager::MAX_Y_POS = 120.0f;					//	Y座標の最大
const float EnemyManager::MIN_Y_POS = 0.0f;						//	Y座標の最小
const float	EnemyManager::SKIP = 1.0f;						//	スキップ

//#define TEST_ENEMY_ENTRY 0

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
EnemyManager::EnemyManager()
{
	m_entryTime = 0.0f;

	//	メモリの確保をすると、使用フラグの初期化
	for (int i = 0; i < ENEMY_KIND::ENEMY_NUM; i++)
	{
		for (int j = 0; j < CommonConstant::MAX_ENEMY_ENTRY_NUM; j++)
		{
			m_entryBuffer[i].m_pBuf[j] = NULL;
			m_entryBuffer[i].m_isInUse[j] = false;
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
EnemyManager::~EnemyManager()
{
	//	最終的な解放処理
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成処理
//-----------------------------------------------------------------------------
void EnemyManager::Create()
{
	//	大本のモデルハンドルの読み込み
	m_sourceModelHandle[ENEMY_KIND::ENEMY_LINE] = MV1LoadModel("Data/Model/Enemy/LineEnemy.mqo");
	m_sourceModelHandle[ENEMY_KIND::ENEMY_FLOATING] = MV1LoadModel("Data/Model/Enemy/FloatingEnemy.mqo");
	m_sourceModelHandle[ENEMY_KIND::ENEMY_CURVE] = MV1LoadModel("Data/Model/Enemy/CurveEnemy.mqo");
	m_sourceModelHandle[ENEMY_KIND::ENEMY_RETURN] = MV1LoadModel("Data/Model/Enemy/ReturnEnemy.mqo");
	m_sourceModelHandle[ENEMY_KIND::ENEMY_TURNING] = MV1LoadModel("Data/Model/Enemy/TurningEnemy.mqo");
	m_sourceModelHandle[ENEMY_KIND::ENEMY_UP_DOWN] = MV1LoadModel("Data/Model/Enemy/UpDownEnemy.mqo");

	CommonDebug::Assert((m_sourceModelHandle[ENEMY_KIND::ENEMY_LINE] <= -1), " [ EnemyManager.cpp ] : error : model loading failed.");
	CommonDebug::Assert((m_sourceModelHandle[ENEMY_KIND::ENEMY_FLOATING] <= -1), " [ EnemyManager.cpp ] : error : model loading failed.");
	CommonDebug::Assert((m_sourceModelHandle[ENEMY_KIND::ENEMY_CURVE] <= -1), " [ EnemyManager.cpp ] : error : model loading failed.");
	CommonDebug::Assert((m_sourceModelHandle[ENEMY_KIND::ENEMY_RETURN] <= -1), " [ EnemyManager.cpp ] : error : model loading failed.");
	CommonDebug::Assert((m_sourceModelHandle[ENEMY_KIND::ENEMY_TURNING] <= -1), " [ EnemyManager.cpp ] : error : model loading failed.");
	CommonDebug::Assert((m_sourceModelHandle[ENEMY_KIND::ENEMY_UP_DOWN] <= -1), " [ EnemyManager.cpp ] : error : model loading failed.");

	//	メモリの確保をする
	for (int i = 0; i < CommonConstant::MAX_ENEMY_ENTRY_NUM; i++)
	{
		m_entryBuffer[ENEMY_KIND::ENEMY_LINE].m_pBuf[i] = new LineEnemy(m_sourceModelHandle[ENEMY_KIND::ENEMY_LINE]);
		m_entryBuffer[ENEMY_KIND::ENEMY_FLOATING].m_pBuf[i] = new FloatingEnemy(m_sourceModelHandle[ENEMY_KIND::ENEMY_FLOATING]);
		m_entryBuffer[ENEMY_KIND::ENEMY_CURVE].m_pBuf[i] = new CurveEnemy(m_sourceModelHandle[ENEMY_KIND::ENEMY_CURVE]);
		m_entryBuffer[ENEMY_KIND::ENEMY_RETURN].m_pBuf[i] = new ReturnEnemy(m_sourceModelHandle[ENEMY_KIND::ENEMY_RETURN]);
		m_entryBuffer[ENEMY_KIND::ENEMY_TURNING].m_pBuf[i] = new TurningEnemy(m_sourceModelHandle[ENEMY_KIND::ENEMY_TURNING]);
		m_entryBuffer[ENEMY_KIND::ENEMY_UP_DOWN].m_pBuf[i] = new UpDownEnemy(m_sourceModelHandle[ENEMY_KIND::ENEMY_UP_DOWN]);
	}

	//	初期化し、エネミー個々の能力の作成
	for (int i = 0; i < ENEMY_KIND::ENEMY_NUM; i++)
	{
		for (int j = 0; j < CommonConstant::MAX_ENEMY_ENTRY_NUM; j++)
		{
			m_entryBuffer[i].m_pBuf[j]->Initialize();
			m_entryBuffer[i].m_pBuf[j]->Create();
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	解放処理
//-----------------------------------------------------------------------------
void EnemyManager::Release()
{
	//	リスト内に登録されているとき、クリアする
	if (!m_useList.empty()) { m_useList.clear(); }

	for (int i = 0; i < ENEMY_KIND::ENEMY_NUM; i++)
	{
		//	モデルのアンロード
		MV1DeleteModel(m_sourceModelHandle[i]);

		for (int j = 0; j < CommonConstant::MAX_ENEMY_ENTRY_NUM; j++)
		{
			//	メモリの解放
			CommonSafe::Delete(m_entryBuffer[i].m_pBuf[j]);
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	初期処理
//-----------------------------------------------------------------------------
void EnemyManager::Initialize()
{
#ifdef TEST_ENEMY_ENTRY
	//	エネミー出現タイムの初期化
	m_entryTime = 0.0f;
#else
	//	エネミー出現タイムの初期化
	m_entryTime = 0.0f;
#endif

	//	エネミーの計測カウントの初期化
	for (int i = 0; i < ENEMY_FORMATION_NUM::FORMATION_NUM; i++)
	{
		m_enemyCounter[i] = 0;
	}
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void EnemyManager::Update(PlayerManager& _playerManager, ShotManager& _shotManager, BossManager& _bossManager, ItemManager& _item, Sound& _sound, SoundEffect& _soundEffect, Warning& _warning)
{
	//	中ボスの登録
	const bool isEntryMediumBoss = m_entryTime == MEDIUM_BOSS_EMERGE_TIME;
	if (isEntryMediumBoss) { _bossManager.SetIsMediumBossEmerge(true); }

	//	SE
	const bool isWarningSE = m_entryTime == WARNING_SE_TIME;
	if (isWarningSE) { _soundEffect.OnPlaySound(_soundEffect.ONE_SE_KIND::ONE_SE_WARNING); _sound.SetIsChageSound(true); _warning.SetIsWarningStart(true); }

	//	最終ボスの登録
	const bool isEntryLastBoss = m_entryTime == LAST_BOSS_EMERGE_TIME;
	if (isEntryLastBoss) { _bossManager.SetIsLastBossEmerge(true); }

	///		ゲーム時間調整のため、削除
	////	演出開始
	//const bool isProduction = m_entryTime == PRODUCTION_TIME;
	//if (isProduction) { PRODUCTION->SetIsSceneProduction(true); }

	//	必殺技以外のときと
	//	クリアしていないときのみ
	//	更新する
	const bool isNotClearProduction = !PRODUCTION->GetIsClearProduction();
	const bool isNotSpecialProduction = !PRODUCTION->GetIsSpecialProduction();
	const bool isNotClear = !_playerManager.GetIsClear();
	const bool isActive = isNotSpecialProduction && isNotClearProduction && isNotClear;
	if (isActive)
	{
		//	リストに登録
		_EnemyEntry();
	}

	//	リストの更新
	_ListUpdate(_playerManager, _shotManager, _soundEffect);

	//	リストから削除
	_RemoveFromList(_item);
}

//-----------------------------------------------------------------------------
//	@brief	描画処理
//-----------------------------------------------------------------------------
void EnemyManager::Draw()
{
	//	使用リストに登録されているとき
	const bool isAlreadyEntry = !m_useList.empty();
	if (isAlreadyEntry)
	{
		//	リストの回数分回す
		std::list<EnemyBase*>::iterator ite;
		for (ite = m_useList.begin(); ite != m_useList.end(); ite++)
		{
			//	描画
			(*ite)->Draw();
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	リストから削除
//-----------------------------------------------------------------------------
void EnemyManager::RemoveFromList(const EnemyBase* _enemyPtr, ItemManager& _item)
{
	//	使用リストに登録されているとき
	const bool isAlreadyEntry = !m_useList.empty();
	if (isAlreadyEntry)
	{
		//	リストの回数分回す
		std::list<EnemyBase*>::iterator ite;
		for (ite = m_useList.begin(); ite != m_useList.end(); ite++)
		{
			//	_enemyPtr と一致した、
			//	エネミーをリストから削除する
			const bool isSame = _enemyPtr == (*ite);
			if (isSame)
			{
				//	エネミーの死亡時にアイテムを持っているなら、
				//	アイテムリストに追加する
				const bool isHavingItems = (*ite)->GetIsHavingItems();
				if (isHavingItems) { _item.RegisterOnList((*ite)->GetPos()); }

				//	エネミーを使用可能にする
				int enemyID = (*ite)->GetEntryID();
				int enemyKind = (*ite)->GetEntryKind();
				m_entryBuffer[enemyKind].m_isInUse[enemyID] = false;

				//	メモリからは消さないので
				//	初期化をする
				(*ite)->Initialize();

				//	使用リストから削除
				m_useList.remove(*ite);

				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	エネミーへのポインタの getter
//-----------------------------------------------------------------------------
EnemyBase* EnemyManager::GetEnemyPtr(const int _num)
{
	int counter = 0;

	//	使用リストに登録されているとき
	const bool isAlreadyEntry = !m_useList.empty();
	if (isAlreadyEntry)
	{
		//	リストの回数分回す
		std::list<EnemyBase*>::iterator ite;
		for (ite = m_useList.begin(); ite != m_useList.end(); ite++)
		{
			//	_num と一致したポインタを返す
			const bool isSame = counter == _num;
			if (isSame) { return (*ite); }
			counter++;
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------
//	@brief	エネミーの登録
//-----------------------------------------------------------------------------
void EnemyManager::_EnemyEntry()
{
#ifdef TEST_ENEMY_ENTRY

#else

	//	エネミーパート１
	_EnemyParts1();

	//	エネミーパート２
	_EnemyParts2();

	//	エネミーパート３
	_EnemyParts3();

	//	エネミーパート４
	_EnemyParts4();

	//	エネミーパート５
	_EnemyParts5();

#endif // TEST_ENEMY_ENTRY

	m_entryTime++;

	//	エネミー出現時間の最大なので、
	//	ボス中のザコパートの繰り返し
	const bool isMaxTime = m_entryTime >= MAX_FLAME;
	if (isMaxTime) { m_entryTime = RESET_FLAME; }
}

//-----------------------------------------------------------------------------
//	@brief	リストの更新
//-----------------------------------------------------------------------------
void EnemyManager::_ListUpdate(PlayerManager& _playerManager, ShotManager& _shot, SoundEffect& _soundEffect)
{
	//	使用リストに登録されているとき
	const bool isAlreadyEntry = !m_useList.empty();
	if (isAlreadyEntry)
	{
		//	リストの回数分回す
		std::list<EnemyBase*>::iterator ite;
		for (ite = m_useList.begin(); ite != m_useList.end(); ite++)
		{
			//	生存時間の加算
			(*ite)->AddAliveTime();

			//	更新
			(*ite)->Update(_playerManager, _shot, _soundEffect);
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	リストから削除
//-----------------------------------------------------------------------------
void EnemyManager::_RemoveFromList(ItemManager& _item)
{
	//	使用リストに登録されているとき
	const bool isAlreadyEntry = !m_useList.empty();
	if (isAlreadyEntry)
	{
		//	リストの回数分回す
		std::list<EnemyBase*>::iterator ite;
		for (ite = m_useList.begin(); ite != m_useList.end(); ite++)
		{
			//	生成してすぐに消滅しないように、
			//	最小の生存時間を超えたら、削除する
			const bool isActiveMinAlive = (*ite)->GetAliveTime() >= MIN_ALIVE_TIME;
			if (isActiveMinAlive)
			{
				//	画面外のとき、削除
				const bool isOverTop = (*ite)->GetPos().y >= MAX_Y_POS;
				const bool isOverButtom = (*ite)->GetPos().y <= MIN_Y_POS;
				const bool isOverRight = (*ite)->GetPos().x >= MAX_X_POS;
				const bool isOverLeft = (*ite)->GetPos().x <= MIN_X_POS;
				const bool isOverPos = isOverTop || isOverButtom || isOverRight || isOverLeft;
				if (isOverPos)
				{
					//	エネミーを使用可能にする
					const int enemyID = (*ite)->GetEntryID();
					const int enemyKind = (*ite)->GetEntryKind();
					m_entryBuffer[enemyKind].m_isInUse[enemyID] = false;

					//	隊列から削除
					const int formationID = (*ite)->GetFormationID();
					SubEnemyCount(formationID);

					//	メモリからは消さないので
					//	初期化をする
					(*ite)->Initialize();

					//	使用リストから削除
					m_useList.remove(*ite);

					break;
				}

				//	何らかにヒットしたので、削除
				const bool isDeleateActive = (*ite)->GetIsDeleate();
				if (isDeleateActive)
				{
					//	エネミーを使用可能にする
					int enemyID = (*ite)->GetEntryID();
					int enemyKind = (*ite)->GetEntryKind();
					m_entryBuffer[enemyKind].m_isInUse[enemyID] = false;

					//	メモリからは消さないので
					//	初期化をする
					(*ite)->Initialize();

					//	使用リストから削除
					m_useList.remove(*ite);

					break;
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	エネミーパート１
//-----------------------------------------------------------------------------
void EnemyManager::_EnemyParts1()
{
	//	前半ザコパート１
	_FormationEntry(ENEMY_KIND::ENEMY_CURVE, 50.0f, 10.0f, VGet(120.0f, 90.0f, 0.0f), 10, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_1);
	_FormationEntry(ENEMY_KIND::ENEMY_CURVE, 200.0f, 10.0f, VGet(120.0f, 20.0f, 0.0f), 10, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_2);
	_FormationEntry(ENEMY_KIND::ENEMY_CURVE, 350.0f, 10.0f, VGet(120.0f, 90.0f, 0.0f), 10, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_3);
	_FormationEntry(ENEMY_KIND::ENEMY_CURVE, 500.0f, 10.0f, VGet(120.0f, 20.0f, 0.0f), 10, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_4);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_RETURN, 700.0f, 20.0f, 25.0f, VGet(-150.0f, 85.0f, 50.0f), 4, false, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	_FormationEntry(ENEMY_KIND::ENEMY_CURVE, 850.0f, 10.0f, VGet(120.0f, 90.0f, 0.0f), 10, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_5);
	_FormationEntry(ENEMY_KIND::ENEMY_CURVE, 850.0f, 10.0f, VGet(120.0f, 20.0f, 0.0f), 10, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_1);
	_FormationEntry(ENEMY_KIND::ENEMY_CURVE, 1050.0f, 10.0f, VGet(120.0f, 60.0f, 0.0f), 10, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_2);
	_FormationEntry(ENEMY_KIND::ENEMY_CURVE, 1050.0f, 10.0f, VGet(120.0f, 40.0f, 0.0f), 10, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_3);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_RETURN, 1300.0f, 0.0f, 15.0f, VGet(-150.0f, 85.0f, 50.0f), 3, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_RETURN, 1300.0f, 0.0f, 15.0f, VGet(-150.0f, 35.0f, 50.0f), 2, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_RETURN, 1400.0f, 0.0f, 15.0f, VGet(-150.0f, 85.0f, 50.0f), 2, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_RETURN, 1400.0f, 0.0f, 15.0f, VGet(-150.0f, 45.0f, 50.0f), 3, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_RETURN, 1450.0f, 0.0f, 15.0f, VGet(120.0f, 85.0f, 50.0f), 2, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_RETURN, 1450.0f, 0.0f, 15.0f, VGet(120.0f, 35.0f, 50.0f), 2, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_RETURN, 1500.0f, 0.0f, 15.0f, VGet(120.0f, 70.0f, 50.0f), 3, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_RETURN, 1600.0f, 0.0f, 15.0f, VGet(-150.0f, 85.0f, 50.0f), 5, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);

}

//-----------------------------------------------------------------------------
//	@brief	エネミーパート２
//-----------------------------------------------------------------------------
void EnemyManager::_EnemyParts2()
{
	//	中ボスパート
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_FLOATING, 1850.0f, 0.0f, 10.0f, VGet(120.0f, 85.0f, 0.0f), 3, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_FLOATING, 1850.0f, 0.0f, 10.0f, VGet(120.0f, 30.0f, 0.0f), 3, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_FLOATING, 1950.0f, 0.0f, 10.0f, VGet(120.0f, 85.0f, 0.0f), 5, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_FLOATING, 2050.0f, 0.0f, 10.0f, VGet(120.0f, 85.0f, 0.0f), 3, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_FLOATING, 2050.0f, 0.0f, 10.0f, VGet(120.0f, 30.0f, 0.0f), 3, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_FLOATING, 2150.0f, 0.0f, 10.0f, VGet(120.0f, 85.0f, 0.0f), 3, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_FLOATING, 2150.0f, 0.0f, 10.0f, VGet(120.0f, 30.0f, 0.0f), 3, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_FLOATING, 2250.0f, 0.0f, 10.0f, VGet(120.0f, 85.0f, 0.0f), 3, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_FLOATING, 2250.0f, 0.0f, 10.0f, VGet(120.0f, 30.0f, 0.0f), 3, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_FLOATING, 2350.0f, 0.0f, 10.0f, VGet(120.0f, 85.0f, 0.0f), 5, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_FLOATING, 2450.0f, 0.0f, 10.0f, VGet(120.0f, 85.0f, 0.0f), 3, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_FLOATING, 2450.0f, 0.0f, 10.0f, VGet(120.0f, 30.0f, 0.0f), 3, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_FLOATING, 2550.0f, 0.0f, 10.0f, VGet(120.0f, 85.0f, 0.0f), 5, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_FLOATING, 2650.0f, 0.0f, 10.0f, VGet(120.0f, 85.0f, 0.0f), 5, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_FLOATING, 2750.0f, 0.0f, 10.0f, VGet(120.0f, 85.0f, 0.0f), 5, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_FLOATING, 2850.0f, 0.0f, 10.0f, VGet(120.0f, 85.0f, 0.0f), 3, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_FLOATING, 2850.0f, 0.0f, 10.0f, VGet(120.0f, 30.0f, 0.0f), 3, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
}

//-----------------------------------------------------------------------------
//	@brief	エネミーパート３
//-----------------------------------------------------------------------------
void EnemyManager::_EnemyParts3()
{
	///		ゲーム時間調整のため、削除した部分をスキップ
	const bool isSkip = m_entryTime == 3000.0f;
	if (isSkip) { m_entryTime = 5500.0f - SKIP; }

	///		ゲーム時間調整のため、削除
	////	前半ザコパート２
	//_FormationEntry(ENEMY_KIND::ENEMY_LINE, 3000.0f, 10.0f, VGet(120.0f, 90.0f, 0.0f), 15, false, false, (int)ENEMY_FORMATION_NUM::FORMATION_1);
	//_FormationEntry(ENEMY_KIND::ENEMY_LINE, 3000.0f, 10.0f, VGet(120.0f, 30.0f, 0.0f), 15, false, false, (int)ENEMY_FORMATION_NUM::FORMATION_2);
	//_FormationEntry(ENEMY_KIND::ENEMY_LINE, 3100.0f, 10.0f, VGet(120.0f, 60.0f, 0.0f), 15, false, false, (int)ENEMY_FORMATION_NUM::FORMATION_3);
	//_VerticalFormationEntry(ENEMY_KIND::ENEMY_RETURN, 3200.0f, 0.0f, 10.0f, VGet(-150.0f, 85.0f, 50.0f), 3, false, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	//_VerticalFormationEntry(ENEMY_KIND::ENEMY_RETURN, 3200.0f, 0.0f, 10.0f, VGet(-150.0f, 35.0f, 50.0f), 3, false, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	//_VerticalFormationEntry(ENEMY_KIND::ENEMY_RETURN, 3300.0f, 0.0f, 10.0f, VGet(-150.0f, 60.0f, 50.0f), 3, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	//_VerticalFormationEntry(ENEMY_KIND::ENEMY_RETURN, 3400.0f, 0.0f, 15.0f, VGet(-150.0f, 85.0f, 50.0f), 5, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	//_VerticalFormationEntry(ENEMY_KIND::ENEMY_RETURN, 3450.0f, 0.0f, 15.0f, VGet(-150.0f, 85.0f, 50.0f), 5, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	//_VerticalFormationEntry(ENEMY_KIND::ENEMY_RETURN, 3500.0f, 0.0f, 15.0f, VGet(-150.0f, 85.0f, 50.0f), 5, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_NONE);
	//_FormationEntry(ENEMY_KIND::ENEMY_CURVE, 3600.0f, 10.0f, VGet(120.0f, 90.0f, 0.0f), 10, false, false, (int)ENEMY_FORMATION_NUM::FORMATION_1);
	//_FormationEntry(ENEMY_KIND::ENEMY_CURVE, 3650.0f, 10.0f, VGet(160.0f, 70.0f, 0.0f), 10, false, false, (int)ENEMY_FORMATION_NUM::FORMATION_2);
	//_FormationEntry(ENEMY_KIND::ENEMY_CURVE, 3700.0f, 10.0f, VGet(200.0f, 51.0f, 0.0f), 10, false, false, (int)ENEMY_FORMATION_NUM::FORMATION_3);
	//_FormationEntry(ENEMY_KIND::ENEMY_CURVE, 4000.0f, 10.0f, VGet(120.0f, 20.0f, 0.0f), 10, false, false, (int)ENEMY_FORMATION_NUM::FORMATION_4);
	//_FormationEntry(ENEMY_KIND::ENEMY_CURVE, 4050.0f, 10.0f, VGet(160.0f, 40.0f, 0.0f), 10, false, false, (int)ENEMY_FORMATION_NUM::FORMATION_5);
	//_FormationEntry(ENEMY_KIND::ENEMY_RETURN, 4300.0f, 10.0f, VGet(-150.0f, 20.0f, 50.0f), 10, false, false, (int)ENEMY_FORMATION_NUM::FORMATION_1);
	//_FormationEntry(ENEMY_KIND::ENEMY_RETURN, 4450.0f, 10.0f, VGet(-150.0f, 40.0f, 50.0f), 10, false, false, (int)ENEMY_FORMATION_NUM::FORMATION_2);
	//_FormationEntry(ENEMY_KIND::ENEMY_RETURN, 4600.0f, 10.0f, VGet(-150.0f, 60.0f, 50.0f), 10, false, false, (int)ENEMY_FORMATION_NUM::FORMATION_3);
	//_FormationEntry(ENEMY_KIND::ENEMY_RETURN, 4750.0f, 10.0f, VGet(-150.0f, 80.0f, 50.0f), 10, false, false, (int)ENEMY_FORMATION_NUM::FORMATION_4);
}

//-----------------------------------------------------------------------------
//	@brief	エネミーパート４
//-----------------------------------------------------------------------------
void EnemyManager::_EnemyParts4()
{
	//	後半ザコパート
	_FormationEntry(ENEMY_KIND::ENEMY_TURNING, 5500.0f, 20.0f, VGet(120.0f, 90.0f, 0.0f), 5, true, false, (int)ENEMY_FORMATION_NUM::FORMATION_1);
	_FormationEntry(ENEMY_KIND::ENEMY_TURNING, 5500.0f, 20.0f, VGet(120.0f, 30.0f, 0.0f), 5, true, false, (int)ENEMY_FORMATION_NUM::FORMATION_2);
	_FormationEntry(ENEMY_KIND::ENEMY_TURNING, 5900.0f, 20.0f, VGet(200.0f, 70.0f, 0.0f), 5, true, false, (int)ENEMY_FORMATION_NUM::FORMATION_3);
	_FormationEntry(ENEMY_KIND::ENEMY_TURNING, 5900.0f, 20.0f, VGet(200.0f, 50.0f, 0.0f), 5, true, false, (int)ENEMY_FORMATION_NUM::FORMATION_4);
	_FormationEntry(ENEMY_KIND::ENEMY_TURNING, 6200.0f, 20.0f, VGet(150.0f, 70.0f, 0.0f), 5, true, false, (int)ENEMY_FORMATION_NUM::FORMATION_5);
	_FormationEntry(ENEMY_KIND::ENEMY_TURNING, 6200.0f, 20.0f, VGet(150.0f, 50.0f, 0.0f), 5, true, false, (int)ENEMY_FORMATION_NUM::FORMATION_1);
	_FormationEntry(ENEMY_KIND::ENEMY_TURNING, 6600.0f, 20.0f, VGet(120.0f, 90.0f, 0.0f), 5, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_2);
	_FormationEntry(ENEMY_KIND::ENEMY_TURNING, 6600.0f, 20.0f, VGet(120.0f, 30.0f, 0.0f), 5, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_3);
	_FormationEntry(ENEMY_KIND::ENEMY_TURNING, 6600.0f, 20.0f, VGet(200.0f, 70.0f, 0.0f), 5, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_4);
	_FormationEntry(ENEMY_KIND::ENEMY_TURNING, 6600.0f, 20.0f, VGet(200.0f, 50.0f, 0.0f), 5, true, true, (int)ENEMY_FORMATION_NUM::FORMATION_5);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_UP_DOWN, 7000.0f, 10.0f, 40.0f, VGet(70.0f, -50.0f, 50.0f), 4, false, false, (int)ENEMY_FORMATION_NUM::FORMATION_1);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_UP_DOWN, 7000.0f, 10.0f, 40.0f, VGet(90.0f, 170.0f, 50.0f), 5, false, false, (int)ENEMY_FORMATION_NUM::FORMATION_2);
	_FormationEntry(ENEMY_KIND::ENEMY_FLOATING, 7300.0f, 10.0f, VGet(120.0f, 85.0f, 0.0f), 10, false, false, (int)ENEMY_FORMATION_NUM::FORMATION_3);
	_FormationEntry(ENEMY_KIND::ENEMY_FLOATING, 7300.0f, 10.0f, VGet(120.0f, 20.0f, 0.0f), 10, false, false, (int)ENEMY_FORMATION_NUM::FORMATION_4);
	_FormationEntry(ENEMY_KIND::ENEMY_LINE, 7400.0f, 10.0f, VGet(120.0f, 60.0f, 0.0f), 10, false, false, (int)ENEMY_FORMATION_NUM::FORMATION_5);

	///		ゲーム時間調整のため、削除した部分をスキップ
	const bool isSkip1 = m_entryTime == 7600.0f;
	if (isSkip1) { m_entryTime = 8800.0f - SKIP; }

	///		ゲーム時間調整のため、削除
	//_FormationEntry(ENEMY_KIND::ENEMY_TURNING, 7700.0f, 20.0f, VGet(120.0f, 90.0f, 0.0f), 5, true, false, (int)ENEMY_FORMATION_NUM::FORMATION_5);
	//_FormationEntry(ENEMY_KIND::ENEMY_TURNING, 7900.0f, 20.0f, VGet(140.0f, 70.0f, 0.0f), 5, true, false, (int)ENEMY_FORMATION_NUM::FORMATION_1);
	//_FormationEntry(ENEMY_KIND::ENEMY_TURNING, 8100.0f, 20.0f, VGet(160.0f, 50.1f, 0.0f), 5, true, false, (int)ENEMY_FORMATION_NUM::FORMATION_2);
	//_FormationEntry(ENEMY_KIND::ENEMY_TURNING, 8300.0f, 20.0f, VGet(120.0f, 20.0f, 0.0f), 5, true, false, (int)ENEMY_FORMATION_NUM::FORMATION_3);
	//_FormationEntry(ENEMY_KIND::ENEMY_TURNING, 8500.0f, 20.0f, VGet(150.0f, 50.0f, 0.0f), 5, true, false, (int)ENEMY_FORMATION_NUM::FORMATION_4);

	_VerticalFormationEntry(ENEMY_KIND::ENEMY_UP_DOWN, 8800.0f, 10.0f, 40.0f, VGet(70.0f, 1750.0f, 50.0f), 4, false, false, (int)ENEMY_FORMATION_NUM::FORMATION_5);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_UP_DOWN, 9000.0f, 10.0f, 40.0f, VGet(90.0f, -50.0f, 50.0f), 5, false, false, (int)ENEMY_FORMATION_NUM::FORMATION_1);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_UP_DOWN, 9200.0f, 10.0f, 40.0f, VGet(70.0f, 170.0f, 50.0f), 4, false, false, (int)ENEMY_FORMATION_NUM::FORMATION_2);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_UP_DOWN, 9400.0f, 10.0f, 40.0f, VGet(90.0f, -50.0f, 50.0f), 5, false, false, (int)ENEMY_FORMATION_NUM::FORMATION_3);
	_VerticalFormationEntry(ENEMY_KIND::ENEMY_UP_DOWN, 9600.0f, 10.0f, 40.0f, VGet(70.0f, 170.0f, 50.0f), 4, false, false, (int)ENEMY_FORMATION_NUM::FORMATION_4);
	
	///		ゲーム時間調整のため、削除した部分をスキップ
	const bool isSkip2 = m_entryTime == 9800.0f;
	if (isSkip2) { m_entryTime = 10300.0f; }

	///		ゲーム時間調整のため、削除
	//_VerticalFormationEntry(ENEMY_KIND::ENEMY_UP_DOWN, 9800.0f, 10.0f, 40.0f, VGet(70.0f, -50.0f, 50.0f), 4, false, false, (int)ENEMY_FORMATION_NUM::FORMATION_5);
	//_VerticalFormationEntry(ENEMY_KIND::ENEMY_UP_DOWN, 9800.0f, 10.0f, 40.0f, VGet(90.0f, 170.0f, 50.0f), 5, false, false, (int)ENEMY_FORMATION_NUM::FORMATION_1);
	//_VerticalFormationEntry(ENEMY_KIND::ENEMY_UP_DOWN, 10000.0f, 10.0f, 40.0f, VGet(70.0f, -50.0f, 50.0f), 4, false, false, (int)ENEMY_FORMATION_NUM::FORMATION_2);
	//_VerticalFormationEntry(ENEMY_KIND::ENEMY_UP_DOWN, 10000.0f, 10.0f, 40.0f, VGet(90.0f, 170.0f, 50.0f), 5, false, false, (int)ENEMY_FORMATION_NUM::FORMATION_3);
}

//-----------------------------------------------------------------------------
//	@brief	エネミーパート５
//-----------------------------------------------------------------------------
void EnemyManager::_EnemyParts5()
{
	//	ボス中のザコパート
	_FormationEntry(ENEMY_KIND::ENEMY_LINE, 10825.0f, 10.0f, VGet(120.0f, 80.0f, 0.0f), 8, true, false, (int)ENEMY_FORMATION_NUM::FORMATION_4);
	_FormationEntry(ENEMY_KIND::ENEMY_LINE, 11225.0f, 10.0f, VGet(120.0f, 30.0f, 0.0f), 8, true, false, (int)ENEMY_FORMATION_NUM::FORMATION_5);
	_FormationEntry(ENEMY_KIND::ENEMY_LINE, 11625.0f, 10.0f, VGet(120.0f, 30.0f, 0.0f), 8, true, false, (int)ENEMY_FORMATION_NUM::FORMATION_1);
	_FormationEntry(ENEMY_KIND::ENEMY_LINE, 12225.0f, 10.0f, VGet(120.0f, 80.0f, 0.0f), 8, true, false, (int)ENEMY_FORMATION_NUM::FORMATION_2);
	_FormationEntry(ENEMY_KIND::ENEMY_LINE, 12870.0f, 10.0f, VGet(120.0f, 80.0f, 0.0f), 8, true, false, (int)ENEMY_FORMATION_NUM::FORMATION_3);
	_FormationEntry(ENEMY_KIND::ENEMY_LINE, 13400.0f, 10.0f, VGet(120.0f, 30.0f, 0.0f), 8, true, false, (int)ENEMY_FORMATION_NUM::FORMATION_4);
	_FormationEntry(ENEMY_KIND::ENEMY_LINE, 14050.0f, 10.0f, VGet(120.0f, 30.0f, 0.0f), 8, true, false, (int)ENEMY_FORMATION_NUM::FORMATION_5);
}

//-----------------------------------------------------------------------------
//	@brief	隊列を組ませて登録
//-----------------------------------------------------------------------------
void EnemyManager::_FormationEntry(const ENEMY_KIND _enemyKind, const float _startTime, const float _intervalTime, const VECTOR _startPos, const int _formationNum, 
									const bool _isUseShot, const bool _isRestriction, const int _formation)
{
	//	種族範囲外判定
	const bool isKindSizeOver = _enemyKind < ENEMY_KIND::NONE || _enemyKind > ENEMY_KIND::ENEMY_NUM;
	CommonDebug::Assert(isKindSizeOver, " [ EnemyManager.cpp ] : error : enemy kind over.");
	if (isKindSizeOver) { return; }

	//	まだ開始時間でない時は無視する
	if (m_entryTime < _startTime) { return; }

	//	隊列内エネミー数の加算
	if (m_entryTime == _startTime) { m_enemyCounter[_formation] = _formationNum; }

	for (int num = 0; num < _formationNum; num++)
	{
		//	まだ開始時間でない時は無視する
		float entryTime = (_startTime + (_intervalTime * num));
		if (m_entryTime != entryTime) { continue; }

		//	登録処理
		_Registration(_enemyKind, _startPos, entryTime, _isUseShot, _isRestriction, _formation);
	}
}

//-----------------------------------------------------------------------------
//	@brief	登録処理
//-----------------------------------------------------------------------------
void EnemyManager::_Registration(const ENEMY_KIND _enemyKind, const VECTOR _startPos, const float _entryTime, const bool _isUseShot, const bool _isRestriction, const int _formation)
{
	//	使用リスト登録処理
	const bool isListEntry = m_entryTime == _entryTime;
	if (isListEntry)
	{
		VECTOR tmpPos = CommonConstant::ORIGIN;

		//	空いているバッファを探し、
		//	エネミーを追加
		switch (_enemyKind)
		{
			//	直線エネミーを追加
		case ENEMY_KIND::ENEMY_LINE:
			tmpPos = VGet(_startPos.x, _startPos.y, _startPos.z);
			if (_OneEnemyRegistrationToShotProbaility((int)ENEMY_KIND::ENEMY_LINE, tmpPos, _isUseShot, _isRestriction, _formation)) { return; }
			break;

			//	浮遊エネミーを追加
		case ENEMY_KIND::ENEMY_FLOATING:
			tmpPos = VGet(_startPos.x, _startPos.y, _startPos.z);
			if (_OneEnemyRegistrationToShotProbaility((int)ENEMY_KIND::ENEMY_FLOATING, tmpPos, _isUseShot, _isRestriction, _formation)) { return; }
			break;

			//	曲線エネミーを追加
		case ENEMY_KIND::ENEMY_CURVE:
			tmpPos = VGet(_startPos.x, _startPos.y, _startPos.z);
			if (_OneEnemyRegistrationToShotProbaility((int)ENEMY_KIND::ENEMY_CURVE, tmpPos, _isUseShot, _isRestriction, _formation)) { return; }
			break;

			//	帰還エネミーを追加
		case ENEMY_KIND::ENEMY_RETURN:

			//	帰還エネミーの X座標 はマイナスの方向からしか、
			//	来られないので、プラスが入ってたら、マイナスに変換する
			if (_startPos.x >= 0) { tmpPos = VGet(-_startPos.x, _startPos.y, _startPos.z); }
			else { tmpPos = VGet(_startPos.x, _startPos.y, _startPos.z); }
			if (_OneEnemyRegistrationToShotProbaility((int)ENEMY_KIND::ENEMY_RETURN, tmpPos, _isUseShot, _isRestriction, _formation)) { return; }
			break;

		case ENEMY_TURNING:
			tmpPos = VGet(_startPos.x, _startPos.y, _startPos.z);
			if (_OneEnemyRegistrationToShotProbaility((int)ENEMY_KIND::ENEMY_TURNING, tmpPos, _isUseShot, _isRestriction, _formation)) { return; }
			break;

		case ENEMY_UP_DOWN:
			tmpPos = VGet(_startPos.x, _startPos.y, _startPos.z);
			if (_OneEnemyRegistrationToShotProbaility((int)ENEMY_KIND::ENEMY_TURNING, tmpPos, _isUseShot, _isRestriction, _formation)) { return; }
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	隊列を組ませて帰還エネミーの登録
//-----------------------------------------------------------------------------
void EnemyManager::_VerticalFormationEntry(const ENEMY_KIND _enemyKind, const float _startTime, const float _intervalTime, const float _shiftInterval, const VECTOR _startPos, 
										   const int _formationNum, const bool _isUseShot, const bool _isHavingItem, const int _formation)
{
	//	まだ開始時間でない時は無視する
	if (m_entryTime < _startTime) { return; }

	if(m_entryTime == _startTime) { m_enemyCounter[_formation] = _formationNum; }
	
	for (int num = 0; num < _formationNum; num++)
	{
		//	まだ開始時間でない時は無視する
		float entryTime = (_startTime + (_intervalTime * num));
		if (m_entryTime != entryTime) { continue; }

		//	エネミーを追加
		_VerticalRegistration(_enemyKind, _startPos, entryTime, _shiftInterval, num, _isUseShot, _isHavingItem, _formation);
	}
}

//-----------------------------------------------------------------------------
//	@brief	帰還エネミーの登録
//-----------------------------------------------------------------------------
void EnemyManager::_VerticalRegistration(const ENEMY_KIND _enemyKind, const VECTOR _startPos, const float _entryTime, const float _shiftInterval,
										 const int _num, const bool _isUseShot, const bool _isHavingItem, const int _formation)
{
	VECTOR tmpPos = CommonConstant::ORIGIN;

	//	使用リスト登録処理
	const bool isListEntry = m_entryTime == _entryTime;
	if (isListEntry)
	{
		//	エネミーを追加
		switch (_enemyKind)
		{
			//	浮遊エネミーを追加
		case ENEMY_KIND::ENEMY_FLOATING:
			//	エネミー一体分の登録
			tmpPos = VGet(_startPos.x, _startPos.y - (_shiftInterval * _num), _startPos.z);
			if (_OneEnemyRegistration((int)ENEMY_KIND::ENEMY_FLOATING, tmpPos, _isUseShot, _isHavingItem, _formation)) { return; }
			break;

			//	帰還エネミーを追加
		case ENEMY_KIND::ENEMY_RETURN:

			//	帰還エネミーの X座標 はマイナスの方向からしか、
			//	来られないので、プラスが入ってたら、マイナスに変換する
			if (_startPos.x >= 0) { tmpPos = VGet(-_startPos.x, _startPos.y - (_shiftInterval * _num), _startPos.z); }
			else { tmpPos = VGet(_startPos.x, _startPos.y - (_shiftInterval * _num), _startPos.z); }
			if (_OneEnemyRegistration((int)ENEMY_KIND::ENEMY_RETURN, tmpPos, _isUseShot, _isHavingItem, _formation)) { return; }
			break;

		case ENEMY_UP_DOWN:
			tmpPos = VGet(_startPos.x - (_shiftInterval * _num), _startPos.y, _startPos.z);
			if (_OneEnemyRegistration((int)ENEMY_KIND::ENEMY_UP_DOWN, tmpPos, _isUseShot, _isHavingItem, _formation)) { return; }
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	一体分のエネミーの登録
//-----------------------------------------------------------------------------
bool EnemyManager::_OneEnemyRegistration(const int _enemyKind, const VECTOR _startPos, const bool _isUseShot, const bool _isHavingItem, const int _formation)
{
	for (int i = 0; i < CommonConstant::MAX_ENEMY_ENTRY_NUM; i++)
	{
		//	使用していないエネミーを探す
		if (!m_entryBuffer[_enemyKind].m_isInUse[i])
		{
			//	初期状態の設定
			m_entryBuffer[_enemyKind].m_pBuf[i]->SetPos(_startPos);
			m_entryBuffer[_enemyKind].m_pBuf[i]->SetEntryID(i);
			m_entryBuffer[_enemyKind].m_pBuf[i]->SetEntryKind(_enemyKind);
			m_entryBuffer[_enemyKind].m_pBuf[i]->SetFormationID((int)_formation);

			//	エネミーを使用状態にする
			m_entryBuffer[_enemyKind].m_isInUse[i] = true;

			//	ショットを使うかどうかと、
			//	アイテムを持っているか
			if (_isUseShot)
			{
				//	ショットを使う
				m_entryBuffer[_enemyKind].m_pBuf[i]->SetIsUseShot(true);
			}
			else
			{
				//	ショットを使わない
				m_entryBuffer[_enemyKind].m_pBuf[i]->SetIsUseShot(false);
			}

			if (_isHavingItem)
			{
				//	アイテムは5分の1で持つ
				bool isActive = CommonFunction::GetRand(0, 5) == 0;
				if (isActive) { m_entryBuffer[_enemyKind].m_pBuf[i]->SetIsHavingItems(true); }
				else { m_entryBuffer[_enemyKind].m_pBuf[i]->SetIsHavingItems(false); }
			}
			else
			{
				m_entryBuffer[_enemyKind].m_pBuf[i]->SetIsHavingItems(false);
			}

			//	使用リストに登録
			m_useList.push_back(m_entryBuffer[_enemyKind].m_pBuf[i]);
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
//	@brief	一体分の登録し、ショットは二分の一の確率で使用する
//-----------------------------------------------------------------------------
bool EnemyManager::_OneEnemyRegistrationToShotProbaility(const int _enemyKind, const VECTOR _startPos, const bool _isUseShot, const bool _isRestriction, const int _formation)
{
	for (int i = 0; i < CommonConstant::MAX_ENEMY_ENTRY_NUM; i++)
	{
		//	使用していないエネミーを探す
		if (!m_entryBuffer[_enemyKind].m_isInUse[i])
		{
			//	初期状態の設定
			m_entryBuffer[_enemyKind].m_pBuf[i]->SetPos(_startPos);
			m_entryBuffer[_enemyKind].m_pBuf[i]->SetEntryID(i);
			m_entryBuffer[_enemyKind].m_pBuf[i]->SetEntryKind(_enemyKind);
			m_entryBuffer[_enemyKind].m_pBuf[i]->SetFormationID((int)_formation);

			//	エネミーを使用状態にする
			m_entryBuffer[_enemyKind].m_isInUse[i] = true;

			//	ショットを使うか
			if (_isUseShot)
			{
				//	制限する
				if (_isRestriction)
				{
					//	添え字が偶数のときは
					//	ショットは使う
					bool isActive = i % 2 == 0;
					if (isActive) { m_entryBuffer[_enemyKind].m_pBuf[i]->SetIsUseShot(true); }
					else { m_entryBuffer[_enemyKind].m_pBuf[i]->SetIsUseShot(false); }
				}
				else
				{
					m_entryBuffer[_enemyKind].m_pBuf[i]->SetIsUseShot(true);
				}
			}
			else
			{
				//	ショットは使わない
				m_entryBuffer[_enemyKind].m_pBuf[i]->SetIsUseShot(false);
			}

			//	アイテムは持たない
			m_entryBuffer[_enemyKind].m_pBuf[i]->SetIsHavingItems(false);

			//	使用リストに登録
			m_useList.push_back(m_entryBuffer[_enemyKind].m_pBuf[i]);
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放処理
//-----------------------------------------------------------------------------
void EnemyManager::_FinalRelease()
{
	//	リスト内に登録されているとき、クリアする
	if (!m_useList.empty()) { m_useList.clear(); }

	for (int i = 0; i < ENEMY_KIND::ENEMY_NUM; i++)
	{
		//	モデルのアンロード
		MV1DeleteModel(m_sourceModelHandle[i]);

		for (int j = 0; j < CommonConstant::MAX_ENEMY_ENTRY_NUM; j++)
		{
			//	メモリの解放
			CommonSafe::Delete(m_entryBuffer[i].m_pBuf[j]);
		}
	}
}
