#pragma once
#include <random>

class Random
{
	Random() {}
public:
	//�V���O���g��
	static Random* GetInstance() {
		static Random instance;
		return &instance;
	}

	int RandomGeneration(int _minimum, int _Maximum);

private:
	std::mt19937 m_random;//���������p�G�𐶐�
};

