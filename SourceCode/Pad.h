
//=============================================================================
//	@file	Pad.h
//	@brief	パッド入力
//	@autor	Takuya Ochi
//	@date	2018/9/2
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
//	@brief	名前空間	[パッドの種類]
//-----------------------------------------------------------------------------
namespace PadInfo
{

//==================== 列挙体 =====================//

	//	パッドの種類
	enum PAD_KIND
	{
		PAD_1,	//	パッド１
		PAD_2,	//	パッド２
		PAD_3,	//	パッド３
		PAD_4,	//	パッド４
	};

	//	ボタンの種類
	enum PAD_BUTTON_KIND
	{
		BUTTON_A,			//	Aボタン
		BUTTON_B,			//	Bボタン
		BUTTON_X,			//	Xボタン
		BUTTON_Y,			//	Yボタン
		BUTTON_LB,			//	Lボタン
		BUTTON_RB,			//	Rボタン
		BUTTON_START,		//	STARTボタン
		BUTTON_BACK,		//	BUCKボタン
		BUTTON_LSB,			//	Lスティック押し込みボタン
		BUTTON_RSB,			//	Rスティック押し込みボタン
		BUTTON_LT,			//	Lトリガー
		BUTTON_RT,			//	Rトリガー
		BUTTON_ALL,			//	すべてのボタン
	};

	//	DPADの種類
	enum PAD_DPAD_KIND
	{
		DPAD_UP,		//	DPAD　上方向
		DPAD_DOWN,		//	DPAD　下方向
		DPAD_RIGHT,		//	DPAD　右方向
		DPAD_LEFT,		//	DPAD　左方向
	};

	//	スティックの種類
	enum PAD_STICK_KIND
	{
		LEFT_STICK_UP,		//	左スティックの上方向
		LEFT_STICK_DOWN,	//	左スティックの下方向
		LEFT_STICK_RIGHT,	//	左スティックの右方向
		LEFT_STICK_LEFT,	//	左スティックの左方向
		RIGHT_STICK_UP,		//	右スティックの上方向
		RIGHT_STICK_DOWN,	//	右スティックの下方向
		RIGHT_STICK_RIGHT,	//	右スティックの右方向
		RIGHT_STICK_LEFT,	//	右スティックの左方向
	};

	//	スティックとDPADの種類
	enum PAD_STICK_TO_DPAD_KIND
	{
		LEFT_STICK_TO_DPAD_UP,		//	左スティックとDPADの上方向
		LEFT_STICK_TO_DPAD_DOWN,	//	左スティックとDPADの下方向
		LEFT_STICK_TO_DPAD_RIGHT,	//	左スティックとDPADの右方向
		LEFT_STICK_TO_DPAD_LEFT,	//	左スティックとDPADの左方向
		RIGHT_STICK_TO_DPAD_UP,		//	右スティックとDPADの上方向
		RIGHT_STICK_TO_DPAD_DOWN,	//	右スティックとDPADの下方向
		RIGHT_STICK_TO_DPAD_RIGHT,	//	右スティックとDPADの右方向
		RIGHT_STICK_TO_DPAD_LEFT,	//	右スティックとDPADの左方向
	};

//===================== 静的定数 ===================//

	static const int TRIGGER_DEAD_ZONE = 128;		//	トリガー入力のデッドゾーン
	static const int STICK_DEAD_ZONE = 10000;		//	スティック入力のデッドゾーン
	static const int PAD_VIBRATION_POWER = 1000;	//	パッドの振動機能 強さ
	static const int PAD_VIBRATION_LOOP = -1;		//	パッドの振動機能 ループ

//===================== 関数 ======================//

	//-----------------------------------------------------------------------------
	//	@brief	名前空間	[関数]
	//-----------------------------------------------------------------------------
	namespace Function
	{
		//	振動機能
		//	振動の強さ( 0～1000 )
		void VibrationFunction(const PadInfo::PAD_KIND _padKind, const int _vibrationPower, const int _vibrationTime);

		//	振動機能
		//	振動の強さ : 最大
		//	永久ループ
		void VibrationFunction(const PadInfo::PAD_KIND _padKind);

		//	振動機能
		//	永久ループ
		void VibrationFunction(const PadInfo::PAD_KIND _padKind, const int _vibrationPower);

		//	振動機能の終了
		void EndVibrationFunction(const PadInfo::PAD_KIND _padKind);
	}
}

//-----------------------------------------------------------------------------
//	@brief	パッド入力クラス
//-----------------------------------------------------------------------------
class Pad final
{
public:

//===================== 関数 ======================//

	//	コンストラクタ / デストラクタ
	Pad();
	~Pad();

	//	すべて使用中にする
	//	シーン切り替え時のときのみ呼ぶ
	void AllInUse();

	// パッドの入力を取る
	bool InputPadNum(const PadInfo::PAD_KIND _padKind);

	//	ボタンを押した瞬間
	bool GetXInputPushButton(const PadInfo::PAD_BUTTON_KIND _buttonKind);

	//	ボタンを離した瞬間
	bool GetXInputDisuniteButton(const PadInfo::PAD_BUTTON_KIND _buttonKind);

	//	ボタンの押している間
	bool GetXInputButton(const PadInfo::PAD_BUTTON_KIND _buttonKind);

	//	トリガーを押した瞬間
	bool GetXInputPushTrigger(const PadInfo::PAD_BUTTON_KIND _buttonKind, const short _deadZone);

	//	トリガーを離した瞬間
	bool GetXInputDisuniteTrigger(const PadInfo::PAD_BUTTON_KIND _buttonKind, const short _deadZone);

	//	トリガーの押している間
	bool GetXInputTrigger(const PadInfo::PAD_BUTTON_KIND _buttonKind, const short _deadZone);

	//	DPADの入力
	bool GetXInputDpad(const PadInfo::PAD_DPAD_KIND _dpadKind);

	//	スティックの入力
	bool GetXInputStick(const PadInfo::PAD_STICK_KIND _stickKind, const short _deadZone);

	//	ステックとDPADの入力
	bool GetXInputStickToDpad(const PadInfo::PAD_STICK_TO_DPAD_KIND _stickToDpadKind, const short _deadZone);

private:

//================== 内部処理関数 ==================//

	//	ボタンの押した瞬間
	bool _PushButton(const int _button, const int _buttonNum);

	//	ボタンの離した瞬間
	bool _DisuniteButton(const int _button, const int _buttonNum);

	//	トリガーの押した瞬間
	bool _PushTrigger(const bool _trigger, const int _buttonNum);

	//	トリガーの離した瞬間
	bool _DisuniteTrigger(const bool _trigger, const int _buttonNum);

//=================== メンバ変数 ===================//

	XINPUT_STATE	m_xInput;												//	xInput
	int				m_nowButton[PadInfo::PAD_BUTTON_KIND::BUTTON_ALL];		//	今フレームのボタンの入力
	int				m_prevButton[PadInfo::PAD_BUTTON_KIND::BUTTON_ALL];		//	前フレームのボタンの入力
};

