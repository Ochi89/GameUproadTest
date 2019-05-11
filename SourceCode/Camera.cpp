
//=============================================================================
//	@file	Camera.cpp
//	@brief	カメラ
//	@autor	Takuya Ochi
//	@date	2018/9/29
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "Camera.h"
#include "Common.h"
#include "PlayerManager.h"
#include "PlayerBase.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const float Camera::V_ANGLE_SPEED_MOUSE = 0.08f;		// 垂直角度の回転速度
const float Camera::H_ANGLE_SPEED_MOUSE = 0.08f;		// 水平角度の回転速度
const float Camera::V_ANGLE_SPEED_PAD = 1.2f;			// 垂直角度の回転速度
const float Camera::H_ANGLE_SPEED_PAD = 1.5f;			// 水平角度の回転速度
const int	Camera::MAX_H_ANGLE = 10;					// 垂直角度の最大
const int	Camera::MIN_H_ANGLE = -10;					// 垂直角度の最小
const int	Camera::MAX_V_ANGLE = 20;					// 水平角度の最大
const int	Camera::MIN_V_ANGLE = -20;					// 水平角度の最小
const float Camera::LERP_SPEED = 0.5f;					//	線形保管の速度
const float Camera::HEIGHT_LENGTH = 60.0f;				//	高さの長さ
const float Camera::DEPTH_LENGTH = 100.0f;				//	奥行きの長さ

//-----------------------------------------------------------------------------
// @brief  コンストラクタ
//-----------------------------------------------------------------------------
Camera::Camera()
{
	// 奥行0.1f～1000.0fまでをカメラの描画範囲とする
	SetCameraNearFar(1.0f, 10000.0f);

	m_pos = CommonConstant::ORIGIN;
	m_lookAtPos = CommonConstant::ORIGIN;
	m_lookAtPos.y = HEIGHT_LENGTH;

	m_verticalAngle = 0.0f;
	m_horizontalAngle = 0.0f;
	m_productionTime = 0.0f;
	m_isProduction = false;
}

//-----------------------------------------------------------------------------
// @brief  デストラクタ
//-----------------------------------------------------------------------------
Camera::~Camera()
{
	// 処理なし
}

//-----------------------------------------------------------------------------
// @brief	タイトル用の初期処理
//-----------------------------------------------------------------------------
void Camera::InitializeForTitle()
{
	//	タイトル用の初期化
	m_pos = CommonConstant::ORIGIN;
	m_lookAtPos = CommonConstant::ORIGIN;

	m_verticalAngle = 20.0f;
	m_horizontalAngle = -45.0f;
	m_productionTime = 0.0f;
	m_isProduction = false;
}

//-----------------------------------------------------------------------------
// @brief	初期処理
//-----------------------------------------------------------------------------
void Camera::Initialize()
{
	m_pos = CommonConstant::ORIGIN;
	m_lookAtPos = CommonConstant::ORIGIN;
	m_lookAtPos.y = HEIGHT_LENGTH;

	m_verticalAngle = 0.0f;
	m_horizontalAngle = 0.0f;
	m_productionTime = 0.0f;
	m_isProduction = false;
}

//-----------------------------------------------------------------------------
// @brief	タイトル用の更新処理
//-----------------------------------------------------------------------------
void Camera::UpdateForTitle(PlayerManager& _playerManager)
{
	if (!m_isProduction)
	{
		//	タイトル用の注視点の設定
		_SetGazingPointForTitle(_playerManager);

		//	カメラの角度を反映した位置を求める
		VECTOR tmpPos = _PosAccordingToAngle();

		//	求めた座標に注視点の位置を加算したものがカメラの位置
		VECTOR target = VAdd(tmpPos, m_lookAtPos);

		//	線形保管を使い追う
		m_pos = CommonFunction::Lerp(m_pos, target, LERP_SPEED);
	}

	//	タイトル用の演出
	_ProductionForTitle(_playerManager);

	// カメラの設定に反映する
	SetCameraPositionAndTarget_UpVecY(m_pos, m_lookAtPos);
}

