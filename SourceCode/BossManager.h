
//=============================================================================
//	@file	BassManager.cpp
//	@brief	ボスマネージャー
//	@autor	Takuya Ochi
//	@date	2018/12/18
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
class LastBoss;
class MediumBoss;
class ShotManager;
class SoundEffect;

//-----------------------------------------------------------------------------
//	@brief	ボス管理クラス
//-----------------------------------------------------------------------------
class BossManager final
{
public:

//===================== 関数 ======================//

	//	コンストラクタ / デストラクタ
	BossManager();
	~BossManager();

	//	作成処理
	void Create();

	//	解放処理
	void Release();

	//	初期処理
	void Initialize();

	//	更新処理
	void Update(ShotManager& _shot, SoundEffect& _soundEffect);

	//	描画処理
	void Draw();

//==================== getter =====================//

	//	中ボスへのポインタの getter
	MediumBoss* GetMediumBoss() { return m_mediumBoss.m_pBuf; }

	//	ボスへのポインタの getter
	LastBoss* GetLastBossPtr() { return m_lastBoss.m_pBuf; }

//==================== setter =====================//

	//	出現フラグ
	void SetIsMediumBossEmerge(const bool _set) { m_mediumBoss.m_isEmerge = _set; }

	//	出現フラグ
	void SetIsLastBossEmerge(const bool _set) { m_lastBoss.m_isEmerge = _set; }

private:

//===================== 構造体 =====================//

	//	中ボスバッファ
	struct MediumBossBuffer
	{
		int				m_sourceModelHandle;				//	大本ハンドル
		MediumBoss*		m_pBuf;								//	バッファ
		bool			m_isEmerge;							//	出現フラグ
	};

	//	最終ボスバッファ
	struct LastBossBuffer
	{
		int				m_sourceModelHandle;				//	大本ハンドル
		int				m_coreSourceModelHandle;			//	コアの大本ハンドル
		int				m_sideBarrelSourceModelHandle;		//	サイドバレルの大本ハンドル
		int				m_armSourceModelHandle;				//	アームの大本ハンドル
		LastBoss*		m_pBuf;								//	バッファ
		bool			m_isEmerge;							//	出現フラグ
	};

//================== 内部処理関数 ==================//

	//	最終的な解放処理
	void _FinalRelease();

//=================== メンバ変数 ===================//

	MediumBossBuffer	m_mediumBoss;						//	中ボス
	LastBossBuffer		m_lastBoss;							//	最終ボス

//===================== 静的定数 ===================//

	static const float OUT_RANGE_MEDIUM_BOSS;			//	中ボスの範囲外
	static const float OUT_RANGE_LAST_BOSS;				//	最終ボスの範囲外

};