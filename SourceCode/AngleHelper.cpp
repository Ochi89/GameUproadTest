
//=============================================================================
//	@file	AngleHelper.cpp
//	@brief	角度処理
//	@autor	Takuya Ochi
//	@date	2018/11/15
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "AngleHelper.h"
#include "Common.h"
#include "Pad.h"
#include "Key.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const float AngleHelper::ADD_ANGLE_SPEED = 0.2f;	//	角度の加算速度
const float AngleHelper::SUB_ANGLE_SPEED = -0.2f;	//	角度の減算速度
const float AngleHelper::LERP_SPEED = 0.1f;			//	線形保管の速度
const float AngleHelper::LERP_RETURN_SPEED = 0.5f;	//	線形保管の戻る速度
const float AngleHelper::ANGLE_NONE = 0.0f;			//	角度の初期化
const float AngleHelper::MAX_ANGLE = 0.2f;			//	垂直方向の角度の最大
const float AngleHelper::MIN_ANGLE = -0.2f;			//	垂直方向の角度の最大

//-----------------------------------------------------------------------------
//	@brief	角度を求める
//-----------------------------------------------------------------------------
void AngleHelper::AskAngle(VECTOR& _angle, Pad& _pad)
{
	//	目的の角度を求める
	_angle = _GetTargetAngle(_angle, _pad);
}

//-----------------------------------------------------------------------------
//	@brief	角度を求める
//-----------------------------------------------------------------------------
void AngleHelper::AskAngle(VECTOR& _angle, VECTOR _dir)
{
	//	目的の角度を求める
	_angle = _GetTargetAngle(_angle, _dir);
}

//-----------------------------------------------------------------------------
//	@brief	目的の角度を求める
//-----------------------------------------------------------------------------
VECTOR AngleHelper::_GetTargetAngle(VECTOR& _angle, Pad& _pad)
{
	VECTOR targetAngle = _angle;

	//	入力による角度の取得
	const bool isUp = _pad.GetXInputStickToDpad(PadInfo::PAD_STICK_TO_DPAD_KIND::LEFT_STICK_TO_DPAD_UP, PadInfo::STICK_DEAD_ZONE) || KEY->Press(KEY_INPUT_W) || KEY->Press(KEY_INPUT_UP);
	const bool isDown = _pad.GetXInputStickToDpad(PadInfo::PAD_STICK_TO_DPAD_KIND::LEFT_STICK_TO_DPAD_DOWN, PadInfo::STICK_DEAD_ZONE) || KEY->Press(KEY_INPUT_S) || KEY->Press(KEY_INPUT_DOWN);

	//	入力された方向に角度の変更
	_ChangeTargetAngleByInput(isUp, targetAngle.z, ADD_ANGLE_SPEED);	//	上向き
	_ChangeTargetAngleByInput(isDown, targetAngle.z, SUB_ANGLE_SPEED);	//	下向き

	//	最大最小の設定
	_SetMaxToMin(targetAngle.z, MAX_ANGLE, MIN_ANGLE);

	//	入力がないときの初期化
	const bool isNotVertical = !isUp && !isDown;			//	垂直方向の入力なし

	//	入力がなければ、線形保管を利用し0に戻す
	_ChangeTargetAngleNoInput(isNotVertical, targetAngle.z);		//	垂直方向の入力なし

		//	線形保管を利用し角度を求める
	VECTOR angle = _angle;
	angle = CommonFunction::Lerp(angle, targetAngle, LERP_SPEED);

	//	角度を返す
	return angle;
}

//-----------------------------------------------------------------------------
//	@brief	目的の角度を求める
//-----------------------------------------------------------------------------
VECTOR AngleHelper::_GetTargetAngle(VECTOR& _angle, VECTOR _dir)
{
	VECTOR targetAngle = _angle;

	//	向きによる角度の取得
	const bool isUp = _dir.y > 0.0f;
	const bool isDown = _dir.y < 0.0f;

	//	入力された方向に角度の変更
	_ChangeTargetAngleByInput(isUp, targetAngle.z, ADD_ANGLE_SPEED);	//	上向き
	_ChangeTargetAngleByInput(isDown, targetAngle.z, SUB_ANGLE_SPEED);	//	下向き

	//	最大最小の設定
	_SetMaxToMin(targetAngle.z, MAX_ANGLE, MIN_ANGLE);

	//	入力がないときの初期化
	const bool isNotVertical = !isUp && !isDown;			//	垂直方向の入力なし

	//	入力がなければ、線形保管を利用し0に戻す
	_ChangeTargetAngleNoInput(isNotVertical, targetAngle.z);		//	垂直方向の入力なし

		//	線形保管を利用し角度を求める
	VECTOR angle = _angle;
	angle = CommonFunction::Lerp(angle, targetAngle, LERP_SPEED);

	//	角度を返す
	return angle;
}

//-----------------------------------------------------------------------------
//	@brief	入力による角度の変更
//-----------------------------------------------------------------------------
void AngleHelper::_ChangeTargetAngleByInput(const bool _isState, float& _angle, const float _speed)
{
	if(_isState) { _angle += _speed; }
}

//-----------------------------------------------------------------------------
//	@brief	最大最小の設定
//-----------------------------------------------------------------------------
void AngleHelper::_SetMaxToMin(float& _angle, const float _maxAngle, const float _minAngle)
{
	if (_angle >= _maxAngle) { _angle = _maxAngle; }		//	最大角度
	if (_angle <= _minAngle) { _angle = _minAngle; }		//	最小角度
}

//-----------------------------------------------------------------------------
//	@brief	入力がない時の対応
//-----------------------------------------------------------------------------
void AngleHelper::_ChangeTargetAngleNoInput(const bool _isState, float& _angle)
{
	if (_isState) { _angle = CommonFunction::Lerp(_angle, ANGLE_NONE, LERP_RETURN_SPEED); }
}
