
//=============================================================================
//	@file	SoundEffect.h
//	@brief	サウンドエフェクト
//	@autor	Takuya Ochi
//	@date	2018/1/04
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
#include <list>

//-----------------------------------------------------------------------------
//	@brief	前方宣言
//-----------------------------------------------------------------------------
class Sound;

//-----------------------------------------------------------------------------
//	@brief	サウンドエフェクトクラス
//-----------------------------------------------------------------------------
class SoundEffect final
{
public:
	
//==================== 列挙体 =====================//

	//	SEの種類
	enum SE_KIND
	{
		SE_ITEM,			//	アイテム取得SE
		SE_SHOT,			//	ショットSE
		SE_MOVE_SHOT,		//	滞在ショットSE１
		SE_STAY_SHOT,		//	滞在ショットSE２
		SE_HIT,				//	ヒットSE
		SE_EXPLOSION,		//	爆発SE
		SE_NUM,				//	SEの数
	};

	//	SEの種類
	enum ONE_SE_KIND
	{
		ONE_SE_SELECT,				//	セレクトSE
		ONE_SE_PLAYER_LASER,		//	プレイヤーレーザーSE
		ONE_SE_ENEMY_LASER,			//	エネミーレーザーSE
		ONE_SE_ROCKET,				//	ロケットSE
		ONE_SE_WARNING,				//	警告音SE
		ONE_SE_TITLE,				//	タイトルSE
		ONE_SE_MENU_OPEN,			//	メニュー開くSE
		ONE_SE_MENU_CLOSE,			//	メニュー閉じるSE
		ONE_SE_MENU_BACK,			//	メニューバックSE
		ONE_SE_MENU_CURSOR,			//	メニューカーソルSE
		ONE_SE_MENU_DECISION,		//	メニュー決定SE
		ONE_SE_MENU_COUNTDOWN,		//	メニューカウントダウンSE
		ONE_SE_GAUGE_MAX,			//	ゲージ最大SE
		ONE_SE_STAY_SHOT_GAUGE_MAX,	//	滞在ショットのゲージ最大SE
		ONE_SE_WIND,				//	演出の風SE
		ONE_SE_SCORE_UP,				//	スコアアップSE
		ONE_SE_NUM,					//	SEの数
	};

//===================== 関数 ======================//

	//	コンストラクタ / デストラクタ
	SoundEffect();
	~SoundEffect();

	//	作成処理
	void Create();

	//	解放処理
	void Release();

	//	更新処理
	void Update();

	//	再生処理
	void OnPlaySound(const SE_KIND _kind);

	//	再生処理
	void OnPlaySound(const ONE_SE_KIND _kind);

private:

//==================== 構造体 =====================//

	//	SEバッファ
	struct SeBuffer
	{
		Sound*	m_pBuf[CommonConstant::MAX_ENEMY_SE_NUM];		//	バッファ
		bool	m_isInUse[CommonConstant::MAX_ENEMY_SE_NUM];	//	メモリの使用状況
	};

	//	SEバッファ
	struct OneSeBuffer
	{
		Sound*	m_pBuf;			//	バッファ
		bool	m_isInUse;		//	メモリの使用状況
	};

//================== 内部処理関数 ==================//
	
	//	リストの更新
	void _ListUpdate();

	//	リストから解放
	void _RemoveFromList();

	//	最終的な解放処理
	void _FinalRelease();

//=================== メンバ変数 ===================//

	int					m_sourceSoundHandle[SE_KIND::SE_NUM];				//	サウンドハンドル
	int					m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_NUM];	//	サウンドハンドル
	SeBuffer			m_seBuffer[SE_KIND::SE_NUM];						//	登録バッファ
	OneSeBuffer			m_oneSeBuffer[ONE_SE_KIND::ONE_SE_NUM];				//	登録バッファ
	std::list<Sound*>	m_useList;											//	使用エネミーリスト

};