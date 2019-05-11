
//=============================================================================
//	@file	Pad.h
//	@brief	パッド
//	@autor	Takuya Ochi
//	@date	2018/9/28
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "Pad.h"

//-----------------------------------------------------------------------------
//	@brief	名前空間	[パッドの]情報
//-----------------------------------------------------------------------------
namespace PadInfo
{
	//-----------------------------------------------------------------------------
	//	@brief	パッドの振動機能のカバー関数
	//-----------------------------------------------------------------------------
	namespace Function
	{
		//-----------------------------------------------------------------------------
		//	@brief	振動機能
		//			振動の強さ( 0～1000 )
		//-----------------------------------------------------------------------------
		void VibrationFunction(const PAD_KIND _padKind, const int _vibrationPower, const int _vibrationTime)
		{
			//	パッドの振動機能
			switch (_padKind)
			{
				//	パッド１
			case PadInfo::PAD_KIND::PAD_1:
				StartJoypadVibration(DX_INPUT_PAD1, _vibrationPower, _vibrationTime);
				break;

				//	パッド２
			case PadInfo::PAD_KIND::PAD_2:
				StartJoypadVibration(DX_INPUT_PAD2, _vibrationPower, _vibrationTime);
				break;

				//	パッド３
			case PadInfo::PAD_KIND::PAD_3:
				StartJoypadVibration(DX_INPUT_PAD3, _vibrationPower, _vibrationTime);
				break;

				//	パッド４
			case PadInfo::PAD_KIND::PAD_4:
				StartJoypadVibration(DX_INPUT_PAD4, _vibrationPower, _vibrationTime);
				break;
			}
		}
		
		//-----------------------------------------------------------------------------
		//	@brief	振動機能
		//			振動の強さ : 最大
		//			永久ループ
		//-----------------------------------------------------------------------------
		void VibrationFunction(const PadInfo::PAD_KIND _padKind)
		{
			//	パッドの振動機能
			switch (_padKind)
			{
				//	パッド１
			case PadInfo::PAD_KIND::PAD_1:
				StartJoypadVibration(DX_INPUT_PAD1, PAD_VIBRATION_POWER, PAD_VIBRATION_LOOP);
				break;

				//	パッド２
			case PadInfo::PAD_KIND::PAD_2:
				StartJoypadVibration(DX_INPUT_PAD2, PAD_VIBRATION_POWER, PAD_VIBRATION_LOOP);
				break;

				//	パッド３
			case PadInfo::PAD_KIND::PAD_3:
				StartJoypadVibration(DX_INPUT_PAD3, PAD_VIBRATION_POWER, PAD_VIBRATION_LOOP);
				break;

				//	パッド４
			case PadInfo::PAD_KIND::PAD_4:
				StartJoypadVibration(DX_INPUT_PAD4, PAD_VIBRATION_POWER, PAD_VIBRATION_LOOP);
				break;
			}
		}

		//-----------------------------------------------------------------------------
		//	@brief	振動機能
		//			振動の強さ( 0～1000 )
		//			永久ループ
		//-----------------------------------------------------------------------------
		void VibrationFunction(const PAD_KIND _padKind, const int _vibrationPower)
		{
			//	パッドの振動機能
			switch (_padKind)
			{
				//	パッド１
			case PadInfo::PAD_KIND::PAD_1:
				StartJoypadVibration(DX_INPUT_PAD1, _vibrationPower, PAD_VIBRATION_LOOP);
				break;

				//	パッド２
			case PadInfo::PAD_KIND::PAD_2:
				StartJoypadVibration(DX_INPUT_PAD2, _vibrationPower, PAD_VIBRATION_LOOP);
				break;

				//	パッド３
			case PadInfo::PAD_KIND::PAD_3:
				StartJoypadVibration(DX_INPUT_PAD3, _vibrationPower, PAD_VIBRATION_LOOP);
				break;

				//	パッド４
			case PadInfo::PAD_KIND::PAD_4:
				StartJoypadVibration(DX_INPUT_PAD4, _vibrationPower, PAD_VIBRATION_LOOP);
				break;
			}
		}

