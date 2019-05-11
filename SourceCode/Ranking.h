
//=============================================================================
//	@file	Ranking.h
//	@brief	ランキング
//	@autor	Takuya Ochi
//	@date	2018/1/3
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	プリプロセッサ
//-----------------------------------------------------------------------------
#pragma once
#pragma warning(disable:4996)

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "Common.h"

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
struct UI;
class Pad;
class SoundEffect;

//-----------------------------------------------------------------------------
//	@brief	ランキングクラス
//-----------------------------------------------------------------------------
class Ranking final
{
public:

//===================== 関数 ======================//

	// コンストラクタ / デストラクタ
	Ranking();
	~Ranking();

	//	初期処理
	void Initialize();

	//	更新処理
	void Update(Pad& _pad, SoundEffect& _soundEffect);

	//	描画処理
	void Draw();

//==================== getter =====================//

	//	ランキング開始フラグの getter
	const bool& GetIsRankingStart() const { return m_isRankingStart; }

	//	シーン切り替えフラグの getter
	const bool& GetIsSceneChange() const { return m_isSceneChange; }

private:

//================== 内部処理関数 ==================//

	//	ライフボーナス
	void _LifeBonus(Pad& _pad, SoundEffect& _soundEffect);

	//	討伐数ボーナス
	void _DestructionBonus(Pad& _pad, SoundEffect& _soundEffect);

	//	ランキング
	void _Ranking(SoundEffect& _soundEffect);

	//	読み込み
	void _Load();

	//	ソート
	void _Sort();

	//	書き込み
	void _Writing();

	//	点滅描画
	void _FlashingDraw(const int _num, const int _rank);

	//	ランク外の点滅描画
	void _AfterRankFlashingDraw();

//=================== メンバ変数 ===================//

	int		m_score[CommonConstant::MAX_RANKING_NUM];										//	スコア
	int		m_rank;																			//	ランク
	char	m_thisTimeScoreText[CommonConstant::MAX_SCORE_NUM];								//	今回のスコアの文字列
	char	m_bonusScoreText[CommonConstant::MAX_SCORE_NUM];								//	ボーナスの文字列
	char	m_scoreText[CommonConstant::MAX_RANKING_NUM][CommonConstant::MAX_SCORE_NUM];	//	スコアの文字列
	bool	m_isLifeBonusScore;																//	残機ボーナス
	bool	m_isDestructionBonusScore;														//	討伐数ボーナス
	bool	m_isRankingStart;																//	ランキングの開始
	float	m_productionTime;																//	演出時間
	float	m_waitTime;																		//	遅延時間
	float	m_lifeBonusScoreVal;															//	ライフボーナス値
	float	m_DestructionBonusScoreVal;														//	討伐数ボーナス値
	float	m_flashingTime;																	//	点滅時間
	float	m_sceneChangeTime;																//	シーン切り替え時間
	bool	m_isSceneChange;																//	シーン切り替え
	UI		m_bonusUI;																		//	ボーナスUI
	UI		m_rankingUI;																	//	ランキングUI

//===================== 静的定数 ===================//

	static const int		TEXT_START;					//	テキストの初期位置
	static const int		TEXT_INTERVAL;				//	テキストの間隔
	static const int		AFTER_RANK;					//	ランク外
	static const float		MAX_PRODUCTION_TIME;		//	演出時間の最大
	static const float		MAX_WAITTIME;				//	遅延時間の最大
	static const float		LIFE_BONUS_SCORE;			//	ライフボーナス
	static const float		DESTRUCTION_BONUS_SCORE;	//	討伐数ボーナス
	static const float		MAX_SCENE_CHANGE_TIME;		//	シーン切り替え時間の最大
};
