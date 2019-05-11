
//=============================================================================
//	@file	SoundManager.cpp
//	@brief	サウンドマネージャー
//	@autor	Takuya Ochi
//	@date	2018/1/04
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "SoundEffect.h"
#include "Sound.h"

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
SoundEffect::SoundEffect()
{
	//	メモリの確保をすると、使用フラグの初期化
	for (int i = 0; i < SE_KIND::SE_NUM; i++)
	{
		for (int j = 0; j < CommonConstant::MAX_ENEMY_SE_NUM; j++)
		{
			m_seBuffer[i].m_pBuf[j] = NULL;
			m_seBuffer[i].m_isInUse[j] = false;
		}

		m_sourceSoundHandle[i] = -1;
	}
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
SoundEffect::~SoundEffect()
{
	//	最終的な解放処理
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成処理
//-----------------------------------------------------------------------------
void SoundEffect::Create()
{
	//	大本の音源の読み込み
	m_sourceSoundHandle[SE_KIND::SE_ITEM] = LoadSoundMem("Data/Sound/SE/GetItem.mp3");
	m_sourceSoundHandle[SE_KIND::SE_SHOT] = LoadSoundMem("Data/Sound/SE/Shot.mp3");
	m_sourceSoundHandle[SE_KIND::SE_MOVE_SHOT] = LoadSoundMem("Data/Sound/SE/MoveShot.mp3");
	m_sourceSoundHandle[SE_KIND::SE_STAY_SHOT] = LoadSoundMem("Data/Sound/SE/StayShot.mp3");
	m_sourceSoundHandle[SE_KIND::SE_HIT] = LoadSoundMem("Data/Sound/SE/Hit.mp3");
	m_sourceSoundHandle[SE_KIND::SE_EXPLOSION] = LoadSoundMem("Data/Sound/SE/Explosion.mp3");

	for (int i = 0; i < CommonConstant::MAX_ENEMY_SE_NUM; i++)
	{
		m_seBuffer[SE_KIND::SE_ITEM].m_pBuf[i] = new Sound(m_sourceSoundHandle[SE_KIND::SE_ITEM]);
		m_seBuffer[SE_KIND::SE_SHOT].m_pBuf[i] = new Sound(m_sourceSoundHandle[SE_KIND::SE_SHOT]);
		m_seBuffer[SE_KIND::SE_MOVE_SHOT].m_pBuf[i] = new Sound(m_sourceSoundHandle[SE_KIND::SE_MOVE_SHOT]);
		m_seBuffer[SE_KIND::SE_STAY_SHOT].m_pBuf[i] = new Sound(m_sourceSoundHandle[SE_KIND::SE_STAY_SHOT]);
		m_seBuffer[SE_KIND::SE_HIT].m_pBuf[i] = new Sound(m_sourceSoundHandle[SE_KIND::SE_HIT]);
		m_seBuffer[SE_KIND::SE_EXPLOSION].m_pBuf[i] = new Sound(m_sourceSoundHandle[SE_KIND::SE_EXPLOSION]);
	}

	//	大本の音源の読み込みs
	m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_SELECT] = LoadSoundMem("Data/Sound/SE/Select.mp3");
	m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_PLAYER_LASER] = LoadSoundMem("Data/Sound/SE/PlayerLaser.mp3");
	m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_ENEMY_LASER] = LoadSoundMem("Data/Sound/SE/EnemyLaser.mp3");
	m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_ROCKET] = LoadSoundMem("Data/Sound/SE/Rocket.mp3");
	m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_WARNING] = LoadSoundMem("Data/Sound/SE/Warning.mp3");
	m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_TITLE] = LoadSoundMem("Data/Sound/SE/Title.mp3");
	m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_MENU_OPEN] = LoadSoundMem("Data/Sound/SE/Open.mp3");
	m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_MENU_CLOSE] = LoadSoundMem("Data/Sound/SE/Close.mp3");
	m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_MENU_BACK] = LoadSoundMem("Data/Sound/SE/Back.mp3");
	m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_MENU_CURSOR] = LoadSoundMem("Data/Sound/SE/Cursor.mp3");
	m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_MENU_DECISION] = LoadSoundMem("Data/Sound/SE/Decision.mp3");
	m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_MENU_COUNTDOWN] = LoadSoundMem("Data/Sound/SE/Countdown.mp3");
	m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_GAUGE_MAX] = LoadSoundMem("Data/Sound/SE/GaugeMax.mp3");
	m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_STAY_SHOT_GAUGE_MAX] = LoadSoundMem("Data/Sound/SE/StayShotGaugeMax.mp3");
	m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_WIND] = LoadSoundMem("Data/Sound/SE/Wind.mp3");
	m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_SCORE_UP] = LoadSoundMem("Data/Sound/SE/ScoreUp.mp3");

	m_oneSeBuffer[ONE_SE_KIND::ONE_SE_SELECT].m_pBuf = new Sound(m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_SELECT]);
	m_oneSeBuffer[ONE_SE_KIND::ONE_SE_PLAYER_LASER].m_pBuf = new Sound(m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_PLAYER_LASER]);
	m_oneSeBuffer[ONE_SE_KIND::ONE_SE_ENEMY_LASER].m_pBuf = new Sound(m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_ENEMY_LASER]);
	m_oneSeBuffer[ONE_SE_KIND::ONE_SE_ROCKET].m_pBuf = new Sound(m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_ROCKET]);
	m_oneSeBuffer[ONE_SE_KIND::ONE_SE_WARNING].m_pBuf = new Sound(m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_WARNING]);
	m_oneSeBuffer[ONE_SE_KIND::ONE_SE_TITLE].m_pBuf = new Sound(m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_TITLE]);
	m_oneSeBuffer[ONE_SE_KIND::ONE_SE_MENU_OPEN].m_pBuf = new Sound(m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_MENU_OPEN]);
	m_oneSeBuffer[ONE_SE_KIND::ONE_SE_MENU_CLOSE].m_pBuf = new Sound(m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_MENU_CLOSE]);
	m_oneSeBuffer[ONE_SE_KIND::ONE_SE_MENU_BACK].m_pBuf = new Sound(m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_MENU_BACK]);
	m_oneSeBuffer[ONE_SE_KIND::ONE_SE_MENU_CURSOR].m_pBuf = new Sound(m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_MENU_CURSOR]);
	m_oneSeBuffer[ONE_SE_KIND::ONE_SE_MENU_DECISION].m_pBuf = new Sound(m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_MENU_DECISION]);
	m_oneSeBuffer[ONE_SE_KIND::ONE_SE_MENU_COUNTDOWN].m_pBuf = new Sound(m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_MENU_COUNTDOWN]);
	m_oneSeBuffer[ONE_SE_KIND::ONE_SE_GAUGE_MAX].m_pBuf = new Sound(m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_GAUGE_MAX]);
	m_oneSeBuffer[ONE_SE_KIND::ONE_SE_STAY_SHOT_GAUGE_MAX].m_pBuf = new Sound(m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_STAY_SHOT_GAUGE_MAX]);
	m_oneSeBuffer[ONE_SE_KIND::ONE_SE_WIND].m_pBuf = new Sound(m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_WIND]);
	m_oneSeBuffer[ONE_SE_KIND::ONE_SE_SCORE_UP].m_pBuf = new Sound(m_sourceOneSoundHandle[ONE_SE_KIND::ONE_SE_SCORE_UP]);
}