//-----------------------------------------------------------------------------
// @brief	リザルト用の更新処理
//-----------------------------------------------------------------------------
void Camera::UpdateForResult(PlayerManager & _playerManager)
{
	//	注視点の設定
	_SetGazingPoint();

	//	カメラの角度を反映した位置を求める
	VECTOR tmpPos = _PosAccordingToAngle();

	//	求めた座標に注視点の位置を加算したものがカメラの位置
	VECTOR target = VAdd(tmpPos, m_lookAtPos);

	//	線形保管を使い追う
	m_pos = CommonFunction::Lerp(m_pos, target, LERP_SPEED);

	// カメラの設定に反映する
	SetCameraPositionAndTarget_UpVecY(m_pos, m_lookAtPos);
}

//-----------------------------------------------------------------------------
// @brief	更新処理
//-----------------------------------------------------------------------------
void Camera::Update(PlayerManager& _playerManager)
{
	if (!m_isProduction)
	{
		//	注視点の設定
		_SetGazingPoint();

		//	カメラの角度を反映した位置を求める
		VECTOR tmpPos = _PosAccordingToAngle();

		//	求めた座標に注視点の位置を加算したものがカメラの位置
		VECTOR target = VAdd(tmpPos, m_lookAtPos);

		//	線形保管を使い追う
		m_pos = CommonFunction::Lerp(m_pos, target, LERP_SPEED);
	}

	//	カメラ演出
	_Production(_playerManager);

	// カメラの設定に反映する
	SetCameraPositionAndTarget_UpVecY(m_pos, m_lookAtPos);
}

//-----------------------------------------------------------------------------
// @brief	タイトル用の演出
//-----------------------------------------------------------------------------
void Camera::_ProductionForTitle(PlayerManager& _playerManager)
{
	if (PRODUCTION->GetIsTitleProduction())
	{
		//	演出開始
		m_isProduction = true;
	}
}

//-----------------------------------------------------------------------------
// @brief	演出
//-----------------------------------------------------------------------------
void Camera::_Production(PlayerManager& _playerManager)
{
	//	シーン演出
	_SceneProduction(_playerManager);

	//	必殺演出
	_SpecialProduction(_playerManager);

	//	クリア演出
	_ClearProduction(_playerManager);
}

