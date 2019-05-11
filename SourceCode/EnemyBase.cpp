
//=============================================================================
//	@file	EnemyBase.cpp
//	@brief	エネミーベース
//	@autor	Takuya Ochi
//	@date	2018/11/14
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "EnemyBase.h"
#include "Common.h"
#include "PlayerManager.h"
#include "ShotManager.h"
#include "PlayerBase.h"
#include "EffekseerEmitter.h"
#include "SoundEffect.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const VECTOR	EnemyBase::RECT_CORRECTION = VGet(6.0f, 6.0f, 6.0f);				//	中心座標の補正
const float		EnemyBase::SHOT_SPEED = 0.8f;										//	ショットの速度
const float		EnemyBase::MAX_WAIT_TIME = 100.0f;									//	ショットの遅延時間
const float		EnemyBase::HIT_RADIUS = 6.0f;										//	当たり判定用の半径
const float		EnemyBase::CENTER_CORRECTION = -1.0f;								//	中心座標の補正
const COLOR_F	EnemyBase::COLOR_NORMAL = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);		//	デフォルト色
const COLOR_F	EnemyBase::COLOR_DECAY_NORMAL = GetColorF(0.0f, 0.0f, 0.0f, 1.0f);	//	デフォルト色の減衰
const COLOR_F	EnemyBase::COLOR_RED = GetColorF(1.0f, 0.0f, 0.0f, 1.0f);			//	赤色
const COLOR_F	EnemyBase::COLOR_DECAY_RED = GetColorF(0.5f, 0.0f, 0.0f, 1.0f);		//	赤色の減衰

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
EnemyBase::EnemyBase(const int _modelHandle)
	: m_effectExplosion(NULL)
{
	//	モデルの複製
	m_modelHandle = MV1DuplicateModel(_modelHandle);
	CommonDebug::Assert((m_modelHandle <= -1), " [ EnemyBase.cpp ] : error : missing duplicat model.");

	m_aliveTime = 0.0f;
	m_isUseShot = false;
	m_isHavingItems = false;
 	m_shotWaitTime = 0.0f;
	m_formationID = 0;
	m_entryID = 0;
	m_entryKind = 0;
	m_isAttenuation = false;
	m_isHit = false;
	m_isOffDraw = false;
	m_isDeleate = false;

	//	エフェクトの読み込み
	m_effectExplosion = new EffekseerEmitter("Data/Effect/Explosion/Explosion3.efk");
	CommonDebug::Assert((m_effectExplosion == NULL), " [ EnemyBase.cpp ] : error : missing effect is null.");
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
EnemyBase::~EnemyBase()
{
	//	モデルのアンロード
	MV1DeleteModel(m_modelHandle);

	//	エフェクトの解放
	CommonSafe::Delete(m_effectExplosion);
}

//-----------------------------------------------------------------------------
//	@brief	描画
//-----------------------------------------------------------------------------
void EnemyBase::Draw()
{
	const bool isDelete = m_isHit;
	if (!isDelete)
	{
		//	減衰色の設定
		_AttenuationColor();

		//	モデルの色を変更
		MV1SetDifColorScale(m_modelHandle, m_color);

		//	モデルの描画
		if (!m_isOffDraw) { MV1DrawModel(m_modelHandle); }
	}
}

//-----------------------------------------------------------------------------
//	@brief	色の変更
//-----------------------------------------------------------------------------
void EnemyBase::_ChangeColor()
{
	//	アイテムを持っているなら
	//	色を変更する
	if (m_isHavingItems) { m_color = COLOR_RED; }
	else { m_color = COLOR_NORMAL; }
}

//-----------------------------------------------------------------------------
//	@brief	ショットの登録
//-----------------------------------------------------------------------------
void EnemyBase::_ShotEntry(PlayerManager& _playerManager, ShotManager& _shotManager)
{
	//	生きている間のみ
	const bool isAlive = !m_isHit;
	if (isAlive)
	{
		//	ポインタの取得
		PlayerBase* player = _playerManager.GetPlayerPtr();
		if (player)
		{
			//	出現演出が終わっていたら、
			//	プレイヤーへのベクトルを求め、
			//	弾の追加をする
			const bool isPossible = !player->GetIsEmerge();
			if (isPossible)
			{
				const bool isActive = m_shotWaitTime++ == MAX_WAIT_TIME;
				if (isActive)
				{
					//	弾を生成する
					VECTOR targetDir = VSub(player->GetCircle().m_centerPoint, m_pos);
					_shotManager.RegisterOnList(ShotManager::SHOT_KIND::ENEMY_SHOT, m_pos, targetDir, SHOT_SPEED);
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	爆発エフェクト
//-----------------------------------------------------------------------------
void EnemyBase::_OnEffectExplosion(SoundEffect& _soundEffect)
{
	//	ヒットしたとき
	if (m_isHit)
	{
		const bool isExplosionStart = m_effectExplosion->GetPlayTime() == 0.0f;
		const bool isExplosionEnd = m_effectExplosion->GetPlayTime() == 200.0f;

		//	爆発エフェクトの開始
		if (isExplosionStart)
		{
			m_effectExplosion->SetPos(m_pos);
			m_effectExplosion->SetPlaySpeed(10.0f);
			m_effectExplosion->SetScale(VGet(2.0f, 2.0f, 2.0f));
			m_effectExplosion->OnPlayEffect();
			m_isOffDraw = true;

			//	討伐数の加算
			HAND_OVER_RESULT->AddDestructionNum();

			//	SEの再生
			_soundEffect.OnPlaySound(_soundEffect.SE_KIND::SE_EXPLOSION);
		}

		//	爆発エフェクトの終了
		if (isExplosionEnd)
		{
			m_effectExplosion->OnEndEffect();
			m_isDeleate = true;
			return;
		}

		m_effectExplosion->AddPlayTime();
		m_effectExplosion->Update();
	}
}

//-----------------------------------------------------------------------------
//	@brief	減色設定
//-----------------------------------------------------------------------------
void EnemyBase::_AttenuationColor()
{
	//	奥から来る敵は、
	//	減衰する
	if (m_isAttenuation)
	{
		//	アイテムを持っているなら
		//	色を変更する
		if (m_isHavingItems) { m_color = COLOR_DECAY_RED;}
		else { m_color = COLOR_DECAY_NORMAL; }
	}
	else
	{
		//	アイテムを持っているなら
		//	色を変更する
		const float lerpSpeed = 0.00001f;
		if (m_isHavingItems) { m_color = CommonFunction::AddColor(m_color, COLOR_RED, lerpSpeed); }
		else { m_color = CommonFunction::AddColor(m_color, COLOR_NORMAL, lerpSpeed); }
	}
}
