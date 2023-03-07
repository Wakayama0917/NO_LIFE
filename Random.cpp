#include "Random.h"

int Random::RandomGeneration(int _minimum, int _maximum)
{
	std::random_device rand_dev;//シード値決定用乱数
	// シード値セット
	m_random.seed(rand_dev());
	//乱数生成
	std::uniform_int_distribution<int32_t> randNum(_minimum, _maximum);

	return randNum(m_random);
}
