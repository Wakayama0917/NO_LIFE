#include "Collider.h"
#include "XAudio2.h"

float Collider::ColliderNum(const DirectX::XMFLOAT4X4& _mtx1, const DirectX::XMFLOAT4X4& _mtx2){
	/*XMFLOAT3 vec1;
	XMFLOAT3 vec2;*/
	float num = 0;//距離
	float numX = 0;
	float numZ = 0;
	
	/*************************************************************
	* X軸とZ軸のみの判定を行う
	* １つ目のXと２つ目のXの差が0でないか確認
	* １つ目のXと２つ目のXの差を2乗する
	* １つ目のZと２つ目のZの差が0でないか確認
	* １つ目のZと２つ目のZの差を2乗する
	* 両者を足した数の非負の平方根を返す。
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
	//平方根
	num = sqrt(numX + numZ);

	//デバッグ用数値表示
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
	//当たり判定、入れた二つ同士の距離を測る
	m_hitDistance = ColliderNum(_mtx1, _mtx2);

	//２つの半径の合計よりも２つの間の距離が小さければ当たっている
	if (m_hitDistance < _Radius) {
		return true;
	}
	else {
		return false;
	}

	//保険
	// 正常に行われなかった場合
	Texture tex;
	MessageBox(nullptr, "当たり判定が正常に終了していません",
		"Error!", MB_ICONERROR | MB_OK);
	tex.texture = nullptr;
	return false;
}

void Collider::AllOnCollision(std::shared_ptr<Player>& _player, std::vector<std::unique_ptr<EnemyObject>>& _enemy,
	std::vector<std::unique_ptr<BulletObject>>& _Pbullet, std::vector<std::unique_ptr<BulletObject>>& _Ebullet)
{
	//プレイヤー側全弾を更新
	for (auto& b : _Pbullet) {

		//先に動かしてから当たり判定
		//b->Move(_player->GetPos());

		bool m_hitFlg = false;
		//敵の弾とプレイヤーの弾当たり判定
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
			//すべての敵とプレイヤーの弾の当たり判定
			for (int i = 0; i < _enemy.size(); i++)
			{
				if (Collider::GetInstance()->OnCollision(_enemy[i], b,_player))
				{
					m_hitFlg = true;
					break;
				}
			}
		}

		//更新
		if (m_hitFlg != true)
		{
			// ターゲットを入れる
			//b->SetTarget(m_sPlayer->GetPos());
			b->Update();

		}
		else
		{
			b->SetDead();
		}
	}

	//敵側弾とプレイヤーの当たり判定
	for (auto& b : _Ebullet) {
		Collider::GetInstance()->OnCollision(_player, b);
	}

	//敵とプレイヤーの当たり判定
	for (int i = 0; i < _enemy.size(); i++)
	{
		Collider::GetInstance()->OnCollision(_player, _enemy[i]);
	}
}

bool Collider::OnCollision(std::shared_ptr<Player>& _player, std::unique_ptr<EnemyObject>& _enemy)
{
	if (_enemy->GetState() == STATE::LIVE)
	{
		//当たり判定、入れた二つ同士の距離を測る
		m_hitDistance = ColliderNum(_player->GetMtx(), _enemy->GetMtx());

		//デバッグ用
		/*float AAA = _player->GetRadius();
		float BBB = _enemy->GetRadius();*/

		//２つの半径の合計よりも２つの間の距離が小さければ当たっている
		if (m_hitDistance < (_player->GetRadius() + _enemy->GetRadius())) {
			//当たった時の処理
			_enemy->SetDead();
			_player->HPDecrease(_enemy->GetCSVStatus(ENEMY_STATUSID::ATTACK));
			//ヒットSE再生
			Sound::instance().Play(SOUND_LABEL_PHIT_SE);
			return true;
		}
		else {
			//当たらなかった時の処理

			return false;
		}
	}
	else
	{
		return false;
	}

	//保険
	// 正常に行われなかった場合
	Texture tex;
	MessageBox(nullptr, "当たり判定が正常に終了していません",
		"Error!", MB_ICONERROR | MB_OK);
	tex.texture = nullptr;
	return false;
}

