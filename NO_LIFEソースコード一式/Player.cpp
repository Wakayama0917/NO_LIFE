#include "player.h"
#include "drawaxis.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include "EnumListPlayer.h"
#include "imgui.h"
#include "Input.h"
#include "ModelMgr.h"
#include "EnumAllModel.h"

bool Player::Init()
{
	// �s�񏉊���
	DX11MtxIdentity(m_mtx);
	m_pos = { 0, 0, 0 };

	// �s�񂩂�N�I�[�^�j�I���𐶐�
	DX11GetQtfromMatrix(m_mtx, m_Quaternion);
	//�����蔻��̎��̉�
	CollisionCreateInit();

	//�g�嗦���Z�b�g
	m_expand = 2.0f;

	//�����Ă���ݒ�
	m_state = STATE::LIVE;

	return true;
}

bool Player::Init(std::unique_ptr<CSVFile>& _csvFile) {
	// �s�񏉊���
	DX11MtxIdentity(m_mtx);
	m_pos = { 0, 0, 0 };

	// �s�񏉊���
	//DX11MtxIdentity(m_Quaternion);

	// �s�񂩂�N�I�[�^�j�I���𐶐�
	DX11GetQtfromMatrix(m_mtx, m_Quaternion);
	//�����蔻��̎��̉�
	CollisionCreateInit();

	//�g�嗦���Z�b�g
	m_expand = 2.0f;
	//m_collision->GetHitdata().r* m_expand;
	//�����Ă���ݒ�
	m_state = STATE::LIVE;

	PlayerCsvSet(_csvFile);

	//CSV�Z�b�g�����̂ŃZ�b�g��̏��������s��
	SetModel();

	return true;
}

void Player::Reset()
{
	//�q�b�g���o�̏�����������
	m_pHitEffectFlg = PHitEffectFlg::NONE;
}

void Player::Draw() {
	if (m_state == STATE::LIVE)
	{
		//�傫����ς��邽�߂̏���
		/*static int Scalenum = 0;
		if (Scalenum == 0)
		{*/
		DirectX::XMFLOAT4X4 big;
		//DX11MtxScale(m_expand, m_expand, m_expand, big);//�傫�����邽�߂�4x4���쐻
		DX11MtxMultiply(big, m_bigMtx, m_mtx);//�擪�Ɍ��2�����������̂�����A���݈ʒu
		//g_player.change4x4(big);//�s�����������

		/*Scalenum++;
	}*/
	// ���f���`��
		ModelMgr::GetInstance().Draw(m_modelNum,big);
		//GetSphere().Draw();
	}

}

void Player::DrawWithAxis() {

	// ���f���`��
	drawaxis(m_mtx, 200, m_pos);
	Draw();

}

void Player::Update()
{
	if (m_state == STATE::LIVE)
	{
		XMFLOAT2 L = Input::GetInstance().GetLStickPlayer();
		XMFLOAT2 R = Input::GetInstance().GetRStickPlayer();

		//�L�[���͍��X�e�B�b�N�̑���
		if ((Input::GetInstance().CheckKeyPress(DIK_A)))  L.x = -1.0f;
		if ((Input::GetInstance().CheckKeyPress(DIK_D)))  L.x = 1.0f;
		if ((Input::GetInstance().CheckKeyPress(DIK_W)))  L.y = 1.0f;
		if ((Input::GetInstance().CheckKeyPress(DIK_S)))  L.y = -1.0f;
		
		//�L�[���͉E�X�e�B�b�N�̑���
		if ((Input::GetInstance().CheckKeyPress(DIK_LEFT)))  R.x = -1.0f;
		if ((Input::GetInstance().CheckKeyPress(DIK_RIGHT))) R.x = 1.0f;
		if ((Input::GetInstance().CheckKeyPress(DIK_UP)))    R.y = 1.0f;
		if ((Input::GetInstance().CheckKeyPress(DIK_DOWN)))  R.y = -1.0f;



		if ((L.x != 0) || (L.y != 0))//���͂������
		{
			//angle.y += X / 5;
			//Keyinput = true;
			UpdatePlayerMove(L.x, L.y);
		}
		if ((R.x != 0) || (R.y != 0))//���͂������
		{
			//Keyinput = true;
			UpdatePlayerDirection(R.x, R.y);
		}

		m_collision->UpdatePosition(m_mtx);//���\��
		BulletReload();//�e���˂̃J�E���g
	}
}

