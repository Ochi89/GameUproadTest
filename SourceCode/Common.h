
//=============================================================================
//	@file	Common.h
//	@brief	共通
//	@autor	Takuya Ochi
//	@date	2018/9/28
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	プリプロセッサ
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include <random>

//-----------------------------------------------------------------------------
//	@brief	前方宣言
//-----------------------------------------------------------------------------
class Camera;
class Pad;

//-----------------------------------------------------------------------------
//	@brief	長方形の構造体
//-----------------------------------------------------------------------------
struct Rect
{
	VECTOR	m_vertexTop;		//	上の頂点
	VECTOR	m_vertexUnder;		//	下の頂点
};

//-----------------------------------------------------------------------------
//	@brief	円形の構造体
//-----------------------------------------------------------------------------
struct Circle
{
	float	m_radius;			//	半径
	VECTOR	m_centerPoint;		//	中心点
};

//-----------------------------------------------------------------------------
//	@brief	UIの構造体
//-----------------------------------------------------------------------------
struct UI
{
	int		m_spriteHendle;		//	スプライトハンドル
	VECTOR	m_pos;				//	ポジション
	float	m_scale;			//	スケール
	float	m_angle;			//	アングル
	float	m_radian;			//	ラジアン
	float	m_flashingTime;		//	点滅描画用の時間
};

//-----------------------------------------------------------------------------
//	@brief	色の構造体
//-----------------------------------------------------------------------------
struct Color
{
	int m_red;			//	赤色
	int m_green;		//	緑色
	int m_blue;			//	青色
	int m_color;		//	色
};

//-----------------------------------------------------------------------------
//	@brief	色の構造体
//-----------------------------------------------------------------------------
struct ColorF
{
	float m_red;			//	赤色
	float m_green;			//	緑色
	float m_blue;			//	青色
	float m_alpha;			//	アルファ
	COLOR_F m_color;		//	色
};

//-----------------------------------------------------------------------------
//	@brief	アニメーションの構造体
//-----------------------------------------------------------------------------
struct Animation
{
	int		m_count;		//	カウント
	float	m_time;			//	タイム
	float	m_maxTime;		//	タイムの最大
	int		m_maxCount;		//	カウントの最大
};

//-----------------------------------------------------------------------------
//	@brief	名前空間	[関数]
//-----------------------------------------------------------------------------
namespace CommonFunction
{

//===================== 関数 ======================//

	//	ラジアンを度に変換する
	float RadToDeg(float _rad);

	//	ラジアンを度に変換する
	VECTOR RadToDeg(VECTOR _rad);

	//	度をラジアンに変換する
	float DegToRad(float _deg);

	//	度をラジアンに変換する
	VECTOR DegToRad(VECTOR _rad);

	//	グリッドの作成
	void CreateGrid();

	//	指定範囲の乱数生成
	int GetRand(const int _min, const int _max);

	//	指定範囲の乱数生成
	float GetRand(const float _min, const float _max);

	//	指定範囲の乱数生成
	VECTOR GetRand(const VECTOR _min, const VECTOR _max);

	//	線形補間
	float Lerp(const float _a, const float _b, const float _t, const float _ignoreVal = 0.0f);

	//	線形補間
	VECTOR Lerp(const VECTOR _a, const VECTOR _b, const float _t, const float _ignoreVal = 0.0f);

	//	色の加算
	COLOR_F AddColor(COLOR_F _a, COLOR_F _b, float _t);

	//	線形補間で角度を求める
	float LerpRadian(float _a, float _b, float _t);

	//	二つのベクトルの角度を返す
	float Vec2Radian(const VECTOR _base, const VECTOR _dir);

	//  カメラによる調整
	VECTOR AdjustmentByCamera(const VECTOR& _velocity, const Camera& _camera);

	//	ベクトルは範囲内か
	bool VectorWithinRange(const VECTOR _vec1, const VECTOR _vec2, const float _range);

	//	ベクトルは等しいか
	bool VectorSame(const VECTOR _vec1, const VECTOR _vec2);

	//	角度から向きを取る
	VECTOR AskDirFromAngle(const float _angle);

	//	向きから角度を取る
	float AskAngleFromDir(const VECTOR _dir);

	//	向きからラジアン度を取る
	float AskRadianFromDir(const VECTOR _dir);

	//	色の取得
	int GetColor(const Color _color);

	//	色の取得
	Color GetColor(const int _red, const int _green, const int _blue);

	//	色の取得
	COLOR_F GetColor(const ColorF _colorf);

	//	色の取得
	ColorF GetColor(const float _red, const float _green, const float _blue, const float _alpha);

	//	アニメーションを求める
	void AskAnimationCount(Animation& _animation);

	//	アニメーションのリセット
	void ResetAnimationCount(Animation& _animation);

	//	配列のサイズの取得
	template<typename Template>
	int GetArraySize(const Template& _array)
	{
		const int array = sizeof(_array);
		const int oneArray = sizeof(_array[0]);
		return (array / oneArray);
	}
}

