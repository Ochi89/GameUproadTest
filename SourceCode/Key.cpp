﻿
//=============================================================================
//	@file	Key.cpp
//	@brief	キー入力
//	@autor	Takuya Ochi
//	@date	2018/9/28
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "Key.h"

//-----------------------------------------------------------------------------
//	@brief	静的メンバ変数の実態の作成
//-----------------------------------------------------------------------------
Key	*Key::s_instance = nullptr;

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
Key::Key()
{
	for (int i = 0; i < CommonConstant::MAX_KEY_INPUT_NUM; i++)
	{
		m_key[i] = 0;
		m_prevKey[i] = 0;
	}
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
Key::~Key()
{
	//	処理なし
}

//-----------------------------------------------------------------------------
//	@brief	インスタンス生成
//-----------------------------------------------------------------------------
void Key::CreateInstance()
{
	//	すでに存在している
	if (s_instance) { CommonDebug::Log(" [ Key.cpp ] : error : Instance has been created."); return; }

	//	nullptrのとき
	if (!s_instance) { Key::s_instance = new Key(); }

	//	作成失敗
	if (!s_instance) { CommonDebug::Log(" [ Key.cpp ] : error : Failed to create instance."); return; }
}

//-----------------------------------------------------------------------------
//	@brief	インスタンス削除
//-----------------------------------------------------------------------------
void Key::DeleteInstance()
{
	CommonSafe::Delete(s_instance);
}

//-----------------------------------------------------------------------------
//	@brief	インスタンス生成
//-----------------------------------------------------------------------------
Key* Key::GetInstance()
{
	return s_instance;
}

//-----------------------------------------------------------------------------
//	@brief	すべて使用中にする
//			シーン切り替え時のときのみ呼ぶ
//-----------------------------------------------------------------------------
void Key::AllInUse()
{
	for (int i = 0; i < CommonConstant::MAX_KEY_INPUT_NUM; i++)
	{
		m_prevKey[i] = 1;
	}
}

//-----------------------------------------------------------------------------
//	@brief	押した瞬間
//-----------------------------------------------------------------------------
bool Key::Push(const int _keyCode)
{
	//	全てのキーの状態をとる
	GetHitKeyStateAll(m_key);

	//	押された瞬間
	const bool isKeyPush = m_key[_keyCode] == KEY_STATE::KEY_ON;
	if (isKeyPush)
	{
		//	前に押されていたか
		const bool isPrevKeyOff = m_prevKey[_keyCode] == KEY_STATE::KEY_OFF;
		if (isPrevKeyOff)
		{
			//	押された
			m_prevKey[_keyCode] = KEY_STATE::KEY_ON;
			return true;
		}

		return false;
	}
	else
	{
		//	押されていない
		m_prevKey[_keyCode] = KEY_STATE::KEY_OFF;
		return false;
	}
}

//-----------------------------------------------------------------------------
//	@brief	押している
//-----------------------------------------------------------------------------
bool Key::Press(const int _keyCode)
{
	//	全てのキーの状態をとる
	GetHitKeyStateAll(m_key);

	//	押された 
	const bool isKeyPress = m_key[_keyCode] == KEY_STATE::KEY_ON;
	if (isKeyPress) { return true; }

	//	押されていない
	return false;
}
