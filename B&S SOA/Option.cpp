#include "Option.h"


void Option::Get_option_price()
{
 #pragma omp parallel 
	{
		#pragma omp for simd 
		for (int i = 0; i < N; i++)
		{
			float d1 = (std::log(S0[i] / K[i]) + (Interest_rest + Volatility * Volatility * 0.5f) * T[i]) / (Volatility * std::sqrt(T[i]));
			float d2 = (std::log(S0[i] / K[i]) + (Interest_rest - Volatility * Volatility * 0.5f) * T[i]) / (Volatility * std::sqrt(T[i]));
			float cdfnorm1 = 0.5f + std::erf(d1 / std::sqrt(2.0f)) * 0.5f;
			float cdfnorm2 = 0.5f + std::erf(d2 / std::sqrt(2.0f)) * 0.5f;
			C[i] = S0[i] * cdfnorm1 - K[i] * std::exp((-1.0f) * Interest_rest * T[i]) * cdfnorm2;
			//if (i == 0)
				//std::cout << C[i] << ' ' << d1 << ' ' << d2 << ' ' << cdfnorm1 << ' ' << cdfnorm2 << ' ' << T[i] << ' ' << S0[i] << ' ' << K[i] << std::endl;
		}
	}
}