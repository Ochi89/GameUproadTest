
//=============================================================================
//	@file	UIBackPixel.h
//	@brief	背景ピクセルUI
//	@autor	Takuya Ochi
//	@date	2018/1/15
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
//	@brief	UIGameOverクラス
//-----------------------------------------------------------------------------
class UIBackPixel final
{
public:

//===================== 関数 ======================//

	//	コンストラクタ/デストラクタ
	UIBackPixel();
	~UIBackPixel();

	//	描画処理
	void Update();

	//	描画処理
	void Draw();

private:

//===================== 構造体 =====================//
	
	//	ピクセル
	struct BackPixel
	{
		VECTOR		m_centerPoint;		//	中心点
		float		m_radius;			//	半径
		float		m_speed;			//	速度
		bool		m_isInUse;			//	使用状況
	};

//================== 内部処理関数 ==================//

	//	点の作成処理
	void _CreateBackPixel();

	//	点の更新処理
	void _UpdateBackPixel();

	//	点の描画処理
	void _DrawBackPixel();

//=================== メンバ変数 ===================//

	BackPixel	m_backPixel[CommonConstant::MAX_BACK_PIXEL_NUM];	//	背景ピクセル

//===================== 静的定数 ===================//

	static const float		MIN_RADIUS_SIZE;	//	半径サイズの最小
	static const float		MAX_RADIUS_SIZE;	//	半径サイズの最大
	static const float		MIN_SPEED;			//	速度の最小
	static const float		MAX_SPEED;			//	速度の最大
	static const VECTOR		START_POSTION;		//	座標の開始位置
	static const VECTOR		MIN_POSTION;		//	座標の最小
	static const VECTOR		MAX_POSTION;		//	座標の最大

};