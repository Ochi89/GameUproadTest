
//=============================================================================
//	@file	EnemyManager.h
//	@brief	エネミーマネージャー
//	@autor	Takuya Ochi
//	@date	2018/11/16
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	プリプロセッサ
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "Common.h"
#include <list>

//-----------------------------------------------------------------------------
//	@brief	前方宣言
//-----------------------------------------------------------------------------
class EnemyBase;
class BossManager;
class PlayerManager;
class ShotManager;
class ItemManager;
class EnemyCounter;
class SoundEffect;
class Sound;
class Warning;

//-----------------------------------------------------------------------------
//	@brief	エネミー管理クラス
//-----------------------------------------------------------------------------
class EnemyManager final
{
public:

//==================== 列挙体 =====================//

	//	エネミーの種類
	enum ENEMY_KIND
	{
		NONE = -1,			//	なし
		ENEMY_LINE = 0,		//	直線
		ENEMY_FLOATING,		//	浮遊
		ENEMY_CURVE,		//	曲線
		ENEMY_RETURN,		//	帰還
		ENEMY_TURNING,		//	旋回
		ENEMY_UP_DOWN,		//	上下
		ENEMY_NUM			//	番兵
	};

	//	敵の隊列数
	enum ENEMY_FORMATION_NUM
	{
		FORMATION_NONE,	//	隊列なし
		FORMATION_1,	//	隊列１
		FORMATION_2,	//	隊列２
		FORMATION_3,	//	隊列３
		FORMATION_4,	//	隊列４
		FORMATION_5,	//	隊列５
		FORMATION_NUM,	//	隊列数
	};

//===================== 関数 ======================//

	//	コンストラクタ / デストラクタ
	EnemyManager();
	~EnemyManager();

	//	作成処理
	void Create();

	//	解放処理
	void Release();

	//	初期処理
	void Initialize();

	//	更新処理
	void Update(PlayerManager& _playerManager, ShotManager& _shotManager, BossManager& _bossManager, ItemManager& _item, Sound& _sound, SoundEffect& _soundEffect, Warning& _warning);

	//	描画処理
	void Draw();

	//	リストから解放
	void RemoveFromList(const EnemyBase* _enemyPtr, ItemManager& _item);

	//	隊列内エネミー数の減算
	void SubEnemyCount(const ENEMY_FORMATION_NUM _formationNum) { m_enemyCounter[(int)_formationNum]--; };

	//	隊列内エネミー数の減算
	void SubEnemyCount(const int _formationNum) { m_enemyCounter[_formationNum]--; };

//==================== getter =====================//

	//	使用リストのサイズの getter
	const int GetListSize() const { return (int)m_useList.size(); }

	//	エネミーへのポインタの getter
	EnemyBase* GetEnemyPtr(const int _num);

	//	隊列内のエネミー数の getter
	const int GetEnemyInFormationNum(const ENEMY_FORMATION_NUM _formationNum) const { return m_enemyCounter[(int)_formationNum]; };

	//	隊列内のエネミー数の getter
	const int GetEnemyInFormationNum(const int _formationNum) const { return m_enemyCounter[_formationNum]; };

	//	隊列内にエネミーは存在するかの getter
	const bool GetIsEnemyExistence(const ENEMY_FORMATION_NUM _formationNum) const { return (m_enemyCounter[(int)_formationNum] > 0); };

	//	隊列内にエネミーは存在するかの getter
	const bool GetIsEnemyExistence(const int _formationNum) const { return (m_enemyCounter[_formationNum] > 0); };

private:

//===================== 構造体 =====================//

	struct EntryBuffer
	{
		EnemyBase*	m_pBuf[CommonConstant::MAX_ENEMY_ENTRY_NUM];			//	バッファ
		bool		m_isInUse[CommonConstant::MAX_ENEMY_ENTRY_NUM];			//	メモリの使用状況
	};

//================== 内部処理関数 ==================//

	//	エネミーの登録
	void _EnemyEntry();

	//	リストの更新
	void _ListUpdate(PlayerManager& _playerManager, ShotManager& _shot, SoundEffect& _soundEffect);

	//	リストから解放
	void _RemoveFromList(ItemManager& _item);

	//	エネミーパート１
	void _EnemyParts1();

	//	エネミーパート２
	void _EnemyParts2();

	//	エネミーパート３
	void _EnemyParts3();

	//	エネミーパート４
	void _EnemyParts4();

	//	エネミーパート５
	void _EnemyParts5();

	//	隊列を組んで登録
	void _FormationEntry(const ENEMY_KIND _enemyKind, const float _startTime, const float _intervalTime, const VECTOR _startPos, const int _formationNum, 
						 const bool _isUseShot, const bool _isRestriction,  const int _formation);

	//	登録処理
	void _Registration(const ENEMY_KIND _enemyKind, const VECTOR _startPos, const float _entryTime, const bool _isUseShot, const bool _isRestriction, const int _formation);

	//	縦隊列を組んで登録
	void _VerticalFormationEntry(const ENEMY_KIND _enemyKind, const float _startTime, const float _intervalTime, const float _shiftInterval, const VECTOR _startPos, 
								 const int _formationNum, const bool _isUseShot, const bool _isHavingItem, const int _formation);

	//	縦に登録処理
	void _VerticalRegistration(const ENEMY_KIND _enemyKind, const VECTOR _startPos, const float _entryTime, const float _shiftInterval, const int _num,
							   const bool _isUseShot, const bool _isHavingItem, const int _formation);

	//	一体分の登録
	bool _OneEnemyRegistration(const int _enemyKind, const VECTOR _startPos, const bool _isUseShot, const bool _isHavingItem, const int _formation);

	//	一体分の登録し、ショットは二分の一の確率で使用する
	bool _OneEnemyRegistrationToShotProbaility(const int _enemyKind, const VECTOR _startPos, const bool _isUseShot, const bool _isRestriction, const int _formation);

	//	最終的な解放処理
	void _FinalRelease();

//=================== メンバ変数 ===================//

	int						m_sourceModelHandle[ENEMY_KIND::ENEMY_NUM];				//	大本ハンドル
	EntryBuffer				m_entryBuffer[ENEMY_KIND::ENEMY_NUM];					//	登録バッファ
	std::list<EnemyBase*>	m_useList;												//	使用エネミーリスト
	float					m_entryTime;											//	出現タイマー
	int						m_enemyCounter[ENEMY_FORMATION_NUM::FORMATION_NUM];		//	隊列内のエネミー数

//===================== 静的定数 ===================//

	static const float	MAX_EMERGNCE_INTERVAL_TIME;	//	出撃タイマーの最大間隔
	static const float	MAX_WAIT_TIME;				//	待機時間の最大
	static const float	PART_INTERAL;				//	パートの間隔
	static const float	MIN_ALIVE_TIME;				//	生存時間の最小
	static const float	MEDIUM_BOSS_EMERGE_TIME;	//	中ボスの出現時の時間
	static const float	WARNING_SE_TIME;			//	警告音の時間
	static const float	LAST_BOSS_EMERGE_TIME;		//	最終ボスの出現時の時間
	static const float	MAX_FLAME;					//	フレームの最大
	static const float	RESET_FLAME;				//	フレームのリセット値
 	static const float	PRODUCTION_TIME;			//	演出の時間
	static const float	MAX_X_POS;					//	X座標の最大
	static const float	MIN_X_POS;					//	X座標の最小
	static const float	MAX_Y_POS;					//	Y座標の最大
	static const float	MIN_Y_POS;					//	Y座標の最小
	static const float	SKIP;						//	スキップ

};