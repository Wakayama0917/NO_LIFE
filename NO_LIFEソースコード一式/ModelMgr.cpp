#include "ModelMgr.h"

void ModelMgr::Init()
{
	//���̉�
	m_csvModelData = std::make_unique<CSVFile>();
	//���f���̃p�X���������t�@�C����ǂݍ���
	m_csvModelData->CsvRead("assets/csv/ModelDataAll.csv", ',');
	//�S�ēǂݍ���
	for (int i = 0; i < m_csvModelData->GetIndex().size(); i++)
	{
		ModelMgr::GetInstance().LoadModel(m_csvModelData->GetModelName().at(i), "assets/shader/vs.cso",
			"assets/shader/ps.cso", m_csvModelData->GetTexFolder().at(i));
	}
}

bool ModelMgr::LoadModel(std::string _modelfilename, std::string _vsfilename,
	std::string _psfilename, std::string _texfilefolder){

	std::unique_ptr<CModel>	p;

	p = std::make_unique<CModel>();

	// ���f���ǂݍ���
	bool sts = p->Init(_modelfilename.c_str(), _vsfilename.c_str(), _psfilename.c_str(), _texfilefolder.c_str());
	if (!sts) {
		return false;
	}

	// unordered_map�R���e�i�Ɋi�[
	m_modelhashmap[_modelfilename].swap(p);

	return true;
}

void ModelMgr::Draw(ALLMODEL _modelNum, XMFLOAT4X4& _mtxworld)
{
	//�w�肳�ꂽ���f���̎��Ԃ��Ăяo���A�}�g���b�N�X��n���ĕ`��
	ModelMgr::GetInstance().GetModelPtr(m_csvModelData->GetModelName().at(
		static_cast<int>(_modelNum)))->Draw(_mtxworld);
}
