#include "gameobject.h"
#include "dx11mathutil.h"
#include "ModelMgr.h"

/*---------------------------------------
２つのベクトルからクオータニオンを作成する
-----------------------------------------*/
DirectX::XMFLOAT4 GameObject::RotationArc(DirectX::XMFLOAT3 _v0, DirectX::XMFLOAT3 _v1, float& _d)
{
	DirectX::XMFLOAT3		Axis;		// 軸
	DirectX::XMFLOAT4		q;			// クオータニオン


	DX11Vec3Cross(Axis, _v0, _v1);

	DX11Vec3Dot(_d, _v0, _v1);
	// ターゲットの方向と自機の方向がほとんど一致した場合、内積の値が１を超える場合がある（浮動小数点の誤差のため）
	if (_d > 1.0) _d = 1.0;							// 補正してやる
	// ターゲットの方向と自機の方向がほとんど一致した場合、内積の値が-１を下回る場合がある（浮動小数点の誤差のため）
	if (_d <= -1.0) _d = -1.0f;						// 補正してやる

	float s = (float)sqrtf((1 + _d) * 2);
	if (s <= FLT_EPSILON) {

		//		DX11QtIdentity(q);			// ターゲット追い越した
		float radian = acos(_d);

		q.x = Axis.x * sin(radian / 2.0f);
		q.y = Axis.y * sin(radian / 2.0f);
		q.z = Axis.z * sin(radian / 2.0f);
		q.w = cos(radian / 2.0f);

	}
	else {
		q.x = Axis.x / s;
		q.y = Axis.y / s;
		q.z = Axis.z / s;
		q.w = s / 2;
	}

	return q;
}

void GameObject::CollisionInitOld()
{
	//境界球作製
	// モデルの全頂点を抜き出す
	//const CModel* pmodel1 = GetModel();
	////const CModel* pmodel1 = (ModelMgr::GetInstance().GetModelPtr("assets/fbx/Player/Player.fbx"));
	//const ModelData& md1 = pmodel1->GetModelData();
	//const std::vector<Mesh>& meshes1 = md1.GetMeshes();

	//std::vector<XMFLOAT3> vertices1;

	//for (auto& m : meshes1) {
	//	for (auto& v : m.m_vertices) {
	//		vertices1.emplace_back(v.m_Pos);
	//	}
	//}
	//// 境界球初期化
	//m_collision->Init(
	//	vertices1,					// 頂点群
	//	XMFLOAT4(1, 1, 1, 0.3f));	// 頂点カラー
	////m_collision2->Init(
	////	vertices1,					// 頂点群
	////	XMFLOAT4(1, 1, 1, 0.3f));	// 頂点カラー

}

void GameObject::CollisionInit()
{
	//境界球作製
	// モデルの全頂点を抜き出す
	//const CModel* pmodel1 = GetModel();
	const CModel* pmodel1 = (ModelMgr::GetInstance().GetModelPtr(m_modelNum));
	const ModelData& md1 = pmodel1->GetModelData();
	const std::vector<Mesh>& meshes1 = md1.GetMeshes();

	std::vector<XMFLOAT3> vertices1;

	for (auto& m : meshes1) {
		for (auto& v : m.m_vertices) {
			vertices1.emplace_back(v.m_Pos);
		}
	}
	// 境界球初期化
	m_collision->Init(
		vertices1,					// 頂点群
		XMFLOAT4(1, 1, 1, 0.3f));	// 頂点カラー
	//m_collision2->Init(
	//	vertices1,					// 頂点群
	//	XMFLOAT4(1, 1, 1, 0.3f));	// 頂点カラー
}

float GameObject::Distance(const DirectX::XMFLOAT3 _targetPosition)
{
	float num = 0;//距離
	float numX = 0;
	float numZ = 0;

	/*************************************************************
	* X軸とZ軸のみの判定を行う
	* １つ目のXと２つ目のXの差が0でないか確認
	* １つ目のXと２つ目のXの差を2乗する
	* １つ目のZと２つ目のZの差が0でないか確認
	* １つ目のZと２つ目のZの差を2乗する
	* 両者を足した数の非負の平方根を返す。
	*************************************************************/
	//X
	numX = m_mtx._41 - _targetPosition.x;
	if (numX != 0)
	{
		numX = pow(m_mtx._41 - _targetPosition.x, 2);
	}
	else
	{
		numX = 0.0f;
	}
	//Z
	numZ = m_mtx._43 - _targetPosition.z;
	if (numZ != 0)
	{
		numZ = pow(m_mtx._43 - _targetPosition.z, 2);
	}
	else
	{
		numZ = 0.0f;
	}
	//平方根
	num = sqrt(numX + numZ);

	return num;
}

void GameObject::SetModel()
{
	//中身がENDなら初期化されていないのでエラー
	if (m_modelNum != ALLMODEL::END)
	{
		//以下当たり判定の初期化、拡大率設定済みなので処理
	//当たり判定初期化
		CollisionInit();
		//描画時のモデルの大きさを決定
		DX11MtxScale(m_expand, m_expand, m_expand, m_bigMtx);//大きくするための4x4を作製
	}
	else
	{
		// 正常に行われなかった場合
		Texture tex;
		MessageBox(nullptr, "モデルが設定されていません",
			"Error!", MB_ICONERROR | MB_OK);
		tex.texture = nullptr;
	}
}




