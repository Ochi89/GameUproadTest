
//=============================================================================
//	@file	Production.h
//	@brief	演出クラス
//	@autor	Takuya Ochi
//	@date	2018/12/24
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
struct Rect;

//-----------------------------------------------------------------------------
//	@brief	演出クラス
//-----------------------------------------------------------------------------
class Production final
{
public:

//===================== 関数 ======================//

	//	デストラクタ
	~Production();

	//	インスタンスの生成
	Production* GetInstance();

	//	更新
	void Update();

	//	描画
	void Draw();

	//	必殺技のリセット
	void ResetSpecialProduction();

	//	すべてのリセット
	void AllReset();

//==================== getter =====================//

	//	ゲーム中のシーン移動の演出の getter
	const bool GetIsSceneProduction() const { return m_isSceneProduction; }

	//	必殺技使用時の演出の getter
	const bool GetIsSpecialProduction() const { return m_isSpecialProduction; }

	//	クリア時の演出の getter
	const bool GetIsClearProduction() const { return m_isClearProduction; }

	//	タイトル時の演出の getter
	const bool GetIsTitleProduction() const { return m_isTitleProduction; }

//==================== setter =====================//

	//	ゲーム中のシーン移動の演出の setter
	void SetIsSceneProduction(const bool _set) { m_isSceneProduction = _set; }

	//	必殺技使用時の演出の setter
	void SetIsSpecialProduction(const bool _set) { m_isSpecialProduction = _set; }

	//	クリア時の演出の setter
	void SetIsClearProduction(const bool _set) { m_isClearProduction = _set; }

	//	タイトル時の演出の setter
	void SetIsTitleProduction(const bool _set) { m_isTitleProduction = _set; }

private:

//================== シングルトン ==================//

	//	コンストラクタ
	Production();

//================== 内部処理関数 ==================//

	//	ゲージの出現
	void _EmergenceGauge();

	//	ゲージを元に戻す
	void _RestoreGauge();

//=================== メンバ変数 ===================//

	static Production	s_instance;						//	インスタンス
	Rect				m_gaugeBar1;					//	演出中のゲージ
	Rect				m_gaugeBar2;					//	演出中のゲージ
	int					m_gaugeColor;					//	ゲージの色
	bool				m_isSceneProduction;			//	ゲーム中のシーン移動の演出中フラグ
	bool				m_isSpecialProduction;			//	必殺技使用時の演出中フラグ
	bool				m_isClearProduction;			//	クリア時の演出中フラグ
	bool				m_isTitleProduction;			//	タイトル時の演出
	float				m_specialProductionTime;		//	必殺技使用時の時間

//===================== 静的定数 ===================//

	static const VECTOR	GAUGE_START_VERTEX_1;		//	ゲージの開始頂点１
	static const VECTOR	GAUGE_START_VERTEX_2;		//	ゲージの開始頂点２
	static const VECTOR	GAUGE_END_VERTEX_1;			//	ゲージの終了頂点１
	static const VECTOR	GAUGE_END_VERTEX_2;			//	ゲージの終了頂点２
	static const float	MAX_Y_AXIS_1;				//	ゲージの最大値
	static const float	MAX_Y_AXIS_2;				//	ゲージの最大値
	static const float	GAUGE_ADD_SPEED;			//	ゲージの加算速度１

};

#define PRODUCTION Production::GetInstance()