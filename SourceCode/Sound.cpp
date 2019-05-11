
//=============================================================================
//	@file	Sound.h
//	@brief	サウンドベース
//	@autor	Takuya Ochi
//	@date	2018/1/04
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "Sound.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const int Sound::NO_SOUND = 0;	//	音無し

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
Sound::Sound(const char* _soundName)
{
	//	サウンドの読み込み
	m_soundHandle = LoadSoundMem(_soundName);
	CommonDebug::Assert((m_soundHandle <= -1), " [ Sound.cpp ] : error : sound loading failed.");
	m_isUsedStatus = false;
	m_isDelete = false;
	m_isChangeSound = false;
	m_nextWaitTime = 0.0f;
}

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
Sound::Sound(const char* _soundName, const char* _nextSoundName)
{
	//	サウンドの読み込み
	m_soundHandle = LoadSoundMem(_soundName);
	m_nextSoundHandle = LoadSoundMem(_nextSoundName);
	CommonDebug::Assert((m_soundHandle <= -1), " [ Sound.cpp ] : error : sound loading failed.");
	CommonDebug::Assert((m_nextSoundHandle <= -1), " [ Sound.cpp ] : error : next sound loading failed.");
	m_isUsedStatus = false;
	m_isDelete = false;
	m_isChangeSound = false;
	m_nextWaitTime = 0.0f;
}

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
Sound::Sound(const int _soundHandle)
{
	//	サウンドの読み込み
	m_soundHandle = _soundHandle;
	CommonDebug::Assert((m_soundHandle <= -1), " [ Sound.cpp ] : error : sound loading failed.");
	m_isUsedStatus = false;
	m_isDelete = false;
	m_isChangeSound = false;
	m_nextWaitTime = 0.0f;
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
Sound::~Sound()
{
	//	最終的な解放処理
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	初期処理
//-----------------------------------------------------------------------------
void Sound::Initialize()
{
	m_isUsedStatus = false;
	m_isDelete = false;
	m_isChangeSound = false;
	m_nextWaitTime = 0.0f;
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void Sound::Update()
{
	//	再生していないとき
	const bool isEndSound = CheckSoundMem(m_soundHandle) == NO_SOUND;
	if (isEndSound) { m_isDelete = true; }
}

//-----------------------------------------------------------------------------
//	@brief	再生処理
//-----------------------------------------------------------------------------
void Sound::OnPlay(const int _playType)
{
	//	再生中ではないときは、
	//	再生する
	if (!m_isUsedStatus)
	{
		PlaySoundMem(m_soundHandle, _playType);
		m_isUsedStatus = true;
	}
}

//-----------------------------------------------------------------------------
//	@brief	サウンドの切り替え
//-----------------------------------------------------------------------------
void Sound::ChangeSound(const int _nextPlayType, const float _nextSoundWaitTime)
{
	if (m_isChangeSound)
	{
		//	前のサウンドの停止
		StopSoundMem(m_soundHandle);
		m_isUsedStatus = false;

		//	次のサウンドの開始
		const bool isActive = !m_isUsedStatus && m_nextWaitTime++ >= _nextSoundWaitTime;
		if (isActive)
		{
			PlaySoundMem(m_nextSoundHandle, _nextPlayType);
			m_isUsedStatus = true;
			m_isChangeSound = false;
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放処理
//-----------------------------------------------------------------------------
void Sound::_FinalRelease()
{
	//	サウンドのアンロード
	DeleteSoundMem(m_soundHandle);
	DeleteSoundMem(m_nextSoundHandle);
}
