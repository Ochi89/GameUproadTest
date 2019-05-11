
//=============================================================================
//	@file	LoadFile.cpp
//	@brief	ファイルの読み込み
//	@autor	Takuya Ochi
//	@_date	2018/10/28
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "LoadFile.h"
#include "Common.h"

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
LoadFile::LoadFile()
{
	for (int i = 0; i < CommonConstant::MAX_CSV_NUM; i++)
	{
		for (int j = 0; j < CommonConstant::MAX_CSV_NUM; j++)
		{
			//	二次元配列の初期化
			m_twoDimensionsData[i][j] = 0;
		}

		//	配列の初期化
		m_data[i] = 0;
	}
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
LoadFile::~LoadFile()
{
	// 処理なし
}

//-----------------------------------------------------------------------------
//	@brief	インスタンス生成
//-----------------------------------------------------------------------------
LoadFile* LoadFile::GetInstance()
{
	return &m_instance;
}

//-----------------------------------------------------------------------------
//	@brief	CSVファイルの読み込み
//-----------------------------------------------------------------------------
bool LoadFile::LoadCsvTwoDimensionsFile(const char* _fileName)
{
	//	ファイルを開く
	std::ifstream ifs(_fileName);
	if (!ifs) { CommonDebug::Log("ファイルが開けません"); return false; }

	//	列を保存する
	std::string line;

	//	行を保存する
	int row = 0;

	//	ファイルの最後までループ
	while (getline(ifs, line))
	{
		//	カンマ区切りで読み込む
		std::vector<std::string> strvec = _Split(line, ',');

		//	列の最後までループ
		for (int col = 0; col < (int)strvec.size(); col++)
		{
			m_twoDimensionsData[row][col] = stoi(strvec.at(col));
		}

		//	行を増やす
		row++;
	}

	return true;
}

//-----------------------------------------------------------------------------
//	@brief	ファイルの読み込み
//-----------------------------------------------------------------------------
bool LoadFile::LoadCsvFile(const char* _fileName)
{
	//	ファイルを開く
	std::ifstream ifs(_fileName);
	if (!ifs) { CommonDebug::Log("ファイルが開けません"); return false; }

	//	列を保存する
	std::string line;

	//	ファイルの最後までループ
	while (getline(ifs, line))
	{
		//	カンマ区切りで読み込む
		std::vector<std::string> strvec = _Split(line, ',');

		//	列の最後までループ
		for (int row = 0; row < (int)strvec.size(); row++)
		{
			m_data[row] = stoi(strvec.at(row));
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
//	@brief	内容の取得
//-----------------------------------------------------------------------------
int LoadFile::GetCsvFile(const int _row, const int _col)
{
	return m_twoDimensionsData[_row][_col];
}

//-----------------------------------------------------------------------------
//	@brief	内容の取得
//-----------------------------------------------------------------------------
int LoadFile::GetCsvFile(const int _row)
{
	return m_data[_row];
}

//-----------------------------------------------------------------------------
//	@brief	CSVファイルの書き込み
//-----------------------------------------------------------------------------
bool LoadFile::WritingCsvFile(const char* _fileName, const float _writingVal)
{
	//	ファイルに追加で書き込む
	std::ofstream fout(_fileName);
	if (!fout) { CommonDebug::Log("ファイルが開けません"); return false; }

	//	カンマ区切りで書き込む
	fout << _writingVal << "," << std::flush;
	return true;
}

//-----------------------------------------------------------------------------
//	@brief	CSVファイルの書き込み
//-----------------------------------------------------------------------------
bool LoadFile::WritingCsvFile(const char* _fileName, const int _writingVal)
{
	//	ファイルに追加で書き込む
	std::ofstream fout(_fileName);
	if (!fout) { CommonDebug::Log("ファイルが開けません"); return false; }

	//	カンマ区切りで書き込む
	fout << _writingVal << "," << std::flush;
	return true;
}

//-----------------------------------------------------------------------------
//	@brief	CSVファイルの書き込み
//-----------------------------------------------------------------------------
bool LoadFile::WritingCsvFile(const char* _fileName, const int _size, int* _writingVal)
{
	//	ファイルに追加で書き込む
	std::ofstream fout(_fileName);
	if (!fout) { CommonDebug::Log("ファイルが開けません"); return false; }

	//	書き込み
	for (int i = 0; i < _size; i++)
	{
		//	カンマ区切りで書き込む
		fout << _writingVal[i] << "," << std::flush;
	}

	return true;
}

//-----------------------------------------------------------------------------
//	@brief	一行読み込み
//-----------------------------------------------------------------------------
std::vector<std::string> LoadFile::_Split(std::string& _input, char _split)
{
	//	列の保存
	std::istringstream stream(_input);

	//	一文字ずつ保存
	std::string field;

	//	文字保存
	std::vector<std::string> result;

	//	文字を最後まで読み込む
	while (getline(stream, field, _split))
	{
		//	文字を後ろに追加していく
		result.push_back(field);
	}

	return result;
}

//-----------------------------------------------------------------------------
//	@brief	書き込み
//-----------------------------------------------------------------------------
bool LoadFile::_Load(const char* _fileName, std::vector<std::string>& _strvec, int* _date)
{
	//	ファイルを開く
	std::ifstream ifs(_fileName);
	if (!ifs) { CommonDebug::Log("ファイルが開けません"); return false; }

	//	列を保存する
	std::string line;

	//	ファイルの読み込み
	getline(ifs, line);
	_strvec = _Split(line, ',');

	//	最後までループ
	for (int i = 0; i < (int)_strvec.size(); i++)
	{
		_date[i] = std::stoi(_strvec.at(i));
	}

	return true;
}

//-----------------------------------------------------------------------------
//	@brief	書き込み
//-----------------------------------------------------------------------------
bool LoadFile::_Load(const char* _fileName, std::vector<std::string>& _strvec, float* _date)
{
	//	ファイルを開く
	std::ifstream ifs(_fileName);
	if (!ifs) { CommonDebug::Log("ファイルが開けません"); return false; }

	//	列を保存する
	std::string line;

	//	ファイルの読み込み
	getline(ifs, line);
	_strvec = _Split(line, ',');

	//	最後までループ
	for (int i = 0; i < (int)_strvec.size(); i++)
	{
		if (i > 10) { break; }
		_date[i] = std::stof(_strvec.at(i));
	}

	return true;
}


//-----------------------------------------------------------------------------
//	@brief	ソート
//-----------------------------------------------------------------------------
void LoadFile::_Sort(std::vector<std::string>& _strvec, const float _writingVal, int* _date)
{
	//	書き込んであるデータと今回のデータの比較
	int val = (int)_writingVal;
	for (int i = 0; i < (int)_strvec.size(); i++)
	{
		//	得点がないときは、処理しない
		if (val <= 0.0f) { break; }

		//	データの入れ替え
		if (val >= _date[i])
		{
			int tmp = _date[i];
			_date[i] = val;
			val = tmp;
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	ソート
//-----------------------------------------------------------------------------
void LoadFile::_Sort(std::vector<std::string>& _strvec, const float _writingVal, float* _date)
{
	//	書き込んであるデータと今回のデータの比較
	float val = _writingVal;
	for (int i = 0; i < (int)_strvec.size(); i++)
	{
		//	得点がないときは、処理しない
		if (val <= 0.0f) { break; }

		//	データの入れ替え
		if (val >= _date[i])
		{
			float tmp = _date[i];
			_date[i] = val;
			val = tmp;
		}
	}
}

//-----------------------------------------------------------------------------
//	@brief	書き込み
//-----------------------------------------------------------------------------
bool LoadFile::_ToWrite(const char* _fileName, std::vector<std::string>& _strvec, int* _date)
{
	//	ファイルに追加で書き込む
	std::ofstream fout(_fileName);
	if (!fout) { CommonDebug::Log("ファイルが開けません"); return false; }

	//	書き込み
	for (int i = 0; i < (int)_strvec.size(); i++)
	{
		//	カンマ区切りで書き込む
		fout << _date[i] << "," << std::flush;
	}

	return true;
}

//-----------------------------------------------------------------------------
//	@brief	書き込み
//-----------------------------------------------------------------------------
bool LoadFile::_ToWrite(const char * _fileName, std::vector<std::string>& _strvec, float* _date)
{
	//	ファイルに追加で書き込む
	std::ofstream fout(_fileName);
	if (!fout) { CommonDebug::Log("ファイルが開けません"); return false; }

	//	書き込み
	for (int i = 0; i < (int)_strvec.size(); i++)
	{
		//	カンマ区切りで書き込む
		fout << _date[i] << "," << std::flush;
	}

	return true;
}