#pragma once
#include "gameobject.h"
#include "CModel.h"

class Skydome : public GameObject
{
private:
	//CModel* m_model;
public:
	bool Init();
	void Draw();
	void Update();
	void Finalize();

	/*void SetModelSky(CModel* _p){
		m_model = _p;
	}*/

	//���I�ɂ��邽��
	/*
	�N���X���ŃX�^�e�B�b�N�ɂ���ƃR���p�C�����ɐ��������B
	���I�����ƂȂ�A�f���[�g������K�v�������Ȃ�B�S�̂Ŏg�p���镨�Ɋւ��Ă͂�����̕����ǂ�
	�O���[�o���A�X�^�e�B�b�N�ANEW�R���ƃO���[�o���͂��蓾�Ȃ��A�c���͏ꍇ�ɂ��
	*/
	static Skydome * GetInstance() {
		static Skydome instance;
		return &instance;
	}
	/*Skydome();
	~Skydome();*/
};


