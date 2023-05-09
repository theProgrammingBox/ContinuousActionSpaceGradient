#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <random>

int main()
{
	olc::vf2d policy = { 0.0f, 1.0f };
	olc::vf2d targetPolicy = { -10.0f, 0.4f };
	olc::vf2d policyGrad;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0.0f, 1.0f);

	while (true)
	{
		float action = policy.x + policy.y * dis(gen);
		float action2 = policy.x + policy.y * dis(gen);
		float targetAction = targetPolicy.x + targetPolicy.y * dis(gen);

		float error = targetAction - action;
		float error2 = targetAction - action2;

		if (error2 < error)
		{
			action = action2;
			error = error2;
		}
		float tmp = 1.0f / policy.y;
		policyGrad.x = (action - policy.x) * tmp * tmp;
		policyGrad.y = policyGrad.x * policyGrad.x * policy.y - tmp;

		policy += 0.1f * error * policyGrad;

		printf("error: %f, policy: %f, %f\n", error, policy.x, policy.y);
	}

	return 0;
}