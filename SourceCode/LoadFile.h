
//=============================================================================
//	@file	LoadFile.h
//	@brief	ファイルの読み込み
//	@autor	Takuya Ochi
//	@date	2018/10/28
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	プロプロセッサ
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "Common.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

//-----------------------------------------------------------------------------
//	@brief	ファイルの読み込みクラス
//-----------------------------------------------------------------------------
class LoadFile final
{
public:

//===================== 関数 ======================//

	~LoadFile();								//	デストラクタ
	
	//	生成
	LoadFile* GetInstance();

	//	CSV ２次元ファイルの読み込み
	bool LoadCsvTwoDimensionsFile(const char* _fileName);

	//	ファイルの読み込み
	bool LoadCsvFile(const char* _fileName);

	//	CSV ２次元ファイルの取得
	int GetCsvFile(const int _row, const int _col);

	//	CSV ファイルの取得
	int GetCsvFile(const int _row = 0);

	//	CSV ファイルの書き込み
	bool WritingCsvFile(const char* _fileName, const float _writingVal);

	//	CSV ファイルの書き込み
	bool WritingCsvFile(const char* _fileName, const int _writingVal);

	//	CSV ファイルの書き込み
	bool WritingCsvFile(const char* _fileName, const int _size, int* _writingVal);

private:

//================== シングルトン ==================//

	//	コンストラクタ
	LoadFile();

//================== 内部処理関数 ==================//
	
	//	読み取りの区切り
	std::vector<std::string> _Split(std::string& _input, char _split);

	//	読み込み
	bool _Load(const char* _fileName, std::vector<std::string>& _strvec, int* date);

	//	読み込み
	bool _Load(const char* _fileName, std::vector<std::string>& _strvec, float* date);

	//	ソート
	void _Sort(std::vector<std::string>& _strvec, const float _writingVal, int* date);

	//	ソート
	void _Sort(std::vector<std::string>& _strvec, const float _writingVal, float* date);

	//	書き込み
	bool _ToWrite(const char* _fileName, std::vector<std::string>& _strvec, int* date);

	//	書き込み
	bool _ToWrite(const char* _fileName, std::vector<std::string>& _strvec, float* date);

//=================== メンバ変数 ===================//

	static LoadFile		m_instance;																		//	インスタンス
	int					m_twoDimensionsData[CommonConstant::MAX_CSV_NUM][CommonConstant::MAX_CSV_NUM];	//	2次元ファイル読み込み用
	int					m_data[CommonConstant::MAX_CSV_NUM];											//	ファイル読み込み用

};

#define LOAD_FILE LoadFile::GetInstance()