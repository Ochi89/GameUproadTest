
//=============================================================================
//	@file	Common.cpp
//	@brief	共通
//	@autor	Takuya Ochi
//	@date	2018/9/28
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "Common.h"
#include "Pad.h"
#include "Camera.h"
#include <math.h>
#include <cstdlib>
#include <cmath>

//-----------------------------------------------------------------------------
//	@brief	名前空間	[関数]
//-----------------------------------------------------------------------------
namespace CommonFunction
{

	//-----------------------------------------------------------------------------
	//	@brief	ラシアンを度に変換する
	//-----------------------------------------------------------------------------
	float RadToDeg(float _rad) { return _rad * (180 / CommonConstant::PI); }
	VECTOR RadToDeg(VECTOR _rad)
	{
		VECTOR tmpVec;
		tmpVec.x = RadToDeg(_rad.x);
		tmpVec.y = RadToDeg(_rad.y);
		tmpVec.z = RadToDeg(_rad.z);
		return tmpVec;
	}

	//-----------------------------------------------------------------------------
	//	@brief	度をラジアンに変換する
	//-----------------------------------------------------------------------------
	float DegToRad(float _deg) { return _deg * (CommonConstant::PI / 180); }
	VECTOR DegToRad(VECTOR _rad)
	{
		VECTOR tmpVec;
		tmpVec.x = DegToRad(_rad.x);
		tmpVec.y = DegToRad(_rad.y);
		tmpVec.z = DegToRad(_rad.z);
		return tmpVec;
	}

	//-----------------------------------------------------------------------------
	//	@brief	グリッドの作成
	//-----------------------------------------------------------------------------
	void CreateGrid()
	{
		//	位置関係が分かるように地面にラインを描画する
		VECTOR Pos1;
		VECTOR Pos2;

		SetUseZBufferFlag(TRUE);

		//	x軸
		Pos1 = VGet(-CommonConstant::LINE_AREA_SIZE / 2.0f, 0.0f, -CommonConstant::LINE_AREA_SIZE / 2.0f);
		Pos2 = VGet(-CommonConstant::LINE_AREA_SIZE / 2.0f, 0.0f, CommonConstant::LINE_AREA_SIZE / 2.0f);
		for (int i = 0; i <= CommonConstant::LINE_NUM; i++)
		{
			DrawLine3D(Pos1, Pos2, DxLib::GetColor(255, 255, 255));
			Pos1.x += CommonConstant::LINE_AREA_SIZE / CommonConstant::LINE_NUM;
			Pos2.x += CommonConstant::LINE_AREA_SIZE / CommonConstant::LINE_NUM;
		}

		//	z軸
		Pos1 = VGet(-CommonConstant::LINE_AREA_SIZE / 2.0f, 0.0f, -CommonConstant::LINE_AREA_SIZE / 2.0f);
		Pos2 = VGet(CommonConstant::LINE_AREA_SIZE / 2.0f, 0.0f, -CommonConstant::LINE_AREA_SIZE / 2.0f);
		for (int i = 0; i < CommonConstant::LINE_NUM; i++)
		{
			DrawLine3D(Pos1, Pos2, DxLib::GetColor(255, 255, 255));
			Pos1.z += CommonConstant::LINE_AREA_SIZE / CommonConstant::LINE_NUM;
			Pos2.z += CommonConstant::LINE_AREA_SIZE / CommonConstant::LINE_NUM;
		}

		SetUseZBufferFlag(FALSE);
	}

	//-----------------------------------------------------------------------------
	//	@brief	指定範囲の乱数を取得（整数）
	//-----------------------------------------------------------------------------
	int GetRand(const int _min, const int _max)
	{
		//	大小が逆のとき
		if (_min > _max)
		{
			int tmpMin = _max;
			int tmpMax = _min;

			std::random_device randDevice;								//	非決定的な乱数生成器を生成
			std::mt19937 mt(randDevice());								//	引数は初期シード値
			std::uniform_int_distribution<int> rand(tmpMin, tmpMax);	//	[tmpMin, tmpMax] 範囲の一様乱数

			return rand(mt);
		}

		std::random_device randDevice;								//	非決定的な乱数生成器を生成
		std::mt19937 mt(randDevice());								//	引数は初期シード値
		std::uniform_int_distribution<int> rand(_min, _max);		//	[_min, _max] 範囲の一様乱数

		return rand(mt);
	}

