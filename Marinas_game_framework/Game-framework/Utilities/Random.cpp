#include "Pch.h"
#include "Random.h"

void CRandom::Seed(void)
{
	m_RandomEngine.seed(std::random_device()());
}

uint32_t CRandom::RandomUint(void)
{
	return m_Distribution(m_RandomEngine);
}

uint32_t CRandom::RandomUint(const uint32_t Min, const uint32_t Max)
{
	return Min + (RandomUint() % (Max - Min + 1));
}

float CRandom::RandomFloat(void)
{
	return (float)m_Distribution(m_RandomEngine) / (float)(std::numeric_limits<uint32_t>::max)();
}

float CRandom::RandomFloat(const float Min, const float Max)
{
	return RandomFloat() * (Max - Min) + Min;
}