//-----------------------------------------------------------------------------
//	@brief	解放処理
//-----------------------------------------------------------------------------
void SoundEffect::Release()
{
	//	リスト内に登録されているとき、クリアする
	if (!m_useList.empty()) { m_useList.clear(); }

	for (int i = 0; i < SE_KIND::SE_NUM; i++)
	{
		for (int j = 0; j < CommonConstant::MAX_ENEMY_SE_NUM; j++)
		{
			//	メモリの解放
			CommonSafe::Delete(m_seBuffer[i].m_pBuf[j]);
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void SoundEffect::Update()
{
	//	リストの更新
	_ListUpdate();

	//	リストから削除
	_RemoveFromList();
}

//-----------------------------------------------------------------------------
//	@brief	サウンドの再生
//-----------------------------------------------------------------------------
void SoundEffect::OnPlaySound(const SE_KIND _kind)
{
	for (int i = 0; i < CommonConstant::MAX_ENEMY_SE_NUM; i++)
	{
		//	まだ使用していないとき
		const bool isNoUse = !m_seBuffer[_kind].m_pBuf[i]->GetIsUsedStatus();
		if (isNoUse)
		{
			//	使用リストに登録
			m_useList.push_back(m_seBuffer[_kind].m_pBuf[i]);
			return;
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	サウンドの再生
//-----------------------------------------------------------------------------
void SoundEffect::OnPlaySound(const ONE_SE_KIND _kind)
{
	//	まだ使用していないとき
	const bool isNoUse = !m_oneSeBuffer[_kind].m_pBuf->GetIsUsedStatus();
	if (isNoUse)
	{
		//	使用リストに登録
		m_useList.push_back(m_oneSeBuffer[_kind].m_pBuf);
		return;
	}
}

//-----------------------------------------------------------------------------
//	@brief	リストの更新
//-----------------------------------------------------------------------------
void SoundEffect::_ListUpdate()
{
	//	使用リストに登録されているとき
	const bool isAlreadyEntry = !m_useList.empty();
	if (isAlreadyEntry)
	{
		//	リストの回数分回す
		std::list<Sound*>::iterator ite;
		for (ite = m_useList.begin(); ite != m_useList.end(); ite++)
		{
			//	SE再生
			(*ite)->OnPlay(DX_PLAYTYPE_BACK);

			//	更新
			(*ite)->Update();
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	リストから解放
//-----------------------------------------------------------------------------
void SoundEffect::_RemoveFromList()
{
	//	使用リストに登録されているとき
	const bool isAlreadyEntry = !m_useList.empty();
	if (isAlreadyEntry)
	{
		//	リストの回数分回す
		std::list<Sound*>::iterator ite;
		for (ite = m_useList.begin(); ite != m_useList.end(); ite++)
		{
			//	削除
			const bool isDelete = (*ite)->GetIsDelete();
			if (isDelete)
			{
				//	初期化
				(*ite)->Initialize();

				//	使用リストから削除
				m_useList.remove(*ite);
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放処理
//-----------------------------------------------------------------------------
void SoundEffect::_FinalRelease()
{
	//	リスト内に登録されているとき、クリアする
	if (!m_useList.empty()) { m_useList.clear(); }

	for (int i = 0; i < SE_KIND::SE_NUM; i++)
	{
		for (int j = 0; j < CommonConstant::MAX_ENEMY_SE_NUM; j++)
		{
			//	メモリの解放
			CommonSafe::Delete(m_seBuffer[i].m_pBuf[j]);
		}
	}
}
