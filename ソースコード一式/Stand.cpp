#include "stand.h"
#include "ModelMgr.h"
#include "EnumAllModel.h"

bool Stand::Init(){
	//�s�񏉊���
	DX11MtxIdentity(m_mtx);

	//��̈ʒu��������Amtx�ŏ����o���Ă���̂ł�����������B
	m_mtx._42 -= 1.5f;

	//���f����ݒ�
	m_modelNum = ALLMODEL::STAGE_NORMAL1;

	return true;
}

void Stand::Draw(){
	//���f���`��
	//m_model->Draw(m_mtx);
}

void Stand::Draw_transform(float _x, float _y, float _z){
	DirectX::XMFLOAT4X4 big, big2;
	DX11MtxScale(_x, _y, _z, big);//�傫�����邽�߂�4x4���쐻
	DX11MtxMultiply(big2, big, m_mtx);//�擪�Ɍ��2�����������̂�����A���݈ʒu
	
	//m_model->Draw(big2);
	ModelMgr::GetInstance().Draw(ALLMODEL::STAGE_NORMAL1, big2);
}

void Stand::Update(){
	//���͉�������
}

void Stand::Finalize(){
	//m_model->Uninit();
}


