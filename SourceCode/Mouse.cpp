
//=============================================================================
//	@file	Mouse.h
//	@brief	マウス入力
//	@autor	Takuya Ochi
//	@date	2018/9/28
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "Mouse.h"
#include "Common.h"

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
Mouse::Mouse()
{
	m_pos = CommonConstant::ORIGIN;
	m_newPosX = 0;
	m_newPosY = 0;

	m_prevLeft = 0;
	m_prevRight = 0;

	//	マウスの表示を消す
	SetMouseDispFlag(FALSE);
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
Mouse::~Mouse()
{
	//	処理なし
}

//-----------------------------------------------------------------------------
//	@brief	インスタンス生成
//-----------------------------------------------------------------------------
Mouse* Mouse::GetInstance()
{
	return &s_instance;
}

//-----------------------------------------------------------------------------
//	@brief	更新
//-----------------------------------------------------------------------------
void Mouse::Update()
{
	_Velocity();
}

//-----------------------------------------------------------------------------
//	@brief	押した瞬間
//-----------------------------------------------------------------------------
bool Mouse::Push(const char* _mode)
{
	if (_mode == "LEFT")
	{
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
		{
			if (m_prevLeft == 0)
			{
				//	押された
				m_prevLeft = 1;
				return true;
			}

			return false;
		}
		else
		{
			//	押されていない
			m_prevLeft = 0;
			return false;
		}
	}
	else if (_mode == "RIGHT")
	{
		if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0)
		{
			if (m_prevRight == 0)
			{
				//	押された
				m_prevRight = 1;
				return true;
			}

			return false;
		}
		else
		{
			//	押されていない
			m_prevRight = 0;
			return false;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
//	@brief	押されている
//-----------------------------------------------------------------------------
bool Mouse::Press(const char * _mode)
{
	if (_mode == "LEFT")
	{
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) { return true; }
	}
	else if (_mode == "RIGHT")
	{
		if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0) { return true; }
	}

	return false;
}

//-----------------------------------------------------------------------------
//	@brief	移動量
//-----------------------------------------------------------------------------
void Mouse::_Velocity()
{
	//	マウスの位置を取得
	GetMousePoint(&m_newPosX, &m_newPosY);

	//	マウスの座標の更新
	m_pos.x = (float)(m_newPosX - CommonConstant::WINDOW_WIDTH / 2);
	m_pos.y = (float)(m_newPosY - CommonConstant::WINDOW_HEIGHT / 2);

	//	マウスの座標の初期化
	SetMousePoint(CommonConstant::WINDOW_WIDTH / 2, CommonConstant::WINDOW_HEIGHT / 2);
}
