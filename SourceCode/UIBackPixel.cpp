
//=============================================================================
//	@file	UIBackPixel.h
//	@brief	背景ピクセルUI
//	@autor	Takuya Ochi
//	@date	2018/1/15
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "UIBackPixel.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const float		UIBackPixel::MIN_RADIUS_SIZE = 0.1f;					//	半径サイズの最小
const float		UIBackPixel::MAX_RADIUS_SIZE = 0.2f;					//	半径サイズの最大
const float		UIBackPixel::MIN_SPEED = 0.1f;							//	X座標の最小
const float		UIBackPixel::MAX_SPEED = 0.2f;							//	X座標の最大
const VECTOR	UIBackPixel::START_POSTION = VGet(-120.0f, 0.0f, 0.0f);	//	X座標の開始位置
const VECTOR	UIBackPixel::MIN_POSTION = VGet(120.0f, 120.0f, 0.0f);	//	X座標の最小
const VECTOR	UIBackPixel::MAX_POSTION = VGet(140.0f, 140.0f, 0.0f);	//	X座標の最大

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
UIBackPixel::UIBackPixel()
{
	//	開始時の設定
	for (int i = 0; i < CommonConstant::MAX_BACK_PIXEL_NUM; i++)
	{
		//	中心位置
		m_backPixel[i].m_centerPoint.x = CommonFunction::GetRand(START_POSTION.x, MIN_POSTION.x);
		m_backPixel[i].m_centerPoint.y = CommonFunction::GetRand(START_POSTION.y, MIN_POSTION.y);
		m_backPixel[i].m_centerPoint.z = 0.0f;

		//	半径
		m_backPixel[i].m_radius = CommonFunction::GetRand(MIN_RADIUS_SIZE, MAX_RADIUS_SIZE);

		//	速度
		m_backPixel[i].m_speed = CommonFunction::GetRand(MIN_SPEED, MAX_SPEED);

		//	使用中
		m_backPixel[i].m_isInUse = true;
	}
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
UIBackPixel::~UIBackPixel()
{
	//	処理なし
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void UIBackPixel::Update()
{
	//	点の作成処理
	_CreateBackPixel();

	//	点の更新処理
	_UpdateBackPixel();
}

//-----------------------------------------------------------------------------
//	@brief	描画処理
//-----------------------------------------------------------------------------
void UIBackPixel::Draw()
{
	//	点の描画処理
	_DrawBackPixel();
}

//-----------------------------------------------------------------------------
//	@brief	点の生成処理
//-----------------------------------------------------------------------------
void UIBackPixel::_CreateBackPixel()
{
	for (int i = 0; i < CommonConstant::MAX_BACK_PIXEL_NUM; i++)
	{
		//	点の使用状況は未使用のとき
		const bool isBackPixelUnused = !m_backPixel[i].m_isInUse;
		if (isBackPixelUnused)
		{
			//	一つの点の生成

			//	中心位置
			m_backPixel[i].m_centerPoint.x = CommonFunction::GetRand(MIN_POSTION.x, MAX_POSTION.x);
			m_backPixel[i].m_centerPoint.y = CommonFunction::GetRand(START_POSTION.y, MAX_POSTION.y);
			m_backPixel[i].m_centerPoint.z = 0.0f;

			//	半径
			m_backPixel[i].m_radius = CommonFunction::GetRand(MIN_RADIUS_SIZE, MAX_RADIUS_SIZE);

			//	速度
			m_backPixel[i].m_speed = CommonFunction::GetRand(MIN_SPEED, MAX_SPEED);

			//	使用中
			m_backPixel[i].m_isInUse = true;
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	点の更新処理
//-----------------------------------------------------------------------------
void UIBackPixel::_UpdateBackPixel()
{
	//	演出時以外
	const bool isNotProduction = !PRODUCTION->GetIsSpecialProduction() || !PRODUCTION->GetIsSceneProduction() || !PRODUCTION->GetIsClearProduction();
	if (isNotProduction)
	{
		for (int i = 0; i < CommonConstant::MAX_BACK_PIXEL_NUM; i++)
		{
			//	点の使用状況は使用中のとき
			const bool isBackPixelUse = m_backPixel[i].m_isInUse;
			if (isBackPixelUse)
			{
				//	移動処理
				m_backPixel[i].m_centerPoint.x -= m_backPixel[i].m_speed;
				m_backPixel[i].m_centerPoint.z = 0.0f;

				//	範囲外処理
				const bool isMinXVal = m_backPixel[i].m_centerPoint.x <= START_POSTION.x;
				const bool isMinYVal = m_backPixel[i].m_centerPoint.y <= START_POSTION.y;
				const bool isMinVal = isMinXVal || isMinYVal;
				if (isMinVal) { m_backPixel[i].m_isInUse = false; }
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	点の描画処理
//-----------------------------------------------------------------------------
void UIBackPixel::_DrawBackPixel()
{
	//	演出時以外
	const bool isNotProduction = !PRODUCTION->GetIsSpecialProduction() && !PRODUCTION->GetIsSceneProduction() && !PRODUCTION->GetIsClearProduction();
	if (isNotProduction)
	{
		for (int i = 0; i < CommonConstant::MAX_BACK_PIXEL_NUM; i++)
		{
			//	点の使用状況は使用中のとき
			const bool isBackPixelUse = m_backPixel[i].m_isInUse;
			if (isBackPixelUse)
			{
				DrawSphere3D(m_backPixel[i].m_centerPoint, m_backPixel[i].m_radius, 5, 0xffffff, 0xffffff, TRUE);
			}
		}
	}
}
