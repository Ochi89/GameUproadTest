
//=============================================================================
//	@file	EffekseerEmitter.h
//	@brief	EffekseerEmitter
//	@autor	Takuya Ochi
//	@date	2018/12/26
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	プリプロセッサ
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "EffekseerForDXLib.h"
#include "Common.h"

//-----------------------------------------------------------------------------
//	@brief	EffekseerEmitterクラス
//-----------------------------------------------------------------------------
class EffekseerEmitter final
{
public:

//===================== 関数 ======================//

	//	コンストラクタ / デストラクタ
	EffekseerEmitter(const char* _fileName);
	~EffekseerEmitter();

	//	エフェクトの再生処理
	void OnPlayEffect();

	//	エフェクトの終了処理
	void OnEndEffect();

	//	エフェクトの更新処理
	void Update();

	//	再生時間の加算
	void AddPlayTime() { m_playTime++; }

//==================== setter =====================//

	//	ポジションの setter
	void SetPos(const VECTOR _set) { m_pos = _set; m_isUsePos = true; }

	//	角度の setter
	void SetAngle(const VECTOR _set) { m_angle = _set; m_isUseAngle = true; }

	//	比率の setter
	void SetScale(const VECTOR _set) { m_scale = _set; m_isUseScale = true; }

	//	再生速度の setter
	void SetPlaySpeed(const float _set) { m_speed = _set; m_isUseSpeed = true; }

	//	再生中かの setter
	void SetIsPlayEffect(const bool _set) { m_isPlayEffect = _set; }

	//	開始するかの setter
	void SetIsStartEffect(const bool _set) { m_isStartEffect = _set; }

	//	再生時間の setter
	void SetPlayTime(const float _set) { m_playTime = _set; }

//==================== getter =====================//
	
	//	再生速度の getter
	const float GetPlaySpeed() const;

	//	再生時間の getter
	const float& GetPlayTime() const { return m_playTime; }

	//	再生中かの getter
	const bool& GetIsPlayEffect() const { return m_isPlayEffect; }

	//	開始するかの getter
	const bool& GetIsStartEffect() const { return m_isStartEffect; }

private:

//================== 内部処理関数 ==================//

	//	最終的な解放処理
	void _FinalRelease();

//=================== メンバ変数 ===================//

	int			m_effectResourceHandle;		//	エフェクトリソースハンドル
	int			m_playingEffectHandle;		//	再生中のエフェクトのハンドル
	VECTOR		m_pos;						//	ポジション
	VECTOR		m_scale;					//	拡大率
	VECTOR		m_angle;					//	角度
	float		m_speed;					//	速度
	bool		m_isUsePos;					//	ポジションの設定を使う
	bool		m_isUseAngle;				//	角度の設定を使う
	bool		m_isUseScale;				//	拡大率の設定を使う
	bool		m_isUseSpeed;				//	速度の設定を使う
	bool		m_isPlayEffect;				//	再生中かのフラグ
	bool		m_isStartEffect;			//	開始するかのフラグ
	float		m_playTime;					//	エフェクトの再生時間
	bool		m_isPrevConditions;			//	前の条件の保存

//===================== 静的定数 ===================//

};