//-----------------------------------------------------------------------------
// @brief	シーン演出
//-----------------------------------------------------------------------------
void Camera::_SceneProduction(PlayerManager& _playerManager)
{
	//	シーン変更
	if (PRODUCTION->GetIsSceneProduction())
	{
		m_productionTime++;
		const bool isProduction1 = m_productionTime >= 0.0f && m_productionTime <= 100.0f;
		const bool isProduction2 = m_productionTime >= 100.0f && m_productionTime <= 210.0f;
		const bool isProduction3 = m_productionTime >= 210.0f && m_productionTime <= 300.0f;
		const bool isProduction4 = m_productionTime >= 300.0f && m_productionTime <= 330.0f;
		const bool isProduction5 = m_productionTime >= 330.0f && m_productionTime <= 360.0f;

		const bool isProductionInit = m_productionTime == 100.0f;
		if (isProductionInit) { m_lookAtPos = CommonConstant::ORIGIN; }

		if (isProduction1)
		{
			PlayerBase* player = _playerManager.GetPlayerPtr();
			if (player)
			{
				//	プレイヤーは移動停止
				player->SetIsStopMove(true);
				player->SetAngle(VGet(0.0f, 1.4f, 0.0f));

				//	プレイヤーのポジションをを初期位置にして、
				//	演出をする
				VECTOR playerPos = player->GetPos();
				const float playerLerpSpeed = 0.05f;
				playerPos = CommonFunction::Lerp(playerPos, player->GetStartPos(), playerLerpSpeed);
				player->SetPos(playerPos);

				//	演出開始
				m_isProduction = true;

				//	点滅している場合、
				//	変なので、点滅を消す
				player->SetIsInvincible(false);

				//	カメラの回転
				const float angleSpeed = 0.2f;
				m_verticalAngle += angleSpeed;

				//	注視点を求める
				const float cameraLerpSpeed = 0.8f;
				m_lookAtPos = CommonFunction::Lerp(m_lookAtPos, player->GetPos(), cameraLerpSpeed);
				m_lookAtPos.y = player->GetPos().y;
				m_lookAtPos.z = 0.0f;

				//	カメラの角度を反映した位置を求める
				VECTOR tmpPos = _PosAccordingToAngle();

				//	求めた座標に注視点の位置を加算したものがカメラの位置
				VECTOR target = VAdd(tmpPos, m_lookAtPos);

				//	線形保管を使い追う
				m_pos = CommonFunction::Lerp(m_pos, target, cameraLerpSpeed);
			}
		}
		if (isProduction2)
		{
			PlayerBase* player = _playerManager.GetPlayerPtr();
			if (player)
			{
				//	カメラの回転
				const float angleSpeed = 3.0f;
				m_horizontalAngle -= angleSpeed;

				//	注視点を求める
				const float lerpSpeed = 0.2f;
				m_lookAtPos = CommonFunction::Lerp(m_lookAtPos, player->GetPos(), lerpSpeed);
				m_lookAtPos.y = player->GetPos().y;
				m_lookAtPos.z = 0.0f;

				//	カメラの角度を反映した位置を求める
				VECTOR tmpPos = _PosAccordingToAngle();

				//	求めた座標に注視点の位置を加算したものがカメラの位置
				VECTOR target = VAdd(tmpPos, m_lookAtPos);

				//	線形保管を使い追う
				m_pos = CommonFunction::Lerp(m_pos, target, lerpSpeed);
			}
		}
		if (isProduction3)
		{
			PlayerBase* player = _playerManager.GetPlayerPtr();
			if (player)
			{
				//	プレイヤーの演出開始
				player->SetIsSceneProduction(true);
			}
		}
		if (isProduction4)
		{
			//	注視点の設定
			_SetGazingPoint();

			//	角度の初期化
			const float angleSpeed = 0.64f;
			m_verticalAngle -= angleSpeed;
			m_horizontalAngle--;

			//	カメラの角度を反映した位置を求める
			VECTOR tmpPos = _PosAccordingToAngle();

			//	求めた座標に注視点の位置を加算したものがカメラの位置
			VECTOR target = VAdd(tmpPos, m_lookAtPos);

			//	線形保管を使い追う
			const float lerpSpeed = 0.2f;
			m_pos = CommonFunction::Lerp(m_pos, target, lerpSpeed);

		}
		if (isProduction5)
		{
			PlayerBase* player = _playerManager.GetPlayerPtr();
			if (player)
			{
				//	プレイヤーは移動可能
				player->SetIsStopMove(false);

				//	プレイヤーの演出終了
				player->SetIsSceneProduction(false);

				//	プレイヤーの登場
				player->SetIsEmerge(true);

				//	演出終了
				m_isProduction = false;
				m_productionTime = 0.0f;
				m_verticalAngle = 0.0f;
				m_horizontalAngle = 0.0f;
				PRODUCTION->SetIsSceneProduction(false);
			}
		}
	}
}

