
//=============================================================================
//	@file	Sound.h
//	@brief	サウンド
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

//-----------------------------------------------------------------------------
//	@brief	サウンドクラス
//-----------------------------------------------------------------------------
class Sound
{
public:

//===================== 関数 ======================//

	//	コンストラクタ / デストラクタ
	Sound(const char* _soundName);
	Sound(const char* _soundName, const char* _nextSoundName);
	Sound(const int _soundHandle);
	~Sound();

	//	初期処理
	void Initialize();
	
	//	更新処理
	void Update();

	//	再生処理
	void OnPlay(const int _playType);

	//	サウンドの切り替え
	void ChangeSound(const int _nextPlayType, const float _nextSoundWaitTime);

//==================== getter =====================//

	//	使用状態フラグの getter
	const bool& GetIsUsedStatus() const { return m_isUsedStatus; }

	//	削除フラグの getter
	const bool& GetIsDelete() const { return m_isDelete; }

	//	サウンドの切り替えフラグの getter
	const bool& GetIsChageSound() const { return m_isChangeSound; }

//==================== setter =====================//

	//	使用状態フラグの setter
	void SetIsUsedStatus(const bool& _set) { m_isUsedStatus = _set; }
	
	//	削除フラグの setter
	void SetIsDelete(const bool& _set) { m_isDelete = _set; }

	//	サウンドの切り替えフラグの setter
	void SetIsChageSound(const bool& _set) { m_isChangeSound = _set; }

private:

//================== 内部処理関数 ==================//

	//	最終的な解放処理
	void _FinalRelease();

//=================== メンバ変数 ===================//

	int		m_soundHandle;			//	サウンドハンドル
	int		m_nextSoundHandle;		//	次のサウンドハンドル
	bool	m_isUsedStatus;			//	使用状態フラグ
	bool	m_isDelete;				//	削除フラグ
	bool	m_isChangeSound;		//	サウンドの切り替えフラグ
	float	m_nextWaitTime;			//	サウンドの遅延時間

//===================== 静的定数 ===================//

	static const int NO_SOUND;	//	音無し

};