	//-----------------------------------------------------------------------------
	//	@brief	指定範囲の乱数を取得（浮動小数）
	//-----------------------------------------------------------------------------
	float GetRand(const float _min, const float _max)
	{
		//	大小が逆のとき
		if (_min > _max)
		{
			float tmpMin = _max;
			float tmpMax = _min;

			std::random_device randDevice;								//	非決定的な乱数生成器を生成
			std::mt19937 mt(randDevice());								//	引数は初期シード値
			std::uniform_real_distribution<float> rand(tmpMin, tmpMax);	//	[tmpMin, tmpMax] 範囲の一様乱数

			return rand(mt);
		}

		std::random_device randDevice;									//	非決定的な乱数生成器を生成
		std::mt19937 mt(randDevice());									//	引数は初期シード値
		std::uniform_real_distribution<float> rand(_min, _max);			//	[_min, _max] 範囲の一様乱数

		return rand(mt);
	}

	//-----------------------------------------------------------------------------
	//	@brief	指定範囲の乱数を取得（ベクトル）
	//-----------------------------------------------------------------------------
	VECTOR GetRand(const VECTOR _min, const VECTOR _max)
	{
		VECTOR tmp;

		tmp.x = GetRand(_min.x, _max.x);
		tmp.y = GetRand(_min.y, _max.y);
		tmp.z = GetRand(_min.z, _max.z);

		return tmp;
	}

	//-----------------------------------------------------------------------------
	//	@brief	線形補間
	//-----------------------------------------------------------------------------
	float Lerp(const float _a, const float _b, const float _t, const float _ignoreVal)
	{
		//	誤差
		const float tmpVal = _b - _a;
		const bool isIgnore = std::abs(tmpVal) <= _ignoreVal;
		if (isIgnore)
		{
			return _b;
		}

		return ((1.0f - _t) * _a) + (_t * _b);
	}

	//-----------------------------------------------------------------------------
	//	@brief	線形補間
	//-----------------------------------------------------------------------------
	VECTOR Lerp(const VECTOR _a, const VECTOR _b, const float _t, const float _ignoreVal)
	{
		VECTOR tmp;

		tmp.x = Lerp(_a.x, _b.x, _t, _ignoreVal);
		tmp.y = Lerp(_a.y, _b.y, _t, _ignoreVal);
		tmp.z = Lerp(_a.z, _b.z, _t, _ignoreVal);

		return tmp;
	}

	//-----------------------------------------------------------------------------
	//	@brief	色の加算
	//-----------------------------------------------------------------------------
	COLOR_F AddColor(COLOR_F _a, COLOR_F _b, float _t)
	{
		COLOR_F tmp;

		tmp.r = _a.r += _t;;		//	赤
		tmp.g = _a.g += _t;;		//	緑
		tmp.b = _a.b += _t;;		//	青
		tmp.a = _a.a += _t;;		//	アルファ値

		if (tmp.r >= _b.r) { tmp.r = _b.r; }
		if (tmp.g >= _b.g) { tmp.g = _b.g; }
		if (tmp.b >= _b.b) { tmp.b = _b.b; }
		if (tmp.a >= _b.a) { tmp.a = _b.a; }

		return tmp;
	}

	//-----------------------------------------------------------------------------
	//	@brief	線形補間
	//-----------------------------------------------------------------------------
	float LerpRadian(float _a, float _b, float _t)
	{
		float result;
		float diff = _b - _a;

		if (diff < -CommonConstant::PI)
		{
			_b += CommonConstant::PI2;
			result = Lerp(_a, _b, _t);
			if (result >= CommonConstant::PI2) { result -= CommonConstant::PI2; }
		}
		else if (diff > CommonConstant::PI)
		{
			_b -= CommonConstant::PI2;
			result = Lerp(_a, _b, _t);
			if (result < 0.f) { result += CommonConstant::PI2; }
		}
		else
		{
			result = Lerp(_a, _b, _t);
		}

		return result;
	}

