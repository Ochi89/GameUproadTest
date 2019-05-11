
//=============================================================================
//	@file	ParticleEmitter.h
//	@brief	パーティクルの生成
//	@autor	Takuya Ochi
//	@date	2018/10/3
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	プリプロセッサ
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "DxLib.h"

//-----------------------------------------------------------------------------
//	@brief	パーティクルの生成クラス
//-----------------------------------------------------------------------------
class ParticleEmitter final
{
public:

//==================== 構造体 =====================//

	//	パラメータの種類
	enum PARAMETER_KIND
	{
		MIN_PARAMETER,
		MAX_PARAMETER,
	};

//===================== 関数 ======================//

	//	コンストラクタ / デストラクタ
	ParticleEmitter(const int _spriteHandle, const int _size);
	~ParticleEmitter();

	//	初期化
	void Initialize();

	//	更新
	void Update();

	//	現在の出ているのみエフェクトを更新する
	void End();

	//	描画
	void Draw();

	//	削除
	void Release();

//==================== getter =====================//

	//	パーティクル数の getter
	const int& GetParticleNum() const { return m_particleNum; }

	//	パーティクルの終了の getter
	const bool& GetIsEndParticle() const { return m_isEndParticle; }

//==================== setter =====================//

	//	パラメータの設定
	void SetParameters(const PARAMETER_KIND _paramKind, const VECTOR _pos, const VECTOR _dir, const float _speed, const float _size, const float _life);

	//	色の setter
	void SetColor(const float _red, const float _green, const float _blue) { m_red = _red; m_green = _green; m_blue = _blue; }

	//	稼働時間の setter
	void SetActiveTime(const float _set) { m_maxActiveTime = _set; }

	//	パーティクルの終了の setter
	void SetIsEndParticle(const bool _set) { m_isEndParticle = _set; }

private:

//==================== 構造体 =====================//

	//	パーティクル
	struct Particle
	{
		VECTOR	m_pos;			//	位置
		VECTOR	m_dir;			//	向き
		float	m_speed;		//	速度
		float	m_size;			//	大きさ
		float	m_life;			//	寿命フレーム数
		float	m_red;			//	赤色
		float	m_green;		//	青色
		float	m_blue;			//	緑色
		float	m_alpha;		//	不透明度
		bool	m_isActive;		//	稼働中かどうか
	};

	//	パーティクルの情報
	struct ParticleInfo
	{
		VECTOR	m_pos;			//	位置
		VECTOR	m_dir;			//	向き
		float	m_size;			//	大きさ
		float	m_speed;		//	速度
		float	m_life;			//	寿命フレーム数
	};

//================== 内部処理関数 ==================//

	//	生成
	void _Create();

	//	最終的な解放処理
	void _FinalRelease();

//=================== メンバ変数 ===================//
	
	Particle*		m_pParticles;			//	パーティクル
	int				m_particleNum;			//	パーティクルの数
	int				m_spriteHandle;			//	スプライトハンドル
	float			m_activeTime;			//	描画時間
	float			m_maxActiveTime;		//	最大時間
	ParticleInfo	m_minParam;				//	最小値の情報
	ParticleInfo	m_maxParam;				//	最大値の情報
	float			m_red;					//	赤
	float			m_green;				//	緑
	float			m_blue;					//	青
	bool			m_isEndParticle;		//	パーティクルの終了
};
