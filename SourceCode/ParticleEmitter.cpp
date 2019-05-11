
//=============================================================================
//	@file	ParticleEmitter.cpp
//	@brief	パーティクルの生成
//	@autor	Takuya Ochi
//	@date	2018/10/3
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "ParticleEmitter.h"
#include "Common.h"

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
ParticleEmitter::ParticleEmitter(const int _spriteHandle, const int _size)
{
	//	画像のコピー
	m_spriteHandle = _spriteHandle;
	CommonDebug::Assert((m_spriteHandle <= -1), "[ ParticleEmitter.cpp ] : error : model loading failed.");

	m_activeTime = 0.0f;			//	描画時間
	m_maxActiveTime = 0.0f;			//	最大時間

	//	パラメータの商品帰化
	SetParameters(PARAMETER_KIND::MIN_PARAMETER, CommonConstant::ORIGIN, CommonConstant::ORIGIN, 0.0f, 0.0f, 0.0f);
	SetParameters(PARAMETER_KIND::MAX_PARAMETER, CommonConstant::ORIGIN, CommonConstant::ORIGIN, 0.0f, 0.0f, 0.0f);
	m_isEndParticle = false;
	m_red = 255.0f;					//	赤
	m_green = 255.0f;				//	青
	m_blue = 255.0f;				//	緑

	//	サイズ分の生成
	m_pParticles = new Particle[_size];

	//	パーティクル数の保存
	m_particleNum = _size;
}

//-----------------------------------------------------------------------------
// @brief  デストラクタ
//-----------------------------------------------------------------------------
ParticleEmitter::~ParticleEmitter()
{
	//	画像のアンロード
	DeleteGraph(m_spriteHandle);
}

//-----------------------------------------------------------------------------
// @brief  初期化
//-----------------------------------------------------------------------------
void ParticleEmitter::Initialize()
{
	//	初期化
	for (int i = 0; i < m_particleNum; i++)
	{
		m_pParticles[i].m_pos = CommonConstant::ORIGIN;
		m_pParticles[i].m_dir = CommonConstant::ORIGIN;
		m_pParticles[i].m_size = 0.0f;
		m_pParticles[i].m_speed = 0.0f;
		m_pParticles[i].m_life = 0.0f;
		m_pParticles[i].m_red = m_red;
		m_pParticles[i].m_green = m_green;
		m_pParticles[i].m_blue = m_blue;
		m_pParticles[i].m_alpha = 0.0f;
		m_pParticles[i].m_isActive = false;
	}
}

//-----------------------------------------------------------------------------
// @brief  更新
//-----------------------------------------------------------------------------
void ParticleEmitter::Update()
{
	//	活動時間内なら
	const bool isNotActive = m_activeTime >= m_maxActiveTime;
	if (isNotActive)
	{
		//	終了処理
		End();

		for (int i = 0; i < m_particleNum; i++)
		{
			//	どれか一つでも実行中なら
			//	終了しない
			if (m_pParticles[i].m_isActive == true) { return; }
			m_isEndParticle = true;
		}

		return;
	}

	//	パーティクルの作成処理
	_Create();

	for (int i = 0; i < m_particleNum; i++)
	{
		//	パーティクルの状態がアクティブなら
		if (m_pParticles[i].m_isActive)
		{
			//	移動
			VECTOR velocity = VScale(m_pParticles[i].m_dir, m_pParticles[i].m_speed);
			m_pParticles[i].m_pos = VAdd(m_pParticles[i].m_pos, velocity);

			//	サイズ変更
			m_pParticles[i].m_size -= 0.05f;

			//	寿命をへらす
			m_pParticles[i].m_life--;

			//	寿命がなくなったら、パーティクルの削除
			if (m_pParticles[i].m_life <= 0) { m_pParticles[i].m_isActive = false; }

		}
	}

	m_activeTime++;
}

//-----------------------------------------------------------------------------
// @brief  現在の出ているのみエフェクトを更新する
//-----------------------------------------------------------------------------
void ParticleEmitter::End()
{
	//	描画しないようにする
	for (int i = 0; i < m_particleNum; i++)
	{
		//	パーティクルの状態がアクティブなら
		if (m_pParticles[i].m_isActive)
		{
			//	移動
			VECTOR velocity = VScale(m_pParticles[i].m_dir, m_pParticles[i].m_speed);
			m_pParticles[i].m_pos = VAdd(m_pParticles[i].m_pos, velocity);

			//	サイズ変更
			m_pParticles[i].m_size -= 0.05f;

			//	寿命をへらす
			m_pParticles[i].m_life--;

			//	寿命がなくなったら、パーティクルの削除
			if (m_pParticles[i].m_life <= 0) { m_pParticles[i].m_isActive = false; }
		}
	}
}