	//-----------------------------------------------------------------------------
	//	@brief	二つのベクトルの角度を返す
	//-----------------------------------------------------------------------------
	float Vec2Radian(const VECTOR _base, const VECTOR _dir)
	{
		VECTOR base;
		VECTOR dir;
		base = _base;
		dir = _dir;

		//	基準方向ベクトルからの角度を求める
		float dot;
		float angle;
		VNorm(base);
		VNorm(dir);

		//	ベースと現在の向きから、角度を取る
		dot = VDot(base, dir);
		angle = acosf(dot);

		//	右回りか左回りか？で回転符号を計算
		VECTOR tmpDir;
		tmpDir = VCross(base, dir);

		//	左向きかどうか
		const bool isLeftDir = tmpDir.y < 0;
		if (isLeftDir) { angle *= -1.0f; }

		return angle;
	}

    //-----------------------------------------------------------------------------
    //	@brief	カメラによる調整
    //-----------------------------------------------------------------------------
    VECTOR AdjustmentByCamera(const VECTOR& _velocity, const Camera& _camera)
    {
        VECTOR tempMoveVector;

        //  カメラの角度に合わせて移動ベクトルを回転してから加算
        float sinParam = sinf(CommonFunction::DegToRad(_camera.GetHorizontalAngle()));
        float cosParam = cosf(CommonFunction::DegToRad(_camera.GetHorizontalAngle()));
        tempMoveVector.x = _velocity.x * cosParam - _velocity.z * sinParam;
        tempMoveVector.y = 0.0f;
        tempMoveVector.z = _velocity.x * sinParam + _velocity.z * cosParam;

        return tempMoveVector;
    }


	//-----------------------------------------------------------------------------
	//	@brief	ベクトルは範囲内か
	//-----------------------------------------------------------------------------
	bool VectorWithinRange(const VECTOR _vec1, const VECTOR _vec2, const float _range)
	{
		VECTOR hitPosRange = VGet(_vec2.x + _range, _vec2.y + _range, _vec2.z);
		const bool isWithinRangeAxisX = _vec1.x >= _vec2.x &&  _vec1.x <= hitPosRange.x;
		const bool isWithinRangeAxisY = _vec1.y >= _vec2.y &&  _vec1.y <= hitPosRange.y;
		const bool isWithinRange = isWithinRangeAxisX && isWithinRangeAxisY;

		return isWithinRange;
	}

	//-----------------------------------------------------------------------------
	//	@brief	ベクトルは等しいか
	//-----------------------------------------------------------------------------
	bool VectorSame(const VECTOR _vec1, const VECTOR _vec2)
	{
		const bool isSameAxisX = _vec1.x == _vec2.x;
		const bool isSameAxisY = _vec1.y == _vec2.y;
		const bool isSameAxisZ = _vec1.z == _vec2.z;

		return (isSameAxisX && isSameAxisY && isSameAxisZ);
	}

	//-----------------------------------------------------------------------------
	//	@brief	角度から向きを取る
	//-----------------------------------------------------------------------------
	VECTOR AskDirFromAngle(const float _angle)
	{
		VECTOR tmp = CommonConstant::ORIGIN;

		//	sin, cos を使い角度から
		//	x, y軸の向きを取る
		tmp.x = cosf(CommonFunction::DegToRad(_angle));
		tmp.y = sinf(CommonFunction::DegToRad(_angle));

		//	正規化
		if (VSquareSize(tmp) != 0) { tmp = VNorm(tmp); }

		return tmp;
	}

	//-----------------------------------------------------------------------------
	//	@brief	向きから角度を取る
	//-----------------------------------------------------------------------------
	float AskAngleFromDir(const VECTOR _dir)
	{
		//	基準方向ベクトルからの角度を求める
		VECTOR base = VGet(0.0f, 1.0f, 0.0f);
		VECTOR dir = _dir;

		//	正規化
		VNorm(dir);

		//	ベースと現在の向きから、角度を取る
		float dot = VDot(base, dir);
		float angle = acosf(dot);

		//	回転符号を計算
		VECTOR tmpDir;
		tmpDir = VCross(base, dir);

		//	左向きのときは、
		//	符号反転
		const bool isLeftDir = tmpDir.y < 0;
		if (isLeftDir) { angle *= -1.0f; }

		return angle;
	}