void Player::UpdatePlayerDirection(float _RX, float _RY)
{
	DX11GetQtfromMatrix(m_mtx, m_Quaternion);// �s�񂩂�N�I�[�^�j�I���𐶐�

	//�X�e�B�b�N�̌����BY��Z
	DirectX::XMFLOAT3 RPosition;
	RPosition.x = _RX;
	RPosition.y = 0.0f;
	RPosition.z = _RY;

	//�v���C���[�̃x�N�g��Z�BY��Z
	DirectX::XMFLOAT3 PPosition;
	PPosition.x = m_mtx._31;
	PPosition.y = 0.0f;
	PPosition.z = m_mtx._33;

	DirectX::XMFLOAT4 TargetQt;					// �^�[�Q�b�g�����̎p��


	DirectX::XMFLOAT3 ZDir = PPosition;			// �v���C���[�̑��x�x�N�g���i�~�T�C��Z�����̌����j

	DX11Vec3Normalize(RPosition, RPosition);	// ���K��
	DX11Vec3Normalize(ZDir, ZDir);					// ���K��

	float	Dot;										// �Q�{�̃x�N�g���̓��ϒl�i���݂̐i�s�����ƃ^�[�Q�b�g�����j
	TargetQt = RotationArc(ZDir, RPosition, Dot);	// �Q�{�̃x�N�g������ׂ��p�x�ƃN�I�[�^�j�I�������߂�
	float angleDiff = acosf(Dot);						// ���W�A���p�x

	float m_AddRotMax = 360.0f;

	// �v���C���[�̎p�������肷��
	if (m_AddRotMax >= angleDiff) {
		 //1�t���[���ōX�V�ł���p�x�������ݎp���ƃ^�[�Q�b�g�����������p�x���������ꍇ
		DX11QtMul(m_Quaternion, m_Quaternion, TargetQt);
	}
	else {
		// 1�t���[���ōX�V�ł���p�x�������ݎp���ƃ^�[�Q�b�g�����������p�x���傫���ꍇ
		float t = m_AddRotMax / angleDiff;

		DirectX::XMFLOAT4 toqt;
		DX11QtMul(toqt, m_Quaternion, TargetQt);

		DX11QtSlerp(m_Quaternion, toqt, t, m_Quaternion);
	}

	// �N�H�[�^�j�I�����s��ɂ���
	// ���݂̎p�����N�I�[�^�j�I���ɂ���
	DX11MtxFromQt(m_mtx, m_Quaternion);

	// �s��Ɉʒu���W���Z�b�g
	m_mtx._41 = m_pos.x;
	m_mtx._42 = m_pos.y;
	m_mtx._43 = m_pos.z;
}

void Player::BulletReload()
{
	if (m_reloadTime >= m_reload) {//�����[�h���Ԉȏ�Ȃ甭��

		m_fireBulletFlg = true;//���˂ɕύX
		m_reloadTime = 0;//���Z�b�g
	}
	else// if (m_HommingBulletWaittime < m_bulletReloadTime)//�����[�h����
	{
		m_reloadTime += ImGui::GetIO().DeltaTime;
	}
}

void Player::Finalize() {
	//m_model->Uninit();
}

void Player::EnemyBulletReload()
{
	if (m_reloadTime >= m_reload) {//�����[�h���Ԉȏ�Ȃ甭��

		m_fireBulletFlg = true;//���˂ɕύX
		m_reloadTime = 0;//���Z�b�g
	}
	else// if (m_HommingBulletWaittime < m_bulletReloadTime)//�����[�h����
	{
		m_reloadTime += ImGui::GetIO().DeltaTime;
	}
}

void Player::HPDecrease(float _damage)
{
	m_hp = m_hp - _damage;
	//0�ȉ��Ȃ玀�S
	if (m_hp <= 0)
	{
		//���S���o�t���O�𗧂Ă�
		m_pHitEffectFlg = PHitEffectFlg::DEATH;
		//���S
		m_state = STATE::DEAD;
		m_hp = 0;
	}
	else
	{
		//���S���o�t���O�𗧂Ă�
		m_pHitEffectFlg = PHitEffectFlg::HIT;
	}
}

void Player::HPIncrease(float _value)
{
	m_hp = m_hp + _value;
	//�ő�l�𒴂��Ă�����~�߂�
	if (m_hp > m_maxhp)
	{
		m_hp = m_maxhp;
	}
}

void Player::UPMAXHP(float _value)
{
	m_maxhp += _value;//�ő�̗͏㏸
	m_hp += _value;//�㏸��������
	//�ő�l�𒴂��Ă�����~�߂�
	if (m_hp > m_maxhp)
	{
		m_hp = m_maxhp;
	}
}

void Player::UPSpeed(float _value)
{
	m_moveSpeed += _value;
}

void Player::UPAttack(float _value)
{
	m_atk += _value;
}

void Player::UPFrontBullet(float _level)
{
	if (_level == 0)
	{
		m_shotDirectionFrontLeft = 1;	//���΂ߑO
		m_shotDirectionFrontRight = 1;	//�E�΂ߑO
	}
	else if(_level == 1)
	{
		m_shotDirectionLeft = 1;        //��
		m_shotDirectionRight = 1;       //�E
	}
	
}

