#include "Collider.h"
#include "XAudio2.h"

float Collider::ColliderNum(const DirectX::XMFLOAT4X4& _mtx1, const DirectX::XMFLOAT4X4& _mtx2){
	/*XMFLOAT3 vec1;
	XMFLOAT3 vec2;*/
	float num = 0;//����
	float numX = 0;
	float numZ = 0;
	
	/*************************************************************
	* X����Z���݂̂̔�����s��
	* �P�ڂ�X�ƂQ�ڂ�X�̍���0�łȂ����m�F
	* �P�ڂ�X�ƂQ�ڂ�X�̍���2�悷��
	* �P�ڂ�Z�ƂQ�ڂ�Z�̍���0�łȂ����m�F
	* �P�ڂ�Z�ƂQ�ڂ�Z�̍���2�悷��
	* ���҂𑫂������̔񕉂̕�������Ԃ��B
	*************************************************************/
	//X
	numX = _mtx1._41 - _mtx2._41;
	if (numX != 0)
	{
		numX = pow(_mtx1._41 - _mtx2._41, 2);
	}
	else
	{
		numX = 0.0f;
	}
	//Z
	numZ = _mtx1._43 - _mtx2._43;
	if (numZ != 0)
	{
		numZ = pow(_mtx1._43 - _mtx2._43, 2);
	}
	else
	{
		numZ = 0.0f;
	}
	//������
	num = sqrt(numX + numZ);

	//�f�o�b�O�p���l�\��
	/*std::cout << "Distance between points (" << mtx1._41 << ", "
		<< mtx1._42 << ", "
		<< mtx1._43 << ") and (" << mtx2._41 << ", "
		<< mtx2._42 << ", "
		<< mtx2._43 << ") is " << num << std::endl;*/

	return num;
}

float Collider::ColliderNum(const DirectX::XMFLOAT3 _pos1, const DirectX::XMFLOAT3 _pos2)
{
	return sqrt(pow(_pos1.x - _pos2.x, 2) + pow(_pos1.z - _pos2.z, 2));

}

bool Collider::OnCollision(const DirectX::XMFLOAT4X4& _mtx1, const DirectX::XMFLOAT4X4& _mtx2, float _Radius)
{
	//�����蔻��A���ꂽ����m�̋����𑪂�
	m_hitDistance = ColliderNum(_mtx1, _mtx2);

	//�Q�̔��a�̍��v�����Q�̊Ԃ̋�������������Γ������Ă���
	if (m_hitDistance < _Radius) {
		return true;
	}
	else {
		return false;
	}

	//�ی�
	// ����ɍs���Ȃ������ꍇ
	Texture tex;
	MessageBox(nullptr, "�����蔻�肪����ɏI�����Ă��܂���",
		"Error!", MB_ICONERROR | MB_OK);
	tex.texture = nullptr;
	return false;
}

void Collider::AllOnCollision(std::shared_ptr<Player>& _player, std::vector<std::unique_ptr<EnemyObject>>& _enemy,
	std::vector<std::unique_ptr<BulletObject>>& _Pbullet, std::vector<std::unique_ptr<BulletObject>>& _Ebullet)
{
	//�v���C���[���S�e���X�V
	for (auto& b : _Pbullet) {

		//��ɓ������Ă��瓖���蔻��
		//b->Move(_player->GetPos());

		bool m_hitFlg = false;
		//�G�̒e�ƃv���C���[�̒e�����蔻��
		for (int i = 0; i < _Ebullet.size(); i++)
		{
			if (Collider::GetInstance()->OnCollision(_Ebullet[i], b))
			{
				//m_bulletsEnemy[i]->SetDead();

				m_hitFlg = true;
				break;
			}
		}
		if (b->GetState() == STATE::LIVE)
		{
			//���ׂĂ̓G�ƃv���C���[�̒e�̓����蔻��
			for (int i = 0; i < _enemy.size(); i++)
			{
				if (Collider::GetInstance()->OnCollision(_enemy[i], b,_player))
				{
					m_hitFlg = true;
					break;
				}
			}
		}

		//�X�V
		if (m_hitFlg != true)
		{
			// �^�[�Q�b�g������
			//b->SetTarget(m_sPlayer->GetPos());
			b->Update();

		}
		else
		{
			b->SetDead();
		}
	}

	//�G���e�ƃv���C���[�̓����蔻��
	for (auto& b : _Ebullet) {
		Collider::GetInstance()->OnCollision(_player, b);
	}

	//�G�ƃv���C���[�̓����蔻��
	for (int i = 0; i < _enemy.size(); i++)
	{
		Collider::GetInstance()->OnCollision(_player, _enemy[i]);
	}
}

bool Collider::OnCollision(std::shared_ptr<Player>& _player, std::unique_ptr<EnemyObject>& _enemy)
{
	if (_enemy->GetState() == STATE::LIVE)
	{
		//�����蔻��A���ꂽ����m�̋����𑪂�
		m_hitDistance = ColliderNum(_player->GetMtx(), _enemy->GetMtx());

		//�f�o�b�O�p
		/*float AAA = _player->GetRadius();
		float BBB = _enemy->GetRadius();*/

		//�Q�̔��a�̍��v�����Q�̊Ԃ̋�������������Γ������Ă���
		if (m_hitDistance < (_player->GetRadius() + _enemy->GetRadius())) {
			//�����������̏���
			_enemy->SetDead();
			_player->HPDecrease(_enemy->GetCSVStatus(ENEMY_STATUSID::ATTACK));
			//�q�b�gSE�Đ�
			Sound::instance().Play(SOUND_LABEL_PHIT_SE);
			return true;
		}
		else {
			//������Ȃ��������̏���

			return false;
		}
	}
	else
	{
		return false;
	}

	//�ی�
	// ����ɍs���Ȃ������ꍇ
	Texture tex;
	MessageBox(nullptr, "�����蔻�肪����ɏI�����Ă��܂���",
		"Error!", MB_ICONERROR | MB_OK);
	tex.texture = nullptr;
	return false;
}

