#pragma once
#include <memory>
#include <wrl/client.h>
#include <unordered_map>
//#include <DirectXMath.h>
#include "CModel.h"
#include "CSV.h"
#include "EnumAllModel.h"
#include "DX11Settransform.h"


using Microsoft::WRL::ComPtr;

class ModelMgr {
private:
	ModelMgr(){
	}
	std::unordered_map<std::string, std::unique_ptr<CModel>> m_modelhashmap;

	//�g�p���郂�f����ǂݍ���
	std::unique_ptr<CSVFile> m_csvModelData;

public:
	ModelMgr(const ModelMgr&) = delete;
	ModelMgr& operator=(const ModelMgr&) = delete;
	ModelMgr(ModelMgr&&) = delete;
	ModelMgr& operator=(ModelMgr&&) = delete;

	static ModelMgr& GetInstance() {
		static ModelMgr Instance;
		return Instance;
	}

	virtual ~ModelMgr() {
		Finalize();
	}

	void Finalize() {
		for (auto itr = m_modelhashmap.begin(); itr != m_modelhashmap.end(); ++itr) {
			(itr->second)->Uninit();
		}
	}

	//CSV���̉������Ă���ɏ����Ă�S�Ẵ��f����ǂݍ���
	void Init();
	//���f���������A�V�F�[�_�[���Ȃǂ�����
	bool LoadModel(std::string _modelfilename, std::string _vsfilename, std::string _psfilename, std::string _texfilefolder);
	//�w�肵�����f����`�悷��A�����̓}�g���b�N�X
	void Draw(ALLMODEL _modelNum, XMFLOAT4X4& mtxworld);

	//�w�肳�ꂽ�t�@�C���p�X�̃��f�������݂���ΕԂ�
	CModel* GetModelPtr(std::string _key) {
		// ���݂��邩���m���߂�
		auto it = m_modelhashmap.find(_key);
		if (it == m_modelhashmap.end()) {
			return nullptr;
		}
		return m_modelhashmap[_key].get();
	}
	//�w�肳�ꂽ���f�������݂���ΕԂ�
	CModel* GetModelPtr(ALLMODEL _modelNum) {
		// ���݂��邩���m���߂�
		auto it = m_modelhashmap.find(m_csvModelData->GetModelName().at(static_cast<int>(_modelNum)));
		if (it == m_modelhashmap.end()) {
			return nullptr;
		}
		return m_modelhashmap[m_csvModelData->GetModelName().at(static_cast<int>(_modelNum))].get();
	}
};