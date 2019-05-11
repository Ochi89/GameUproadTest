
//=============================================================================
//	@file	Moving.cpp
//	@brief	移動処理
//	@autor	Takuya Ochi
//	@date	2018/11/15
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "MoveHelper.h"
#include "Common.h"
#include "Pad.h"
#include "Key.h"

//-----------------------------------------------------------------------------
//	@brief	移動量を求める
//-----------------------------------------------------------------------------
VECTOR MoveHelper::AskMoveAmount(VECTOR& _dir, Pad& _pad, const float _moveSpeed, const bool _isInputKey, const bool _isUseAxisXY)
{
	if (_isUseAxisXY) { return _MoveAxisXY(_dir, _pad, _moveSpeed, _isInputKey); }		//	XY軸での移動
	else { return _MoveAxisXZ(_dir, _pad, _moveSpeed, _isInputKey); }					//	XZ軸での移動
}

//-----------------------------------------------------------------------------
//	@brief	移動量を求める
//-----------------------------------------------------------------------------
VECTOR MoveHelper::AskMoveAmount(VECTOR& _dir, const float _moveSpeed)
{
	return _AlwaysMove(_dir, _moveSpeed);
}

//-----------------------------------------------------------------------------
//	@brief	リセット
//-----------------------------------------------------------------------------
void MoveHelper::_ResetDir(VECTOR& _dir)
{
	_dir = CommonConstant::ORIGIN;
}

//-----------------------------------------------------------------------------
//	@brief	XZ軸での移動処理
//-----------------------------------------------------------------------------
VECTOR MoveHelper::_MoveAxisXZ(VECTOR& _dir, Pad& _pad, const float _moveSpeed, const bool _isInputKey)
{
	//	向きのリセット
	_ResetDir(_dir);

	if (_isInputKey)
	{
		//	ステックとキー入力による向きの設定
		_SetInputDirKeyToPad(_dir.z, _dir.x, _pad);
	}
	else
	{
		//	ステック入力による向きの設定
		_SetInputDirPad(_dir.z, _dir.x, _pad);
	}

	//	移動処理
	return _Move(_dir, _moveSpeed);
}

//-----------------------------------------------------------------------------
//	@brief	XY軸での移動処理
//-----------------------------------------------------------------------------
VECTOR MoveHelper::_MoveAxisXY(VECTOR& _dir, Pad& _pad, const float _moveSpeed, const bool _isInputKey)
{
	//	向きのリセット
	_ResetDir(_dir);

	if (_isInputKey)
	{
		//	ステックとキー入力による向きの設定
		_SetInputDirKeyToPad(_dir.y, _dir.x, _pad);
	}
	else
	{
		//	ステック入力による向きの設定
		_SetInputDirPad(_dir.y, _dir.x, _pad);
	}

	//	移動処理
	return _Move(_dir, _moveSpeed);
}

//-----------------------------------------------------------------------------
//	@brief	XY軸での移動処理
//-----------------------------------------------------------------------------
VECTOR MoveHelper::_AlwaysMove(VECTOR& _dir, const float _moveSpeed)
{
	//	移動処理
	return _Move(_dir, _moveSpeed);
}

//-----------------------------------------------------------------------------
//	@brief	入力時の向きを設定する
//-----------------------------------------------------------------------------
void MoveHelper::_SetInputDirKeyToPad(float& _verticalAxis, float& _horizontalAxis, Pad& _pad)
{
	//	キー入力
	const bool isUpKey = KEY->Press(KEY_INPUT_W) || KEY->Press(KEY_INPUT_UP);
	const bool isDownKey = KEY->Press(KEY_INPUT_S) || KEY->Press(KEY_INPUT_DOWN);
	const bool isRightKey = KEY->Press(KEY_INPUT_D) || KEY->Press(KEY_INPUT_RIGHT);
	const bool isLeftKey = KEY->Press(KEY_INPUT_A) || KEY->Press(KEY_INPUT_LEFT);

	//	パッド入力
	const bool isUpStick = _pad.GetXInputStickToDpad(PadInfo::PAD_STICK_TO_DPAD_KIND::LEFT_STICK_TO_DPAD_UP, PadInfo::STICK_DEAD_ZONE);
	const bool isDownStick = _pad.GetXInputStickToDpad(PadInfo::PAD_STICK_TO_DPAD_KIND::LEFT_STICK_TO_DPAD_DOWN, PadInfo::STICK_DEAD_ZONE);
	const bool isRightStick = _pad.GetXInputStickToDpad(PadInfo::PAD_STICK_TO_DPAD_KIND::LEFT_STICK_TO_DPAD_RIGHT, PadInfo::STICK_DEAD_ZONE);
	const bool isLeftStick = _pad.GetXInputStickToDpad(PadInfo::PAD_STICK_TO_DPAD_KIND::LEFT_STICK_TO_DPAD_LEFT, PadInfo::STICK_DEAD_ZONE);

	//	キーとパッドの入力
	const bool isUp = isUpStick || isUpKey;
	const bool isDown = isDownStick || isDownKey;
	const bool isRight = isRightStick || isRightKey;
	const bool isLeft = isLeftStick || isLeftKey;

	//	入力があれば向きを取得
	if (isUp) { _verticalAxis = 1; }
	if (isDown) { _verticalAxis = -1; }
	if (isRight) { _horizontalAxis = 1; }
	if (isLeft) { _horizontalAxis = -1; }
}

//-----------------------------------------------------------------------------
//	@brief	入力時の向きを設定する
//-----------------------------------------------------------------------------
void MoveHelper::_SetInputDirPad(float& _verticalAxis, float& _horizontalAxis, Pad& _pad)
{
	//	パッド入力
	const bool isUpStick = _pad.GetXInputStickToDpad(PadInfo::PAD_STICK_TO_DPAD_KIND::LEFT_STICK_TO_DPAD_UP, PadInfo::STICK_DEAD_ZONE);
	const bool isDownStick = _pad.GetXInputStickToDpad(PadInfo::PAD_STICK_TO_DPAD_KIND::LEFT_STICK_TO_DPAD_DOWN, PadInfo::STICK_DEAD_ZONE);
	const bool isRightStick = _pad.GetXInputStickToDpad(PadInfo::PAD_STICK_TO_DPAD_KIND::LEFT_STICK_TO_DPAD_RIGHT, PadInfo::STICK_DEAD_ZONE);
	const bool isLeftStick = _pad.GetXInputStickToDpad(PadInfo::PAD_STICK_TO_DPAD_KIND::LEFT_STICK_TO_DPAD_LEFT, PadInfo::STICK_DEAD_ZONE);

	//	入力があれば向きを取得
	if (isUpStick) { _verticalAxis = 1; }
	if (isDownStick) { _verticalAxis = -1; }
	if (isRightStick) { _horizontalAxis = 1; }
	if (isLeftStick) { _horizontalAxis = -1; }
}

//-----------------------------------------------------------------------------
//	@brief	移動処理
//-----------------------------------------------------------------------------
VECTOR MoveHelper::_Move(VECTOR& _dir, const float _moveSpeed)
{
	//	移動量の計測
	VECTOR velocity = VScale(_dir, _moveSpeed);

	//	正規化
	if (VSquareSize(_dir) != 0) { _dir = VNorm(_dir); }

	//	移動量を返す
	return velocity;
}
