
//=============================================================================
//	@file	Opening.h
//	@brief	オープニング
//	@autor	Takuya Ochi
//	@date	2018/1/18
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
//	@brief	前方宣言
//-----------------------------------------------------------------------------
struct UI;
struct Rect;
struct Color;
class Pad;

//-----------------------------------------------------------------------------
//	@brief	オープニングクラス
//-----------------------------------------------------------------------------
class Opening final
{
public:

//===================== 関数 ======================//

	// コンストラクタ / デストラクタ
	Opening();
	~Opening();

	//	生成
	Opening* GetInstance();

	//	更新処理
	void Update(Pad& _pad);

	//	描画処理
	void Draw();

//==================== getter =====================//

	//	オープニング終了フラグの　getter
	const bool GetIsEndOpening() const { return m_isEndOpening; }

private:

//==================== 列挙体 =====================//

	//	オープニングの種類
	enum OPENING_KIND
	{
		OPENING_NONE,		//	なし
		OPENING_1,			//	オープニング１
		OPENING_2,			//	オープニング２
		OPENING_3,			//	オープニング３
		OPENING_4,			//	オープニング４
		OPENING_5,			//	オープニング５
		OPENING_6,			//	オープニング６
		OPENING_NUM,		//	オープニングの数
	};

//================== 内部処理関数 ==================//

	//	UIの読み込み
	void _LoadUI(UI& _ui, const char* _fileName);

	//	UIの描画
	void _DrawUI(UI _ui);

	//	アルファ値の変更
	void _ChangeAlpha();

	//	アルファ値の加速
	void _AcceleAlpha(Pad& _pad);

	//	オープニングのスキップ
	void _OpeningSkip(Pad& _pad);

	//	終了
	void _EndOpening();

	//	最終的な解放処理
	void _FinalRelease();

//=================== メンバ変数 ===================//

	static Opening	m_instance;				//	インスタンス
	UI				m_OpeningUI1;			//	オープニングUI 1
	UI				m_OpeningUI2;			//	オープニングUI 2
	UI				m_OpeningUI3;			//	オープニングUI 3
	UI				m_OpeningUI4;			//	オープニングUI 4
	UI				m_OpeningUI5;			//	オープニングUI 5
	UI				m_OpeningUI6;			//	オープニングUI 6
	Rect			m_backGroundRect;		//	背景
	Color			m_backGroundColor;		//	背景色
	int				m_openingCount;			//	オープニングカウント
	int				m_alpha;				//	アルファ値
	int				m_alphaSpeed;			//	アルファ速度
	float			m_waitTime;				//	遅延時間
	bool			m_isChangeAlpha;		//	アルファ値の切り替えフラグ
	bool			m_isStart;				//	開始フラグ
	bool			m_isEndOpening;			//	オープニング終了フラグ

//===================== 静的定数 ===================//

	static const int		MIN_ALPHA;				//	アルファ値の最小
	static const int		MAX_ALPHA;				//	アルファ値の最大
	static const int		ALPHA_DEFAULT_SPEED;	//	アルファ値のデフォルト速度
	static const int		ALPHA_ACCELE_SPEED;		//	アルファ値のアクセル速度

};

#define OPENING Opening::GetInstance()
