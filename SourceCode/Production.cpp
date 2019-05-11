
//=============================================================================
//	@file	Production.h
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
#include "Production.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const VECTOR	Production::GAUGE_START_VERTEX_1 = VGet(0.0f, 0.0f, 0.0f);			//	ゲージの開始頂点１
const VECTOR	Production::GAUGE_START_VERTEX_2 = VGet(0.0f, 1080.0f, 0.0f);		//	ゲージの開始頂点２
const VECTOR	Production::GAUGE_END_VERTEX_1 = VGet(1920.0f, 0.0f, 0.0f);			//	ゲージの終了頂点１
const VECTOR	Production::GAUGE_END_VERTEX_2 = VGet(1920.0f, 1080.0f, 0.0f);		//	ゲージの終了頂点２
const float		Production::MAX_Y_AXIS_1 = 120.0f;									//	ゲージの最大値
const float		Production::MAX_Y_AXIS_2 = 960.0f;									//	ゲージの最大値
const float		Production::GAUGE_ADD_SPEED = 10.0f;								//	ゲージの加算速度１

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
Production::Production()
{
	//	各変数の初期化
	m_gaugeBar1.m_vertexTop = GAUGE_START_VERTEX_1;
	m_gaugeBar1.m_vertexUnder = GAUGE_END_VERTEX_1;
	m_gaugeBar2.m_vertexTop = GAUGE_START_VERTEX_2;
	m_gaugeBar2.m_vertexUnder = GAUGE_END_VERTEX_2;
	m_gaugeColor = GetColor(30, 30, 30);
	m_isSceneProduction = false;
	m_isSpecialProduction = false;
	m_isClearProduction = false;
	m_isTitleProduction = false;
	m_specialProductionTime = 0.0f;
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
Production::~Production()
{
	//	処理なし
}

//-----------------------------------------------------------------------------
//	@brief	インスタンス生成
//-----------------------------------------------------------------------------
Production* Production::GetInstance()
{
	return &s_instance;
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void Production::Update()
{
	//	演出中
	const bool isActive = m_isSceneProduction || m_isSpecialProduction || m_isClearProduction;
	if (isActive)
	{
		//	演出中なので、
		//	ゲージを出現させる
		_EmergenceGauge();
		return;
	}

	//	演出中ではないので、
	//	ゲージを元の状態に戻す
	_RestoreGauge();
}

//-----------------------------------------------------------------------------
//	@brief	描画処理
//-----------------------------------------------------------------------------
void Production::Draw()
{
	//	ゲージの描画
	DrawBox((int)m_gaugeBar1.m_vertexTop.x, (int)m_gaugeBar1.m_vertexTop.y, (int)m_gaugeBar1.m_vertexUnder.x, (int)m_gaugeBar1.m_vertexUnder.y, m_gaugeColor, TRUE);
	DrawBox((int)m_gaugeBar2.m_vertexTop.x, (int)m_gaugeBar2.m_vertexTop.y, (int)m_gaugeBar2.m_vertexUnder.x, (int)m_gaugeBar2.m_vertexUnder.y, m_gaugeColor, TRUE);
}

//-----------------------------------------------------------------------------
//	@brief	必殺技のリセット
//-----------------------------------------------------------------------------
void Production::ResetSpecialProduction()
{
	m_specialProductionTime = 0.0f; 
	m_isSpecialProduction = false; 
}

//-----------------------------------------------------------------------------
//	@brief	すべてのリセット
//-----------------------------------------------------------------------------
void Production::AllReset()
{
	//	各変数の初期化
	m_gaugeBar1.m_vertexTop = GAUGE_START_VERTEX_1;
	m_gaugeBar1.m_vertexUnder = GAUGE_END_VERTEX_1;
	m_gaugeBar2.m_vertexTop = GAUGE_START_VERTEX_2;
	m_gaugeBar2.m_vertexUnder = GAUGE_END_VERTEX_2;
	m_gaugeColor = GetColor(30, 30, 30);
	m_isSceneProduction = false;
	m_isSpecialProduction = false;
	m_isClearProduction = false;
	m_isTitleProduction = false;
	m_specialProductionTime = 0.0f;
}

//-----------------------------------------------------------------------------
//	@brief	ゲージの出現
//-----------------------------------------------------------------------------
void Production::_EmergenceGauge()
{
	m_gaugeBar1.m_vertexUnder.y += GAUGE_ADD_SPEED;
	m_gaugeBar2.m_vertexTop.y -= GAUGE_ADD_SPEED;

	//	ゲージの最大最小処理
	const bool isMaxGauge1 = m_gaugeBar1.m_vertexUnder.y >= MAX_Y_AXIS_1;
	const bool isMaxGauge2 = m_gaugeBar2.m_vertexTop.y <= MAX_Y_AXIS_2;
	const bool isMax = isMaxGauge1 || isMaxGauge2;
	if (isMax)
	{
		m_gaugeBar1.m_vertexUnder.y = MAX_Y_AXIS_1;
		m_gaugeBar2.m_vertexTop.y = MAX_Y_AXIS_2;
	}
}

//-----------------------------------------------------------------------------
//	@brief	ゲージを元に戻す
//-----------------------------------------------------------------------------
void Production::_RestoreGauge()
{
	m_gaugeBar1.m_vertexUnder.y -= GAUGE_ADD_SPEED;
	m_gaugeBar2.m_vertexTop.y += GAUGE_ADD_SPEED;

	//	ゲージの最大最小処理
	const bool isMaxGauge1 = m_gaugeBar1.m_vertexUnder.y <= GAUGE_END_VERTEX_1.y;
	const bool isMaxGauge2 = m_gaugeBar2.m_vertexTop.y >= GAUGE_START_VERTEX_2.y;
	const bool isMax = isMaxGauge1 || isMaxGauge2;
	if (isMax)
	{
		m_gaugeBar1.m_vertexUnder.y = GAUGE_END_VERTEX_1.y;
		m_gaugeBar2.m_vertexTop.y = GAUGE_START_VERTEX_2.y;
	}
}
