
//=============================================================================
//	@file	Item.
//	@brief	アイテムひとつ分の処理
//	@autor	Takuya Ochi
//	@date	2018/12/14
//=============================================================================

//-----------------------------------------------------------------------------
//	@brief	インクルード
//-----------------------------------------------------------------------------
#include "Item.h"
#include "Common.h"

//-----------------------------------------------------------------------------
//	@brief	静的定数
//-----------------------------------------------------------------------------
const VECTOR	Item::RECT_CORRECTION = VGet(4.0f, 4.0f, 1.0f);		//	座標の補正
const float		Item::MOVE_SPEED = 0.5f;							//	移動速度
const float		Item::HIT_RADIUS = 4.0f;							//	当たり判定用の半径
const float		Item::ANGLE_SPEED_1 = 0.05f;						//	回転速度１
const float		Item::ANGLE_SPEED_2 = 0.03f;						//	回転速度２

//-----------------------------------------------------------------------------
//	@brief	コンストラクタ
//-----------------------------------------------------------------------------
Item::Item(const int _modelHandle1, const int _modelHandle2, const int _modelHandle3)
{
	//	モデルの複製する
	m_modelHandle1 = MV1DuplicateModel(_modelHandle1);
	m_modelHandle2 = MV1DuplicateModel(_modelHandle2);
	m_modelHandle3 = MV1DuplicateModel(_modelHandle3);
	CommonDebug::Assert((m_modelHandle1 <= -1), " [ Item.cpp ] : error : missing duplicat model.");
	CommonDebug::Assert((m_modelHandle2 <= -1), " [ Item.cpp ] : error : missing duplicat model.");
	CommonDebug::Assert((m_modelHandle3 <= -1), " [ Item.cpp ] : error : missing duplicat model.");

	m_pos = CommonConstant::ORIGIN;
	m_dir = VGet(-1.0f, 0.0f, 0.0f);
	m_isOneTime = false;
	m_radian = 0.0f;
	m_angle1 = CommonConstant::ORIGIN;
	m_angle2 = CommonConstant::ORIGIN;

	//	当たり判定用の構造体の初期化
	m_hitCircle.m_radius = HIT_RADIUS;
	m_hitCircle.m_centerPoint = CommonConstant::ORIGIN;
	m_hitRect.m_vertexTop = CommonConstant::ORIGIN;
	m_hitRect.m_vertexUnder = CommonConstant::ORIGIN;
}

//-----------------------------------------------------------------------------
//	@brief	デストラクタ
//-----------------------------------------------------------------------------
Item::~Item()
{
	//	最終的な解放処理
	_FinalRelease();
}

//-----------------------------------------------------------------------------
//	@brief	初期処理
//-----------------------------------------------------------------------------
void Item::Initialize()
{
	m_pos = CommonConstant::ORIGIN;
	m_dir = VGet(-1.0f, 0.0f, 0.0f);
	m_isOneTime = false;
	m_radian = 0.0f;
	m_angle1 = CommonConstant::ORIGIN;
	m_angle2 = CommonConstant::ORIGIN;

	//	当たり判定用の構造体の初期化
	m_hitCircle.m_radius = HIT_RADIUS;
	m_hitCircle.m_centerPoint = CommonConstant::ORIGIN;
	m_hitRect.m_vertexTop = CommonConstant::ORIGIN;
	m_hitRect.m_vertexUnder = CommonConstant::ORIGIN;
}

//-----------------------------------------------------------------------------
//	@brief	更新処理
//-----------------------------------------------------------------------------
void Item::Update()
{
	//	当たり判定用の中心座標の更新
	m_hitCircle.m_centerPoint = m_pos;

	//	当たり判定用の座標の更新
	m_hitRect.m_vertexTop = VSub(m_pos, RECT_CORRECTION);
	m_hitRect.m_vertexUnder = VAdd(m_pos, RECT_CORRECTION);

	//	移動処理
	_Move();

	//	回転処理
	_Rotation();
}

//-----------------------------------------------------------------------------
//	@brief	描画処理
//-----------------------------------------------------------------------------
void Item::Draw()
{
	//	モデルの描画
	MV1DrawModel(m_modelHandle1);
	MV1DrawModel(m_modelHandle2);
	MV1DrawModel(m_modelHandle3);
}

//-----------------------------------------------------------------------------
//	@brief	ショットクラス
//-----------------------------------------------------------------------------
void Item::_Move()
{
	//	移動
	VECTOR velocity = VScale(m_dir, MOVE_SPEED);
	m_pos = VAdd(m_pos, velocity);

	//	モデルのポジションを割り当て
	MV1SetPosition(m_modelHandle1, m_pos);
	MV1SetPosition(m_modelHandle2, m_pos);
	MV1SetPosition(m_modelHandle3, m_pos);
}

//-----------------------------------------------------------------------------
//	@brief	回転処理
//-----------------------------------------------------------------------------
void Item::_Rotation()
{
	m_angle1.x += ANGLE_SPEED_1;
	m_angle2.x += ANGLE_SPEED_2;

	//	モデルの回転の割り当て
	MV1SetRotationXYZ(m_modelHandle2, m_angle1);
	MV1SetRotationXYZ(m_modelHandle3, m_angle2);
}

//-----------------------------------------------------------------------------
//	@brief	最終的な解放処理
//-----------------------------------------------------------------------------
void Item::_FinalRelease()
{
	//	モデルのアンロード
	MV1DeleteModel(m_modelHandle1);
	MV1DeleteModel(m_modelHandle2);
	MV1DeleteModel(m_modelHandle3);
}
