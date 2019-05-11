
//=============================================================================
//	@file	Item.h
//	@brief	アイテムひとつ分の処理
//	@autor	Takuya Ochi
//	@date	2018/12/14
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
struct Circle;
struct Rect;

//-----------------------------------------------------------------------------
//	@brief	アイテムクラス
//-----------------------------------------------------------------------------
class Item final
{
public:

//===================== 関数 ======================//

	//	コンストラクタ / デストラクタ
	Item(const int _modelHandle1, const int _modelHandle2, const int _modelHandle3);
	~Item();

	//	初期処理
	void Initialize();

	//	更新処理
	void Update();

	//	描画処理
	void Draw();

//==================== setter =====================//

	//	開始位置の setter
	void SetStartPos(const VECTOR _set) { m_pos = VGet(_set.x, _set.y, 0.0f); }

	//	一度のみの setter
	void SetIsOneTime(const bool _set) { m_isOneTime = _set; }

//==================== getter =====================//

	//	ポジションの getter
	const VECTOR& GetPos() const { return m_pos; }

	//	一度のみの getter
	const bool& GetIsOneTime() const { return m_isOneTime; }

	//	当たり判定用の円形の getter
	const Circle& GetCircle() const { return m_hitCircle; }

	//	当たり判定用の長方形の　setter
	const Rect& GetRect() const { return m_hitRect; };

private:

//================== 内部処理関数 ==================//

	//	移動処理
	void _Move();

	//	回転処理
	void _Rotation();

	//	最終的な解放処理
	void _FinalRelease();

//=================== メンバ変数 ===================//

	int			m_modelHandle1;		//	モデルハンドル
	int			m_modelHandle2;		//	モデルハンドル
	int			m_modelHandle3;		//	モデルハンドル
	VECTOR		m_pos;				//	位置
	VECTOR		m_dir;				//	向き
	bool		m_isOneTime;		//	一度のみ処理
	float		m_radian;			//	角度
	VECTOR		m_angle1;			//	角度１
	VECTOR		m_angle2;			//	角度２

	Circle		m_hitCircle;		//	当たり判定用の円形の構造体
	Rect		m_hitRect;			//	当たり判定用の長方形の構造体

//===================== 静的定数 ===================//

	static const VECTOR		RECT_CORRECTION;	//	座標の補正
	static const float		MOVE_SPEED;			//	移動速度
	static const float		HIT_RADIUS;			//	当たり判定用の半径
	static const float		ANGLE_SPEED_1;		//	回転速度１
	static const float		ANGLE_SPEED_2;		//	回転速度２

};