bool Collider::OnCollision(std::shared_ptr<Player>& _player, std::unique_ptr<BulletObject>& _bullet)
{
	//当たり判定、入れた二つ同士の距離を測る
	m_hitDistance = ColliderNum(_player->GetMtx(), _bullet->GetMtx());

	//デバッグ用
	/*float AAA = _player->GetRadius();
	float BBB = _enemy->GetRadius();*/

	//２つの半径の合計よりも２つの間の距離が小さければ当たっている
	if (m_hitDistance < (_player->GetRadius() + _bullet->GetRadius())) {
		//当たった時の処理
		_bullet->SetDead();
		_player->HPDecrease(_bullet->GetCSVStatus(BULLET_STATUSID::ATTACK_POWER));
		//ヒットSE再生
		Sound::instance().Play(SOUND_LABEL_PHIT_SE);

		return true;
	}
	else {
		//当たらなかった時の処理
		_bullet->Update();

		return false;
	}

	//保険
	// 正常に行われなかった場合
	Texture tex;
	MessageBox(nullptr, "当たり判定が正常に終了していません",
		"Error!", MB_ICONERROR | MB_OK);
	tex.texture = nullptr;
	return false;
}

bool Collider::OnCollision(std::unique_ptr<EnemyObject>& _enemy, std::unique_ptr<BulletObject>& _bullet, 
	std::shared_ptr<Player>& _player)
{
	//生きていれば処理
	if (_enemy->GetState() == STATE::LIVE)
	{
		//当たり判定、入れた二つ同士の距離を測る
		m_hitDistance = ColliderNum(_enemy->GetMtx(), _bullet->GetMtx());

		//デバッグ用
		/*float AAA = _enemy->GetRadius();
		float BBB = _bullet->GetRadius();*/

		//２つの半径の合計よりも２つの間の距離が小さければ当たっている
		if (m_hitDistance < (_enemy->GetRadius() + _bullet->GetRadius())) {
			//当たった時の処理
			//弾死亡
			_bullet->SetDead();
			//敵の体力を引く
			_enemy->HPDecrease(_bullet->GetCSVStatus(BULLET_STATUSID::ATTACK_POWER) * 
				_player->GetCSVStatus(PLAYER_STATUSID::ATTACK));
			//エフェクト発生
			_enemy->EffectReset();
			//ヒットSE再生
			Sound::instance().Play(SOUND_LABEL_EHIT_SE);

			return true;
		}
		else {
			//当たらなかった時の処理
			return false;
		}
	}
	else
	{
		return false;
	}

	//保険
	// 正常に行われなかった場合
	Texture tex;
	MessageBox(nullptr, "当たり判定が正常に終了していません",
		"Error!", MB_ICONERROR | MB_OK);
	tex.texture = nullptr;
	return false;
}

//弾と弾オブジェクト
bool Collider::OnCollision(std::unique_ptr<BulletObject>& _bullet1, std::unique_ptr<BulletObject>& _bullet2)
{
	//当たり判定、入れた二つ同士の距離を測る
	m_hitDistance = ColliderNum(_bullet1->GetPos(), _bullet2->GetPos());
	
	//デバッグ用
	/*float AAA = _enemy->GetRadius();
	float BBB = _bullet->GetRadius();*/

	//２つの半径の合計よりも２つの間の距離が小さければ当たっている
	if (m_hitDistance < (_bullet1->GetRadius() + _bullet2->GetRadius())) {
		//当たった時の処理
		_bullet1->SetDead();
		_bullet2->SetDead();

		return true;
	}
	else {
		//当たらなかった時の処理
		return false;
	}

	//保険
	// 正常に行われなかった場合
	Texture tex;
	MessageBox(nullptr, "当たり判定が正常に終了していません",
		"Error!", MB_ICONERROR | MB_OK);
	tex.texture = nullptr;
	return false;
}

void Collider::OnCollision(Player& _player, EnemyLoop& _enemy)
{
	//当たり判定、入れた二つ同士の距離を測る
	m_hitDistance = ColliderNum(_player.GetMtx(), _enemy.GetMtx());

	float AAA = _player.GetRadius();
	float BBB = _enemy.GetRadius();

	//２つの半径の合計よりも２つの間の距離が小さければ当たっている
	if (m_hitDistance < (_player.GetRadius() + _enemy.GetRadius())) {
		//当たった時の処理
		int AA = 0;
	}
	else {
		//当たらなかった時の処理
		int AA = 0;
	}
}



