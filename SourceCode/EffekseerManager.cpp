
//=============================================================================
//	@file	EffekseerManager.cpp
//	@brief	EffekseerManager
//	@autor	Takuya Ochi
//	@date	2018/12/31
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "EffekseerManager.h"

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
EffekseerManager::EffekseerManager()
{
	//	Effekseer初期化処理
	const int maxParticleNum = 8000;
	CommonDebug::Assert((Effkseer_Init(maxParticleNum) == -1), " [ EffekseerManager.cpp ] : error : failed to initialize the effkseer .");
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
EffekseerManager::~EffekseerManager()
{
	//	Effekseerの終了処理
	Effkseer_End();
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void EffekseerManager::Update()
{
	//	DXライブラリのカメラとEffekseerEmitterのカメラを同期
	Effekseer_Sync3DSetting();

	//	EffekseerEmitterにより再生中のエフェクトを更新
	UpdateEffekseer3D();
}

//-----------------------------------------------------------------------------
//	@brief	描画処理
//-----------------------------------------------------------------------------
void EffekseerManager::Draw()
{
	//	Zバッファを使用する設定に変更
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	//	EffekseerEmitterにより再生中のエフェクトを描画
	DrawEffekseer3D();

	//	Zバッファを使用しない設定に変更
	SetUseZBuffer3D(FALSE);
	SetWriteZBuffer3D(FALSE);
}
