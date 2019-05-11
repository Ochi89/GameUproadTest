
//=============================================================================
//	@file	Ranking.cpp
//	@brief	ランキング
//	@autor	Takuya Ochi
//	@date	2018/1/3
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "Ranking.h"
#include "LoadFile.h"
#include "UIScore.h"
#include "Pad.h"
#include "SoundEffect.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const int			Ranking::TEXT_START = 29;						//	テキストの初期位置
const int			Ranking::TEXT_INTERVAL = 91;					//	次までの間隔
const int			Ranking::AFTER_RANK = -1;						//	ランク外
const float			Ranking::MAX_PRODUCTION_TIME = 120.0f;			//	演出時間の最大
const float			Ranking::MAX_WAITTIME = 60.0f;					//	遅延時間の最大
const float			Ranking::LIFE_BONUS_SCORE = 50000.0f;			//	ライフボーナス
const float			Ranking::DESTRUCTION_BONUS_SCORE = 5000.0f;		//	討伐数ボーナス
const float			Ranking::MAX_SCENE_CHANGE_TIME = 3600.0f;		//	シーン切り替え時間の最大

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
Ranking::Ranking()
{
	//	内容の初期化
	for (int i = 0; i < CommonConstant::MAX_RANKING_NUM; i++)
	{
		m_score[i] = 0;
	}

	m_rank = AFTER_RANK;
	m_isLifeBonusScore = false;
	m_isDestructionBonusScore = false;
	m_isRankingStart = false;
	m_productionTime = 0.0f;
	m_waitTime = 0.0f;
	m_lifeBonusScoreVal = 0.0f;
	m_DestructionBonusScoreVal = 0.0f;
	m_flashingTime = 0.0f;
	m_sceneChangeTime = 0.0f;
	m_isSceneChange = false;

	m_bonusUI.m_spriteHendle = LoadGraph("Data/Sprite/Result/Bonus.png");
	CommonDebug::Assert((m_bonusUI.m_spriteHendle <= -1), " [ Ranking.cpp ] : error : sprite loading failed.");
	m_bonusUI.m_pos = VGet(10.0f, 10.0f, 0.0f);

	m_rankingUI.m_spriteHendle = LoadGraph("Data/Sprite/Result/Ranking.png");
	CommonDebug::Assert((m_rankingUI.m_spriteHendle <= -1), " [ Ranking.cpp ] : error : sprite loading failed.");
	m_rankingUI.m_pos = VGet(10.0f, 10.0f, 0.0f);
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
Ranking::~Ranking()
{
	//	スプライトのアンロード
	DeleteGraph(m_bonusUI.m_spriteHendle);
	DeleteGraph(m_rankingUI.m_spriteHendle);
}

//-----------------------------------------------------------------------------
//	@brief	初期処理
//-----------------------------------------------------------------------------
void Ranking::Initialize()
{
	//	読み込み
	_Load();

	//	クリアしているとき、
	//	ボーナスをつける
	const bool isClear = HAND_OVER_RESULT->GetIsClear();
	if (isClear) { m_isLifeBonusScore = true; }
	else { m_isRankingStart = true; }
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void Ranking::Update(Pad& _pad, SoundEffect& _soundEffect)
{
	//	ライフボーナス
	_LifeBonus(_pad, _soundEffect);

	//	討伐数ボーナス
	_DestructionBonus(_pad, _soundEffect);

	//	ランキング
	_Ranking(_soundEffect);
}

//-----------------------------------------------------------------------------
//	@brief	描画処理
//-----------------------------------------------------------------------------
void Ranking::Draw()
{
	//	ライフボーナス
	if (m_isLifeBonusScore)
	{
		FONT->Draw(700, 400, m_thisTimeScoreText);
		FONT->Draw(700, 500, m_bonusScoreText);
	}

	//	討伐ボーナス
	if (m_isDestructionBonusScore)
	{
		FONT->Draw(700, 400, m_thisTimeScoreText);
		FONT->Draw(700, 500, m_bonusScoreText);
	}

	//	ボーナス時
	const bool isBonus = m_isLifeBonusScore || m_isDestructionBonusScore;
	if (isBonus) { DrawGraph((int)m_bonusUI.m_pos.x, (int)m_bonusUI.m_pos.y, m_bonusUI.m_spriteHendle, TRUE); }

	//	ランキング
	if (m_isRankingStart)
	{
		//	スコアの表示
		for (int i = 0; i < CommonConstant::MAX_RANKING_NUM; i++)
		{
			_FlashingDraw(i, m_rank);
		}

		//	ランク外の表示
		_AfterRankFlashingDraw();

		DrawGraph((int)m_rankingUI.m_pos.x, (int)m_rankingUI.m_pos.y, m_rankingUI.m_spriteHendle, TRUE);
	}
}

//-----------------------------------------------------------------------------
//	@brief	ライフボーナス
//-----------------------------------------------------------------------------
void Ranking::_LifeBonus(Pad& _pad, SoundEffect& _soundEffect)
{
	if (m_isLifeBonusScore)
	{
		const bool isBonusStart = m_productionTime == 0.0f;
		const bool isBonusSE = m_productionTime == 30.0f;
		const bool isAddBonus = m_productionTime >= 30.0f;
		const bool isBonusEnd = m_lifeBonusScoreVal <= 0.0f;

		if (isBonusStart)
		{
			//	ボーナス値を求める
			m_lifeBonusScoreVal = LIFE_BONUS_SCORE * HAND_OVER_RESULT->GetLife();
			SCORE->AddScore(m_lifeBonusScoreVal);
		}

		//	SEの再生
		if (isBonusSE)
		{
			const bool isSE = m_lifeBonusScoreVal > 0.0f;
			if (isSE) { _soundEffect.OnPlaySound(_soundEffect.ONE_SE_KIND::ONE_SE_SCORE_UP); }
		}

		//	ボーナスの加算
		if (isAddBonus)
		{
			SCORE->Update();
			const float ignore = 1.0f;
			const float lerpSpeed = 0.2f;
			m_lifeBonusScoreVal = CommonFunction::Lerp(m_lifeBonusScoreVal, 0.0f, lerpSpeed, ignore);
		}

		//	討伐数ボーナスへ移行する
		if (isBonusEnd)
		{
			const bool isActive = m_waitTime++ >= MAX_WAITTIME;
			if (isActive)
			{
				m_isLifeBonusScore = false;
				m_isDestructionBonusScore = true;
				m_productionTime = 0.0f;
				m_waitTime = 0.0f;
				return;
			}
		}

		m_productionTime++;

		//	演出のスキップ
		const bool isKeyActive = KEY->Push(KEY_INPUT_RETURN) || KEY->Push(KEY_INPUT_SPACE) || KEY->Push(KEY_INPUT_X) || KEY->Push(KEY_INPUT_K) || KEY->Push(KEY_INPUT_A);
		const bool isNextActive = isKeyActive || _pad.GetXInputPushButton(PadInfo::PAD_BUTTON_KIND::BUTTON_A);
		if (isNextActive)
		{
			SCORE->SkipScore();
			m_lifeBonusScoreVal = 0.0f;
			m_isLifeBonusScore = false;
			m_isDestructionBonusScore = true;
			m_productionTime = 0.0f;
			m_waitTime = 0.0f;

			//	SEの再生
			_soundEffect.OnPlaySound(_soundEffect.ONE_SE_KIND::ONE_SE_MENU_DECISION);
		}

		sprintf(m_thisTimeScoreText, "%010d", SCORE->GetScore());
		sprintf(m_bonusScoreText, "%010d", (int)m_lifeBonusScoreVal);

	}
}

//-----------------------------------------------------------------------------
//	@brief	討伐数ボーナス
//-----------------------------------------------------------------------------
void Ranking::_DestructionBonus(Pad& _pad, SoundEffect& _soundEffect)
{
	if (m_isDestructionBonusScore)
	{
		const bool isBonusStart = m_productionTime == 0.0f;
		const bool isBonusSE = m_productionTime == 30.0f;
		const bool isAddBonus = m_productionTime >= 30.0f;
		const bool isBonusEnd = m_DestructionBonusScoreVal <= 0.0f;

		if (isBonusStart)
		{
			//	ボーナス値を求める
			m_DestructionBonusScoreVal = DESTRUCTION_BONUS_SCORE * HAND_OVER_RESULT->GetDestructionNum();
			SCORE->AddScore(m_DestructionBonusScoreVal);
		}

		if (isBonusSE)
		{
			//	SEの再生
			const bool isSE = m_DestructionBonusScoreVal > 0.0f;
			if (isSE) { _soundEffect.OnPlaySound(_soundEffect.ONE_SE_KIND::ONE_SE_SCORE_UP); }
		}

		//	ボーナスの加算
		if (isAddBonus)
		{
			SCORE->Update();

			const float ignore = 1.0f;
			const float lerpSpeed = 0.2f;
			m_DestructionBonusScoreVal = CommonFunction::Lerp(m_DestructionBonusScoreVal, 0.0f, lerpSpeed, ignore);
		}

		//	ランキングへ移行する
		if (isBonusEnd)
		{
			const bool isActive = m_waitTime++ >= MAX_WAITTIME;
			if (isActive)
			{
				m_isDestructionBonusScore = false;
				m_isRankingStart = true;
				m_productionTime = 0.0f;
				m_waitTime = 0.0f;
				return;
			}
		}

		m_productionTime++;

		//	演出のスキップ
		const bool isKeyActive = KEY->Push(KEY_INPUT_RETURN) || KEY->Push(KEY_INPUT_SPACE) || KEY->Push(KEY_INPUT_X) || KEY->Push(KEY_INPUT_K) || KEY->Push(KEY_INPUT_A);
		const bool isNextActive = isKeyActive || _pad.GetXInputPushButton(PadInfo::PAD_BUTTON_KIND::BUTTON_A);
		if (isNextActive)
		{
			SCORE->SkipScore();
			m_DestructionBonusScoreVal = 0.0f;
			m_isDestructionBonusScore = false;
			m_isRankingStart = true;
			m_productionTime = 0.0f;
			m_waitTime = 0.0f;

			//	SEの再生
			_soundEffect.OnPlaySound(_soundEffect.ONE_SE_KIND::ONE_SE_MENU_DECISION);
		}

		sprintf(m_thisTimeScoreText, "%010d", (int)SCORE->GetScore());
		sprintf(m_bonusScoreText, "%010d", (int)m_DestructionBonusScoreVal);
	}
}

//-----------------------------------------------------------------------------
//	@brief	ランキング
//-----------------------------------------------------------------------------
void Ranking::_Ranking(SoundEffect& _soundEffect)
{
	if (m_isRankingStart)
	{
		const bool isRankingStart = m_productionTime == 0.0f;
		if (isRankingStart)
		{
			//	ソート
			_Sort();

			//	書き込み
			_Writing();

			m_productionTime++;
		}

		//	数値を文字にする
		for (int i = 0; i < CommonConstant::MAX_RANKING_NUM; i++)
		{
			sprintf(m_scoreText[i], "%010d", (int)m_score[i]);
		}

		//	シーン切り替え
		m_sceneChangeTime++;
		m_isSceneChange = m_sceneChangeTime >= MAX_SCENE_CHANGE_TIME;
	}
}

//-----------------------------------------------------------------------------
//	@brief	読み込み
//-----------------------------------------------------------------------------
void Ranking::_Load()
{
	//	ファイルの読み込み
	LOAD_FILE->LoadCsvFile("Data/CSV/Score.csv");

	//	内容の取得
	for (int i = 0; i < CommonConstant::MAX_RANKING_NUM; i++)
	{
		m_score[i] = LOAD_FILE->GetCsvFile(i);
	}
}

//-----------------------------------------------------------------------------
//	@brief	ソート
//-----------------------------------------------------------------------------
void Ranking::_Sort()
{
	//	今回のスコアと、今までのスコアの比較
	for (int i = 0; i < CommonConstant::MAX_RANKING_NUM; i++)
	{
		//	スコアがランキング内スコアより大きい時
		const bool isActive = m_score[i] <= SCORE->GetScore();
		if (isActive)
		{
			//	その地点に代入する
			int tmp1 = m_score[i];
			m_score[i] = SCORE->GetScore();

			//	順位
			m_rank = i;

			//	それ以降を、一つずつずらす
			for (int j = (i + 1); j < CommonConstant::MAX_RANKING_NUM; j++)
			{
				int tmp2 = m_score[j];
				m_score[j] = tmp1;
				tmp1 = tmp2;
			}

			//	スコアの挿入は終わったので、
			//	ループから抜ける
			return;
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	書き込み
//-----------------------------------------------------------------------------
void Ranking::_Writing()
{
	//	書き込み
	LOAD_FILE->WritingCsvFile("Data/CSV/Score.csv", CommonConstant::MAX_RANKING_NUM, m_score);
}

//-----------------------------------------------------------------------------
//	@brief	点滅描画
//-----------------------------------------------------------------------------
void Ranking::_FlashingDraw(const int _num, const int _rank)
{
	
	//	点滅描画
	const bool isFlashing = _num == _rank;
	if (isFlashing)
	{
		const bool isDraw = m_flashingTime >= 0.0f && m_flashingTime <= 15.0f;
		const bool isReset = m_flashingTime >= 20.0f;

		if (isDraw) { FONT->Draw(782, TEXT_START + (TEXT_INTERVAL * _num), m_scoreText[_num]); }
		if (isReset) { m_flashingTime = 0.0f; return; }

		m_flashingTime++;
		return;
	}

	//	点滅なし描画
	FONT->Draw(782, TEXT_START + (TEXT_INTERVAL * _num), m_scoreText[_num]);
}

//-----------------------------------------------------------------------------
//	@brief	ランク外の点滅描画
//-----------------------------------------------------------------------------
void Ranking::_AfterRankFlashingDraw()
{
	//	点滅描画
	const bool isAfterRank = m_rank == AFTER_RANK;
	if (isAfterRank)
	{
		const bool isDraw = m_flashingTime >= 0.0f && m_flashingTime <= 15.0f;
		const bool isReset = m_flashingTime >= 20.0f;

		if (isDraw) { FONT->Draw(782, 975, m_thisTimeScoreText); }
		if (isReset) { m_flashingTime = 0.0f; return; }

		m_flashingTime++;
		return;
	}
}
