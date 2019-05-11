
//=============================================================================
//	@file	Shot.h
//	@brief	弾ひとつ分の処理
//	@autor	Takuya Ochi
//	@date	2018/12/12
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
//	@brief	ショットクラス
//-----------------------------------------------------------------------------
class Shot final
{
public:

//===================== 関数 ======================//

	//	コンストラクタ / デストラクタ
	Shot(const int _modelHandle);
	~Shot();
	
	//	更新処理
	void Update();

	//	描画処理
	void Draw();

//==================== setter =====================//

	//	開始位置の setter
	void SetStartPos(const VECTOR _set) { m_pos = _set; }

	//	開始向きの setter
	void SetStartDir(const VECTOR _set) { m_dir = VNorm(_set);}

	//	移動速度の setter
	void SetSpeed(const float _set) { m_speed = _set; }

	//	一度のみの setter
	void SetIsOneTime(const bool _set) { m_isOneTime = _set; }

//==================== getter =====================//

	//	ポジションの getter
	const VECTOR& GetPos() const { return m_pos; }

	//	一度のみの getter
	const bool& GetIsOneTime() const { return m_isOneTime; }

	//	当たり判定用の円形の getter
	const Circle& GetCircle() const { return m_hitCircle; }

	//	当たり判定用の長方形の getter
	const Rect& GetRect() const { return m_hitRect; }

private:

//================== 内部処理関数 ==================//

	//	移動処理
	void _Move();

	//	当たり判定用の点の更新
	void _UpdateHitPoint();

	//	最終的な解放処理
	void _FinalRelease();

//=================== メンバ変数 ===================//

	int			m_modelHandle;		//	モデルハンドル
	VECTOR		m_pos;				//	位置
	VECTOR		m_dir;				//	向き
	float		m_speed;			//	速度
	bool		m_isOneTime;		//	一度のみ処理

	Circle		m_hitCircle;		//	当たり判定用の円形
	Rect		m_hitRect;			//	当たり判定用の長方形

//===================== 静的定数 ===================//

	static const float HIT_RADIUS;			//	当たり判定用の半径
	static const VECTOR		ADJUSTMENT_RECT;	//	当たり判定調整用

};