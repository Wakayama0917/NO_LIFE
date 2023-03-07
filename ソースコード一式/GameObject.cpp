#include "gameobject.h"
#include "dx11mathutil.h"
#include "ModelMgr.h"

/*---------------------------------------
�Q�̃x�N�g������N�I�[�^�j�I�����쐬����
-----------------------------------------*/
DirectX::XMFLOAT4 GameObject::RotationArc(DirectX::XMFLOAT3 _v0, DirectX::XMFLOAT3 _v1, float& _d)
{
	DirectX::XMFLOAT3		Axis;		// ��
	DirectX::XMFLOAT4		q;			// �N�I�[�^�j�I��


	DX11Vec3Cross(Axis, _v0, _v1);

	DX11Vec3Dot(_d, _v0, _v1);
	// �^�[�Q�b�g�̕����Ǝ��@�̕������قƂ�ǈ�v�����ꍇ�A���ς̒l���P�𒴂���ꍇ������i���������_�̌덷�̂��߁j
	if (_d > 1.0) _d = 1.0;							// �␳���Ă��
	// �^�[�Q�b�g�̕����Ǝ��@�̕������قƂ�ǈ�v�����ꍇ�A���ς̒l��-�P�������ꍇ������i���������_�̌덷�̂��߁j
	if (_d <= -1.0) _d = -1.0f;						// �␳���Ă��

	float s = (float)sqrtf((1 + _d) * 2);
	if (s <= FLT_EPSILON) {

		//		DX11QtIdentity(q);			// �^�[�Q�b�g�ǂ��z����
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
	//���E���쐻
	// ���f���̑S���_�𔲂��o��
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
	//// ���E��������
	//m_collision->Init(
	//	vertices1,					// ���_�Q
	//	XMFLOAT4(1, 1, 1, 0.3f));	// ���_�J���[
	////m_collision2->Init(
	////	vertices1,					// ���_�Q
	////	XMFLOAT4(1, 1, 1, 0.3f));	// ���_�J���[

}

void GameObject::CollisionInit()
{
	//���E���쐻
	// ���f���̑S���_�𔲂��o��
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
	// ���E��������
	m_collision->Init(
		vertices1,					// ���_�Q
		XMFLOAT4(1, 1, 1, 0.3f));	// ���_�J���[
	//m_collision2->Init(
	//	vertices1,					// ���_�Q
	//	XMFLOAT4(1, 1, 1, 0.3f));	// ���_�J���[
}

float GameObject::Distance(const DirectX::XMFLOAT3 _targetPosition)
{
	float num = 0;//����
	float numX = 0;
	float numZ = 0;

	/*************************************************************
	* X����Z���݂̂̔�����s��
	* �P�ڂ�X�ƂQ�ڂ�X�̍���0�łȂ����m�F
	* �P�ڂ�X�ƂQ�ڂ�X�̍���2�悷��
	* �P�ڂ�Z�ƂQ�ڂ�Z�̍���0�łȂ����m�F
	* �P�ڂ�Z�ƂQ�ڂ�Z�̍���2�悷��
	* ���҂𑫂������̔񕉂̕�������Ԃ��B
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
	//������
	num = sqrt(numX + numZ);

	return num;
}

void GameObject::SetModel()
{
	//���g��END�Ȃ珉��������Ă��Ȃ��̂ŃG���[
	if (m_modelNum != ALLMODEL::END)
	{
		//�ȉ������蔻��̏������A�g�嗦�ݒ�ς݂Ȃ̂ŏ���
	//�����蔻�菉����
		CollisionInit();
		//�`�掞�̃��f���̑傫��������
		DX11MtxScale(m_expand, m_expand, m_expand, m_bigMtx);//�傫�����邽�߂�4x4���쐻
	}
	else
	{
		// ����ɍs���Ȃ������ꍇ
		Texture tex;
		MessageBox(nullptr, "���f�����ݒ肳��Ă��܂���",
			"Error!", MB_ICONERROR | MB_OK);
		tex.texture = nullptr;
	}
}




