
//=============================================================================
//	@file	ItemManager.h
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
#include "DxLib.h"
#include "Common.h"
#include <list>

//-----------------------------------------------------------------------------
//	@brief	前方宣言
//-----------------------------------------------------------------------------
class Item;

//-----------------------------------------------------------------------------
//	@brief	アイテムマネージャー
//-----------------------------------------------------------------------------
class ItemManager final
{
public:

//===================== 関数 ======================//

	//	コンストラクタ / デストラクタ
	ItemManager();
	~ItemManager();

	//	作成処理
	void Create();

	//	解放処理
	void Release();

	//	更新処理
	void Update();

	//	描画処理
	void Draw();

	//	リストに登録
	void RegisterOnList(const VECTOR _startPos);

	//	リストから削除
	void RemoveFromList(const Item* _itemPtr);

//==================== getter =====================//

	//	使用リストのサイズの getter
	const int GetListSize() const { return (int)m_useList.size(); }

	//	ショットへのポインタの getter
	Item* GetItemPtr(const int _num);

private:

//================== 内部処理関数 ==================//

	//	リストの更新
	void _UpdateList();

	//	リストの描画
	void _DrawList();

	//	リストから削除
	void _RemoveFromList();

	//	最終的な解放処理
	void _FinalRelease();

//=================== メンバ変数 ===================//

	Item*						m_pItem[CommonConstant::MAX_ITEM_NUM];	//	アイテム
	std::list<Item*>			m_useList;								//	使用アイテムリスト
	std::list<Item*>			m_unusedList;							//	未使用アイテムリスト
	int							m_sourceItem1ModelHandle;				//	大本のモデルハンドル
	int							m_sourceItem2ModelHandle;				//	大本のモデルハンドル
	int							m_sourceItem3ModelHandle;				//	大本のモデルハンドル

//===================== 静的定数 ===================//

	static const float MIN_X_POS;		//	X座標の最小
};