	//-----------------------------------------------------------------------------
	//	@brief	向きからラジアン度を取る
	//-----------------------------------------------------------------------------
	float AskRadianFromDir(const VECTOR _dir)
	{
		//	基準方向ベクトルからの角度を求める
		VECTOR base = VGet(0.0f, 1.0f, 0.0f);;
		VECTOR dir = _dir;

		//	正規化
		VNorm(dir);

		//	ベースと現在の向きから、角度を取る
		float dot = VDot(base, dir);
		float angle = acosf(dot);

		//	回転符号を計算
		VECTOR tmpDir;
		tmpDir = VCross(base, dir);

		//	左向きのときは、
		//	符号反転
		const bool isLeftDir = tmpDir.y < 0;
		if (isLeftDir) { angle *= -1.0f; }

		return DegToRad(angle);
	}

	//-----------------------------------------------------------------------------
	//	@brief	色の取得
	//-----------------------------------------------------------------------------
	int GetColor(const Color _color)
	{
		return DxLib::GetColor(_color.m_red, _color.m_green, _color.m_blue);
	}

	//-----------------------------------------------------------------------------
	//	@brief	色の取得
	//-----------------------------------------------------------------------------
	Color GetColor(const int _red, const int _green, const int _blue)
	{
		Color tmpColor;
		tmpColor.m_red = _red;
		tmpColor.m_green = _green;
		tmpColor.m_blue = _blue;
		tmpColor.m_color = DxLib::GetColor(_red, _green, _blue);
		return tmpColor;
	}

	//-----------------------------------------------------------------------------
	//	@brief	色の取得
	//-----------------------------------------------------------------------------
	COLOR_F GetColor(const ColorF _colorf)
	{
		return DxLib::GetColorF(_colorf.m_red, _colorf.m_green, _colorf.m_blue, _colorf.m_alpha);
	}

	//-----------------------------------------------------------------------------
	//	@brief	色の取得
	//-----------------------------------------------------------------------------
	ColorF GetColor(const float _red, const float _green, const float _blue, const float _alpha)
	{
		ColorF tmpColor;
		tmpColor.m_red = _red;
		tmpColor.m_green = _green;
		tmpColor.m_blue = _blue;
		tmpColor.m_alpha = _alpha;
		tmpColor.m_color = DxLib::GetColorF(_red, _green, _blue, _alpha);
		return tmpColor;
	}

	//-----------------------------------------------------------------------------
	//	@brief	アニメーションを求める
	//-----------------------------------------------------------------------------
	void AskAnimationCount(Animation& _animation)
	{
		//	アニメーション
		const bool isAddanim = _animation.m_time >= _animation.m_maxTime;
		if (isAddanim)
		{
			_animation.m_count++;
			_animation.m_time = 0.0f;
		}

		//	最大処理
		const bool isMaxAnim = _animation.m_count >= _animation.m_maxCount;
		if (isMaxAnim) { _animation.m_count = 0; }

		_animation.m_time++;
	}

	//-----------------------------------------------------------------------------
	//	@brief	アニメーションのリセット
	//-----------------------------------------------------------------------------
	void ResetAnimationCount(Animation & _animation)
	{
		_animation.m_count = 0;
		_animation.m_time = 0.0f;
	}

}

