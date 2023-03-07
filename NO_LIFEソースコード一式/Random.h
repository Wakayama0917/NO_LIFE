#pragma once
#include <random>

class Random
{
	Random() {}
public:
	//シングルトン
	static Random* GetInstance() {
		static Random instance;
		return &instance;
	}

	int RandomGeneration(int _minimum, int _Maximum);

private:
	std::mt19937 m_random;//乱数発生用敵を生成
};

