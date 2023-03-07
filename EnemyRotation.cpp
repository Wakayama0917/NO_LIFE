#include "EnemyRotation.h"
#include "drawaxis.h"
#include "imgui.h"


//������
bool EnemyRotation::Init(std::unique_ptr<CSVFile>& _csvFile, std::shared_ptr<Player> _player) {
	//�s�񏉊���
	DX11MtxIdentity(m_mtx);

	//�����蔻�菉����
	CollisionCreateInit();

	//�����Ă���ݒ�
	m_state = STATE::LIVE;

	m_enemyNum = ENEMY_TYPE::ROTATION;//���̓G���ݒ�

	EnemyCsvSet(*_csvFile);//�X�e�[�^�X������

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

void EnemyRotation::EnemyMove()
{
	XMFLOAT4 axisX;
	XMFLOAT4 axisY;
	XMFLOAT4 axisZ;

	//AutoPilot();//�v���C���[�Ō����L�[����

	//�ȉ���΂�����Ɠ�����
	//X�������o��
	axisX.x = m_mtx._11;
	axisX.y = m_mtx._12;
	axisX.z = m_mtx._13;
	axisX.w = 0.0f;

	//Y�������o��
	axisY.x = m_mtx._21;
	axisY.y = m_mtx._22;
	axisY.z = m_mtx._23;
	axisY.w = 0.0f;

	//Z�������o��
	axisZ.x = m_mtx._31;
	axisZ.y = m_mtx._32;
	axisZ.z = m_mtx._33;
	axisZ.w = 0.0f;


	XMFLOAT4 qt;//�N�H�[�^�j�I��

	//�s�񂩂�N�I�[�^�j�I���𐶐�
	DX11GetQtfromMatrix(m_mtx, qt);

	XMFLOAT4 qtx, qty, qtz;//�N�I�[�^�j�I��

	//�w�莲��]�̃N�I�[�^�j�I������
	DX11QtRotationAxis(qtx, axisX, m_angle.x);
	DX11QtRotationAxis(qty, axisY, m_angle.y);
	DX11QtRotationAxis(qtz, axisZ, m_angle.z);

	//�N�I�[�^�j�I������
	XMFLOAT4 tempqt1;
	DX11QtMul(tempqt1, qt, qtx);

	XMFLOAT4 tempqt2;
	DX11QtMul(tempqt2, qty, qtz);

	XMFLOAT4 tempqt3;
	DX11QtMul(tempqt3, tempqt1, tempqt2);

	//�N�I�[�^�j�I�����m�[�}���C�Y
	DX11QtNormalize(tempqt3, tempqt3);

	//�N�I�[�^�j�I������s����쐬
	DX11MtxFromQt(m_mtx, tempqt3);


	//Z�������o��
	axisZ.x = m_mtx._31;
	axisZ.y = m_mtx._32;
	axisZ.z = m_mtx._33;
	axisZ.w = 0.0f;

	//��Ɉړ������邽�߂ɉ��Z
	m_pos.x += axisZ.x * m_moveSpeed;
	m_pos.y += axisZ.y * m_moveSpeed;
	m_pos.z += axisZ.z * m_moveSpeed;

	//�G���A�O�ɏo�Ȃ��悤�ɒ���
	if (m_pos.x > (m_stageX / 2))
	{
		m_pos.x = (m_stageX / 2);
	}
	if (m_pos.x < -(m_stageX / 2))
	{
		m_pos.x = -(m_stageX / 2);
	}
	if (m_pos.z > (m_stageZ / 2))
	{
		m_pos.z = (m_stageZ / 2);
	}
	if (m_pos.z < -(m_stageZ / 2))
	{
		m_pos.z = -(m_stageZ / 2);
	}

	m_mtx._41 = m_pos.x;
	m_mtx._42 = m_pos.y;
	m_mtx._43 = m_pos.z;
}

//void EnemyRotation::Draw(){
//	//���f���`��
//	m_model->Draw(m_mtx);
//	
//}

void EnemyRotation::DrawWithAxis(){
	//���f���`��
	drawaxis(m_mtx, 200, m_pos);
	//m_model->Draw(m_mtx);
}

void EnemyRotation::Update() {
	
	EnemyMove();
	BulletReload();

	
}

void EnemyRotation::Finalize(){
	//m_model->Uninit();
}