void Player::UPBackBullet(float _level)
{
	if (_level == 0)
	{
		m_shotDirectionBack = 1;          //���
	}
	else if (_level == 1)
	{
		m_shotDirectionBackLeft = 1;      //�����
		m_shotDirectionBackRight = 1;     //�E���
	}
}

void Player::UPReload(float _value)
{
	m_reload = (m_reload - _value);
}

void Player::SetPos(float _x, float _z)
{
	//pos�X�V
	m_pos.x = _x;
	m_pos.z = _z;

	//�s��X�V
	m_mtx._41 = m_pos.x;
	m_mtx._43 = m_pos.z;
}

void Player::SetStartPos(float _x, float _z)
{
	//�X�e�[�W�̏c������ۑ�
	m_stageX = _x;
	m_stageZ = _z;

	//pos�X�V
	m_pos.x = 0;
	m_pos.z = -(_z / 2) + 10;

	//�s��X�V
	m_mtx._41 = m_pos.x;
	m_mtx._43 = m_pos.z;
}

void Player::SetAxis(float _x, float _y)
{

	DX11GetQtfromMatrix(m_mtx, m_Quaternion);// �s�񂩂�N�I�[�^�j�I���𐶐�

	//�X�e�B�b�N�̌����BY��Z
	DirectX::XMFLOAT3 RPosition;
	RPosition.x = _x;
	RPosition.y = 0.0f;
	RPosition.z = _y;

	//�v���C���[�̃x�N�g��Z�BY��Z
	DirectX::XMFLOAT3 PPosition;
	PPosition.x = m_mtx._31;
	PPosition.y = 0.0f;
	PPosition.z = m_mtx._33;

	/*bool sts = false;
	float lng;*/

	DirectX::XMFLOAT4 TargetQt;					// �^�[�Q�b�g�����̎p��


	DirectX::XMFLOAT3 ZDir = PPosition;			// �~�T�C���̑��x�x�N�g���i�~�T�C��Z�����̌����j

	DX11Vec3Normalize(RPosition, RPosition);	// ���K��
	DX11Vec3Normalize(ZDir, ZDir);					// ���K��

	float	Dot;										// �Q�{�̃x�N�g���̓��ϒl�i���݂̐i�s�����ƃ^�[�Q�b�g�����j
	TargetQt = RotationArc(ZDir, RPosition, Dot);	// �Q�{�̃x�N�g������ׂ��p�x�ƃN�I�[�^�j�I�������߂�
	float angleDiff = acosf(Dot);						// ���W�A���p�x

	float m_AddRotMax = 360.0f;

	//�Z�b�g�Ȃ̂ŁA�P�t���[���̍ő�p�x������l���Ȃ�
	DX11QtMul(m_Quaternion, m_Quaternion, TargetQt);
	
	// �N�H�[�^�j�I�����s��ɂ���
	// ���݂̎p�����N�I�[�^�j�I���ɂ���
	DX11MtxFromQt(m_mtx, m_Quaternion);

	// �s��Ɉʒu���W���Z�b�g
	m_mtx._41 = m_pos.x;
	m_mtx._42 = m_pos.y;
	m_mtx._43 = m_pos.z;
}

void Player::SetAllStatus(float _playerChangeData[static_cast<int>(PLAYER_STATUSID::END)])
{

	m_moveSpeed = _playerChangeData[static_cast<int>(PLAYER_STATUSID::MOVE_SPEED)];          //�ړ����x
	m_maxhp = _playerChangeData[static_cast<int>(PLAYER_STATUSID::MAXHP)];				//�ő�̗�
	m_hp = _playerChangeData[static_cast<int>(PLAYER_STATUSID::HP)];                 //�̗�
	m_atk = _playerChangeData[static_cast<int>(PLAYER_STATUSID::ATTACK)];              //�U����
	//int   m_bulletType = 0;         //�e�̎��
	m_reload = _playerChangeData[static_cast<int>(PLAYER_STATUSID::RELOAD)];                 //���̒e�����܂ł̎���
}

void Player::SetPHitEffectFlg(PHitEffectFlg _pHitEffectFlg)
{
	//�t���O��Ԃ�ύX
	m_pHitEffectFlg = _pHitEffectFlg;
}

