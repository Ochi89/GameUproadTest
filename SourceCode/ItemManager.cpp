
//=============================================================================
//	@file	ItemManager.cpp
//	@brief	アイテムマネージャー
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
#include "ItemManager.h"
#include "Item.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const float ItemManager::MIN_X_POS = -110.0f;	//	X座標の最小

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
ItemManager::ItemManager()
{
	//	まだどこも指していないので、NULLで初期化
	for (int i = 0; i < CommonConstant::MAX_ITEM_NUM; i++)
	{
		m_pItem[i] = NULL;
	}

	//	各変数の初期化
	m_sourceItem1ModelHandle = -1;
	m_sourceItem2ModelHandle = -1;
	m_sourceItem3ModelHandle = -1;
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
ItemManager::~ItemManager()
{
	//	最終的な解放処理
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成処理
//-----------------------------------------------------------------------------
void ItemManager::Create()
{
	//	大本モデルのロード
	m_sourceItem1ModelHandle = MV1LoadModel("Data/Model/Item/Item1.mqo");
	m_sourceItem2ModelHandle = MV1LoadModel("Data/Model/Item/Item2.mqo");
	m_sourceItem3ModelHandle = MV1LoadModel("Data/Model/Item/Item3.mqo");
	CommonDebug::Assert((m_sourceItem1ModelHandle <= -1), " [ Shooter.cpp ] : error : model loading failed.");
	CommonDebug::Assert((m_sourceItem2ModelHandle <= -1), " [ Shooter.cpp ] : error : model loading failed.");
	CommonDebug::Assert((m_sourceItem3ModelHandle <= -1), " [ Shooter.cpp ] : error : model loading failed.");

	//	初期処理
	for (int i = 0; i < CommonConstant::MAX_ITEM_NUM; i++)
	{
		//	ショットの作成
		m_pItem[i] = new Item(m_sourceItem1ModelHandle, m_sourceItem2ModelHandle, m_sourceItem3ModelHandle);

		//	未使用リストに登録
		m_unusedList.push_back(m_pItem[i]);
	}
}

//-----------------------------------------------------------------------------
//	@brief	解放処理
//-----------------------------------------------------------------------------
void ItemManager::Release()
{
	//	大本モデルのアンロード
	MV1DeleteModel(m_sourceItem1ModelHandle);
	MV1DeleteModel(m_sourceItem2ModelHandle);
	MV1DeleteModel(m_sourceItem3ModelHandle);

	//	リストに登録されていたら削除
	if (!m_unusedList.empty()) { m_unusedList.clear(); }
	if (!m_useList.empty()) { m_useList.clear(); }

	//	ショットの解放
	for (int i = 0; i < CommonConstant::MAX_ITEM_NUM; i++)
	{
		CommonSafe::Delete(m_pItem[i]);
	}
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void ItemManager::Update()
{
	//	必殺技のときは更新しない
	const bool isNotSpecialProduction = !PRODUCTION->GetIsSpecialProduction();
	if (isNotSpecialProduction)
	{
		//	解放処理
		_RemoveFromList();

		//	リストの更新
		_UpdateList();
	}
}

//-----------------------------------------------------------------------------
//	@brief	描画処理
//-----------------------------------------------------------------------------
void ItemManager::Draw()
{
	_DrawList();
}

//-----------------------------------------------------------------------------
//	@brief	リストに登録
//-----------------------------------------------------------------------------
void ItemManager::RegisterOnList(const VECTOR _startPos)
{
	//	未使用リストが空でないとき
	if (!m_unusedList.empty())
	{
		//	未使用リストの先頭のイテレーターを取る
		std::list<Item*>::iterator 	ite;
		ite = m_unusedList.begin();

		//	まだ向きを設定していないときは設定する
		if (!(*ite)->GetIsOneTime())
		{
			//	内容の初期化
			(*ite)->SetStartPos(_startPos);
			(*ite)->SetIsOneTime(true);
			(*ite)->Update();
		}

		//	未使用リストから使用リストに登録
		m_useList.push_back(*ite);

		//	未使用リストから削除
		m_unusedList.remove(*ite);
	}
}

//-----------------------------------------------------------------------------
//	@brief	リストから削除
//-----------------------------------------------------------------------------
void ItemManager::RemoveFromList(const Item* _itemPtr)
{
	//	使用リストが空でないとき
	if (!m_useList.empty())
	{
		//	リストの回数分回す
		std::list<Item*>::iterator 	ite;
		for (ite = m_useList.begin(); ite != m_useList.end(); ite++)
		{
			//	_num と一致したポインタを返す
			const bool isSame = _itemPtr == (*ite);
			if (isSame)
			{
				//	初期化にする
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
//	@brief	アイテムのポインタの getter
//-----------------------------------------------------------------------------
Item* ItemManager::GetItemPtr(const int _num)
{
	int counter = 0;

	//	使用リストが空でないとき
	if (!m_useList.empty())
	{
		//	リストの回数分回す
		std::list<Item*>::iterator 	ite;
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
void ItemManager::_UpdateList()
{
	//	使用リストが空でないとき
	if (!m_useList.empty())
	{
		//	リストの回数分回す
		std::list<Item*>::iterator 	ite;
		for (ite = m_useList.begin(); ite != m_useList.end(); ite++)
		{
			//	更新処理
			(*ite)->Update();
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	描画処理
//-----------------------------------------------------------------------------
void ItemManager::_DrawList()
{
	//	使用リストが空でないとき
	if (!m_useList.empty())
	{
		//	リストの回数分回す
		std::list<Item*>::iterator 	ite;
		for (ite = m_useList.begin(); ite != m_useList.end(); ite++)
		{
			(*ite)->Draw();
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	リストから削除
//-----------------------------------------------------------------------------
void ItemManager::_RemoveFromList()
{
	//	使用リストが空でないとき
	if (!m_useList.empty())
	{
		//	リストの回数分回す
		std::list<Item*>::iterator 	ite;
		for (ite = m_useList.begin(); ite != m_useList.end(); ite++)
		{
			//	範囲外のときは、削除する
			const bool isActive = (*ite)->GetPos().x <= MIN_X_POS;
			if (isActive)
			{
				//	設定可能にする
				(*ite)->SetIsOneTime(false);

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
void ItemManager::_FinalRelease()
{
	//	大本モデルのアンロード
	MV1DeleteModel(m_sourceItem1ModelHandle);
	MV1DeleteModel(m_sourceItem2ModelHandle);
	MV1DeleteModel(m_sourceItem3ModelHandle);

	//	リストに登録されていたら削除
	if (!m_unusedList.empty()) { m_unusedList.clear(); }
	if (!m_useList.empty()) { m_useList.clear(); }

	//	ショットの解放
	for (int i = 0; i < CommonConstant::MAX_ITEM_NUM; i++)
	{
		CommonSafe::Delete(m_pItem[i]);
	}
}
