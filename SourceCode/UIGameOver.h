﻿
//=============================================================================
//	@file	UIGameOver.h
//	@brief	ゲームオーバーUI
//	@autor	Takuya Ochi
//	@date	2018/12/30
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
//	@brief	前方宣言
//-----------------------------------------------------------------------------
struct UI;
class PlayerManager;

//-----------------------------------------------------------------------------
//	@brief	UIGameOverクラス
//-----------------------------------------------------------------------------
class UIGameOver final
{
public:

//===================== 関数 ======================//

	//	コンストラクタ/デストラクタ
	UIGameOver();
	~UIGameOver();

	//	作成処理
	void Create();

	//	解放処理
	void Release();

	//	描画処理
	void Update(PlayerManager& _playerManager);

	//	描画処理
	void Draw();

private:

//================== 内部処理関数 ==================//

	//	最終的な解放処理
	void _FinalRelease();

//=================== メンバ変数 ===================//

	UI		m_gameOverUI;		//	ゲームオーバーUI
	int		m_alpha;			//	アルファ値
	bool	m_isChangeAlpha;	//	アルファ値の切り替えフラグ
	bool	m_isGameOver;		//	ゲームオーバーフラグ

//===================== 静的定数 ===================//

	static const int		MIN_ALPHA;			//	アルファ値の最小
	static const int		MAX_ALPHA;			//	アルファ値の最大
	static const int		ALPHA_SPEED;		//	アルファ値の速度

};