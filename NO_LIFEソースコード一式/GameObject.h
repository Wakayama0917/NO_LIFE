#pragma once
#include <directxmath.h>
#include <memory>
#include "dx11mathutil.h"
#include "BoundingSphere.h"
#include "CModel.h"
#include "CSV.h"
#include "EnumAllModel.h"

using namespace std;

//�����Ă邩�ǂ���
enum  class STATE
{
	LIVE,
	DEAD
};

class GameObject {
	uint64_t m_id = 0;						//���ʎq
	
protected:
	/*************************************************************
	* �X�e�[�^�X�֘A�̕ϐ��A�֐�
	*************************************************************/
	STATE		m_state;//�X�e�[�^�X
public:
	//���݃X�e�[�^�X�i�����Ă�A����ł�j��Ԃ�
	STATE GetState() {
		return m_state;
	}
	//�X�e�[�^�X��Ԃ��Z�b�g
	void SetState(STATE _status) {
		m_state = _status;
	}
	//�X�e�[�^�X�����S�ɕς���
	void SetDead() { m_state = STATE::DEAD; };
protected:
	/*************************************************************
	* �ʒu��p���n�ϐ�
	*************************************************************/
	DirectX::XMFLOAT3 m_pos{};				//�ʒu
	DirectX::XMFLOAT4X4 m_mtx{};			//�p��
	float m_expand = 1.0f;					//�g�嗦
	DirectX::XMFLOAT4X4 m_bigMtx;			//�g�嗦���������s��
	
public:
	/*************************************************************
	* �ʒu��p���n�֐�
	*************************************************************/
	//���O�̓G���[���o���Ă����
	GameObject() {};
	virtual ~GameObject() {}

	//�Q�̃x�N�g������N�I�[�^�j�I�����쐬����
	DirectX::XMFLOAT4 RotationArc(DirectX::XMFLOAT3 _v0, DirectX::XMFLOAT3 _v1, float& _d);

	// �p�����擾
	DirectX::XMFLOAT4X4 GetMtx() {
		return m_mtx;
	}

	// �ʒu���擾
	DirectX::XMFLOAT3 GetPos() {
		return m_pos;
	}

	// �h�c���擾
	uint64_t GetID() {
		return m_id;
	}

	/*************************************************************
	* �����蔻��n
	*************************************************************/
protected:
	// ���E���v���C���[�A�X�}�[�g�|�C���^�\
	std::unique_ptr<BoundingSphere> m_collision;
	//���L�����̂��������ꍇ�́Arelease�֐����g��
	//���̏ꍇ�A�������̉�����͎̂����ōs���K�v������
	//pint = ptr.release();
	//delete pint;
public:
	//���̉��A�e����Init���ɌĂяo��
	void CollisionCreateInit() {
		if (m_collision == nullptr)
		{
			//std::unique_ptr<BoundingSphere> ptr;
			m_collision = std::make_unique<BoundingSphere>();
		}
		else
		{
			// ����ɍs���Ȃ������ꍇ
			Texture tex;
			MessageBox(nullptr, "�X�t�B�A�I�u�W�F�N�g�������ł��܂���",
				"Error!", MB_ICONERROR | MB_OK);
			tex.texture = nullptr;
		}
		
	};

	//�����蔻�菉���������A���f���ǂݍ��݌�Ɏg�p
	void CollisionInitOld();
	void CollisionInit();

	//�����蔻��Ԃ��悤
	BoundingSphere GetSphere() {
		return *m_collision;
		//return *m_collision2;
	}

	//�����蔻��̍ۂ̔��a��Ԃ��悤�֐�
	float GetRadius() {
		//�g�債�Ă����ꍇ�A���̕�������
		return m_collision->GetHitdata().r * m_expand;
	}

	//���g�Ƒ���̓����蔻������
	float Distance(const DirectX::XMFLOAT3 _targetPosition);

	/*************************************************************
	* ���f���n
	*************************************************************/
protected:
	//�S�ẴI�u�W�F�N�g�ɕK�v�ȏ��
	//���̔ԍ��̃��f���̓����蔻��𐶐����A�`�悷��B
	//END�ŏ������AEND�������Ă���ƃG���[���N����悤�ɐ���
	ALLMODEL m_modelNum = ALLMODEL::END;//���̃��f���̔ԍ�

	//CModel* m_model;//���f��
public:
	ALLMODEL GetModelNum() {
		return m_modelNum;
	}

	//void SetModel(CModel* _p) {
	//	m_model = _p;

	//	//�ȉ������蔻��̏������A�g�嗦�ݒ�ς݂Ȃ̂ŏ���
	//	//�����蔻�菉����
	//	CollisionInitOld();
	//	//�`�掞�̃��f���̑傫��������
	//	DX11MtxScale(m_expand, m_expand, m_expand, m_bigMtx);//�傫�����邽�߂�4x4���쐻

	//}
	void SetModel();

	/*const CModel* GetModel() {
		return m_model;
	}*/

};