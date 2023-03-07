#pragma once
#include <vector>
#include <memory>

class Quad2D;
class SceneChange
{
public:
	//�g�p����i�s�̃��X�g
	enum class SCENECHANGEID
	{
		FADDEIN,		//�t�F�[�h�C��������
		FADDEOUT,		//�t�F�[�h�A�E�g������
		END,			//�����I��
	};
private:
	//UI����
	//�����
	std::vector<std::unique_ptr<Quad2D>> m_sceneChangeuiUP;
	int m_countUP = 0;
	//������
	std::vector<std::unique_ptr<Quad2D>> m_sceneChangeuiDOWN;
	int m_countDOWN = 0;
	//�i�s��Ԃ��Ǘ�����t���O
	SCENECHANGEID m_sceneChangeState = SCENECHANGEID::FADDEIN;

public:
	SceneChange();

	//�V���O���g��
	static SceneChange* GetInstance() {
		static SceneChange instance;
		return &instance;
	}

	//����������
	void init();
	//�t�F�[�h������1�ɂ܂Ƃ߂����́A�ŏ���init�����͂���
	bool UpdateAll();
	//�t�F�[�h�C��
	bool UpdateIN();
	//�t�F�[�h�A�E�g
	bool UpdateOUT();
	//�`��
	void Draw();

	//�t���O���擾
	SCENECHANGEID GetSceneChangeFlg() { return m_sceneChangeState; }
};