//-----------------------------------------------------------------------------
//	@brief	名前空間	[デバック]
//-----------------------------------------------------------------------------
namespace CommonDebug
{
	void Log() { printfDx("\n"); }
	void Log(const char* _log) { printfDx("%s\n", _log); }
	void Log(const int _value) { printfDx("[ %d ] \n", _value); }
	void Log(const float _value) { printfDx("[ %f ] \n", _value); }
	void Log(const VECTOR _value) { printfDx("[ %f, %f, %f ] \n", _value.x, _value.y, _value.z); }
	void Log(const int _value1, const int _value2) { printfDx("[ %d, %d ] \n", _value1, _value2); }
	void Log(const float _value1, const float _value2) { printfDx("[ %f, %f ] \n", _value1, _value2); }
	void Log(const VECTOR _value1, const VECTOR _value2) { printfDx("[ %f, %f, %f ], [ %f, %f, %f] \n", _value1.x, _value1.y, _value1.z, _value2.x, _value2.y, _value2.z); }
	void Log(const int _value1, const int _value2, const int _value3) { printfDx("[ %d, %d, %d  ] \n", _value1, _value2, _value3); }
	void Log(const float _value1, const float _value2, const float _value3) { printfDx("[ %f, %f ] \n", _value1, _value2); }
	void Log(const VECTOR _value1, const VECTOR _value2, const VECTOR _value3) { printfDx("[ %f, %f, %f ], [ %f, %f, %f  ], [ %f, %f, %f  ] \n", _value1.x, _value1.y, _value1.z, _value2.x, _value2.y, _value2.z, _value3.x, _value3.y, _value3.z); }
	void Log(const char* _log, const int _value)	{ printfDx("%s : [ %d ] \n", _log, _value); }
	void Log(const char* _log, const float _value)	{ printfDx("%s : [ %f ] \n", _log, _value); }
	void Log(const char* _log, const VECTOR _value)	{ printfDx("%s : [ %f, %f, %f ] \n", _log, _value.x, _value.y, _value.z); }
	void Log(const char* _log, const int _value1, const int _value2) { printfDx("%s : [ %d, %d ] \n", _log, _value1, _value2); }
	void Log(const char* _log, const float _value1, const float _value2) { printfDx("%s : [ %f, %f ] \n", _log, _value1, _value2); }
	void Log(const char* _log, const VECTOR _value1, const VECTOR _value2) { printfDx("%s : [ %f, %f, %f ], [ %f, %f, %f ] \n", _log, _value1.x, _value1.y, _value1.z, _value2.x, _value2.y, _value2.z); }
	void Log(const char* _log, const int _value1, const int _value2, const int _value3) { printfDx("%s : [ %d, %d, %d ] \n", _log, _value1, _value2, _value3); }
	void Log(const char* _log, const float _value1, const float _value2, const float _value3) { printfDx("%s : [ %f, %f, %f ] \n", _log, _value1, _value2, _value3); }
	void Log(const char* _log, const VECTOR _value1, const VECTOR _value2, const VECTOR _value3) { printfDx("%s : [ %f, %f, %f ], [ %f, %f, %f ] , [ %f, %f, %f ] \n", _log, _value1.x, _value1.y, _value1.z, _value2.x, _value2.y, _value2.z, _value3.x, _value3.y, _value3.z); }
	bool Assert(const bool _isConditions, const char* _assate) { if (_isConditions) { printfDx("%s\n", _assate); return true; } return false; }

	void PadInputPushTest(Pad& _pad)
	{
		//	ボタン
		if (_pad.GetXInputPushButton(PadInfo::PAD_BUTTON_KIND::BUTTON_A)) { Log("PushTest : [ A ] Button"); }
		if (_pad.GetXInputPushButton(PadInfo::PAD_BUTTON_KIND::BUTTON_B)) { Log("PushTest : [ B ] Button"); }
		if (_pad.GetXInputPushButton(PadInfo::PAD_BUTTON_KIND::BUTTON_X)) { Log("PushTest : [ X ] Button"); }
		if (_pad.GetXInputPushButton(PadInfo::PAD_BUTTON_KIND::BUTTON_Y)) { Log("PushTest : [ Y ] Button"); }
		if (_pad.GetXInputPushButton(PadInfo::PAD_BUTTON_KIND::BUTTON_LB)) { Log("PushTest : [ Left ] Button"); }
		if (_pad.GetXInputPushButton(PadInfo::PAD_BUTTON_KIND::BUTTON_RB)) { Log("PushTest : [ Right ] Button"); }
		if (_pad.GetXInputPushButton(PadInfo::PAD_BUTTON_KIND::BUTTON_START)) { Log("PushTest : [ Start ] Button"); }
		if (_pad.GetXInputPushButton(PadInfo::PAD_BUTTON_KIND::BUTTON_BACK)) { Log("PushTest : [ Back ] Button"); }
		if (_pad.GetXInputPushButton(PadInfo::PAD_BUTTON_KIND::BUTTON_RSB)) { Log("PushTest : [ Right ] Stick Button"); }
		if (_pad.GetXInputPushButton(PadInfo::PAD_BUTTON_KIND::BUTTON_LSB)) { Log("PushTest : [ Left ] Stick Button"); }

		//	トリガー
		if (_pad.GetXInputPushTrigger(PadInfo::PAD_BUTTON_KIND::BUTTON_LT, PadInfo::TRIGGER_DEAD_ZONE)) { Log("PushTest : [ Left ] Trigger"); }
		if (_pad.GetXInputPushTrigger(PadInfo::PAD_BUTTON_KIND::BUTTON_RT, PadInfo::TRIGGER_DEAD_ZONE)) { Log("PushTest : [ Right ] Trigger"); }
	}