//-----------------------------------------------------------------------------
//	@brief	名前空間	[定数]
//-----------------------------------------------------------------------------
namespace CommonConstant
{
//===================== 静的定数 ===================//

	static const int		WINDOW_WIDTH = 1920;							//	ウィンドウの幅
	static const int		WINDOW_HEIGHT = 1080;							//	ウィンドウの高さ
	static const int		COLOR_BIT = 16;									//	カラービット数
	static const VECTOR		ORIGIN = VGet(0.0f, 0.0f, 0.0f);				//	原点
	static const float		LINE_AREA_SIZE = 1000.0f;						//	ラインを描く範囲
	static const int		LINE_NUM = 100;									//	ラインの数
	static const float		PI = 3.14159265359f;							//	π
	static const float		PI2 = 6.28318530718f;							//	2π
	static const int		MAX_KEY_INPUT_NUM = 256;						//	キーの数
	static const int		MAX_CSV_NUM = 256;								//	CSVの読み込み範囲の最大
	static const int		MAX_ENEMY_ENTRY_NUM = 50;						//	エネミーの登録数
	static const int		MAX_ITEM_NUM = 10;								//	アイテムの登録数
	static const int		MAX_SHOT_NUM = 50;								//	弾の登録数
	static const int		MAX_STAY_SHOT_NUM = 10;							//	滞在弾の登録数
	static const int		MAX_PLAYER_LIFE = 5;							//	ライフ
	static const int		MAX_EFFECT_NUM = 50;							//	エフェクトの登録数
	static const int		MAX_BOSS_EXPLOSION_NUM = 5;						//	ボス爆発エフェクト数
	static const int		MAX_SCORE_NUM = 128;							//	スコアの数
	static const int		FONT_NUM = 255;									//	フォントの最大数
	static const int		MAX_RANKING_NUM = 10;							//	ランキングの最大数
	static const int		MAX_ENEMY_SE_NUM = 20;							//	SEの登録数
	static const int		STAY_SHOT_UI_ANIM_NUM = 10;						//	ステイショットUIのアニメーション数
	static const int		MAX_BACK_PIXEL_NUM = 150;						//	背景ピクセル数
}

//-----------------------------------------------------------------------------
//	@brief	名前空間	[デバッグ]
//-----------------------------------------------------------------------------
namespace CommonDebug
{

//===================== 関数 ======================//

	//	ログ
	void Log();
	void Log(const char* _log);
	void Log(const int _value);
	void Log(const float _value);
	void Log(const VECTOR _value);
	void Log(const int _value1, const int _value2);
	void Log(const float _value1, const float _value2);
	void Log(const VECTOR _value1, const VECTOR _value2);
	void Log(const int _value1, const int _value2, const int _value3);
	void Log(const float _value1, const float _value2, const float _value3);
	void Log(const VECTOR _value1, const VECTOR _value2, const VECTOR _value3);
	void Log(const char* _log, const int _value);
	void Log(const char* _log, const float _value);
	void Log(const char* _log, const VECTOR _value);
	void Log(const char* _log, const int _value1, const int _value2);
	void Log(const char* _log, const float _value1, const float _value2);
	void Log(const char* _log, const VECTOR _value1, const VECTOR _value2);
	void Log(const char* _log, const int _value1, const int _value2, const int _value3);
	void Log(const char* _log, const float _value1, const float _value2, const float _value3);
	void Log(const char* _log, const VECTOR _value1, const VECTOR _value2, const VECTOR _value3);
	
	//	アサート
	bool Assert(const bool _isConditions, const char* _assate);

	//	パッド入力テスト [押した瞬間]
	void PadInputPushTest(Pad& _pad);

	//	パッド入力テスト [離した瞬間]
	void PadInputDisuniteTest(Pad& _pad);

	//	パッド入力テスト [押している間]
	void PadInputTest(Pad& _pad);
}

//-----------------------------------------------------------------------------
//	@brief	名前空間	[安全処理]
//  TODO :: nullチェックが必要な処理をまとめた名前空間、メモリの解放などをしている
//  TODO :: CommonSafeでは名前が不適正なので、変更したほうが良いかも
//-----------------------------------------------------------------------------
namespace CommonSafe
{
    //-----------------------------------------------------------------------------
    //	@brief	削除処理
    //-----------------------------------------------------------------------------
    template<class Template>
    void Delete(Template*& _pointer)
    {
		if (_pointer)
		{
			delete _pointer;
			_pointer = NULL;
			return;
		}
    }

    //-----------------------------------------------------------------------------
    //	@brief	解放処理
    //-----------------------------------------------------------------------------
    template<class Template>
    void Release(Template*& _pointer)
    {
        if (_pointer)
        {
            _pointer->Release();
            delete _pointer;
            _pointer = NULL;
			return;
		}
    }
}

//-----------------------------------------------------------------------------
//	@brief	共通インクルード
//-----------------------------------------------------------------------------
#include "Key.h"
#include "Production.h"
#include "Menu.h"
#include "HandOverToResult.h"
#include "LoadScene.h"
#include "Font.h"
