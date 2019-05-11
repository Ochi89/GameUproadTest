
//=============================================================================
//	@file	Menu.h
//	@brief	メニュー
//	@autor	Takuya Ochi
//	@date	2018/1/8
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	プリプロセッサ
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "Common.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
class Pad;
struct Rect;
struct UI;
class SoundEffect;

//-----------------------------------------------------------------------------
//	@brief	スコアクラス
//-----------------------------------------------------------------------------
class Menu final
{
public:

//===================== 関数 ======================//

	//	デストラクタ
	~Menu();

	//	生成
	Menu* GetInstance();

	//	初期処理
	void Initialize();

	//	更新
	void Update(Pad& _pad, SoundEffect& _soundEffect);

	//	描画
	void Draw();

//==================== getter =====================//

	//	タイトルフラグの getter
	const bool& GetIsGoToTitle() const { return m_isGoToTitle; }

	//	更新スキップフラグの getter
	const bool& GetIsSkipUpdate() const { return m_isSkipUpdate; }

private:

//================== シングルトン ==================//

	//	コンストラクタ
	Menu();

//==================== 列挙体 =====================//

	//	モード
	enum MODE
	{
		MODE_OPERATION,		//	操作説明
		MODE_TITLE,			//	タイトルへ
		MODE_BACK,			//	戻る
		MODE_NUM,			//	モードの数
	};

	//	カウントダウン
	enum COUNTDOWN
	{
		COUNTDOWN_1,		//	１
		COUNTDOWN_2,		//	２
		COUNTDOWN_3,		//	３
		COUNTDOWN_NUM,		//	数
	};

//================== 内部処理関数 ==================//

	//	メニュー開閉
	void _MenuOpeningAndClosing(Pad& _pad, SoundEffect& _soundEffect);

	//	モード切り替え
	void _ChangeMode(Pad& _pad, SoundEffect& _soundEffect);

	//	選択位置の変更
	void _ChangeSelectPos();

	//	モード
	void _Mode(Pad& _pad, SoundEffect& _soundEffect);

	//	選択フレームの拡大率
	void _ChangeSelectScale();

	//	カウントダウン
	void _Countdown(SoundEffect& _soundEffect);

	//	メニューの描画
	void _MenuDraw();

	//	操作説明の描画
	void _OperationDraw();

	//	カウントダウンの描画
	void _CountdownDraw();

	//	最終的な解放処理
	void _FinalRelease();

//=================== メンバ変数 ===================//

	static Menu		s_instance;									//	インスタンス
	int				m_modeNumber;								//	モード番号
	int				m_countdown;								//	カウントダウン
	float			m_watiTime;									//	遅延時間
	float			m_countdownTime;							//	カウントダウン時間
	bool			m_isMenuState;								//	メニュ－の状態
	bool			m_isMenuDraw;								//	メニュ－の描画
	bool			m_isMenu;									//	メニュ－
	bool			m_isOperation;								//	操作説明
	bool			m_isGoToTitle;								//	タイトルへ
	bool			m_isSelectScaleState;						//	選択フレームの拡大率の状態
	bool			m_isSkipUpdate;								//	更新のスキップ
	bool			m_isCountdownStart;							//	カウントダウンの開始
	Rect			m_menuFlame;								//	メニューフレーム
	UI				m_topFlame;									//	上フレームUI
	UI				m_underFlame;								//	下フレームUI
	UI				m_mode[MODE::MODE_NUM];						//	モードUI
	UI				m_select;									//	選択UI
	UI				m_operation;								//	説明UI
	UI				m_countdownUI[COUNTDOWN::COUNTDOWN_NUM];	//	カウントダウンUI

//===================== 静的定数 ===================//

	static const int		MAX_MODE;					//	最大
	static const int		MIN_MODE;					//	最小
	static const VECTOR		SELECT_CORRECTION;			//	選択位置の調整
	static const float		MAX_FLAME;					//	最大フレーム
	static const float		MIN_FLAME;					//	最小フレーム
	static const float		MAX_WAIT_TIME;				//	遅延
	static const float		FONT_SIZE;					//	文字サイズ
	static const float		SELECT_POS_1;				//	選択フレームの位置１
	static const float		SELECT_POS_2;				//	選択フレームの位置２
	static const float		SELECT_POS_3;				//	選択フレームの位置３
	static const float		SELECT_SCALE_SPEED;			//	選択フレームの拡大率の速度
	static const float		MIN_SELECT_SCALE;			//	選択フレームの拡大率の最小
	static const float		MAX_SELECT_SCALE;			//	選択フレームの拡大率の最大
	static const float		MAX_COUNTDOWN_TIME;			//	カウントダウン時間の最大
	static const int		MAX_COUNTDOWN;				//	カウントダウンの最大
	static const float		COUNTDOWN_SCALE_SPEED;		//	カウントダウンの拡大率の速度
};

#define MENU Menu::GetInstance()