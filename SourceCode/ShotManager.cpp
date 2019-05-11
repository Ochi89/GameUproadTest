
//=============================================================================
//	@file	ShotManager.h
//	@brief	ショットマネージャー
//	@autor	Takuya Ochi
//	@date	2018/12/12
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	プリプロセッサ
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "ShotManager.h"
#include "Shot.h"
#include "SoundEffect.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const float ShotManager::MAX_X_POS = 110.0f;	//	X座標の最大
const float ShotManager::MIN_X_POS = -110.0f;	//	X座標の最小
const float ShotManager::MAX_Y_POS = 120.0f;	//	Y座標の最大
const float ShotManager::MIN_Y_POS = 0.0f;		//	Y座標の最小

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
ShotManager::ShotManager()
{
	//	まだどこも指していないので、NULLで初期化
	for (int i = 0; i < CommonConstant::MAX_SHOT_NUM; i++)
	{
		m_pPlayerShot[i] = NULL;
		m_pEnemyShot[i] = NULL;
	}

	//	各変数の初期化
	m_sourcePlayerShotModelHandle = -1;
	m_sourceEnemyShotModelHandle = -1;
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
ShotManager::~ShotManager()
{
	//	最終的な解放処理
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	作成処理
//-----------------------------------------------------------------------------
void ShotManager::Create()
{
	//	大本モデルのロード
	m_sourcePlayerShotModelHandle = MV1LoadModel("Data/Model/Player/PlayerShot.mqo");
	m_sourceEnemyShotModelHandle = MV1LoadModel("Data/Model/Enemy/EnemyShot.mqo");

	CommonDebug::Assert((m_sourcePlayerShotModelHandle <= -1), " [ ShotManager.cpp ] : error : model loading failed.");
	CommonDebug::Assert((m_sourceEnemyShotModelHandle <= -1), " [ ShotManager.cpp ] : error : model loading failed.");

	//	初期処理
	for (int i = 0; i < CommonConstant::MAX_SHOT_NUM; i++)
	{
		//	ショットの作成
		m_pPlayerShot[i] = new Shot(m_sourcePlayerShotModelHandle);
		m_pEnemyShot[i] = new Shot(m_sourceEnemyShotModelHandle);

		//	未使用リストに登録
		m_unusedPlayerShotList.push_back(m_pPlayerShot[i]);
		m_unusedEnemyShotList.push_back(m_pEnemyShot[i]);
	}
}

//-----------------------------------------------------------------------------
//	@brief	解放処理
//-----------------------------------------------------------------------------
void ShotManager::Release()
{
	//	大本モデルのアンロード
	MV1DeleteModel(m_sourcePlayerShotModelHandle);
	MV1DeleteModel(m_sourceEnemyShotModelHandle);

	//	リストに登録されていたら削除
	if (!m_unusedPlayerShotList.empty()) { m_unusedPlayerShotList.clear(); }
	if (!m_unusedEnemyShotList.empty()) { m_unusedEnemyShotList.clear(); }
	if (!m_usePlayerShotList.empty()) { m_usePlayerShotList.clear(); }
	if (!m_useEnemyShotList.empty()) { m_useEnemyShotList.clear(); }

	//	ショットの解放
	for (int i = 0; i < CommonConstant::MAX_SHOT_NUM; i++)
	{
		CommonSafe::Delete(m_pPlayerShot[i]);
		CommonSafe::Delete(m_pEnemyShot[i]);
	}
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void ShotManager::Update()
{
	//	必殺技のときは更新しない
	const bool isNotSpecialProduction = !PRODUCTION->GetIsSpecialProduction();
	if (isNotSpecialProduction)
	{
		//	リストの更新
		_UpdateList(SHOT_KIND::PLAYER_SHOT);
		_UpdateList(SHOT_KIND::ENEMY_SHOT);

		//	解放処理
		_RemoveFromList(SHOT_KIND::PLAYER_SHOT);
		_RemoveFromList(SHOT_KIND::ENEMY_SHOT);
	}
}

//-----------------------------------------------------------------------------
//	@brief	描画処理
//-----------------------------------------------------------------------------
void ShotManager::Draw()
{
	_DrawList(SHOT_KIND::PLAYER_SHOT);
	_DrawList(SHOT_KIND::ENEMY_SHOT);
}

//-----------------------------------------------------------------------------
//	@brief	リストに登録
//-----------------------------------------------------------------------------
void ShotManager::RegisterOnList(const SHOT_KIND _shotKind, const VECTOR _startPos, const VECTOR _startDir, const float _speed, SoundEffect& _soundEffect)
{
	//	未使用リストから使用リストに登録
	switch (_shotKind)
	{
	case SHOT_KIND::PLAYER_SHOT:
		_RegisterOnList(m_usePlayerShotList, m_unusedPlayerShotList, _startPos, _startDir, _speed, _soundEffect);
		break;

	case SHOT_KIND::ENEMY_SHOT:
		_RegisterOnList(m_useEnemyShotList, m_unusedEnemyShotList, _startPos, _startDir, _speed, _soundEffect);
		break;
	}
}

//-----------------------------------------------------------------------------
//	@brief	リストに登録
//-----------------------------------------------------------------------------
void ShotManager::RegisterOnList(const SHOT_KIND _shotKind, const VECTOR _startPos, const VECTOR _startDir, const float _speed)
{
	//	未使用リストから使用リストに登録
	switch (_shotKind)
	{
	case SHOT_KIND::PLAYER_SHOT:
		_RegisterOnList(m_usePlayerShotList, m_unusedPlayerShotList, _startPos, _startDir, _speed);
		break;

	case SHOT_KIND::ENEMY_SHOT:
		_RegisterOnList(m_useEnemyShotList, m_unusedEnemyShotList, _startPos, _startDir, _speed);
		break;
	}
}

//-----------------------------------------------------------------------------
//	@brief	リストから削除
//-----------------------------------------------------------------------------
void ShotManager::RemoveFromList(const SHOT_KIND _shotKind, const Shot* _shotPtr)
{
	//	使用リストから削除
	switch (_shotKind)
	{
	case SHOT_KIND::PLAYER_SHOT:
		_RemoveFromList(m_usePlayerShotList, m_unusedPlayerShotList, _shotPtr);
		break;

	case SHOT_KIND::ENEMY_SHOT:
		_RemoveFromList(m_useEnemyShotList, m_unusedEnemyShotList, _shotPtr);
		break;
	}
}

//-----------------------------------------------------------------------------
//	@brief	リストのサイズの取得
//-----------------------------------------------------------------------------
const int ShotManager::GetListSize(const SHOT_KIND _shotKind) const
{
	//	リストのサイズの取得
	switch (_shotKind)
	{
	case SHOT_KIND::PLAYER_SHOT:
		return m_usePlayerShotList.size();

	case SHOT_KIND::ENEMY_SHOT:
		return m_useEnemyShotList.size();
	}

	return 0;
}

//-----------------------------------------------------------------------------
//	@brief	ショットへのポインタの取得
//-----------------------------------------------------------------------------
Shot* ShotManager::GetShotPtr(const SHOT_KIND _shotKind, const int _num)
{
	//	ショットへのポインタの取得
	switch (_shotKind)
	{
	case SHOT_KIND::PLAYER_SHOT:
		return _GetShotPtr(m_usePlayerShotList, _num);

	case SHOT_KIND::ENEMY_SHOT:
		return _GetShotPtr(m_useEnemyShotList, _num);
	}

	return NULL;
}

//-----------------------------------------------------------------------------
//	@brief	ショットへのポインタの取得
//-----------------------------------------------------------------------------
Shot* ShotManager::_GetShotPtr(std::list<Shot*>& _useList, const int _num)
{
	int counter = 0;

	//	使用リストが空でないとき
	if (!_useList.empty())
	{
		//	リストの回数分回す
		std::list<Shot*>::iterator 	ite;
		for (ite = _useList.begin(); ite != _useList.end(); ite++)
		{
			//	_num と一致したポインタを返す
			const bool isSame = counter == _num;
			if (isSame) { return (*ite); }
			counter++;
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------
//	@brief	リストに登録
//-----------------------------------------------------------------------------
void ShotManager::_RegisterOnList(std::list<Shot*>& _useList, std::list<Shot*>& _unusedList, const VECTOR _startPos, const VECTOR _startDir, const float _speed, SoundEffect& _soundEffect)
{
	//	未使用リストが空でないとき
	if (!_unusedList.empty())
	{
		//	未使用リストの先頭のイテレーターを取る
		std::list<Shot*>::iterator 	ite;
		ite = _unusedList.begin();

		//	まだ向きを設定していないときは設定する
		if (!(*ite)->GetIsOneTime())
		{
			//	内容の初期化
			(*ite)->SetStartPos(_startPos);
			(*ite)->SetStartDir(_startDir);
			(*ite)->SetSpeed(_speed);
			(*ite)->SetIsOneTime(true);
			_soundEffect.OnPlaySound(_soundEffect.SE_KIND::SE_SHOT);
		}

		//	未使用リストから使用リストに登録
		_useList.push_back(*ite);

		//	未使用リストから削除
		_unusedList.remove(*ite);
	}
}

//-----------------------------------------------------------------------------
//	@brief	リストに登録
//-----------------------------------------------------------------------------
void ShotManager::_RegisterOnList(std::list<Shot*>& _useList, std::list<Shot*>& _unusedList, const VECTOR _startPos, const VECTOR _startDir, const float _speed)
{
	//	未使用リストが空でないとき
	if (!_unusedList.empty())
	{
		//	未使用リストの先頭のイテレーターを取る
		std::list<Shot*>::iterator 	ite;
		ite = _unusedList.begin();

		//	まだ向きを設定していないときは設定する
		if (!(*ite)->GetIsOneTime())
		{
			//	内容の初期化
			(*ite)->SetStartPos(_startPos);
			(*ite)->SetStartDir(_startDir);
			(*ite)->SetSpeed(_speed);
			(*ite)->SetIsOneTime(true);
		}

		//	未使用リストから使用リストに登録
		_useList.push_back(*ite);

		//	未使用リストから削除
		_unusedList.remove(*ite);
	}
}

//-----------------------------------------------------------------------------
//	@brief	リストの更新
//-----------------------------------------------------------------------------
void ShotManager::_UpdateList(const SHOT_KIND _shotKind)
{
	//	使用リストの更新
	switch (_shotKind)
	{
	case SHOT_KIND::PLAYER_SHOT:
		_UpdateList(m_usePlayerShotList);
		break;

	case SHOT_KIND::ENEMY_SHOT:
		_UpdateList(m_useEnemyShotList);
		break;
	}
}

//-----------------------------------------------------------------------------
//	@brief	リストの更新
//-----------------------------------------------------------------------------
void ShotManager::_UpdateList(std::list<Shot*>& _useList)
{
	//	使用リストが空でないとき
	if (!_useList.empty())
	{
		//	リストの回数分回す
		std::list<Shot*>::iterator 	ite;
		for (ite = _useList.begin(); ite != _useList.end(); ite++)
		{
			//	更新処理
			(*ite)->Update();
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	描画処理
//-----------------------------------------------------------------------------
void ShotManager::_DrawList(const SHOT_KIND _shotKind)
{
	//	使用リストの描画
	switch (_shotKind)
	{
	case SHOT_KIND::PLAYER_SHOT:
		_DrawList(m_usePlayerShotList);
		break;

	case SHOT_KIND::ENEMY_SHOT:
		_DrawList(m_useEnemyShotList);
		break;
	}
}

//-----------------------------------------------------------------------------
//	@brief	描画処理
//-----------------------------------------------------------------------------
void ShotManager::_DrawList(std::list<Shot*>& _useList)
{
	//	使用リストが空でないとき
	if (!_useList.empty())
	{
		//	リストの回数分回す
		std::list<Shot*>::iterator 	ite;
		for (ite = _useList.begin(); ite != _useList.end(); ite++)
		{
			(*ite)->Draw();
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	リストから削除
//-----------------------------------------------------------------------------
void ShotManager::_RemoveFromList(const SHOT_KIND _shotKind)
{
	//	使用リストから削除
	switch (_shotKind)
	{
	case SHOT_KIND::PLAYER_SHOT:
		_RemoveFromList(m_usePlayerShotList, m_unusedPlayerShotList);
		break;

	case SHOT_KIND::ENEMY_SHOT:
		_RemoveFromList(m_useEnemyShotList, m_unusedEnemyShotList);
		break;
	}
}

//-----------------------------------------------------------------------------
//	@brief	リストから削除
//-----------------------------------------------------------------------------
void ShotManager::_RemoveFromList(std::list<Shot*>& _useList, std::list<Shot*>& _unusedList)
{
	//	使用リストが空でないとき
	if (!_useList.empty())
	{
		//	リストの回数分回す
		std::list<Shot*>::iterator 	ite;
		for (ite = _useList.begin(); ite != _useList.end(); ite++)
		{
			//	範囲外のときは、削除する
			const bool isArrivalTop = (*ite)->GetPos().y >= MAX_Y_POS;
			const bool isArrivalButtom = (*ite)->GetPos().y <= MIN_Y_POS;
			const bool isArrivalRight = (*ite)->GetPos().x >= MAX_X_POS;
			const bool isArrivalLeft = (*ite)->GetPos().x <= MIN_X_POS;
			const bool isActive = isArrivalTop || isArrivalButtom || isArrivalRight || isArrivalLeft;
			if (isActive)
			{
				//	設定可能にする
				(*ite)->SetIsOneTime(false);

				//	使用リストから未使用リストに登録
				_unusedList.push_back(*ite);

				//	使用リストから削除
				_useList.remove(*ite);

				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	リストから削除
//-----------------------------------------------------------------------------
void ShotManager::_RemoveFromList(std::list<Shot*>& _useList, std::list<Shot*>& _unusedList, const Shot* _shotPtr)
{
	//	使用リストが空でないとき
	if (!_useList.empty())
	{
		//	リストの回数分回す
		std::list<Shot*>::iterator 	ite;
		for (ite = _useList.begin(); ite != _useList.end(); ite++)
		{
			//	_num と一致したポインタを返す
			const bool isSame = _shotPtr == (*ite);
			if (isSame)
			{
				//	設定可能にする
				(*ite)->SetIsOneTime(false);

				//	使用リストから未使用リストに登録
				_unusedList.push_back(*ite);

				//	使用リストから削除
				_useList.remove(*ite);

				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放処理
//-----------------------------------------------------------------------------
void ShotManager::_FinalRelease()
{
	//	大本モデルのアンロード
	MV1DeleteModel(m_sourcePlayerShotModelHandle);
	MV1DeleteModel(m_sourceEnemyShotModelHandle);

	//	リストに登録されていたら削除
	if (!m_unusedPlayerShotList.empty()) { m_unusedPlayerShotList.clear(); }
	if (!m_unusedEnemyShotList.empty()) { m_unusedEnemyShotList.clear(); }
	if (!m_usePlayerShotList.empty()) { m_usePlayerShotList.clear(); }
	if (!m_useEnemyShotList.empty()) { m_useEnemyShotList.clear(); }

	//	ショットの解放
	for (int i = 0; i < CommonConstant::MAX_SHOT_NUM; i++)
	{
		CommonSafe::Delete(m_pPlayerShot[i]);
		CommonSafe::Delete(m_pEnemyShot[i]);
	}
}
