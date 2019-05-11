
//=============================================================================
//	@file	EnemyBase.h
//	@brief	エネミーベース
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
#include "CharacterBase.h"

//-----------------------------------------------------------------------------
//	@brief	前方宣言
//-----------------------------------------------------------------------------
class PlayerManager;
class ShotManager;
class EffekseerEmitter;
class SoundEffect;

//-----------------------------------------------------------------------------
//	@brief	エネミー基底クラス
//-----------------------------------------------------------------------------
class EnemyBase : public CharacterBase
{
public:

//===================== 関数 ======================//

	//	コンストラクタ / デストラクタ
	EnemyBase(const int _modelHandle);
	virtual ~EnemyBase();

	//	作成処理
	virtual void Create() override = 0;

	//	解放処理
	virtual void Release() override = 0;

	//	初期処理
	virtual void Initialize() override = 0;

	//	更新処理
	virtual void Update(PlayerManager& _playerManager, ShotManager& _shotManager, SoundEffect& _soundEffect) = 0;

	//	描画処理
	virtual void Draw() override final;

	//	生存時間の加算
	void AddAliveTime() { m_aliveTime++; }

//==================== getter =====================//

	//	敵IDの getter
	const int GetFormationID() const { return m_formationID; }

	//	敵IDの getter
	const int GetEntryID() const { return m_entryID; }

	//	敵種の getter
	const int GetEntryKind() const { return m_entryKind; }

	//	生存時間の getter
	const float GetAliveTime() const { return m_aliveTime; }

	//	アイテムを持っているかの getter
	const bool& GetIsHavingItems() const { return m_isHavingItems; }

	//	ヒットフラグの getter
	const bool& GetIsHIt() const { return m_isHit; }

	//	削除フラグの getter
	const bool& GetIsDeleate() const { return m_isDeleate; }

//==================== setter =====================//

	//	敵IDの setter
	void SetFormationID(const int _set) { m_formationID = _set; }

	//	敵IDの setter
	void SetEntryID(const int _set) { m_entryID = _set; }

	//	敵種の setter
	void SetEntryKind(const int _set) { m_entryKind = _set; }
	
	//	アイテムを持っているかの setter
	void SetIsHavingItems(const bool _set) { m_isHavingItems = _set; }

	//	ショットを使うかの setter
	void SetIsUseShot(const bool _set) { m_isUseShot = _set; }

	//	ヒットフラグの setter
	void SetIsHit(const bool _set) { m_isHit = _set; }

protected:

//================== 内部処理関数 ==================//

	//	向きの切り替え処理
	virtual void _ChangeDir() = 0;

	//	色の変更
	void _ChangeColor();

	//	ショットの登録
	void _ShotEntry(PlayerManager& _playerManager, ShotManager& _shotManager);

	//	爆発エフェクト
	void _OnEffectExplosion(SoundEffect& _soundEffect);

	//	当たり判定用の点の更新
	virtual void _UpdateHitPoint() = 0;

	//	最終的な解放処理
	virtual void _FinalRelease() = 0;

//=================== メンバ変数 ===================//

	float	m_behaviorTime;		//	行動時間
	float	m_moveSpeed;		//	移動速度
	float	m_aliveTime;		//	生成してからの時間
	bool	m_isUseShot;		//	ショットを使うかのフラグ
	bool	m_isHavingItems;	//	アイテムを持っているかのフラグ
	int		m_formationID;		//	隊列の番号
	int		m_entryID;			//	使用している敵ID
	int		m_entryKind;		//	使用している敵種
	bool	m_isAttenuation;	//	減衰フラグ
	bool	m_isHit;			//	ヒットフラグ
	bool	m_isOffDraw;		//	描画しないフラグ
	bool	m_isDeleate;		//	削除フラグ

	//	エフェクト関係
	EffekseerEmitter*	m_effectExplosion;	//	爆発エフェクト

//===================== 静的定数 ===================//

	static const VECTOR		RECT_CORRECTION;	//	長方形の補正
	static const float		SHOT_SPEED;			//	ショット速度
	static const float		MAX_WAIT_TIME;		//	ショットの遅延時間
	static const float		HIT_RADIUS;			//	当たり判定用の半径
	static const float		CENTER_CORRECTION;	//	中心座標の補正

private:

//================== 内部処理関数 ==================//

	//	減色設定
	void _AttenuationColor();

//=================== メンバ変数 ===================//

	COLOR_F		m_color;			//	モデルの色

//===================== 静的定数 ===================//

	static const COLOR_F	COLOR_NORMAL;		//	デフォルト色
	static const COLOR_F	COLOR_DECAY_NORMAL;	//	デフォルト色の減衰
	static const COLOR_F	COLOR_RED;			//	赤色
	static const COLOR_F	COLOR_DECAY_RED;	//	赤色の減衰

};