//-----------------------------------------------------------------------------
// @brief  描画
//-----------------------------------------------------------------------------
void ParticleEmitter::Draw()
{
	//	Zバッファを使用する設定に変更
	SetUseZBufferFlag(TRUE);

	//	Zバッファへの書き込みは行わない
	SetWriteZBufferFlag(FALSE);

	for (int i = 0; i < m_particleNum; i++)
	{
		//	描画光度も標準設定にする
		SetDrawBright((int)m_pParticles[i].m_red, (int)m_pParticles[i].m_green, (int)m_pParticles[i].m_blue);
	}

	for (int i = 0; i < m_particleNum; i++)
	{
		//	パーティクルの状態がアクティブなら
		if (m_pParticles[i].m_isActive)
		{
			if (m_pParticles[i].m_alpha <= 0.0f) { continue; }

			//	減算ブレンドでパーティクルの描画
			SetDrawBlendMode(DX_BLENDMODE_SUB, (int)(m_pParticles[i].m_alpha * 255.0f));
			DrawBillboard3D(m_pParticles[i].m_pos, 0.5f, 0.5f, m_pParticles[i].m_size * m_pParticles[i].m_alpha, 0.0f, m_spriteHandle, TRUE);
		}
	}

	for (int i = 0; i < m_particleNum; i++)
	{
		//	パーティクルの状態がアクティブなら
		if (m_pParticles[i].m_isActive)
		{
			if (m_pParticles[i].m_alpha <= 0.0f) { continue; }

			//	加算ブレンドでパーティクルの描画
			SetDrawBlendMode(DX_BLENDMODE_ADD_X4, (int)(m_pParticles[i].m_alpha * 255.0f));
			DrawBillboard3D(m_pParticles[i].m_pos, 0.5f, 0.5f, m_pParticles[i].m_size * m_pParticles[i].m_alpha, 0.0f, m_spriteHandle, TRUE);
		}
	}

	//	ブレンドモードを標準設定にする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	//	描画光度も標準設定にする
	SetDrawBright(255, 255, 255);

	//	Zバッファを使用しない設定に変更
	SetUseZBufferFlag(FALSE);
}

//-----------------------------------------------------------------------------
// @brief  削除
//-----------------------------------------------------------------------------
void ParticleEmitter::Release()
{
	//  最終的な解放処理
	_FinalRelease();
}

//-----------------------------------------------------------------------------
// @brief  パラメータの設定
//-----------------------------------------------------------------------------
void ParticleEmitter::SetParameters(const PARAMETER_KIND _paramKind, const VECTOR _pos, const VECTOR _dir, const float _speed, const float _size, const float _life)
{
	switch (_paramKind)
	{
	case PARAMETER_KIND::MIN_PARAMETER:
		m_minParam.m_pos = _pos;
		m_minParam.m_dir = _dir;
		m_minParam.m_speed = _speed;
		m_minParam.m_size = _size;
		m_minParam.m_life = _life;
		break;

	case PARAMETER_KIND::MAX_PARAMETER:
		m_maxParam.m_pos = _pos;
		m_maxParam.m_dir = _dir;
		m_maxParam.m_speed = _speed;
		m_maxParam.m_size = _size;
		m_maxParam.m_life = _life;
		break;
	}
}

//-----------------------------------------------------------------------------
// @brief  パーティクルの作成処理
//-----------------------------------------------------------------------------
void ParticleEmitter::_Create()
{
	//	作成
	for (int i = 0; i < m_particleNum; i++)
	{
		if (!m_pParticles[i].m_isActive)
		{
			//	ポジション
			m_pParticles[i].m_pos = CommonFunction::GetRand(m_minParam.m_pos, m_maxParam.m_pos);

			//	向き
			m_pParticles[i].m_dir = CommonFunction::GetRand(m_minParam.m_dir, m_maxParam.m_dir);

			//	速度
			m_pParticles[i].m_speed = CommonFunction::GetRand(m_minParam.m_speed, m_maxParam.m_speed);

			//	サイズ
			m_pParticles[i].m_size = CommonFunction::GetRand(m_minParam.m_size, m_maxParam.m_size);

			//	ライフ
			m_pParticles[i].m_life = CommonFunction::GetRand(m_minParam.m_life, m_maxParam.m_life);

			//	色の設定
			m_pParticles[i].m_red = m_red;
			m_pParticles[i].m_green = m_green;
			m_pParticles[i].m_blue = m_blue;

			//	不透明度
			m_pParticles[i].m_alpha = 1.0f;

			//	アクティブか
			m_pParticles[i].m_isActive = true;
		}
	}
}

//-----------------------------------------------------------------------------
// @brief  最終的な解放処理
//-----------------------------------------------------------------------------
void ParticleEmitter::_FinalRelease()
{
	//	画像ハンドルの解放
	DeleteGraph(m_spriteHandle);
}
