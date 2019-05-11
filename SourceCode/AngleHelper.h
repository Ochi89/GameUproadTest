
//=============================================================================
//	@file	AngleHelper.h
//	@brief	角度処理
//	@autor	Takuya Ochi
//	@date	2018/11/15
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	プリプロセッサ
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "DxLib.h"

//-----------------------------------------------------------------------------
//	@brief	前方宣言
//-----------------------------------------------------------------------------
class Pad;

//-----------------------------------------------------------------------------
//	@brief	角度処理クラス
//-----------------------------------------------------------------------------
class AngleHelper final
{
public:

//===================== 関数 ======================//

	//	角度を求める
	static void AskAngle(VECTOR& _angle, Pad& _pad);

	//	角度を求める
	static void AskAngle(VECTOR& _angle, VECTOR _dir);

private:

//================== 内部処理関数 ==================//

	//	目的の角度を求める
	static VECTOR _GetTargetAngle(VECTOR& _angle, Pad& _pad);

	//	目的の角度を求める
	static VECTOR _GetTargetAngle(VECTOR& _angle, VECTOR _dir);

	//	入力による角度の変更
	static void _ChangeTargetAngleByInput(const bool _isState, float& _angle, const float _speed);

	//	目的の角度の最大と最小の設定
	static void _SetMaxToMin(float& _angle, const float _maxAngle, const float _minAngle);

	//	入力がないときの角度の変更
	static void _ChangeTargetAngleNoInput(const bool _isState, float& _angle);

//===================== 静的定数 ===================//

	static const float ADD_ANGLE_SPEED;		//	角度の加算速度
	static const float SUB_ANGLE_SPEED;		//	角度の減算速度
	static const float LERP_SPEED;			//	線形保管の速度
	static const float LERP_RETURN_SPEED;	//	線形保管の戻る速度
	static const float ANGLE_NONE;			//	角度の初期化
	static const float MAX_ANGLE;			//	垂直方向の角度の最大
	static const float MIN_ANGLE;			//	垂直方向の角度の最小
};