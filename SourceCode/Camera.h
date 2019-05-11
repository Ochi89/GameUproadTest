
//=============================================================================
//	@file	Camera.h
//	@brief	カメラ
//	@autor	Takuya Ochi
//	@date	2018/9/29
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	プリプロセッサ
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "DxLib.h"

//-----------------------------------------------------------------------------
//	@brief	前方宣言
//-----------------------------------------------------------------------------
class PlayerManager;

//-----------------------------------------------------------------------------
//	@brief	カメラクラス
//-----------------------------------------------------------------------------
class Camera final
{
public:

//===================== 関数 ======================//

	// コンストラクタ / デストラクタ
	Camera();
	~Camera();

	//	タイトル用の初期処理
	void InitializeForTitle();

	//	初期処理
	void Initialize();

	//	タイトル用の更新処理
	void UpdateForTitle(PlayerManager& _playerManager);

	//	リザルト用の更新処理
	void UpdateForResult(PlayerManager& _playerManager);

	//	更新処理
	void Update(PlayerManager& _playerManager);

//==================== getter =====================//

	//	水平角度の getter
	const float& GetHorizontalAngle() const { return m_horizontalAngle; }

private:

//================== 内部処理関数 ==================//

	//	タイトル用の演出
	void _ProductionForTitle(PlayerManager& _playerManager);

	//	演出
	void _Production(PlayerManager& _playerManager);

	//	シーン演出
	void _SceneProduction(PlayerManager& _playerManager);

	//	必殺演出
	void _SpecialProduction(PlayerManager& _playerManager);

	//	クリア演出
	void _ClearProduction(PlayerManager& _playerManager);

	//	タイトル用の注視点の設定
	void _SetGazingPointForTitle(PlayerManager& _playerManager);

	//	注視点の設定
	void _SetGazingPoint();

	//	カメラの角度によるポジションの変更
	VECTOR _PosAccordingToAngle();

//=================== メンバ変数 ===================//

	VECTOR		m_pos;					//	ポジション
	VECTOR		m_lookAtPos;			//	注視点
	float		m_verticalAngle;		//	垂直角度
	float		m_horizontalAngle;		//	水平角度
	float		m_productionTime;		//	演出時間
	bool		m_isProduction;			//	演出フラグ

//===================== 静的定数 ===================//

	static const float		V_ANGLE_SPEED_MOUSE;	// 垂直角度の回転速度
	static const float		H_ANGLE_SPEED_MOUSE;	// 水平角度の回転速度
	static const float		V_ANGLE_SPEED_PAD;		// 垂直角度の回転速度
	static const float		H_ANGLE_SPEED_PAD;		// 水平角度の回転速度
	static const int		MAX_H_ANGLE;			// 垂直角度の最大
	static const int		MIN_H_ANGLE;			// 垂直角度の最小
	static const int		MAX_V_ANGLE;			// 水平角度の最大
	static const int		MIN_V_ANGLE;			// 水平角度の最小
	static const float		LERP_SPEED;				//	線形保管の速度
	static const float		HEIGHT_LENGTH;			//	高さの長さ
	static const float		DEPTH_LENGTH;			//	奥行きの長さ
};