#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <random>
#include <algorithm>

int main()
{
	const uint32_t samples = 256;
	float actions[samples];
	float err[samples];

	olc::vf2d policy = { 0.0f, 1.0f };
	olc::vf2d targetPolicy = { -10.0f, 1.0f };
	olc::vf2d policyGrad;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::normal_distribution<> dis(0.0f, 1.0f);

	printf("Policy: %f %f\n", policy.x, policy.y);
	for (int itr = 0; itr < 64; itr++)
	{
		float avgErr = 0.0f;
		for (uint32_t i = 0; i < samples; i++)
		{
			actions[i] = policy.x + policy.y * dis(gen);
			err[i] = abs((targetPolicy.x + targetPolicy.y * dis(gen)) - actions[i]);
			avgErr += err[i];
		}
		avgErr /= samples;

		olc::vf2d policyAccuminator = { 0.0f, 0.0f };
		for (uint32_t i = 0; i < samples; i++)
		{
			float tmp = 1.0f / policy.y;
			policyGrad.x = (actions[i] - policy.x) * tmp * tmp;
			policyGrad.y = policyGrad.x * policyGrad.x * policy.y - tmp;
			float logProb = 0.5f * policyGrad.y * policy.y + log(policy.y) + 1.4189385332046727f;
			float advantage = avgErr - err[i];
			policyAccuminator += advantage * policyGrad * logProb;
		}
		policy += policyAccuminator * 0.001f;
		policy.y = std::max(0.1f, policy.y);
		printf("Policy: %f %f\n", policy.x, policy.y);
	}

	return 0;
}