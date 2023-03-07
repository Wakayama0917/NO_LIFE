#include "Quad2d.h"
#include "SceneChange.h"

SceneChange::SceneChange() {
	//UI�I�u�W�F�N�g��K�v�Ȑ��������̉�
	for (int i = 0; i < 100; i++)
	{
		m_sceneChangeuiUP.push_back(std::make_unique<Quad2D>());
		//������
		m_sceneChangeuiUP[i]->Init(129.0f, 73.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f));
		m_sceneChangeuiUP[i]->LoadTexture("assets/image/battle/TriangleUP.png");
		m_sceneChangeuiUP[i]->updateAnimeNoPos(64 + (i % 10 * 129), 36 + (i / 10 * 73));//�ő�1290�A730
	}
	//UI�I�u�W�F�N�g��K�v�Ȑ��������̉�
	for (int i = 0; i < 110; i++)
	{
		m_sceneChangeuiDOWN.push_back(std::make_unique<Quad2D>());
		//������
		m_sceneChangeuiDOWN[i]->Init(129.0f, 73.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f));
		m_sceneChangeuiDOWN[i]->LoadTexture("assets/image/battle/TriangleDOWN.png");
		m_sceneChangeuiDOWN[i]->updateAnimeNoPos((i % 11 * 129),36 + (i / 11 * 73));//�ő�1290�A730
	}
}

void SceneChange::init()
{
	//�J�E���g������
	m_countUP = 0;
	m_countDOWN = 0;
	if (m_sceneChangeState == SCENECHANGEID::END)
	{
		m_sceneChangeState = SCENECHANGEID::FADDEIN;
	}
}

bool SceneChange::UpdateAll()
{
	switch (m_sceneChangeState)
	{
	case SceneChange::SCENECHANGEID::FADDEIN:
		UpdateIN();
		break;
	case SceneChange::SCENECHANGEID::FADDEOUT:
		UpdateOUT();
		break;
	case SceneChange::SCENECHANGEID::END:
		//�������I������̂ŕԂ�
		return true;
		break;
	default:
		break;
	}
	//�I����Ă��Ȃ��̂ŋU
	return false;
}

bool SceneChange::UpdateIN()
{
	//�`�悳�ꂽ���̂���F��ǉ�
	if (m_countUP < 100)
	{
		m_countUP += 5;
	}
	for (int i = 0; i < m_countUP; i++)
	{
		m_sceneChangeuiUP[i]->FaddeIN(0.1f);
	}

	if (m_countDOWN < 110)
	{
		//m_sceneChangeuiDOWN[109 - m_countDOWN]->updateAnimeNoColour(XMFLOAT4{ 1.0f,1.0f,1.0f,1.0f });
		m_countDOWN += 5;
	}
	for (int i = 0; i < m_countDOWN; i++)
	{
		m_sceneChangeuiDOWN[109 - i]->FaddeIN(0.1f);
	}

	//�Ō�̍��ڂ��h��ꂽ��
	if (m_sceneChangeuiDOWN[0]->Getfadde() >= 1.0f)
	{
		m_sceneChangeState = SCENECHANGEID::FADDEOUT;
		init();//�t���O������
		return true;
	}
	else
	{
		return false;
	}
}

bool SceneChange::UpdateOUT()
{
	//�`�悳�ꂽ���̂���F��ǉ�
	if (m_countUP < 100)
	{
		m_countUP += 5;
	}
	for (int i = 0; i < m_countUP; i++)
	{
		m_sceneChangeuiUP[i]->FaddeOUT(0.1f);
	}

	if (m_countDOWN < 110)
	{
		//m_sceneChangeuiDOWN[109 - m_countDOWN]->updateAnimeNoColour(XMFLOAT4{ 1.0f,1.0f,1.0f,1.0f });
		m_countDOWN += 5;
	}
	for (int i = 0; i < m_countDOWN; i++)
	{
		m_sceneChangeuiDOWN[109 - i]->FaddeOUT(0.1f);
	}

	//�Ō�̍��ڂ������Ȃ�����
	if (m_sceneChangeuiDOWN[0]->Getfadde() <= 0.0f)
	{
		m_sceneChangeState = SCENECHANGEID::END;

		for (int i = 0; i < m_countUP; i++)
		{
			m_sceneChangeuiUP[i]->FaddeReset();
		}
		for (int i = 0; i < m_countDOWN; i++)
		{
			m_sceneChangeuiDOWN[i]->FaddeReset();
		}

		return true;
	}
	else
	{
		return false;
	}
}

void SceneChange::Draw()
{
	
	for (int i = 0; i < 100; i++)
	{
		m_sceneChangeuiUP[i]->Draw();
	}
	for (int i = 0; i < 110; i++)
	{
		m_sceneChangeuiDOWN[i]->Draw();
	}
}
