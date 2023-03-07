#include "EnemyHoming.h"
#include "drawaxis.h"


bool EnemyHoming::Init(std::unique_ptr<CSVFile>& _csvFile, std::shared_ptr<Player> _player)
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

	m_enemyNum = ENEMY_TYPE::HOMMING;//���̓G���ݒ�

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

//void EnemyHoming::Draw() {
//	//���f���`��
//	m_model->Draw(m_mtx);
//}

void EnemyHoming::DrawWithAxis() {
	//���f���`��
	drawaxis(m_mtx, 200, m_pos);//�_���o�����
	//m_model->Draw(m_mtx);
}

/*----------------------------------------
	�G�l�~�[�̃f�[�^���X�V����
	�^�[�Q�b�g�ʒu�Ɛ��l�����Ă���

	(�^�[�Q�b�g�Ƃ̋�����lng��菬�����Ȃ�����true��Ԃ�)
-----------------------------------------*/
void EnemyHoming::Update() {
	EnemyMove();
	BulletReload();
}

void EnemyHoming::Finalize() {
	//m_model->Uninit();
}

void EnemyHoming::EnemyMove()
{
	bool sts = false;
	float lng;

	DirectX::XMFLOAT4 TargetQt;					// �^�[�Q�b�g�����̎p��
	DirectX::XMFLOAT3 TargetVector;
	//�z�[�~���O�G�l�~�[�̃x�N�g��Z���BY��Z
	DirectX::XMFLOAT3 PPosition;
	PPosition.x = m_mtx._31;
	PPosition.y = 0.0f;
	PPosition.z = m_mtx._33;


	TargetVector.x = m_sPlayer->GetMtx()._41 - m_pos.x;
	TargetVector.y = m_sPlayer->GetMtx()._42 - m_pos.y;
	TargetVector.z = m_sPlayer->GetMtx()._43 - m_pos.z;
	

	DX11Vec3Length(TargetVector, lng);
	//if (lng <= m_sPlayer->GetRadius()) {
	//	//�������Ă���̂Ő^��Ԃ�
	//	//return true;
	//}

	DirectX::XMFLOAT3 ZDir = PPosition;			// �~�T�C���̑��x�x�N�g���i�~�T�C��Z�����̌����j

	DX11Vec3Normalize(TargetVector, TargetVector);	// ���K��
	DX11Vec3Normalize(ZDir, ZDir);					// ���K��

	float	Dot;										// �Q�{�̃x�N�g���̓��ϒl�i���݂̐i�s�����ƃ^�[�Q�b�g�����j
	TargetQt = RotationArc(ZDir, TargetVector, Dot);	// �Q�{�̃x�N�g������ׂ��p�x�ƃN�I�[�^�j�I�������߂�
	float angleDiff = acosf(Dot);						// ���W�A���p�x


	// �G�l�~�[�̎p�������肷��
	if (m_maxRot >= angleDiff) {
		// 1�t���[���ōX�V�ł���p�x�������ݎp���ƃ^�[�Q�b�g�����������p�x���������ꍇ
		DX11QtMul(m_Quaternion, m_Quaternion, TargetQt);
	}
	else {
		// 1�t���[���ōX�V�ł���p�x�������ݎp���ƃ^�[�Q�b�g�����������p�x���傫���ꍇ
		float t = m_maxRot / angleDiff;
		//float t = m_maxRot;

		DirectX::XMFLOAT4 toqt;
		DX11QtMul(toqt, m_Quaternion, TargetQt);

		DX11QtSlerp(m_Quaternion, toqt, t, m_Quaternion);
	}

	// �N�H�[�^�j�I�����s��ɂ���
	// ���݂̎p�����N�I�[�^�j�I���ɂ���
	DX11MtxFromQt(m_mtx, m_Quaternion);

	// �~�T�C���̐i�s�����ɃX�s�[�h���|���Ĉړ��ʁi���x�x�N�g�������߂�j
	PPosition.x = (m_mtx._31 * m_moveSpeed);
	PPosition.y = (m_mtx._32 * m_moveSpeed);
	PPosition.z = (m_mtx._33 * m_moveSpeed);

	// �ʒu���X�V�i���x�x�N�g���ōX�V�j
	m_pos.x += PPosition.x;
	m_pos.y += PPosition.y;
	m_pos.z += PPosition.z;

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

	// �s��Ɉʒu���W���Z�b�g
	m_mtx._41 = m_pos.x;
	m_mtx._42 = m_pos.y;
	m_mtx._43 = m_pos.z;

	//return sts;


	
}

