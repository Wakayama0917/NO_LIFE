#include <memory>
#include "HommingMissile.h"
#include "dx11mathutil.h"
#include "CModel.h"
#include "CDirectInput.h"
#include "ModelMgr.h"

constexpr float BULLETSPEED = 5.0f;
constexpr uint32_t BULLETLIFE = 60;


bool HomingMissile::Init() {

	bool sts = true;

	// �s�񏉊���
	DX11MtxIdentity(m_mtx);
	//�����蔻��̎��̉�
	CollisionCreateInit();

	return sts;
}

//void HomingMissile::Draw() {
//
//	// ���f���`��
//	if (m_state == STATE::LIVE) {
//		m_model->Draw(m_mtx);
//	}
//}

void HomingMissile::Update() {

	m_liveTime--;
	if (m_liveTime > 0) {

		// �^�[�Q�b�g�ɂނ��Ĉʒu���X�V
		//Move(m_target);

	}
	else {
		m_state = STATE::DEAD;
	}

}

void HomingMissile::Finalize() {

}

/*----------------------------------------
	�~�T�C���̃f�[�^���X�V����
	�^�[�Q�b�g�ʒu�Ɛ��l�����Ă���

	(�^�[�Q�b�g�Ƃ̋�����lng��菬�����Ȃ�����true��Ԃ�)
-----------------------------------------*/
void HomingMissile::Move(const DirectX::XMFLOAT3 _targetPosition) {

	bool sts = false;
	float lng;

	DirectX::XMFLOAT4 TargetQt;					// �^�[�Q�b�g�����̎p��

	DirectX::XMFLOAT3 TargetVector;

	// ���݈ʒu����^�[�Q�b�g�����ւ̃x�N�g�������߂�
	TargetVector.x = _targetPosition.x - m_pos.x;
	TargetVector.y = _targetPosition.y - m_pos.y;
	TargetVector.z = _targetPosition.z - m_pos.z;

	//DX11Vec3Length(TargetVector, lng);

	//lng = Distance(_targetPosition);
	//if (lng <= _l) {
	//	//�������Ă���̂Ő^��Ԃ�
	//	return true;
	//}

	DirectX::XMFLOAT3 ZDir = m_Velocity;			// �~�T�C���̑��x�x�N�g���i�~�T�C��Z�����̌����j

	DX11Vec3Normalize(TargetVector, TargetVector);	// ���K��
	DX11Vec3Normalize(ZDir, ZDir);					// ���K��

	float	Dot;										// �Q�{�̃x�N�g���̓��ϒl�i���݂̐i�s�����ƃ^�[�Q�b�g�����j
	TargetQt = RotationArc(ZDir, TargetVector, Dot);	// �Q�{�̃x�N�g������ׂ��p�x�ƃN�I�[�^�j�I�������߂�
	float angleDiff = acosf(Dot);						// ���W�A���p�x

	
	// �~�T�C���̎p�������肷��
	if (m_maxRot >= angleDiff) {
		// 1�t���[���ōX�V�ł���p�x�������ݎp���ƃ^�[�Q�b�g�����������p�x���������ꍇ
		DX11QtMul(m_Quaternion, m_Quaternion, TargetQt);
	}
	else {
		// 1�t���[���ōX�V�ł���p�x�������ݎp���ƃ^�[�Q�b�g�����������p�x���傫���ꍇ
		float t = m_maxRot / angleDiff;

		DirectX::XMFLOAT4 toqt;
		DX11QtMul(toqt, m_Quaternion, TargetQt);

		DX11QtSlerp(m_Quaternion, toqt, t, m_Quaternion);
	}

	// �N�H�[�^�j�I�����s��ɂ���
	// ���݂̎p�����N�I�[�^�j�I���ɂ���
	DX11MtxFromQt(m_mtx, m_Quaternion);

	// �~�T�C���̐i�s�����ɃX�s�[�h���|���Ĉړ��ʁi���x�x�N�g�������߂�j
	m_Velocity.x = m_mtx._31*m_attackSpeed;
	m_Velocity.y = m_mtx._32*m_attackSpeed;
	m_Velocity.z = m_mtx._33*m_attackSpeed;

	// �ʒu���X�V�i���x�x�N�g���ōX�V�j
	m_pos.x += m_Velocity.x;
	m_pos.y += m_Velocity.y;
	m_pos.z += m_Velocity.z;

	// �s��Ɉʒu���W���Z�b�g
	m_mtx._41 = m_pos.x;
	m_mtx._42 = m_pos.y;
	m_mtx._43 = m_pos.z;

	//return sts;
}
