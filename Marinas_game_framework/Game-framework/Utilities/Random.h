#pragma once

// Random number generator
class CRandom
{
public:

	 CRandom(void) {}
	~CRandom(void) {}

	void		Seed(void);

	uint32_t	RandomUint(void);
	uint32_t	RandomUint(const uint32_t Min, const uint32_t Max);

	float		RandomFloat(void);
	float		RandomFloat(const float Min, const float Max);

private:

	std::mt19937												m_RandomEngine;
	std::uniform_int_distribution<std::mt19937::result_type>	m_Distribution;

};