	void PadInputDisuniteTest(Pad& _pad)
	{
		//	ボタン
		if (_pad.GetXInputDisuniteButton(PadInfo::PAD_BUTTON_KIND::BUTTON_A)) { Log("DisuniteTest [ A ] Button"); }
		if (_pad.GetXInputDisuniteButton(PadInfo::PAD_BUTTON_KIND::BUTTON_B)) { Log("DisuniteTest [ B ] Button"); }
		if (_pad.GetXInputDisuniteButton(PadInfo::PAD_BUTTON_KIND::BUTTON_X)) { Log("DisuniteTest [ X ] Button"); }
		if (_pad.GetXInputDisuniteButton(PadInfo::PAD_BUTTON_KIND::BUTTON_Y)) { Log("DisuniteTest [ Y ] Button"); }
		if (_pad.GetXInputDisuniteButton(PadInfo::PAD_BUTTON_KIND::BUTTON_LB)) { Log("DisuniteTest [ Left ] Button"); }
		if (_pad.GetXInputDisuniteButton(PadInfo::PAD_BUTTON_KIND::BUTTON_RB)) { Log("DisuniteTest [ Right ] Button"); }
		if (_pad.GetXInputDisuniteButton(PadInfo::PAD_BUTTON_KIND::BUTTON_START)) { Log("DisuniteTest [ Start ] Button"); }
		if (_pad.GetXInputDisuniteButton(PadInfo::PAD_BUTTON_KIND::BUTTON_BACK)) { Log("DisuniteTest [ Back ] Button"); }
		if (_pad.GetXInputDisuniteButton(PadInfo::PAD_BUTTON_KIND::BUTTON_RSB)) { Log("DisuniteTest [ Right ] Stick Button"); }
		if (_pad.GetXInputDisuniteButton(PadInfo::PAD_BUTTON_KIND::BUTTON_LSB)) { Log("DisuniteTest [ Left ] Stick Button"); }

		//	トリガー
		if (_pad.GetXInputDisuniteTrigger(PadInfo::PAD_BUTTON_KIND::BUTTON_LT, PadInfo::TRIGGER_DEAD_ZONE)) { Log("DisuniteTest [ Left ] Trigger"); }
		if (_pad.GetXInputDisuniteTrigger(PadInfo::PAD_BUTTON_KIND::BUTTON_RT, PadInfo::TRIGGER_DEAD_ZONE)) { Log("DisuniteTest [ Right ] Trigger"); }
	}

