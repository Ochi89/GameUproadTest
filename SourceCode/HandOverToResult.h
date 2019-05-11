
//=============================================================================
//	@file	HandOverToResult.h
//	@brief	リザルトへ引き渡し
//	@autor	Takuya Ochi
//	@date	2018/1/03
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

//-----------------------------------------------------------------------------
//	@brief	引き渡しクラス
//-----------------------------------------------------------------------------
class HandOverToResult final
{
public:

//===================== 関数 ======================//

	//	デストラクタ
	~HandOverToResult();

	//	生成
	HandOverToResult* GetInstance();

	//	リセット
	void Reset();

	//	討伐数の加算
	void AddDestructionNum() { m_destructionNum++; }

//==================== getter =====================//

	//	ライフの getter
	const int& GetLife() const { return m_life; }

	//	討伐数の getter
	const int& GetDestructionNum() const { return m_destructionNum; }

	//	クリアしたかの getter
	const bool& GetIsClear() const { return m_isClear; }

//==================== setter =====================//

	//	ライフの setter
	void SetLife(const int _set) { m_life = _set; }

	//	討伐数の setter
	void SetDestructionNum(const int _set) { m_destructionNum = _set; }

	//	クリアしたかの setter
	void SetIsClear(const bool _set) { m_isClear = _set; }

private:

//================== シングルトン ==================//

	// コンストラクタ
	HandOverToResult();

//=================== メンバ変数 ===================//

	static HandOverToResult		s_instance;				//	インスタンス
	int							m_life;					//	ライフ
	int							m_destructionNum;		//	討伐数
	bool						m_isClear;				//	クリアしたか

};

#define HAND_OVER_RESULT HandOverToResult::GetInstance()