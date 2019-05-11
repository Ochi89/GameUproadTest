
//=============================================================================
//	@file	ShotManager.h
//	@brief	ショットマネージャー
//	@autor	Takuya Ochi
//	@date	2018/12/12
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
class Shot;
class SoundEffect;

//-----------------------------------------------------------------------------
//	@brief	ショットマネージャー
//-----------------------------------------------------------------------------
class ShotManager final
{
public:

//==================== 列挙体 =====================//

	//	ショットの種類
	enum SHOT_KIND
	{
		PLAYER_SHOT,
		ENEMY_SHOT,
	};

//===================== 関数 ======================//

	//	コンストラクタ / デストラクタ
	ShotManager();
	~ShotManager();

	//	作成処理
	void Create();

	//	解放処理
	void Release();

	//	更新処理
	void Update();

	//	描画処理
	void Draw();

	//	リストに登録
	void RegisterOnList(const SHOT_KIND _shotKind, const VECTOR _startPos, const VECTOR _startDir, const float _speed, SoundEffect& _soundEffect);

	//	リストに登録
	void RegisterOnList(const SHOT_KIND _shotKind, const VECTOR _startPos, const VECTOR _startDir, const float _speed);

	//	リストから削除
	void RemoveFromList(const SHOT_KIND _shotKind, const Shot* _shotPtr);

//==================== getter =====================//

	//	使用リストのサイズの getter
	const int GetListSize(const SHOT_KIND _shotKind) const;

	//	ショットへのポインタの getter
	Shot* GetShotPtr(const SHOT_KIND _shotKind, const int _num);

private:

//================== 内部処理関数 ==================//

	//	ポインタの取得
	Shot* _GetShotPtr(std::list<Shot*>& _useList, const int _num);
	
	//	リストの登録
	void _RegisterOnList(std::list<Shot*>& _useList, std::list<Shot*>& _unusedList, const VECTOR _startPos, const VECTOR _startDir, const float _speed, SoundEffect& _soundEffect);

	//	リストの登録
	void _RegisterOnList(std::list<Shot*>& _useList, std::list<Shot*>& _unusedList, const VECTOR _startPos, const VECTOR _startDir, const float _speed);

	//	リストの更新
	void _UpdateList(const SHOT_KIND _shotKind);
	void _UpdateList(std::list<Shot*>& _useList);

	//	リストの描画
	void _DrawList(const SHOT_KIND _shotKind);
	void _DrawList(std::list<Shot*>& _useList);

	//	リストから削除
	void _RemoveFromList(const SHOT_KIND _shotKind);
	void _RemoveFromList(std::list<Shot*>& _useList, std::list<Shot*>& _unusedList);
	void _RemoveFromList(std::list<Shot*>& _useList, std::list<Shot*>& _unusedList, const Shot* _shotPtr);

	//	最終的な解放処理
	void _FinalRelease();

//=================== メンバ変数 ===================//

	Shot*						m_pPlayerShot[CommonConstant::MAX_SHOT_NUM];	//	ショット
	Shot*						m_pEnemyShot[CommonConstant::MAX_SHOT_NUM];		//	ショット
	std::list<Shot*>			m_usePlayerShotList;							//	プレイヤー用の使用ショットリスト
	std::list<Shot*>			m_useEnemyShotList;								//	エネミー用の使用ショットリスト
	std::list<Shot*>			m_unusedPlayerShotList;							//	未使用ショットリスト
	std::list<Shot*>			m_unusedEnemyShotList;							//	未使用ショットリスト
	int							m_sourceEnemyShotModelHandle;					//	大本のエネミー弾のモデルハンドル
	int							m_sourcePlayerShotModelHandle;					//	大本のプレイヤー弾のモデルハンドル

//===================== 静的定数 ===================//

	static const float MAX_X_POS;		//	X座標の最大
	static const float MIN_X_POS;		//	X座標の最小
	static const float MAX_Y_POS;		//	Y座標の最大
	static const float MIN_Y_POS;		//	Y座標の最小

};