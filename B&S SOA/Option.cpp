#include "Option.h"

void Option::Get_option_price()
{


	int vl = vsetvlmax_e32m4();
	vfloat32m4_t Int_rest = vfmv_v_f_f32m4(Interest_rest, vl);
	vfloat32m4_t Volat = vfmv_v_f_f32m4(Volatility, vl);
	vfloat32m4_t constant1 = vfmv_v_f_f32m4(Interest_rest+(Volatility * Volatility * 0.5f), vl);
	vfloat32m4_t constant2 = vfmv_v_f_f32m4(Interest_rest - (Volatility * Volatility * 0.5f), vl);
	vfloat32m4_t Minus_Int_rest = vfmv_v_f_f32m4(Interest_rest * (-1.0f), vl);
	float* logmas = new float[4];
	float* sqrtmas = new float[4];
	float* expmas = new float[4];
	for (; N > 0; N -= vl)
	{
		vl = vsetvl_e32m4(N);
		vfloat32m4_t so = vle_v_f32m4(S0, vl);
		vfloat32m4_t k = vle_v_f32m4(K, vl);
		vfloat32m4_t so_dibv_k = vfdiv_vv_f32m4(so, k, vl);
		vse_v_f32m4(logmas, so_dibv_k, vl);
		for (int i = 0; i < 4; i++)
		{
			logmas[i] = std::log(logmas[i]);
		}
		vfloat32m4_t log_div_sok = vle_v_f32m4(logmas, vl);
		
		vfloat32m4_t t = vle_v_f32m4(T, vl);
		vfloat32m4_t const_mult_T = vfmult_vv_f32m4(t, constant1, vl);
		    
		vse_v_f32m4(sqrtmas,t, vl);
		for (int i = 0; i < 4; i++)
		{
			sqrtmas[i] = std::sqrt(sqrtmas[i]);
		}
		vfloat32m4_t sqrt_t = vle_v_f32m4(sqrtmas, vl);
		vfloat32m4_t vol_mult_sqrt_t = vfmult_vv_f32m4(sqrt_t, Volat, vl);
		vfloat32m4_t const_mult_T_div_vol_mult_sqrt_t = vfdiv_vv_f32m4(const_mult_T, vol_mult_sqrt_t, vl);
		vfloat32m4_t d1 = vfadd_vv_f32m4(const_mult_T_div_vol_mult_sqrt_t, log_div_sok);
		 
		vfloat32m4_t const_mult_T2 = vfmult_vv_f32m4(t, constant2, vl);
		vfloat32m4_t const_mult_T_div_vol_mult_sqrt_t2 = vfdiv_vv_f32m4(const_mult_T2, vol_mult_sqrt_t, vl);
		vfloat32m4_t  d2 = vfadd_vv_f32m4(const_mult_T_div_vol_mult_sqrt_t2, log_div_sok);


		vfloat32m4_t Time = vle_v_f32m4(T, vl);
		vfloat32m4_t Time_mult_minus_int_rest = vfmult_vv_f32m4(Time, Minus_Int_rest, vl);
		vse_v_f32m4(expmas, Time_mult_minus_int_rest, vl);
		for (int i = 0; i < 4; i++)
		{
			expmas[i] = std::exp(expmas[i]);
		}
		vfloat32m4_t exp_Time_mult_minus_int_rest = vle_v_f32m4(expmas, vl);
		vfloat32m4_t exp_etc_mult_d2 = vfmult_vv_f32m4(exp_Time_mult_minus_int_rest, d2, vl);
		vfloat32m4_t so_mult_d1 = vfmult_vv_f32m4(so, d1, vl);
		vfloat32m4_t so_mult_d1_minus_exp_etc_mult_d2 = vfsub_vv_f32m4(so_mult_d1, exp_etc_mult_d2, vl);
		void vse_v_f32m4(C, so_mult_d1_minus_exp_etc_mult_d2, vl);
		S0 += vl;
		K += vl;
		T += vl;
		C += vl;



	}
}


//void Option::Get_option_price()
//{
//#pragma omp parallel 
//	{
//#pragma omp for simd 
//		for (int i = 0; i < N; i++)
//		{
//			float d1 = (std::log(S0[i] / K[i]) + ((Interest_rest + (Volatility * Volatility / 2.0f)) * T[i]) / Volatility * std::sqrtf(T[i]));
//			float d2 = (std::log(S0[i] / K[i]) + ((Interest_rest - (Volatility * Volatility / 2.0f)) * T[i]) / Volatility * std::sqrtf(T[i]));
//			float cdfnorm1 = 0.5f + std::erf(d1 / std::sqrtf(2.0f)) / 2.0f;
//			float cdfnorm2 = 0.5f + std::erf(d2 / std::sqrtf(2.0f)) / 2.0f;
//			C[i] = S0[i] * cdfnorm1 - K[i] * std::exp((-1.0f) * Interest_rest * T[i]) * cdfnorm2;
//			//if (i == 0)
//				//std::cout << C[i] << ' ' << d1 << ' ' << d2 << ' ' << cdfnorm1 << ' ' << cdfnorm2 << ' ' << T[i] << ' ' << S0[i] << ' ' << K[i] << std::endl;
//		}
//	}
//}
