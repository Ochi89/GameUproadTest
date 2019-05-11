
//=============================================================================
//	@file	StayShotManager.h
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
#include "StayShotManager.h"
#include "StayShot.h"
#include "SoundEffect.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const float StayShotManager::MAX_X_POS = 110.0f;	//	X座標の最大

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
StayShotManager::StayShotManager()
{
	//	まだどこも指していないので、NULLで初期化
	for (int i = 0; i < CommonConstant::MAX_STAY_SHOT_NUM; i++)
	{
		m_pStayShot[i] = NULL;
	}
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
StayShotManager::~StayShotManager()
{
	//	最終的な解放処理
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成処理
//-----------------------------------------------------------------------------
void StayShotManager::Create()
{
	//	初期処理
	for (int i = 0; i < CommonConstant::MAX_STAY_SHOT_NUM; i++)
	{
		//	ショットの作成
		m_pStayShot[i] = new StayShot();

		//	未使用リストに登録
		m_unusedList.push_back(m_pStayShot[i]);
	}
}

//-----------------------------------------------------------------------------
//	@brief	解放処理
//-----------------------------------------------------------------------------
void StayShotManager::Release()
{
	//	リストに登録されていたら削除
	if (!m_unusedList.empty()) { m_unusedList.clear(); }
	if (!m_useList.empty()) { m_useList.clear(); }

	//	ショットの解放
	for (int i = 0; i < CommonConstant::MAX_STAY_SHOT_NUM; i++)
	{
		CommonSafe::Delete(m_pStayShot[i]);
	}
}

//-----------------------------------------------------------------------------
//	@brief	初期処理
//-----------------------------------------------------------------------------
void StayShotManager::Initialize()
{
	//	未使用リストが空でないとき
	if (!m_unusedList.empty())
	{
		//	初期化
		std::list<StayShot*>::iterator 	ite;
		for (ite = m_unusedList.begin(); ite != m_unusedList.end(); ite++)
		{
			(*ite)->Initialize();
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void StayShotManager::Update(SoundEffect& _soundEffect)
{
	//	必殺技のときは更新しない
	const bool isNotSpecialProduction = !PRODUCTION->GetIsSpecialProduction();
	if (isNotSpecialProduction)
	{
		//	リストの更新
		_UpdateList(_soundEffect);

		//	リストから解放
		_RemoveFromList();
	}
}

//-----------------------------------------------------------------------------
//	@brief	リストに登録
//-----------------------------------------------------------------------------
void StayShotManager::RegisterOnList(const VECTOR _startPos)
{
	//	未使用リストが空でないとき
	const bool isAlreadyEntry = !m_unusedList.empty();
	if (isAlreadyEntry)
	{
		//	未使用リストの先頭のイテレーターを取る
		std::list<StayShot*>::iterator 	ite;
		ite = m_unusedList.begin();

		//	まだ向きを設定していないときは設定する
		const bool isNotSet = !(*ite)->GetIsOneTime();
		if (isNotSet)
		{
			//	内容の初期化
			(*ite)->SetStartPos(_startPos);
			(*ite)->SetIsOneTime(true);
		}

		//	未使用リストから使用リストに登録
		m_useList.push_back(*ite);

		//	未使用リストから削除
		m_unusedList.remove(*ite);
	}
}

//-----------------------------------------------------------------------------
//	@brief	ショットへのポインタの取得
//-----------------------------------------------------------------------------
StayShot* StayShotManager::GetStayShotPtr(const int _num)
{
	int counter = 0;

	//	使用リストが空でないとき
	if (!m_useList.empty())
	{
		//	リストの回数分回す
		std::list<StayShot*>::iterator 	ite;
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
//	@brief	リストの更新
//-----------------------------------------------------------------------------
void StayShotManager::_UpdateList(SoundEffect& _soundEffect)
{
	//	使用リストが空でないとき
	if (!m_useList.empty())
	{
		//	リストの回数分回す
		std::list<StayShot*>::iterator 	ite;
		for (ite = m_useList.begin(); ite != m_useList.end(); ite++)
		{
			//	更新処理
			(*ite)->Update(_soundEffect);
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	リストから削除
//-----------------------------------------------------------------------------
void StayShotManager::_RemoveFromList()
{
	//	使用リストが空でないとき
	if (!m_useList.empty())
	{
		//	リストの回数分回す
		std::list<StayShot*>::iterator 	ite;
		for (ite = m_useList.begin(); ite != m_useList.end(); ite++)
		{
			//	範囲外か、ショットが終了していたら、
			//	リストから削除する
			const bool isArrivalRight = (*ite)->GetPos().x >= MAX_X_POS;
			const bool isActive = isArrivalRight || (*ite)->GetIsShotEnd();
			if (isActive)
			{
				//	初期化
				(*ite)->Initialize();

				//	使用リストから未使用リストに登録
				m_unusedList.push_back(*ite);

				//	使用リストから削除
				m_useList.remove(*ite);

				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放処理
//-----------------------------------------------------------------------------
void StayShotManager::_FinalRelease()
{
	//	リストに登録されていたら削除
	if (!m_unusedList.empty()) { m_unusedList.clear(); }
	if (!m_useList.empty()) { m_useList.clear(); }

	//	ショットの解放
	for (int i = 0; i < CommonConstant::MAX_STAY_SHOT_NUM; i++)
	{
		CommonSafe::Delete(m_pStayShot[i]);
	}
}
