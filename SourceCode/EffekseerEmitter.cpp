
//=============================================================================
//	@file	EffekseerEmitter.cpp
//	@brief	EffekseerEmitter
//	@autor	Takuya Ochi
//	@date	2018/12/26
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "EffekseerEmitter.h"
#include "Common.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
EffekseerEmitter::EffekseerEmitter(const char* _fileName)
{
	//	エフェクトの読み込み
	m_effectResourceHandle = LoadEffekseerEffect(_fileName);
	CommonDebug::Assert((m_effectResourceHandle <= -1), " [ EffekseerEmitter.cpp ] : error : effect resource loading failed.");

	//	各変数の初期化
	m_playingEffectHandle = -1;
	m_pos = CommonConstant::ORIGIN;
	m_scale = CommonConstant::ORIGIN;
	m_angle = CommonConstant::ORIGIN;
	m_speed = 0.0f;
	m_isUsePos = false;
	m_isUseAngle = false;
	m_isUseScale = false;
	m_isUseSpeed = false;
	m_isPlayEffect = false;
	m_isStartEffect = false;
	m_isPrevConditions = false;
	m_playTime = 0.0f;
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
EffekseerEmitter::~EffekseerEmitter()
{
	//	最終的な解放処理
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	エフェクトの再生処理
//-----------------------------------------------------------------------------
void EffekseerEmitter::OnPlayEffect()
{
	//	まだエフェクトを開始していなければ、開始
	if (!m_isPlayEffect)
	{
		//	エフェクトの再生
		m_playingEffectHandle = PlayEffekseer3DEffect(m_effectResourceHandle);
		m_isPlayEffect = true;
	}
}

//-----------------------------------------------------------------------------
//	@brief	エフェクトの終了処理
//-----------------------------------------------------------------------------
void EffekseerEmitter::OnEndEffect()
{
	//	変数の初期化
	m_pos = CommonConstant::ORIGIN;
	m_scale = CommonConstant::ORIGIN;
	m_angle = CommonConstant::ORIGIN;
	m_speed = 0.0f;
	m_isUsePos = false;
	m_isUseAngle = false;
	m_isUseScale = false;
	m_isUseSpeed = false;
	m_isPlayEffect = false;
	m_isStartEffect = false;
	m_isPrevConditions = false;
	m_playTime = 0.0f;

	SetPosPlayingEffekseer3DEffect(m_playingEffectHandle, m_pos.x, m_pos.y, m_pos.z);
	SetRotationPlayingEffekseer3DEffect(m_playingEffectHandle, m_angle.x, m_angle.y, m_angle.z);
	SetScalePlayingEffekseer3DEffect(m_playingEffectHandle, m_scale.x, m_scale.y, m_scale.z);
	SetSpeedPlayingEffekseer3DEffect(m_playingEffectHandle, m_speed);

	//	エフェクトの停止
	StopEffekseer3DEffect(m_playingEffectHandle);
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void EffekseerEmitter::Update()
{
	if (m_isUsePos)
	{
		//	ポジションを設定する
		SetPosPlayingEffekseer3DEffect(m_playingEffectHandle, m_pos.x, m_pos.y, m_pos.z);
	}

	if (m_isUseAngle)
	{
		//	ラジアンに変換し、設定する
		VECTOR radian = CommonFunction::DegToRad(m_angle);
		SetRotationPlayingEffekseer3DEffect(m_playingEffectHandle, radian.x, radian.y, radian.z);
	}

	if (m_isUseScale)
	{
		//	拡大率を設定する
		SetScalePlayingEffekseer3DEffect(m_playingEffectHandle, m_scale.x, m_scale.y, m_scale.z);
	}

	if (m_isUseSpeed)
	{
		//	再生速度を設定する
		SetSpeedPlayingEffekseer3DEffect(m_playingEffectHandle, m_speed);
	}
}

//-----------------------------------------------------------------------------
//	@brief	再生速度の getter
//-----------------------------------------------------------------------------
const float EffekseerEmitter::GetPlaySpeed() const
{
	//	再生速度を取得する
	return GetSpeedPlayingEffekseer3DEffect(m_playingEffectHandle);
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放処理
//-----------------------------------------------------------------------------
void EffekseerEmitter::_FinalRelease()
{
	//	エフェクトリソースのアンロード
	DeleteEffekseerEffect(m_playingEffectHandle);
	DeleteEffekseerEffect(m_effectResourceHandle);
}