		//-----------------------------------------------------------------------------
		//	@brief	振動機能の終了
		//-----------------------------------------------------------------------------
		void EndVibrationFunction(const PAD_KIND _padKind)
		{
			//	振動機能のループを終了する
			switch (_padKind)
			{
				//	パッド１
			case PadInfo::PAD_KIND::PAD_1:
				StopJoypadVibration(DX_INPUT_PAD1);
				break;

				//	パッド２
			case PadInfo::PAD_KIND::PAD_2:
				StopJoypadVibration(DX_INPUT_PAD2);
				break;

				//	パッド３
			case PadInfo::PAD_KIND::PAD_3:
				StopJoypadVibration(DX_INPUT_PAD3);
				break;

				//	パッド４
			case PadInfo::PAD_KIND::PAD_4:
				StopJoypadVibration(DX_INPUT_PAD4);
				break;
			}
		}
	}
}



//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
Pad::Pad()
{
	for (int i = 0; i < PadInfo::PAD_BUTTON_KIND::BUTTON_ALL; i++)
	{
		m_nowButton[i] = 0;
		m_prevButton[i] = 0;
	}
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
Pad::~Pad()
{
	// 処理なし
}

//-----------------------------------------------------------------------------
//	@brief	すべて使用中にする
//			シーン切り替え時のときのみ呼ぶ
//-----------------------------------------------------------------------------
void Pad::AllInUse()
{
	for (int i = 0; i < PadInfo::PAD_BUTTON_KIND::BUTTON_ALL; i++)
	{
		m_prevButton[i] = 1;
	}
}

//-----------------------------------------------------------------------------
//	@brief	パッドの登録
//-----------------------------------------------------------------------------
bool Pad::InputPadNum(const PadInfo::PAD_KIND _padKind)
{
	//	パッドの入力を取る
	switch (_padKind)
	{
		//	パッド１
	case PadInfo::PAD_KIND::PAD_1:
		GetJoypadXInputState(DX_INPUT_PAD1, &m_xInput);
		return true;

		//	パッド２
	case PadInfo::PAD_KIND::PAD_2:
		GetJoypadXInputState(DX_INPUT_PAD2, &m_xInput);
		return true;

		//	パッド３
	case PadInfo::PAD_KIND::PAD_3:
		GetJoypadXInputState(DX_INPUT_PAD3, &m_xInput);
		return true;

		//	パッド４
	case PadInfo::PAD_KIND::PAD_4:
		GetJoypadXInputState(DX_INPUT_PAD4, &m_xInput);
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
//	@brief	ボタンが押された瞬間
//-----------------------------------------------------------------------------
bool Pad::GetXInputPushButton(const PadInfo::PAD_BUTTON_KIND _buttonKind)
{
	switch (_buttonKind)
	{

	//	Aボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_A:
		if (_PushButton(XINPUT_BUTTON_A, PadInfo::PAD_BUTTON_KIND::BUTTON_A)) { return true; }
		return false;

	//	Bボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_B:
		if (_PushButton(XINPUT_BUTTON_B, PadInfo::PAD_BUTTON_KIND::BUTTON_B)) { return true; }
		return false;

	//	Xボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_X:
		if (_PushButton(XINPUT_BUTTON_X, PadInfo::PAD_BUTTON_KIND::BUTTON_X)) { return true; }
		return false;

	//	Yボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_Y:
		if (_PushButton(XINPUT_BUTTON_Y, PadInfo::PAD_BUTTON_KIND::BUTTON_Y)) { return true; }
		return false;

	//	LBボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_LB:
		if (_PushButton(XINPUT_BUTTON_LEFT_SHOULDER, PadInfo::PAD_BUTTON_KIND::BUTTON_LB)) { return true; }
		return false;

	//	RBボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_RB:
		if (_PushButton(XINPUT_BUTTON_RIGHT_SHOULDER, PadInfo::PAD_BUTTON_KIND::BUTTON_RB)) { return true; }
		return false;

	//	STARTボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_START:
		if (_PushButton(XINPUT_BUTTON_START, PadInfo::PAD_BUTTON_KIND::BUTTON_START)) { return true; }
		return false;

	//	BACKボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_BACK:
		if (_PushButton(XINPUT_BUTTON_BACK, PadInfo::PAD_BUTTON_KIND::BUTTON_BACK)) { return true; }
		return false;

	//	左スチック押し込みボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_LSB:
		if (_PushButton(XINPUT_BUTTON_LEFT_THUMB, PadInfo::PAD_BUTTON_KIND::BUTTON_LSB)) { return true; }
		return false;

	//	右スティック押し込みボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_RSB:
		if (_PushButton(XINPUT_BUTTON_RIGHT_THUMB, PadInfo::PAD_BUTTON_KIND::BUTTON_RSB)) { return true; }
		return false;

	//	すべて
	case PadInfo::PAD_BUTTON_KIND::BUTTON_ALL:
		if (_PushButton(XINPUT_BUTTON_A, PadInfo::PAD_BUTTON_KIND::BUTTON_A)) { return true; }				//	Aボタン
		if (_PushButton(XINPUT_BUTTON_B, PadInfo::PAD_BUTTON_KIND::BUTTON_B)) { return true; }				//	Bボタン
		if (_PushButton(XINPUT_BUTTON_X, PadInfo::PAD_BUTTON_KIND::BUTTON_X)) { return true; }				//	Xボタン
		if (_PushButton(XINPUT_BUTTON_Y, PadInfo::PAD_BUTTON_KIND::BUTTON_Y)) { return true; }				//	Yボタン
		if (_PushButton(XINPUT_BUTTON_LEFT_SHOULDER, PadInfo::PAD_BUTTON_KIND::BUTTON_LB)) { return true; }	//	LBボタン
		if (_PushButton(XINPUT_BUTTON_RIGHT_SHOULDER, PadInfo::PAD_BUTTON_KIND::BUTTON_RB)) { return true; }	//	RBボタン
		if (_PushButton(XINPUT_BUTTON_START, PadInfo::PAD_BUTTON_KIND::BUTTON_START)) { return true; }		//	STARTボタン
		if (_PushButton(XINPUT_BUTTON_BACK, PadInfo::PAD_BUTTON_KIND::BUTTON_BACK)) { return true; }			//	BACKボタン
		if (_PushButton(XINPUT_BUTTON_LEFT_THUMB, PadInfo::PAD_BUTTON_KIND::BUTTON_LSB)) { return true; }	//	左スチック押し込みボタン
		if (_PushButton(XINPUT_BUTTON_RIGHT_THUMB, PadInfo::PAD_BUTTON_KIND::BUTTON_RSB)) { return true; }	//	右スティック押し込みボタン
		return false;
	}

	return false;
}

//-----------------------------------------------------------------------------
//	@brief	ボタンが離された瞬間
//-----------------------------------------------------------------------------
bool Pad::GetXInputDisuniteButton(const PadInfo::PAD_BUTTON_KIND _buttonKind)
{
	switch (_buttonKind)
	{

		//	Aボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_A:
		if (_DisuniteButton(XINPUT_BUTTON_A, PadInfo::PAD_BUTTON_KIND::BUTTON_A)) { return true; }
		return false;

		//	Bボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_B:
		if (_DisuniteButton(XINPUT_BUTTON_B, PadInfo::PAD_BUTTON_KIND::BUTTON_B)) { return true; }
		return false;

		//	Xボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_X:
		if (_DisuniteButton(XINPUT_BUTTON_X, PadInfo::PAD_BUTTON_KIND::BUTTON_X)) { return true; }
		return false;

		//	Yボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_Y:
		if (_DisuniteButton(XINPUT_BUTTON_Y, PadInfo::PAD_BUTTON_KIND::BUTTON_Y)) { return true; }
		return false;

		//	LBボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_LB:
		if (_DisuniteButton(XINPUT_BUTTON_LEFT_SHOULDER, PadInfo::PAD_BUTTON_KIND::BUTTON_LB)) { return true; }
		return false;

		//	RBボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_RB:
		if (_DisuniteButton(XINPUT_BUTTON_RIGHT_SHOULDER, PadInfo::PAD_BUTTON_KIND::BUTTON_RB)) { return true; }
		return false;

		//	STARTボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_START:
		if (_DisuniteButton(XINPUT_BUTTON_START, PadInfo::PAD_BUTTON_KIND::BUTTON_START)) { return true; }
		return false;

		//	BACKボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_BACK:
		if (_DisuniteButton(XINPUT_BUTTON_BACK, PadInfo::PAD_BUTTON_KIND::BUTTON_BACK)) { return true; }
		return false;

		//	左スチック押し込みボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_LSB:
		if (_DisuniteButton(XINPUT_BUTTON_LEFT_THUMB, PadInfo::PAD_BUTTON_KIND::BUTTON_LSB)) { return true; }
		return false;

		//	右スティック押し込みボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_RSB:
		if (_DisuniteButton(XINPUT_BUTTON_RIGHT_THUMB, PadInfo::PAD_BUTTON_KIND::BUTTON_RSB)) { return true; }
		return false;

		//	すべて
	case PadInfo::PAD_BUTTON_KIND::BUTTON_ALL:
		if (_DisuniteButton(XINPUT_BUTTON_A, PadInfo::PAD_BUTTON_KIND::BUTTON_A)) { return true; }				//	Aボタン
		if (_DisuniteButton(XINPUT_BUTTON_B, PadInfo::PAD_BUTTON_KIND::BUTTON_B)) { return true; }				//	Bボタン
		if (_DisuniteButton(XINPUT_BUTTON_X, PadInfo::PAD_BUTTON_KIND::BUTTON_X)) { return true; }				//	Xボタン
		if (_DisuniteButton(XINPUT_BUTTON_Y, PadInfo::PAD_BUTTON_KIND::BUTTON_Y)) { return true; }				//	Yボタン
		if (_DisuniteButton(XINPUT_BUTTON_LEFT_SHOULDER, PadInfo::PAD_BUTTON_KIND::BUTTON_LB)) { return true; }	//	LBボタン
		if (_DisuniteButton(XINPUT_BUTTON_RIGHT_SHOULDER, PadInfo::PAD_BUTTON_KIND::BUTTON_RB)) { return true; }	//	RBボタン
		if (_DisuniteButton(XINPUT_BUTTON_START, PadInfo::PAD_BUTTON_KIND::BUTTON_START)) { return true; }		//	STARTボタン
		if (_DisuniteButton(XINPUT_BUTTON_BACK, PadInfo::PAD_BUTTON_KIND::BUTTON_BACK)) { return true; }			//	BACKボタン
		if (_DisuniteButton(XINPUT_BUTTON_LEFT_THUMB, PadInfo::PAD_BUTTON_KIND::BUTTON_LSB)) { return true; }	//	左スチック押し込みボタン
		if (_DisuniteButton(XINPUT_BUTTON_RIGHT_THUMB, PadInfo::PAD_BUTTON_KIND::BUTTON_RSB)) { return true; }	//	右スティック押し込みボタン
		return false;
	}

	return false;
}

//-----------------------------------------------------------------------------
//	@brief	ボタン入力
//-----------------------------------------------------------------------------
bool Pad::GetXInputButton(const PadInfo::PAD_BUTTON_KIND _buttonKind)
{
	switch (_buttonKind)
	{
		//	Aボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_A:
		if (m_xInput.Buttons[XINPUT_BUTTON_A]) { return true; }
		return false;

		//	Bボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_B:
		if (m_xInput.Buttons[XINPUT_BUTTON_B]) { return true; }
		return false;

		//	Xボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_X:
		if (m_xInput.Buttons[XINPUT_BUTTON_X]) { return true; }
		return false;

		//	Yボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_Y:
		if (m_xInput.Buttons[XINPUT_BUTTON_Y]) { return true; }
		return false;

		//	LBボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_LB:
		if (m_xInput.Buttons[XINPUT_BUTTON_LEFT_SHOULDER]) { return true; }
		return false;

		//	RBボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_RB:
		if (m_xInput.Buttons[XINPUT_BUTTON_RIGHT_SHOULDER]) { return true; }
		return false;

		//	STARTボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_START:
		if (m_xInput.Buttons[XINPUT_BUTTON_START]) { return true; }
		return false;

		//	BACKボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_BACK:
		if (m_xInput.Buttons[XINPUT_BUTTON_BACK]) { return true; }
		return false;

		//	左スチック押し込みボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_LSB:
		if (m_xInput.Buttons[XINPUT_BUTTON_LEFT_THUMB]) { return true; }
		return false;

		//	右スティック押し込みボタン
	case PadInfo::PAD_BUTTON_KIND::BUTTON_RSB:
		if (m_xInput.Buttons[XINPUT_BUTTON_RIGHT_THUMB]) { return true; }
		return false;

		//	すべて
	case PadInfo::PAD_BUTTON_KIND::BUTTON_ALL:
		if (m_xInput.Buttons[XINPUT_BUTTON_A]) { return true; }					//	Aボタン
		if (m_xInput.Buttons[XINPUT_BUTTON_B]) { return true; }					//	Bボタン
		if (m_xInput.Buttons[XINPUT_BUTTON_X]) { return true; }					//	Xボタン
		if (m_xInput.Buttons[XINPUT_BUTTON_Y]) { return true; }					//	Yボタン
		if (m_xInput.Buttons[XINPUT_BUTTON_LEFT_SHOULDER]) { return true; }		//	LBボタン
		if (m_xInput.Buttons[XINPUT_BUTTON_RIGHT_SHOULDER]) { return true; }	//	RBボタン
		if (m_xInput.Buttons[XINPUT_BUTTON_START]) { return true; }				//	STARTボタン
		if (m_xInput.Buttons[XINPUT_BUTTON_BACK]) { return true; }				//	BACKボタン
		if (m_xInput.Buttons[XINPUT_BUTTON_LEFT_THUMB]) { return true; }		//	左スチック押し込みボタン
		if (m_xInput.Buttons[XINPUT_BUTTON_RIGHT_THUMB]) { return true; }		//	右スティック押し込みボタン
		return false;
	}

	return false;
}

//-----------------------------------------------------------------------------
//	@brief	トリガー押した瞬間
//-----------------------------------------------------------------------------
bool Pad::GetXInputPushTrigger(const PadInfo::PAD_BUTTON_KIND _buttonKind, const short _deadZone)
{
	switch (_buttonKind)
	{
		//	左トリガー
	case PadInfo::PAD_BUTTON_KIND::BUTTON_LT:
		if (_PushTrigger(m_xInput.LeftTrigger >= _deadZone, PadInfo::PAD_BUTTON_KIND::BUTTON_LT)) { return true; }
		return false;

		//	右トリガー
	case PadInfo::PAD_BUTTON_KIND::BUTTON_RT:
		if (_PushTrigger(m_xInput.RightTrigger >= _deadZone, PadInfo::PAD_BUTTON_KIND::BUTTON_RT)) { return true; }
		return false;
	}

	return false;
}

//-----------------------------------------------------------------------------
//	@brief	トリガー離した瞬間
//-----------------------------------------------------------------------------
bool Pad::GetXInputDisuniteTrigger(const PadInfo::PAD_BUTTON_KIND _buttonKind, const short _deadZone)
{
	switch (_buttonKind)
	{
		//	左トリガー
	case PadInfo::PAD_BUTTON_KIND::BUTTON_LT:
		if (_DisuniteTrigger(m_xInput.LeftTrigger >= _deadZone, PadInfo::PAD_BUTTON_KIND::BUTTON_LT)) { return true; }
		return false;

		//	右トリガー
	case PadInfo::PAD_BUTTON_KIND::BUTTON_RT:
		if (_DisuniteTrigger(m_xInput.RightTrigger >= _deadZone, PadInfo::PAD_BUTTON_KIND::BUTTON_RT)) { return true; }
		return false;
	}

	return false;
}

//-----------------------------------------------------------------------------
//	@brief	トリガー入力
//-----------------------------------------------------------------------------
bool Pad::GetXInputTrigger(const PadInfo::PAD_BUTTON_KIND _buttonKind, const short _deadZone)
{
	switch (_buttonKind)
	{
		//	左トリガー
	case PadInfo::PAD_BUTTON_KIND::BUTTON_LT:
		if (m_xInput.LeftTrigger >= _deadZone) { return true; }
		return false;

		//	右トリガー
	case PadInfo::PAD_BUTTON_KIND::BUTTON_RT:
		if (m_xInput.RightTrigger >= _deadZone) { return true; }
		return false;
	}

	return false;
}

//-----------------------------------------------------------------------------
//	@brief	DPAD入力
//-----------------------------------------------------------------------------
bool Pad::GetXInputDpad(const PadInfo::PAD_DPAD_KIND _dpadKind)
{
	switch (_dpadKind)
	{
		//	十字キーの上方向
	case PadInfo::PAD_DPAD_KIND::DPAD_UP:
		if (m_xInput.Buttons[XINPUT_BUTTON_DPAD_UP]) { return true; }
		return false;

	//	十字キーの下方向
	case PadInfo::PAD_DPAD_KIND::DPAD_DOWN:
		if (m_xInput.Buttons[XINPUT_BUTTON_DPAD_DOWN]) { return true; }
		return false;

	//	十字キーの右方向
	case PadInfo::PAD_DPAD_KIND::DPAD_RIGHT:
		if (m_xInput.Buttons[XINPUT_BUTTON_DPAD_RIGHT]) { return true; }
		return false;

	//	十字キーの左方向
	case PadInfo::PAD_DPAD_KIND::DPAD_LEFT:
		if (m_xInput.Buttons[XINPUT_BUTTON_DPAD_LEFT]) { return true; }
		return false;
	}
	return false;
}

//-----------------------------------------------------------------------------
//	@brief	ステック入力
//-----------------------------------------------------------------------------
bool Pad::GetXInputStick(const PadInfo::PAD_STICK_KIND _stickKind, const short _deadZone)
{
	switch (_stickKind)
	{

		//	左スティック　上方向
	case PadInfo::PAD_STICK_KIND::LEFT_STICK_UP:
		if (m_xInput.ThumbLY >= _deadZone) { return true; }
		return false;

		//	左スティック　下方向
	case PadInfo::PAD_STICK_KIND::LEFT_STICK_DOWN:
		if (m_xInput.ThumbLY <= -_deadZone) { return true; }
		return false;

		//	左スティック　右方向
	case PadInfo::PAD_STICK_KIND::LEFT_STICK_RIGHT:
		if (m_xInput.ThumbLX >= _deadZone) { return true; }
		return false;

		//	左スティック　左方向
	case PadInfo::PAD_STICK_KIND::LEFT_STICK_LEFT:
		if (m_xInput.ThumbLX <= -_deadZone) { return true; }
		return false;

		//	右スティック　上方向
	case PadInfo::PAD_STICK_KIND::RIGHT_STICK_UP:
		if (m_xInput.ThumbRY >= _deadZone) { return true; }
		return false;

		//	右スティック　下方向
	case PadInfo::PAD_STICK_KIND::RIGHT_STICK_DOWN:
		if (m_xInput.ThumbRY <= -_deadZone) { return true; }
		return false;

		//	右スティック　右方向
	case PadInfo::PAD_STICK_KIND::RIGHT_STICK_RIGHT:
		if (m_xInput.ThumbRX >= _deadZone) { return true; }
		return false;

		//	右スティック　左方向
	case PadInfo::PAD_STICK_KIND::RIGHT_STICK_LEFT:
		if (m_xInput.ThumbRX <= -_deadZone) { return true; }
		return false;
	}

	return false;
}

//-----------------------------------------------------------------------------
//	@brief	ステック入力とDPAD入力
//-----------------------------------------------------------------------------
bool Pad::GetXInputStickToDpad(const PadInfo::PAD_STICK_TO_DPAD_KIND _stickToDpadKind, const short _deadZone)
{
	switch (_stickToDpadKind)
	{
		//	左スティックと十字キーの上方向
	case PadInfo::PAD_STICK_TO_DPAD_KIND::LEFT_STICK_TO_DPAD_UP:
	{
		const bool isActive = m_xInput.Buttons[XINPUT_BUTTON_DPAD_UP] || m_xInput.ThumbLY >= _deadZone;
		if (isActive) { return true; }
		return false;
	}

	//	左スティックと十字キーの下方向
	case PadInfo::PAD_STICK_TO_DPAD_KIND::LEFT_STICK_TO_DPAD_DOWN:
	{
		const bool isActive = m_xInput.Buttons[XINPUT_BUTTON_DPAD_DOWN] || m_xInput.ThumbLY <= -_deadZone;
		if (isActive) { return true; }
		return false;
	}

	//	左スティックと十字キーの右方向
	case PadInfo::PAD_STICK_TO_DPAD_KIND::LEFT_STICK_TO_DPAD_RIGHT:
	{
		const bool isActive = m_xInput.Buttons[XINPUT_BUTTON_DPAD_RIGHT] || m_xInput.ThumbLX >= _deadZone;
		if (isActive) { return true; }
		return false;
	}

	//	左スティックと十字キーの左方向
	case PadInfo::PAD_STICK_TO_DPAD_KIND::LEFT_STICK_TO_DPAD_LEFT:
	{
		const bool isActive = m_xInput.Buttons[XINPUT_BUTTON_DPAD_LEFT] || m_xInput.ThumbLX <= -_deadZone;
		if (isActive) { return true; }
		return false;
	}

	//	右スティックと十字キーの上方向
	case PadInfo::PAD_STICK_TO_DPAD_KIND::RIGHT_STICK_TO_DPAD_UP:
	{
		const bool isActive = m_xInput.Buttons[XINPUT_BUTTON_DPAD_UP] || m_xInput.ThumbRY >= _deadZone;
		if (isActive) { return true; }
		return false;
	}

	//	右スティックと十字キーの下方向
	case PadInfo::PAD_STICK_TO_DPAD_KIND::RIGHT_STICK_TO_DPAD_DOWN:
	{
		const bool isActive = m_xInput.Buttons[XINPUT_BUTTON_DPAD_DOWN] || m_xInput.ThumbRY <= -_deadZone;
		if (isActive) { return true; }
		return false;
	}

	//	右スティックと十字キーの右方向
	case PadInfo::PAD_STICK_TO_DPAD_KIND::RIGHT_STICK_TO_DPAD_RIGHT:
	{
		const bool isActive = m_xInput.Buttons[XINPUT_BUTTON_DPAD_RIGHT] || m_xInput.ThumbRX >= _deadZone;
		if (isActive) { return true; }
		return false;
	}

	//	右スティックと十字キーの左方向
	case PadInfo::PAD_STICK_TO_DPAD_KIND::RIGHT_STICK_TO_DPAD_LEFT:
	{
		const bool isActive = m_xInput.Buttons[XINPUT_BUTTON_DPAD_LEFT] || m_xInput.ThumbRX <= -_deadZone;
		if (isActive) { return true; }
		return false;
	}

	}

	return false;
}

//-----------------------------------------------------------------------------
//	@brief	ボタンを押した瞬間
//-----------------------------------------------------------------------------
bool Pad::_PushButton(const int _button, const int _buttonNum)
{
	if (m_xInput.Buttons[_button])
	{
		if (m_prevButton[_buttonNum] == 0)
		{
			//	押した
			m_prevButton[_buttonNum] = 1;
			return true;
		}
		return false;
	}
	else
	{
		//	離した
		m_prevButton[_buttonNum] = 0;
		return false;
	}
}

//-----------------------------------------------------------------------------
//	@brief	ボタンを離した瞬間
//-----------------------------------------------------------------------------
bool Pad::_DisuniteButton(const int _button, const int _buttonNum)
{
	if (m_xInput.Buttons[_button])
	{
		if (m_prevButton[_buttonNum] == 0)
		{
			//	押した
			m_nowButton[_buttonNum] = 1;
		}
	}
	else
	{
		if (m_prevButton[_buttonNum] == 1)
		{
			//	離した
			m_nowButton[_buttonNum] = 0;
			m_prevButton[_buttonNum] = m_nowButton[_buttonNum];
			return true;
		}
	}

	m_prevButton[_buttonNum] = m_nowButton[_buttonNum];
	return false;
}

//-----------------------------------------------------------------------------
//	@brief	ボタンを押した瞬間
//-----------------------------------------------------------------------------
bool Pad::_PushTrigger(const bool _trigger, const int _buttonNum)
{
	if (_trigger)
	{
		if (m_prevButton[_buttonNum] == 0)
		{
			//	押した
			m_prevButton[_buttonNum] = 1;
			return true;
		}
		return false;
	}
	else
	{
		//	離した
		m_prevButton[_buttonNum] = 0;
		return false;
	}
}

//-----------------------------------------------------------------------------
//	@brief	ボタンを離した瞬間
//-----------------------------------------------------------------------------
bool Pad::_DisuniteTrigger(const bool _trigger, const int _buttonNum)
{
	if (_trigger)
	{
		if (m_prevButton[_buttonNum] == 0)
		{
			//	押した
			m_nowButton[_buttonNum] = 1;
		}
	}
	else
	{
		if (m_prevButton[_buttonNum] == 1)
		{
			//	離した
			m_nowButton[_buttonNum] = 0;
			m_prevButton[_buttonNum] = m_nowButton[_buttonNum];
			return true;
		}
	}

	m_prevButton[_buttonNum] = m_nowButton[_buttonNum];
	return false;
}
