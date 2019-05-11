
//=============================================================================
//	@file	Menu.cpp
//	@brief	メニュー
//	@autor	Takuya Ochi
//	@date	2018/1/8
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "Menu.h"
#include "Pad.h"
#include "SoundEffect.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const int		Menu::MAX_MODE = 2;										//	最大
const int		Menu::MIN_MODE = 0;										//	最小
const VECTOR	Menu::SELECT_CORRECTION = VGet(25.0f, 15.0f, 0.0f);		//	選択位置の調整
const float		Menu::MAX_FLAME = 900.0f;								//	最大フレーム
const float		Menu::MIN_FLAME = 200.0f;								//	最小フレーム
const float		Menu::MAX_WAIT_TIME = 10.0f;							//	遅延
const float		Menu::FONT_SIZE = 10.0f;								//	文字サイズ
const float		Menu::SELECT_POS_1 = 390.0f;							//	選択フレームの位置１
const float		Menu::SELECT_POS_2 = 540.0f;							//	選択フレームの位置２
const float		Menu::SELECT_POS_3 = 690.0f;							//	選択フレームの位置３
const float		Menu::SELECT_SCALE_SPEED = 0.002f;						//	選択フレームの拡大率の速度
const float		Menu::MIN_SELECT_SCALE = 1.0f;							//	選択フレームの拡大率の最小
const float		Menu::MAX_SELECT_SCALE = 1.05f;							//	選択フレームの拡大率の最大
const float		Menu::MAX_COUNTDOWN_TIME = 60.0f;						//	カウントダウン時間の最大
const int		Menu::MAX_COUNTDOWN = 2;								//	カウントダウンの最大
const float		Menu::COUNTDOWN_SCALE_SPEED = 0.02f;					//	カウントダウンの拡大率の速度

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
Menu::Menu()
{
	m_modeNumber = (int)MODE::MODE_OPERATION;
	m_countdown = 0;
	m_watiTime = 0.0f;
	m_countdownTime = 0.0f;
	m_isMenuState = false;
	m_isMenu = false;
	m_isMenuDraw = false;
	m_isOperation = false;
	m_isGoToTitle = false;
	m_isSelectScaleState = false;
	m_isSkipUpdate = false;
	m_isCountdownStart = false;

	//	上フレーム
	m_topFlame.m_spriteHendle = LoadGraph("Data/Sprite/Menu/Flame.png");
	CommonDebug::Assert((m_topFlame.m_spriteHendle <= -1), " [ Menu.cpp ] : error : sprite loading failed.");
	m_topFlame.m_pos = VGet(400.0f, 200.0f, 0.0f);

	//	下フレーム
	m_underFlame.m_spriteHendle = LoadGraph("Data/Sprite/Menu/Flame.png");
	CommonDebug::Assert((m_underFlame.m_spriteHendle <= -1), " [ Menu.cpp ] : error : sprite loading failed.");
	m_underFlame.m_pos = VGet(400.0f, 200.0f, 0.0f);

	m_menuFlame.m_vertexTop = VGet(450.0f, 200.0f, 0.0f);
	m_menuFlame.m_vertexUnder = VGet(1500.0f, 200.0f, 0.0f);

	//	モードUI
	m_mode[MODE::MODE_OPERATION].m_spriteHendle = LoadGraph("Data/Sprite/Menu/Operation.png");
	m_mode[MODE::MODE_TITLE].m_spriteHendle = LoadGraph("Data/Sprite/Menu/Title.png");
	m_mode[MODE::MODE_BACK].m_spriteHendle = LoadGraph("Data/Sprite/Menu/Close.png");
	CommonDebug::Assert((m_mode[MODE::MODE_OPERATION].m_spriteHendle <= -1), " [ Menu.cpp ] : error : sprite loading failed.");
	CommonDebug::Assert((m_mode[MODE::MODE_TITLE].m_spriteHendle <= -1), " [ Menu.cpp ] : error : sprite loading failed.");
	CommonDebug::Assert((m_mode[MODE::MODE_BACK].m_spriteHendle <= -1), " [ Menu.cpp ] : error : sprite loading failed.");
	m_mode[MODE::MODE_OPERATION].m_pos = VGet(600.0f, 350.0f, 0.0f);
	m_mode[MODE::MODE_TITLE].m_pos = VGet(600.0f, 500.0f, 0.0f);
	m_mode[MODE::MODE_BACK].m_pos = VGet(600.0f, 650.0f, 0.0f);

	//	選択
	m_select.m_spriteHendle = LoadGraph("Data/Sprite/Menu/Select.png");
	CommonDebug::Assert((m_select.m_spriteHendle <= -1), " [ Menu.cpp ] : error : sprite loading failed.");
	m_select.m_pos = VGet(950.0f, 390.0, 0.0f);
	m_select.m_scale = MAX_SELECT_SCALE;

	//	説明
	m_operation.m_spriteHendle = LoadGraph("Data/Sprite/Menu/Pad.png");
	CommonDebug::Assert((m_operation.m_spriteHendle <= -1), " [ Menu.cpp ] : error : sprite loading failed.");
	m_operation.m_pos = VGet(450.0f, -50.0f, 0.0f);

	//	カウントダウン
	m_countdownUI[COUNTDOWN::COUNTDOWN_1].m_spriteHendle = LoadGraph("Data/Sprite/Menu/Countdown1.png");
	m_countdownUI[COUNTDOWN::COUNTDOWN_2].m_spriteHendle = LoadGraph("Data/Sprite/Menu/Countdown2.png");
	m_countdownUI[COUNTDOWN::COUNTDOWN_3].m_spriteHendle = LoadGraph("Data/Sprite/Menu/Countdown3.png");
	CommonDebug::Assert((m_countdownUI[COUNTDOWN::COUNTDOWN_1].m_spriteHendle <= -1), " [ Menu.cpp ] : error : sprite loading failed.");
	CommonDebug::Assert((m_countdownUI[COUNTDOWN::COUNTDOWN_2].m_spriteHendle <= -1), " [ Menu.cpp ] : error : sprite loading failed.");
	CommonDebug::Assert((m_countdownUI[COUNTDOWN::COUNTDOWN_3].m_spriteHendle <= -1), " [ Menu.cpp ] : error : sprite loading failed.");

	for (int i = 0; i <= COUNTDOWN::COUNTDOWN_NUM; i++)
	{
		m_countdownUI[i].m_pos = VGet(950.0f, 600.0f, 0.0f);
		m_countdownUI[i].m_scale = 1.5f;
	}

}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
Menu::~Menu()
{
	//	最終的な解放
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	インスタンス生成
//-----------------------------------------------------------------------------
Menu* Menu::GetInstance()
{
	return &s_instance;
}

//-----------------------------------------------------------------------------
//	@brief	初期処理
//-----------------------------------------------------------------------------
void Menu::Initialize()
{
	//	初期化
	m_modeNumber = (int)MODE::MODE_OPERATION;
	m_countdown = -1;
	m_watiTime = 0.0f;
	m_countdownTime = 0.0f;
	m_isMenuState = false;
	m_isMenuDraw = false;
	m_isMenu = false;
	m_isOperation = false;
	m_isGoToTitle = false;
	m_isSelectScaleState = false;
	m_isSkipUpdate = false;
	m_isCountdownStart = false;

	//	初期位置に戻す
	m_topFlame.m_pos = VGet(400.0f, 200.0f, 0.0f);
	m_underFlame.m_pos = VGet(400.0f, 200.0f, 0.0f);
	m_menuFlame.m_vertexTop = VGet(450.0f, 200.0f, 0.0f);
	m_menuFlame.m_vertexUnder = VGet(1500.0f, 200.0f, 0.0f);
	m_mode[MODE::MODE_OPERATION].m_pos = VGet(600.0f, 350.0f, 0.0f);
	m_mode[MODE::MODE_TITLE].m_pos = VGet(600.0f, 500.0f, 0.0f);
	m_mode[MODE::MODE_BACK].m_pos = VGet(600.0f, 650.0f, 0.0f);
	m_select.m_pos = VGet(950.0f, 390.0, 0.0f);
	m_select.m_scale = MAX_SELECT_SCALE;
	m_operation.m_pos = VGet(450.0f, -50.0f, 0.0f);

	for (int i = 0; i <= COUNTDOWN::COUNTDOWN_NUM; i++)
	{
		m_countdownUI[i].m_pos = VGet(950.0f, 600.0f, 0.0f);
		m_countdownUI[i].m_scale = 1.5f;
	}

}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void Menu::Update(Pad& _pad, SoundEffect& _soundEffect)
{
	//	演出中でないとき
	const bool isNotProduction = !PRODUCTION->GetIsClearProduction() && !PRODUCTION->GetIsSceneProduction() && !PRODUCTION->GetIsSpecialProduction();
	if (isNotProduction)
	{
		//	メニューの開閉
		_MenuOpeningAndClosing(_pad, _soundEffect);

		//	モード切替
		_ChangeMode(_pad, _soundEffect);

		//	選択位置の切り替え
		_ChangeSelectPos();

		//	モード
		_Mode(_pad, _soundEffect);

		//	選択フレームの拡大率の変更
		_ChangeSelectScale();

		//	カウントダウン
		_Countdown(_soundEffect);
	}
}

//-----------------------------------------------------------------------------
//	@brief	描画処理
//-----------------------------------------------------------------------------
void Menu::Draw()
{
	if (m_isMenuDraw)
	{
		//	半透明表示
		const int blendVal = 128;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, blendVal);
		DrawBox(
			(int)m_menuFlame.m_vertexTop.x,
			(int)m_menuFlame.m_vertexTop.y,
			(int)m_menuFlame.m_vertexUnder.x,
			(int)m_menuFlame.m_vertexUnder.y,
			GetColor(0, 128, 128),
			TRUE
		);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//	メニュー
	_MenuDraw();

	//	操作説明
	_OperationDraw();

	if (m_isMenuDraw)
	{
		//	フレームの描画
		DrawGraph((int)m_topFlame.m_pos.x, (int)m_topFlame.m_pos.y, m_topFlame.m_spriteHendle, TRUE);
		DrawGraph((int)m_underFlame.m_pos.x, (int)m_underFlame.m_pos.y, m_underFlame.m_spriteHendle, TRUE);
	}

	//	カウントダウンの描画
	_CountdownDraw();
}

//-----------------------------------------------------------------------------
//	@brief	メニューの開閉
//-----------------------------------------------------------------------------
void Menu::_MenuOpeningAndClosing(Pad& _pad, SoundEffect& _soundEffect)
{
	//	メニューのオンオフ切り替え
	const bool isKeyActive = KEY->Push(KEY_INPUT_R) || KEY->Push(KEY_INPUT_V);
	const bool isButtonActive = (isKeyActive || _pad.GetXInputPushButton(PadInfo::PAD_BUTTON_KIND::BUTTON_START)) && !m_isCountdownStart;
	if (isButtonActive)
	{
		m_isMenuState = !m_isMenuState;

		//	SEの再生
		if (m_isMenuState) { _soundEffect.OnPlaySound(_soundEffect.ONE_SE_KIND::ONE_SE_MENU_OPEN); }
		else { _soundEffect.OnPlaySound(_soundEffect.ONE_SE_KIND::ONE_SE_MENU_CLOSE); }
	}

	const bool isOpen = m_isMenuState && !m_isMenu;
	if (isOpen)
	{
		m_isMenuDraw = true;

		//	下へ移動
		const VECTOR tmpDir = VGet(0.0f, 1.0f, 0.0f);
		const float tmpSpeed = 80.0f;
		VECTOR velocity = VScale(tmpDir, tmpSpeed);
		m_underFlame.m_pos = VAdd(m_underFlame.m_pos, velocity);

		//	最大
		const bool isMaxVal = m_underFlame.m_pos.y >= MAX_FLAME;
		if (isMaxVal)
		{
			m_underFlame.m_pos.y = MAX_FLAME;
			m_isMenu = true;
			m_isSkipUpdate = true;
			m_isOperation = false;
			m_isGoToTitle = false;
			m_countdown = MAX_COUNTDOWN;

		}

		//	相対座標
		m_menuFlame.m_vertexUnder.y = m_underFlame.m_pos.y;
	}

	const bool isClose = !m_isMenuState;
	if (isClose)
	{
		//	上へ移動
		const VECTOR tmpDir = VGet(0.0f, -1.0f, 0.0f);
		const float tmpSpeed = 80.0f;
		VECTOR velocity = VScale(tmpDir, tmpSpeed);
		m_underFlame.m_pos = VAdd(m_underFlame.m_pos, velocity);

		//	最小
		const bool isMinVal = m_underFlame.m_pos.y <= MIN_FLAME;
		if (isMinVal) { m_underFlame.m_pos.y = MIN_FLAME; m_isMenu = false; m_isMenuDraw = false; }

		//	相対座標
		m_menuFlame.m_vertexUnder.y = m_underFlame.m_pos.y;
	}

	//	メニューがオフのときは初期化
	if (!m_isMenu)
	{
		m_modeNumber = (int)MODE::MODE_OPERATION;
		m_watiTime = 0.0f;
		m_isMenu = false;
		m_isOperation = false;
		m_isGoToTitle = false;
		m_isSelectScaleState = false;
		m_select.m_scale = MAX_SELECT_SCALE;
	}
}

//-----------------------------------------------------------------------------
//	@brief	モードの切り替え
//-----------------------------------------------------------------------------
void Menu::_ChangeMode(Pad& _pad, SoundEffect& _soundEffect)
{
	//	メニュー中で操作説明以外のとき
	const bool isActive = m_isMenu && !m_isOperation;
	if (isActive)
	{
		//	モード切替
		m_watiTime--;
		const bool isUp = (_pad.GetXInputStickToDpad(PadInfo::PAD_STICK_TO_DPAD_KIND::LEFT_STICK_TO_DPAD_UP, PadInfo::STICK_DEAD_ZONE) || KEY->Press(KEY_INPUT_W) || KEY->Press(KEY_INPUT_UP)) && m_watiTime <= 0.0f;
		const bool isDown = (_pad.GetXInputStickToDpad(PadInfo::PAD_STICK_TO_DPAD_KIND::LEFT_STICK_TO_DPAD_DOWN, PadInfo::STICK_DEAD_ZONE) || KEY->Press(KEY_INPUT_S) || KEY->Press(KEY_INPUT_DOWN)) && m_watiTime <= 0.0f;
		if (isUp) { m_modeNumber--; m_watiTime = MAX_WAIT_TIME; }
		if (isDown) { m_modeNumber++; m_watiTime = MAX_WAIT_TIME; }

		//	SEの再生
		const bool isOnSe = isUp || isDown;
		if (isOnSe) { _soundEffect.OnPlaySound(_soundEffect.ONE_SE_KIND::ONE_SE_MENU_CURSOR); }

		//	最大最小
		const bool isMinVal = m_modeNumber < MIN_MODE;
		const bool isMaxVal = m_modeNumber > MAX_MODE;
		if (isMinVal) { m_modeNumber = MAX_MODE; }
		if (isMaxVal) { m_modeNumber = MIN_MODE; }
	}
}

//-----------------------------------------------------------------------------
//	@brief	選択位置の変更
//-----------------------------------------------------------------------------
void Menu::_ChangeSelectPos()
{
	switch (m_modeNumber)
	{
		//	操作説明
	case (int)MODE::MODE_OPERATION:
		m_select.m_pos.y = SELECT_POS_1;
		break;

		//	タイトルへ戻る
	case (int)MODE::MODE_TITLE:
		m_select.m_pos.y = SELECT_POS_2;
		break;

		//	ゲームへ戻る
	case (int)MODE::MODE_BACK:
		m_select.m_pos.y = SELECT_POS_3;
		break;
	}
}

//-----------------------------------------------------------------------------
//	@brief	モード
//-----------------------------------------------------------------------------
void Menu::_Mode(Pad& _pad, SoundEffect& _soundEffect)
{
	//	決定
	const bool isKeyActive = KEY->Push(KEY_INPUT_RETURN) || KEY->Push(KEY_INPUT_SPACE) || KEY->Push(KEY_INPUT_X) || KEY->Push(KEY_INPUT_K);
	const bool isButtonActive = isKeyActive || _pad.GetXInputPushButton(PadInfo::PAD_BUTTON_KIND::BUTTON_A);
	const bool isDecision = isButtonActive && m_isMenu;
	if (isDecision)
	{
		switch (m_modeNumber)
		{
			//	操作説明
		case (int)MODE::MODE_OPERATION:
			m_isOperation = !m_isOperation;

			//	SEの再生
			if (m_isOperation) { _soundEffect.OnPlaySound(_soundEffect.ONE_SE_KIND::ONE_SE_MENU_DECISION); }
			else { _soundEffect.OnPlaySound(_soundEffect.ONE_SE_KIND::ONE_SE_MENU_BACK); }

			break;

			//	タイトルへ戻る
		case (int)MODE::MODE_TITLE:
			m_isGoToTitle = true;
			_soundEffect.OnPlaySound(_soundEffect.ONE_SE_KIND::ONE_SE_MENU_DECISION);
			break;

			//	ゲームへ戻る
		case (int)MODE::MODE_BACK:
			m_watiTime = 0.0f;
			m_isMenuState = false;
			m_isOperation = false;
			m_isGoToTitle = false;
			m_isSelectScaleState = false;
			m_select.m_scale = MAX_SELECT_SCALE;
			_soundEffect.OnPlaySound(_soundEffect.ONE_SE_KIND::ONE_SE_MENU_DECISION);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	選択フレームの拡大率
//-----------------------------------------------------------------------------
void Menu::_ChangeSelectScale()
{
	//	拡大率の変更
	if (m_isSelectScaleState) { m_select.m_scale -= SELECT_SCALE_SPEED; }
	else { m_select.m_scale += SELECT_SCALE_SPEED; }

	//	最大最小
	const bool isMinVal = m_select.m_scale <= MIN_SELECT_SCALE;
	const bool isMaxVal = m_select.m_scale >= MAX_SELECT_SCALE;
	if (isMinVal) { m_select.m_scale = MIN_SELECT_SCALE; m_isSelectScaleState = false; }
	if (isMaxVal) { m_select.m_scale = MAX_SELECT_SCALE; m_isSelectScaleState = true; }
}

//-----------------------------------------------------------------------------
//	@brief	カウントダウン
//-----------------------------------------------------------------------------
void Menu::_Countdown(SoundEffect& _soundEffect)
{
	const bool isCoundown = !m_isMenu && m_isSkipUpdate;
	if (isCoundown)
	{
		//	SE
		const bool isSE = m_countdownTime == 1.0f;
		if (isSE) { _soundEffect.OnPlaySound(_soundEffect.ONE_SE_KIND::ONE_SE_MENU_COUNTDOWN); m_isCountdownStart = true; }

		//	カウントダウン時間
		m_countdownTime++;
		const bool isMaxVal = m_countdownTime >= MAX_COUNTDOWN_TIME;
		if (isMaxVal) { m_countdownTime = 0.0f; m_countdown--; }

		//	拡大率を小さくする
		m_countdownUI[m_countdown].m_scale -= COUNTDOWN_SCALE_SPEED;

		//	カウントダウンの終了
		const bool isEndCountdown = m_countdown <= -1;
		if (isEndCountdown) { m_countdown = -1; m_isSkipUpdate = false; m_isCountdownStart = false; }
	}

	if (!m_isSkipUpdate)
	{
		for (int i = 0; i <= COUNTDOWN::COUNTDOWN_NUM; i++)
		{
			m_countdownUI[i].m_scale = 1.5f;
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	メニューの描画
//-----------------------------------------------------------------------------
void Menu::_MenuDraw()
{
	if (!m_isOperation)
	{
		//	選択フレーム
		const bool isSelectUI = m_underFlame.m_pos.y >= m_select.m_pos.y + FONT_SIZE;
		if (isSelectUI) { DrawRotaGraph((int)m_select.m_pos.x, (int)m_select.m_pos.y, (double)m_select.m_scale, 0.0, m_select.m_spriteHendle, TRUE); }

		//	メニューの表示
		//	操作
		const bool isOperationUI = m_underFlame.m_pos.y >= m_mode[MODE::MODE_OPERATION].m_pos.y + FONT_SIZE;
		if (isOperationUI) { DrawGraph((int)m_mode[MODE::MODE_OPERATION].m_pos.x, (int)m_mode[MODE::MODE_OPERATION].m_pos.y, m_mode[MODE::MODE_OPERATION].m_spriteHendle, TRUE); }

		//	タイトルへ
		const bool isTitleUI = m_underFlame.m_pos.y >= m_mode[MODE::MODE_TITLE].m_pos.y + FONT_SIZE;
		if (isTitleUI) { DrawGraph((int)m_mode[MODE::MODE_TITLE].m_pos.x, (int)m_mode[MODE::MODE_TITLE].m_pos.y, m_mode[MODE::MODE_TITLE].m_spriteHendle, TRUE); }

		//	閉じる
		const bool isBackUI = m_underFlame.m_pos.y >= m_mode[MODE::MODE_BACK].m_pos.y + FONT_SIZE;
		if (isBackUI) { DrawGraph((int)m_mode[MODE::MODE_BACK].m_pos.x, (int)m_mode[MODE::MODE_BACK].m_pos.y, m_mode[MODE::MODE_BACK].m_spriteHendle, TRUE); }
	}
}

//-----------------------------------------------------------------------------
//	@brief	操作説明の描画
//-----------------------------------------------------------------------------
void Menu::_OperationDraw()
{
	if (m_isOperation) { DrawGraph((int)m_operation.m_pos.x, (int)m_operation.m_pos.y, m_operation.m_spriteHendle, TRUE); }
}

//-----------------------------------------------------------------------------
//	@brief	カウントダウンの描画
//-----------------------------------------------------------------------------
void Menu::_CountdownDraw()
{
	const bool isCoundown = !m_isMenu && m_isSkipUpdate;
	if (isCoundown)
	{
		DrawRotaGraph((int)m_countdownUI[m_countdown].m_pos.x, (int)m_countdownUI[m_countdown].m_pos.y, (double)m_countdownUI[m_countdown].m_scale, 0.0, m_countdownUI[m_countdown].m_spriteHendle, TRUE);
	}
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放
//-----------------------------------------------------------------------------
void Menu::_FinalRelease()
{
	//	スプライトのアンロード
	DeleteGraph(m_topFlame.m_spriteHendle);
	DeleteGraph(m_underFlame.m_spriteHendle);
	DeleteGraph(m_mode[MODE::MODE_TITLE].m_spriteHendle);
	DeleteGraph(m_mode[MODE::MODE_OPERATION].m_spriteHendle);
	DeleteGraph(m_mode[MODE::MODE_BACK].m_spriteHendle);
	DeleteGraph(m_select.m_spriteHendle);
	DeleteGraph(m_operation.m_spriteHendle);
}
