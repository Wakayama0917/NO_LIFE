#pragma once
#include "myimgui.h"

/*************************************************************
* �e�V�[���̊��ƂȂ�N���X
*************************************************************/
class SceneBase
{
protected:
private:
	
	
public:
	//�ǂ̃V�[�����𔻒f����
	enum class SCENESTATUS
	{
		Title,
		Game,
		GameBoss,
		Editor,
		End,
	};
	SCENESTATUS SceneStatus;//���̃V�[����������\��

	SCENESTATUS NextSceneStatus;//���̃V�[��������������
	//�������������s������
	bool SceneInitFlg = true;
	//�V�[���̏I���������邩�ǂ���
	bool SceneDisposeFlg = false;

	//�t���O�̏�Ԃ�ς���֐�
	//����������
	void SceneFlgInit() {
		SceneInitFlg = false;
		SceneDisposeFlg = false;
	}
	//�V�[���I�������J�n�A���̃V�[�����I�����鎞�Ɏg�p�A�N���A���Ȃ�
	void SceneFlgDispose() {
		SceneInitFlg = false;
		SceneDisposeFlg = true;
	}
	//�t���O��Ԃ��������A�ʂ̃V�[���Ɉړ�����ۂɎg��
	void SceneFlgReset() {
		SceneInitFlg = true;
		SceneDisposeFlg = false;
	}

	//�f�o�b�O�������I���I�t�ǂ���ɂ��邩
	//true�I���Afalse�I�t
	bool DebugFlg = false;
	//�V�[���J�ڏ����ɓ��邩�ǂ���
	bool m_sceneChangeFlg = false;

	virtual void Imgui(){};
	virtual void Init(){};
	virtual void Exit(){};
	virtual void Update(){};
	virtual void Draw(){};

	////�����̃V�[���ɍs����
	//SCENESTATUS NextScene() {
	//	
	//}

	//�V�[���ړ����̃A�j���[�V����
	
};