//-----------------------------------------------------------------------------
// @brief	必殺演出
//-----------------------------------------------------------------------------
void Camera::_SpecialProduction(PlayerManager& _playerManager)
{
	//	必殺時
	if (PRODUCTION->GetIsSpecialProduction())
	{
		m_productionTime++;
		const bool isProduction1 = m_productionTime >= 5.0f && m_productionTime <= 25.0f;
		const bool isProduction2 = m_productionTime >= 285.0f && m_productionTime <= 305.0f;
		const bool isProduction3 = m_productionTime >= 305.0f;

		if (isProduction1)
		{
			PlayerBase* player = _playerManager.GetPlayerPtr();
			if (player)
			{
				//	プレイヤーは移動停止
				player->SetIsStopMove(true);
				player->SetAngle(VGet(0.0f, 1.4f, 0.0f));

				//	カメラの角度を変更
				const float horizontalAngleSpeed = 2.0f;
				const float verticalAngleSpeed = 1.0f;
				m_horizontalAngle -= horizontalAngleSpeed;
				m_verticalAngle += verticalAngleSpeed;

				//	注視点を求める
				VECTOR adjustment = VGet(60.0f, 10.0f, 0.0f);
				VECTOR targetPos = player->GetPos();
				targetPos.x += adjustment.x;
				targetPos.y -= adjustment.y;
				m_lookAtPos = targetPos;

				//	カメラの角度を反映した位置を求める
				VECTOR tmpPos = _PosAccordingToAngle();

				//	求めた座標に注視点の位置を加算したものがカメラの位置
				VECTOR target = VAdd(tmpPos, m_lookAtPos);

				//	線形保管を使い追う
				const float lerpSpeed = 0.2f;
				m_pos = CommonFunction::Lerp(m_pos, target, lerpSpeed);

				//	点滅している場合、
				//	変なので、点滅を消す
				player->SetIsInvincible(false);

				//	演出開始
				m_isProduction = true;
			};

		}
		if (isProduction2)
		{
			PlayerBase* player = _playerManager.GetPlayerPtr();
			if (player)
			{
				//	カメラの角度を変更する
				const float horizontalAngleSpeed = 2.0f;
				const float verticalAngleSpeed = 1.0f;
				m_horizontalAngle += horizontalAngleSpeed;
				m_verticalAngle -= verticalAngleSpeed;

				//	注視点を求める
				VECTOR adjustment = VGet(60.0f, 10.0f, 0.0f);
				VECTOR targetPos = player->GetPos();
				targetPos.x += adjustment.x;
				targetPos.y -= adjustment.y;
				m_lookAtPos = targetPos;

				//	カメラの角度を反映した位置を求める
				VECTOR tmpPos = _PosAccordingToAngle();

				//	求めた座標に注視点の位置を加算したものがカメラの位置
				VECTOR target = VAdd(tmpPos, m_lookAtPos);

				//	線形保管を使い追う
				const float lerpSpeed = 0.2f;
				m_pos = CommonFunction::Lerp(m_pos, target, lerpSpeed);
			};
		}
		if (isProduction3)
		{
			PlayerBase* player = _playerManager.GetPlayerPtr();
			if (player)
			{
				//	プレイヤーを移動可能にし、
				//	少しだけ無敵にする
				player->SetIsStopMove(false);
				player->SetIsInvincibleAfterLaser(true);

				//	演出終了
				m_isProduction = false;
				m_productionTime = 0.0f;
				PRODUCTION->ResetSpecialProduction();
			}
		}
	}
}

