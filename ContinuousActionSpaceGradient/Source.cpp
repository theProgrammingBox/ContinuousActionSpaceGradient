#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <random>
#include <algorithm>

int main()
{
	const uint32_t samples = 10;
	float actions[samples];

	olc::vf2d policy = { 0.0f, 1.0f };
	olc::vf2d targetPolicy = { -100.0f, 100.0f };
	olc::vf2d policyGrad;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::normal_distribution<> dis(0.0f, 1.0f);

	while (true)
	{
		float targetAction = targetPolicy.x + targetPolicy.y * dis(gen);

		for (uint32_t i = 0; i < samples; i++)
			actions[i] = policy.x + policy.y * dis(gen);

		// sort actions by errors closest to 0
		std::sort(actions, actions + samples, [targetAction](float a, float b) { return std::abs(targetAction - a) < std::abs(targetAction - b); });

		for (uint32_t i = samples * 0.5f; i < samples; i++)
		{
			float tmp = 1.0f / policy.y;
			policyGrad.x = (actions[i] - policy.x) * tmp * tmp;
			policyGrad.y = policyGrad.x * policyGrad.x * policy.y - tmp;

			policy -= policyGrad * 0.01f;
			policy.y = std::max(0.2f, policy.y);
		}

		for (uint32_t i = samples * 0.5f; i--;)
		{
			float tmp = 1.0f / policy.y;
			policyGrad.x = (actions[i] - policy.x) * tmp * tmp;
			policyGrad.y = policyGrad.x * policyGrad.x * policy.y - tmp;

			policy += policyGrad * 0.01f;
			policy.y = std::max(0.2f, policy.y);
		}

		printf("Policy: %f %f\n", policy.x, policy.y);
	}

	return 0;
}