bool Collider::OnCollision(std::shared_ptr<Player>& _player, std::unique_ptr<BulletObject>& _bullet)
{
	//�����蔻��A���ꂽ����m�̋����𑪂�
	m_hitDistance = ColliderNum(_player->GetMtx(), _bullet->GetMtx());

	//�f�o�b�O�p
	/*float AAA = _player->GetRadius();
	float BBB = _enemy->GetRadius();*/

	//�Q�̔��a�̍��v�����Q�̊Ԃ̋�������������Γ������Ă���
	if (m_hitDistance < (_player->GetRadius() + _bullet->GetRadius())) {
		//�����������̏���
		_bullet->SetDead();
		_player->HPDecrease(_bullet->GetCSVStatus(BULLET_STATUSID::ATTACK_POWER));
		//�q�b�gSE�Đ�
		Sound::instance().Play(SOUND_LABEL_PHIT_SE);

		return true;
	}
	else {
		//������Ȃ��������̏���
		_bullet->Update();

		return false;
	}

	//�ی�
	// ����ɍs���Ȃ������ꍇ
	Texture tex;
	MessageBox(nullptr, "�����蔻�肪����ɏI�����Ă��܂���",
		"Error!", MB_ICONERROR | MB_OK);
	tex.texture = nullptr;
	return false;
}

bool Collider::OnCollision(std::unique_ptr<EnemyObject>& _enemy, std::unique_ptr<BulletObject>& _bullet, 
	std::shared_ptr<Player>& _player)
{
	//�����Ă���Ώ���
	if (_enemy->GetState() == STATE::LIVE)
	{
		//�����蔻��A���ꂽ����m�̋����𑪂�
		m_hitDistance = ColliderNum(_enemy->GetMtx(), _bullet->GetMtx());

		//�f�o�b�O�p
		/*float AAA = _enemy->GetRadius();
		float BBB = _bullet->GetRadius();*/

		//�Q�̔��a�̍��v�����Q�̊Ԃ̋�������������Γ������Ă���
		if (m_hitDistance < (_enemy->GetRadius() + _bullet->GetRadius())) {
			//�����������̏���
			//�e���S
			_bullet->SetDead();
			//�G�̗̑͂�����
			_enemy->HPDecrease(_bullet->GetCSVStatus(BULLET_STATUSID::ATTACK_POWER) * 
				_player->GetCSVStatus(PLAYER_STATUSID::ATTACK));
			//�G�t�F�N�g����
			_enemy->EffectReset();
			//�q�b�gSE�Đ�
			Sound::instance().Play(SOUND_LABEL_EHIT_SE);

			return true;
		}
		else {
			//������Ȃ��������̏���
			return false;
		}
	}
	else
	{
		return false;
	}

	//�ی�
	// ����ɍs���Ȃ������ꍇ
	Texture tex;
	MessageBox(nullptr, "�����蔻�肪����ɏI�����Ă��܂���",
		"Error!", MB_ICONERROR | MB_OK);
	tex.texture = nullptr;
	return false;
}

//�e�ƒe�I�u�W�F�N�g
bool Collider::OnCollision(std::unique_ptr<BulletObject>& _bullet1, std::unique_ptr<BulletObject>& _bullet2)
{
	//�����蔻��A���ꂽ����m�̋����𑪂�
	m_hitDistance = ColliderNum(_bullet1->GetPos(), _bullet2->GetPos());
	
	//�f�o�b�O�p
	/*float AAA = _enemy->GetRadius();
	float BBB = _bullet->GetRadius();*/

	//�Q�̔��a�̍��v�����Q�̊Ԃ̋�������������Γ������Ă���
	if (m_hitDistance < (_bullet1->GetRadius() + _bullet2->GetRadius())) {
		//�����������̏���
		_bullet1->SetDead();
		_bullet2->SetDead();

		return true;
	}
	else {
		//������Ȃ��������̏���
		return false;
	}

	//�ی�
	// ����ɍs���Ȃ������ꍇ
	Texture tex;
	MessageBox(nullptr, "�����蔻�肪����ɏI�����Ă��܂���",
		"Error!", MB_ICONERROR | MB_OK);
	tex.texture = nullptr;
	return false;
}

void Collider::OnCollision(Player& _player, EnemyLoop& _enemy)
{
	//�����蔻��A���ꂽ����m�̋����𑪂�
	m_hitDistance = ColliderNum(_player.GetMtx(), _enemy.GetMtx());

	float AAA = _player.GetRadius();
	float BBB = _enemy.GetRadius();

	//�Q�̔��a�̍��v�����Q�̊Ԃ̋�������������Γ������Ă���
	if (m_hitDistance < (_player.GetRadius() + _enemy.GetRadius())) {
		//�����������̏���
		int AA = 0;
	}
	else {
		//������Ȃ��������̏���
		int AA = 0;
	}
}



