
//=============================================================================
//	@file	UIScore.h
//	@brief	スコアUI
//	@autor	Takuya Ochi
//	@date	2018/12/30
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	プリプロセッサ
//-----------------------------------------------------------------------------
#pragma once
#pragma warning(disable:4996)

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "Common.h"

//-----------------------------------------------------------------------------
//	@brief	スコアクラス
//-----------------------------------------------------------------------------
class UIScore final
{
public:

//==================== 列挙体 =====================//

	//	スコア
	enum ADD_SCORE
	{
		ADD_SCORE_ENEMY = 500,
		ADD_SCORE_ITEM = 3000,
		ADD_SCORE_MEDIUM_BOSS = 5000,
		ADD_SCORE_LAST_BOSS_PART = 8000,
		ADD_SCORE_LAST_BOSS = 20000,
	};

//===================== 関数 ======================//

	//	デストラクタ
	~UIScore();

	//	生成
	UIScore* GetInstance();

	//	更新
	void Update();

	//	描画
	void Draw();

	//	スコアの加算
	void AddScore(const ADD_SCORE _score) { m_targetScore += (int)_score; }

	//	スコアの加算
	void AddScore(const float _score) { m_targetScore += (int)_score; }

	//	スコア加算のスキップ
	void SkipScore() { m_score = m_targetScore; }

	//	スコアのリセット
	void ScoreReset();

//==================== getter =====================//

	//	スコアの取得
	const int GetScore() const { return (int)m_score; }

private:

//================== シングルトン ==================//

	//	コンストラクタ
	UIScore();

//=================== メンバ変数 ===================//

	static UIScore	s_instance;										//	インスタンス
	VECTOR			m_pos;											//	ポジション
	float			m_score;										//	スコア
	float			m_targetScore;									//	スコアのターゲット
	char			m_scoreText[CommonConstant::MAX_SCORE_NUM];		//	スコアの文字列

//===================== 静的定数 ===================//

};

#define SCORE UIScore::GetInstance()