bool Player::GetShotDirection(int _num)
{
	//�e�����̐^�U�m�F
	switch (_num)
	{
	case 0: {
		return m_shotDirectionFront;
		break;
	}
	case 1: {
		return m_shotDirectionLeft;
		break;
	}
	case 2: {
		return m_shotDirectionRight;
		break;
	}
	case 3: {
		return m_shotDirectionBack;
		break;
	}
	case 4: {
		return m_shotDirectionFrontLeft;
		break;
	}
	case 5: {
		return m_shotDirectionFrontRight;
		break;
	}
	case 6: {
		return m_shotDirectionBackLeft;
		break;
	}
	case 7: {
		return m_shotDirectionBackRight;
		break;
	}
	default:
		break;
	}

	return false;
}

float Player::GetCSVStatus(PLAYER_STATUSID _statusid)
{
	switch (_statusid)
	{
	case PLAYER_STATUSID::MOVE_SPEED: {
		return m_moveSpeed;
	}
		break;
	case PLAYER_STATUSID::MAXHP: {
		return m_maxhp;
	}
		break;
	case PLAYER_STATUSID::HP: {
		return m_hp;
	}
		break;
	case PLAYER_STATUSID::ATTACK: {
		return m_atk;
	}
		break;
	case PLAYER_STATUSID::BULLET_TYPE: {
		return m_bulletType;
	}
		break;
	case PLAYER_STATUSID::RELOAD: {
		return m_reload;
	}
		break;
	case PLAYER_STATUSID::DIRECTION_FRONT: {
		return m_shotDirectionFront;
	}
										break;
	case PLAYER_STATUSID::DIRECTION_LEFT: {
		return m_shotDirectionLeft;
	}
									   break;
	case PLAYER_STATUSID::DIRECTION_RIGHT: {
		return m_shotDirectionRight;
	}
										break;
	case PLAYER_STATUSID::DIRECTION_BACK: {
		return m_shotDirectionBack;
	}
									   break;
	case PLAYER_STATUSID::DIRECTION_FRONT_LEFT: {
		return m_shotDirectionFrontLeft;
	}
											 break;
	case PLAYER_STATUSID::DIRECTION_FRONT_RIGHT: {
		return m_shotDirectionFrontRight;
	}
											  break;
	case PLAYER_STATUSID::DIRECTION_BACK_LEFT: {
		return m_shotDirectionBackLeft;
	}
											break;
	case PLAYER_STATUSID::DIRECTION_BACK_RIGHT: {
		return m_shotDirectionBackRight;
	}
											 break;
	case PLAYER_STATUSID::END: {
		// �ی�
		Texture tex;
		MessageBox(nullptr, "�v���C���[�I�u�W�F�N�g�F�����Ȑ��l�������Ă��܂�",
			"Error!", MB_ICONERROR | MB_OK);
		tex.texture = nullptr;
		return 0.0f;
	}
	break;
	default:
		break;
	}
}

bool Player::PlayerCsvSet(std::unique_ptr<CSVFile>& _csvFile)
{
		//���ʕϐ�������
		//GetCell().at(���̓G�̎�ޗ�).at(�l)
		m_modelNum = static_cast<ALLMODEL>(_csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::MODEL_NUM)));//���f���i���o�[
		m_moveSpeed = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::MOVE_SPEED));//�ړ����x
		m_maxhp = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::MAXHP));//�̗�
		m_hp = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::HP));//�̗�
		//m_atkSpeed = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::ATK_SPEED));//�U�����x
		m_atk = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::ATTACK));//�U����
		//m_rotation = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::ROTATION));//��]���邩
		//m_maxRot = _csvFile.GetCell().at(static_cast<int>(m_enemyNum)).at(static_cast<int>(ENEMY_STATUSID::MAX_ROT));//�P�t���[�����̍ő��]�p�x
		m_bulletType = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::BULLET_TYPE));//�e�̎��
		m_reload = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::RELOAD));//�e�̎��;
		m_shotDirectionFront = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::DIRECTION_FRONT));//�e��������
		m_shotDirectionLeft = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::DIRECTION_LEFT));//�e��������
		m_shotDirectionRight = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::DIRECTION_RIGHT));//�e��������
		m_shotDirectionBack = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::DIRECTION_BACK));//�e��������
		m_shotDirectionFrontLeft = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::DIRECTION_FRONT_LEFT));//�e��������
		m_shotDirectionFrontRight = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::DIRECTION_FRONT_RIGHT));//�e��������
		m_shotDirectionBackLeft = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::DIRECTION_BACK_LEFT));//�e��������
		m_shotDirectionBackRight = _csvFile->GetCell().at(0).at(static_cast<int>(PLAYER_STATUSID::DIRECTION_BACK_RIGHT));//�e��������
	
	return true;
}

void Player::UpdatePlayerMove(float _LX, float _LY){
	
	//�ړ������Z
	m_pos.x += m_moveSpeed * _LX;
	m_pos.z += m_moveSpeed * _LY;

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
	
	//�ʒu���X�V
	m_mtx._41 = m_pos.x;
	m_mtx._43 = m_pos.z;
}




