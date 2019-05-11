
//=============================================================================
//	@file	CharacterBase.h
//	@brief	キャラクターベース
//	@autor	Takuya Ochi
//	@date	2018/11/14
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
//	@brief	キャラクター基底クラス
//-----------------------------------------------------------------------------
class CharacterBase
{
public:

//===================== 関数 ======================//

	//	コンストラクタ / デストラクタ
	CharacterBase();
	virtual ~CharacterBase();

	//	作成処理gett
	virtual void Create() = 0;

	//	解放処理
	virtual void Release() = 0;

	//	初期処理
	virtual void Initialize() = 0;

	//	描画処理
    virtual void Draw() = 0;

//==================== getter =====================//

	//	モデルハンドルの getter
	const int& GetModelHandle() const { return m_modelHandle; }

	//	ポジションの getter
	const VECTOR& GetPos() const { return m_pos; }

	//	ディレクションの getter
	const VECTOR& GetDir() const { return m_dir; }

	//	アングルの getter
	const VECTOR& GetAngle() const { return m_angle; }

	//	サイズの getter
	const VECTOR& GetSize() const { return m_size; }

	//	当たり判定用の円形の　getter
	const Circle& GetCircle() const { return m_hitCircle; };

	//	当たり判定用の長方形の　getter
	const Rect& GetRect() const { return m_hitRect; };

//==================== setter =====================//

	//	ポジションの setter
	const void SetPos(const VECTOR& _set) { m_pos = _set; }

	//	ディレクションの setter
	const void SetDir(const VECTOR& _set) { m_dir = _set; }

	//	アングルの setter
	const void SetAngle(const VECTOR& _set) { m_angle = _set; }

	//	サイズの setter
	const void SetSize(const VECTOR& _set) { m_size = _set; }
	
protected:
	
//================== 内部処理関数 ==================//

	//	最終的な解放処理
	virtual void _FinalRelease() = 0;

//=================== メンバ変数 ===================//
	
	int			m_modelHandle;		//	モデルハンドル
	VECTOR		m_pos;				//	ポジション
	VECTOR		m_dir;				//	ディレクション
	VECTOR		m_angle;			//	アングル
	VECTOR		m_size;				//	サイズ

	Circle		m_hitCircle;		//	当たり判定用の円の構造体
	Rect		m_hitRect;			//	当たり判定用の長方形の構造体

	float		m_shotWaitTime;		//	弾の遅延時間

};
