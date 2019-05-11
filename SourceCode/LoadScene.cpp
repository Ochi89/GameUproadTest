﻿
//=============================================================================
//	@file	LoadScene.h
//	@brief	ローディング
//	@autor	Takuya Ochi
//	@date	2018/12/30
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "LoadScene.h"

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
Loading::Loading()
{
	m_spriteHendle = LoadGraph("Data/Sprite/Loading/NowLoading.png");
	CommonDebug::Assert((m_spriteHendle <= -1), " [ Loading.cpp ] : error : sprite loading failed.");
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
Loading::~Loading()
{
	//	スプライトのアンロード
	DeleteGraph(m_spriteHendle);
}

//-----------------------------------------------------------------------------
//	@brief	インスタンス生成
//-----------------------------------------------------------------------------
Loading* Loading::GetInstance()
{
	return &m_instance;
}

//-----------------------------------------------------------------------------
//	@brief	描画
//-----------------------------------------------------------------------------
void Loading::Draw()
{
	DrawGraph(0, 0, m_spriteHendle, TRUE);

	//	画面の更新
	ScreenFlip();
}
