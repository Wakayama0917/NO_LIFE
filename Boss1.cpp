#include "Boss1.h"
#include "drawaxis.h"


bool Boss1::Init(std::unique_ptr<CSVFile>& _csvFile, std::shared_ptr<Player> _player)
{
	//�s�񏉊���
	DX11MtxIdentity(m_mtx);

	//m_actionKeepTime = 10;

	// �s�񂩂�N�I�[�^�j�I���𐶐�
	DX11GetQtfromMatrix(m_mtx, m_Quaternion);

	//�����蔻�菉����
	CollisionCreateInit();

	//�����Ă���ݒ�
	m_state = STATE::LIVE;

	m_enemyNum = ENEMY_TYPE::BOSS1;//���̓G���ݒ�

	EnemyCsvSet(*_csvFile);//�X�e�[�^�X������

	m_modelNum = ALLMODEL::BULLET_HARD;//���f����ݒ�

	//�傫�����R�{�ɂ���
	m_expand = 3.0f;
	//CSV�Z�b�g�����̂ŃZ�b�g��̏��������s��
	SetModel();


	//�v���C���[�I�u�W�F�N�g�̃|�C���^���i�[
	if (_player != nullptr)
	{
		m_sPlayer = _player;
	}
	else
	{
		//����������Ă��Ȃ��̂Ń��b�Z�[�W��\��
		Texture tex;
		MessageBox(nullptr, "�v���C���[�̒��g��NULL�ł�",
			"Error!", MB_ICONERROR | MB_OK);
		tex.texture = nullptr;

		return false;
	}

	return true;
}

//void EnemyHoming::Draw() {
//	//���f���`��
//	m_model->Draw(m_mtx);
//}

void Boss1::DrawWithAxis() {
	//���f���`��
	drawaxis(m_mtx, 200, m_pos);//�_���o�����
	//m_model->Draw(m_mtx);
}

/*----------------------------------------
	�G�l�~�[�̃f�[�^���X�V����
	�^�[�Q�b�g�ʒu�Ɛ��l�����Ă���

	(�^�[�Q�b�g�Ƃ̋�����lng��菬�����Ȃ�����true��Ԃ�)
-----------------------------------------*/
void Boss1::Update() {
	EnemyMove();
	BulletReload();
}

void Boss1::Finalize() {
	//m_model->Uninit();
}

void Boss1::EnemyMove()
{
	//�����������
	XMFLOAT4 axisY;
	XMFLOAT4 axisZ;
	XMFLOAT4X4 mtx = m_mtx;
	XMFLOAT4 qt, qty;//�N�H�[�^�j�I��

	axisZ.x = mtx._21;
	axisZ.y = mtx._22;
	axisZ.z = mtx._23;
	axisZ.w = 0.0f;

	//�s�񂩂�N�I�[�^�j�I���𐶐�
	DX11GetQtfromMatrix(mtx, qt);

	DX11QtRotationAxis(qty, axisZ, -0.5);//�w��m�x���ς���

	//�N�I�[�^�j�I������
	XMFLOAT4 tempqt1;
	DX11QtMul(tempqt1, qt, qty);

	//�N�I�[�^�j�I�����m�[�}���C�Y
	DX11QtNormalize(tempqt1, tempqt1);

	//�N�I�[�^�j�I������s����쐬
	DX11MtxFromQt(mtx, tempqt1);

	m_mtx._11 = mtx._11;
	m_mtx._12 = mtx._12;
	m_mtx._13 = mtx._13;

	m_mtx._21 = mtx._21;
	m_mtx._22 = mtx._22;
	m_mtx._23 = mtx._23;

	m_mtx._31 = mtx._31;
	m_mtx._32 = mtx._32;
	m_mtx._33 = mtx._33;

	m_mtx._41 = m_pos.x;
	m_mtx._42 = m_pos.y;
	m_mtx._43 = m_pos.z;

	m_mtx._14 = 0;
	m_mtx._24 = 0;
	m_mtx._34 = 0;
	m_mtx._44 = 1;
}
