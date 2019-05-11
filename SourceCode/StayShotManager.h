
//=============================================================================
//	@file	StayShotManager.h
//	@brief	滞在ショットマネージャー
//	@autor	Takuya Ochi
//	@date	2018/12/25
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
class StayShot;
class SoundEffect;

//-----------------------------------------------------------------------------
//	@brief	ステイショットマネージャー
//-----------------------------------------------------------------------------
class StayShotManager final
{
public:

//===================== 関数 ======================//

	//	コンストラクタ / デストラクタ
	StayShotManager();
	~StayShotManager();

	//	作成処理
	void Create();

	//	解放処理
	void Release();

	//	初期処理
	void Initialize();

	//	更新処理
	void Update(SoundEffect& _soundEffect);

	//	リストに登録
	void RegisterOnList(const VECTOR _startPos);

//==================== getter =====================//

	//	使用リストのサイズの getter
	const int GetListSize() const { return (int)m_useList.size(); }

	//	ショットへのポインタの getter
	StayShot* GetStayShotPtr(const int _num);

private:

	//================== 内部処理関数 ==================//

	//	リストの更新
	void _UpdateList(SoundEffect& _soundEffect);

	//	リストから解放
	void _RemoveFromList();

	//	最終的な解放処理
	void _FinalRelease();

	//=================== メンバ変数 ===================//

	StayShot*				m_pStayShot[CommonConstant::MAX_STAY_SHOT_NUM];		//	滞在ショット
	std::list<StayShot*>	m_useList;											//	使用ショットリスト
	std::list<StayShot*>	m_unusedList;										//	未使用ショットリスト

//===================== 静的定数 ===================//

	static const float MAX_X_POS;		//	X座標の最大

};