//-----------------------------------------------------------------------------
// @brief	クリア演出
//-----------------------------------------------------------------------------
void Camera::_ClearProduction(PlayerManager & _playerManager)
{
	//	クリア時
	if (PRODUCTION->GetIsClearProduction())
	{
		m_productionTime++;
		const bool isProduction1 = m_productionTime >= 5.0f && m_productionTime <= 60.0f;
		const bool isProduction2 = m_productionTime >= 60.0f && m_productionTime <= 130.0f;
		const bool isProduction3 = m_productionTime >= 160.0f;

		if (isProduction1)
		{
			PlayerBase* player = _playerManager.GetPlayerPtr();
			if (player)
			{
				//	プレイヤーは移動停止
				player->SetIsStopMove(true);
				player->SetAngle(VGet(0.0f, 1.4f, 0.0f));

				//	プレイヤーを画面の中心へ移動
				const float lerpSpeed = 0.1f;
				const VECTOR screenCenter = VGet(0.0f, 55.0f, 0.0f);
				VECTOR playerPos = player->GetPos();
				playerPos = CommonFunction::Lerp(playerPos, screenCenter, lerpSpeed);
				player->SetPos(playerPos);

				//	演出開始
				m_isProduction = true;

				//	点滅している場合、
				//	変なので、点滅を消す
				player->SetIsInvincible(false);
			}
		}
		if (isProduction2)
		{
			PlayerBase* player = _playerManager.GetPlayerPtr();
			if (player)
			{
				//	カメラの角度を変更
				const float horizontalAngleSpeed = 4.0f;
				m_horizontalAngle += horizontalAngleSpeed;

				//	注視点を求める
				VECTOR targetPos = player->GetPos();
				m_lookAtPos = targetPos;

				//	カメラの角度を反映した位置を求める
				VECTOR tmpPos = _PosAccordingToAngle();

				//	求めた座標に注視点の位置を加算したものがカメラの位置
				VECTOR target = VAdd(tmpPos, m_lookAtPos);

				//	線形保管を使い追う
				const float lerpSpeed = 0.2f;
				m_pos = CommonFunction::Lerp(m_pos, target, lerpSpeed);
			}
		}
		if (isProduction3)
		{
			PlayerBase* player = _playerManager.GetPlayerPtr();
			if (player)
			{
				//	プレイヤーの演出開始
				player->SetIsClearProduction(true);
			}
		}
	}
}

//-----------------------------------------------------------------------------
// @brief	タイトル用の注視点の設定
//-----------------------------------------------------------------------------
void Camera::_SetGazingPointForTitle(PlayerManager& _playerManager)
{
	PlayerBase* player = _playerManager.GetPlayerPtr();
	if (player)
	{
		//	注視点を求める
		m_lookAtPos = VGet(60.0f, 42.0f, 16.0f);

		//	カメラの角度を反映した位置を求める
		VECTOR tmpPos = _PosAccordingToAngle();

		//	求めた座標に注視点の位置を加算したものがカメラの位置
		VECTOR target = VAdd(tmpPos, m_lookAtPos);

		//	線形保管を使い追う
		const float lerpSpeed = 0.2f;
		m_pos = CommonFunction::Lerp(m_pos, target, lerpSpeed);
	}
}

//-----------------------------------------------------------------------------
// @brief	注視点の取得
//-----------------------------------------------------------------------------
void Camera::_SetGazingPoint()
{
	//	奥行きのみ追う
	VECTOR lookAtPos = CommonConstant::ORIGIN;
	lookAtPos.y = HEIGHT_LENGTH;

	m_lookAtPos = CommonFunction::Lerp(m_lookAtPos, lookAtPos, 0.05f);
}

//-----------------------------------------------------------------------------
// @brief	角度に応じた位置
//-----------------------------------------------------------------------------
VECTOR Camera::_PosAccordingToAngle()
{
	// カメラの位置の調整
	VECTOR tmpPos1;
	VECTOR tmpPos2;

	// 垂直角度を反映した位置
	float m_sinParam = sinf(CommonFunction::DegToRad(m_verticalAngle));
	float m_cosParam = cosf(CommonFunction::DegToRad(m_verticalAngle));
	tmpPos1.x = 0.0f;
	tmpPos1.y = m_sinParam * DEPTH_LENGTH;
	tmpPos1.z = -m_cosParam * DEPTH_LENGTH;

	// 水平角度を反映した位置
	m_sinParam = sinf(CommonFunction::DegToRad(m_horizontalAngle));
	m_cosParam = cosf(CommonFunction::DegToRad(m_horizontalAngle));
	tmpPos2.x = m_cosParam * tmpPos1.x - m_sinParam * tmpPos1.z;
	tmpPos2.y = tmpPos1.y;
	tmpPos2.z = m_sinParam * tmpPos1.x + m_cosParam * tmpPos1.z;

	return tmpPos2;
}
