#include "skydome.h"
#include "dx11mathutil.h"
#include "ModelMgr.h"
#include "EnumAllModel.h"

bool Skydome::Init() {
	//�s�񏉊���
	DX11MtxIdentity(m_mtx);

	//���f����ݒ�
	m_modelNum = ALLMODEL::SKY_NORMAL1;

	return true;
}

void Skydome::Draw() {
	//���f���`��
	//m_model->Draw(m_mtx);
	ModelMgr::GetInstance().Draw(m_modelNum, m_mtx);
}

void Skydome::Update(){
	
}

void Skydome::Finalize(){
	//m_model->Uninit();
}


