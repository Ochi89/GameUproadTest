
//=============================================================================
//	@file	Moving.h
//	@brief	移動処理
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
//	@brief	移動処理クラス
//-----------------------------------------------------------------------------
class MoveHelper final
{
public:

//===================== 関数 ======================//

	//	移動量を求める
	static VECTOR AskMoveAmount(VECTOR& _dir, Pad& _pad, const float _moveSpeed, const bool _isInputKey, const bool _isUseAxisXY);

	//	移動量を求める
	static VECTOR AskMoveAmount(VECTOR& _dir, const float _moveSpeed);

private:

//================== 内部処理関数 ==================//

	//	リセット
	static void _ResetDir(VECTOR& _dir);

	//	3D時の移動処理
	static VECTOR _MoveAxisXZ(VECTOR& _dir, Pad& _pad, const float _moveSpeed, const bool _isInputKey);

	//	2D時の移動処理
	static VECTOR _MoveAxisXY(VECTOR& _dir, Pad& _pad, const float _moveSpeed, const bool _isInputKey);

	//	常に移動処理
	static VECTOR _AlwaysMove(VECTOR& _dir, const float _moveSpeed);

	//	移動処理
	static VECTOR _Move(VECTOR& _dir, const float _moveSpeed);

	//	入力時の向きを設定する
	static void _SetInputDirKeyToPad(float& _verticalAxis, float& _horizontalAxis, Pad& _pad);

	//	入力時の向きを設定する
	static void _SetInputDirPad(float& _verticalAxis, float& _horizontalAxis, Pad& _pad);

};