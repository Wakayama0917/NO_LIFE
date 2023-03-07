#include "Random.h"

int Random::RandomGeneration(int _minimum, int _maximum)
{
	std::random_device rand_dev;//�V�[�h�l����p����
	// �V�[�h�l�Z�b�g
	m_random.seed(rand_dev());
	//��������
	std::uniform_int_distribution<int32_t> randNum(_minimum, _maximum);

	return randNum(m_random);
}