	void PadInputTest(Pad& _pad)
	{
		//	ボタン
		if (_pad.GetXInputButton(PadInfo::PAD_BUTTON_KIND::BUTTON_A)) { Log("InputTest [ A ] Button"); }
		if (_pad.GetXInputButton(PadInfo::PAD_BUTTON_KIND::BUTTON_B)) { Log("InputTest [ B ] Button"); }
		if (_pad.GetXInputButton(PadInfo::PAD_BUTTON_KIND::BUTTON_X)) { Log("InputTest [ X ] Button"); }
		if (_pad.GetXInputButton(PadInfo::PAD_BUTTON_KIND::BUTTON_Y)) { Log("InputTest [ Y ] Button"); }
		if (_pad.GetXInputButton(PadInfo::PAD_BUTTON_KIND::BUTTON_LB)) { Log("InputTest [ Left ] Button"); }
		if (_pad.GetXInputButton(PadInfo::PAD_BUTTON_KIND::BUTTON_RB)) { Log("InputTest [ Right ] Button"); }
		if (_pad.GetXInputButton(PadInfo::PAD_BUTTON_KIND::BUTTON_START)) { Log("InputTest [ Start ] Button"); }
		if (_pad.GetXInputButton(PadInfo::PAD_BUTTON_KIND::BUTTON_BACK)) { Log("InputTest [ Back ] Button"); }
		if (_pad.GetXInputButton(PadInfo::PAD_BUTTON_KIND::BUTTON_RSB)) { Log("InputTest [ Right ] Stick Button"); }
		if (_pad.GetXInputButton(PadInfo::PAD_BUTTON_KIND::BUTTON_LSB)) { Log("InputTest [ Left ] Stick Button"); }
		if (_pad.GetXInputTrigger(PadInfo::PAD_BUTTON_KIND::BUTTON_LT, PadInfo::TRIGGER_DEAD_ZONE)) { Log("InputTest [ Left ] Trigger"); }
		if (_pad.GetXInputTrigger(PadInfo::PAD_BUTTON_KIND::BUTTON_RT, PadInfo::TRIGGER_DEAD_ZONE)) { Log("InputTest [ Right ] Trigger"); }

		//	DPAD
		if (_pad.GetXInputDpad(PadInfo::PAD_DPAD_KIND::DPAD_UP)) { Log("InputTest Dpad [ Up ] "); }
		if (_pad.GetXInputDpad(PadInfo::PAD_DPAD_KIND::DPAD_DOWN)) { Log("InputTest Dpad [ Down ] "); }
		if (_pad.GetXInputDpad(PadInfo::PAD_DPAD_KIND::DPAD_RIGHT)) { Log("InputTest Dpad [ Right ] "); }
		if (_pad.GetXInputDpad(PadInfo::PAD_DPAD_KIND::DPAD_LEFT)) { Log("InputTest Dpad [ Left ] "); }

		//	スティック
		if (_pad.GetXInputStick(PadInfo::PAD_STICK_KIND::LEFT_STICK_UP, PadInfo::STICK_DEAD_ZONE)) { Log("InputTest Left Stick [ Up ] "); }
		if (_pad.GetXInputStick(PadInfo::PAD_STICK_KIND::LEFT_STICK_DOWN, PadInfo::STICK_DEAD_ZONE)) { Log("InputTest Left Stick [ Down ] "); }
		if (_pad.GetXInputStick(PadInfo::PAD_STICK_KIND::LEFT_STICK_RIGHT, PadInfo::STICK_DEAD_ZONE)) { Log("InputTest Left Stick [ Right ] "); }
		if (_pad.GetXInputStick(PadInfo::PAD_STICK_KIND::LEFT_STICK_LEFT, PadInfo::STICK_DEAD_ZONE)) { Log("InputTest Left Stick [ Left ] "); }
		if (_pad.GetXInputStick(PadInfo::PAD_STICK_KIND::RIGHT_STICK_UP, PadInfo::STICK_DEAD_ZONE)) { Log("InputTest Right Stick [ Up ] "); }
		if (_pad.GetXInputStick(PadInfo::PAD_STICK_KIND::RIGHT_STICK_DOWN, PadInfo::STICK_DEAD_ZONE)) { Log("InputTest Right Stick [ Down ] "); }
		if (_pad.GetXInputStick(PadInfo::PAD_STICK_KIND::RIGHT_STICK_RIGHT, PadInfo::STICK_DEAD_ZONE)) { Log("InputTest Right Stick [ Right ] "); }
		if (_pad.GetXInputStick(PadInfo::PAD_STICK_KIND::RIGHT_STICK_LEFT, PadInfo::STICK_DEAD_ZONE)) { Log("InputTest Right Stick [ Left ] "); }
	}

}
