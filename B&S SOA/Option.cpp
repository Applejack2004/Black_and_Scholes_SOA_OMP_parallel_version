#include "Option.h"

void Option::Get_option_price()
{
	for (size_t i = 0; i < N; i++)
	{
		float d1 = (std::log(S0[i] / K[i]) + ((Interest_rest + (Volatility * Volatility / 2)) * T[i]) / Volatility * std::sqrtf(T[i]));
		float d2 = (std::log(S0[i] / K[i]) + ((Interest_rest - (Volatility * Volatility / 2)) * T[i]) / Volatility * std::sqrtf(T[i]));
		float cdfnorm1 = 0.5f + std::erf(d1 / std::sqrtf(2.0f)) / 2.0f;
		float cdfnorm2 = 0.5f + std::erf(d2 / std::sqrtf(2.0f)) / 2.0f;
		C[i] = S0[i] * cdfnorm1 - K[i] * std::exp((-1) * Interest_rest * T[i]) * cdfnorm2;
	}
}
