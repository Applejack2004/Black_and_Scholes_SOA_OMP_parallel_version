#include "Option.h"
void Option::Get_option_price()
{


	int vl = vsetvlmax_e32m1();
	vfloat32m1_t Int_rest = vfmv_v_f_f32m1(Interest_rest, vl);
	vfloat32m1_t Volat = vfmv_v_f_f32m1(Volatility, vl);
	vfloat32m1_t constant1 = vfmv_v_f_f32m1(Interest_rest + (Volatility * Volatility * 0.5f), vl);
	vfloat32m1_t constant2 = vfmv_v_f_f32m1(Interest_rest - (Volatility * Volatility * 0.5f), vl);
	vfloat32m1_t Minus_Int_rest = vfmv_v_f_f32m1(Interest_rest * (-1.0f), vl);

	float logmas[4], expmas[4], sqrtmas[4];
	float* pC = C, * pK = K, * pS0 = S0, * pT = T;
	int n = N;
	//printf("!!!!!!!!!!!!!!!\%d", (int)N);
	for (; n > 0; n -= vl)
	{
		vl = vsetvl_e32m1(n);
		//printf("!!!!!!!!!!!!!!!\%d", (int)vl);
		vfloat32m1_t so = vle_v_f32m1(pS0, vl);
		vfloat32m1_t k = vle_v_f32m1(pK, vl);
		vfloat32m1_t so_dibv_k = vfdiv_vv_f32m1(so, k, vl);
		vse_v_f32m1(logmas, so_dibv_k, vl);
		for (int i = 0; i < 4; i++)
		{
			logmas[i] = std::log(logmas[i]);
		}
		vfloat32m1_t log_div_sok = vle_v_f32m1(logmas, vl);

		vfloat32m1_t t = vle_v_f32m1(pT, vl);
		vfloat32m1_t const_mult_T = vfmul_vv_f32m1(t, constant1, vl);

		vse_v_f32m1(sqrtmas, t, vl);
		for (int i = 0; i < 4; i++)
		{
			sqrtmas[i] = std::sqrt(sqrtmas[i]);
		}
		vfloat32m1_t sqrt_t = vle_v_f32m1(sqrtmas, vl);
		vfloat32m1_t vol_mult_sqrt_t = vfmul_vv_f32m1(sqrt_t, Volat, vl);
		vfloat32m1_t const_mult_T_div_vol_mult_sqrt_t = vfdiv_vv_f32m1(const_mult_T, vol_mult_sqrt_t, vl);
		vfloat32m1_t d1 = vfadd_vv_f32m1(const_mult_T_div_vol_mult_sqrt_t, log_div_sok, vl);

		vfloat32m1_t const_mult_T2 = vfmul_vv_f32m1(t, constant2, vl);
		vfloat32m1_t const_mult_T_div_vol_mult_sqrt_t2 = vfdiv_vv_f32m1(const_mult_T2, vol_mult_sqrt_t, vl);
		vfloat32m1_t  d2 = vfadd_vv_f32m1(const_mult_T_div_vol_mult_sqrt_t2, log_div_sok, vl);


		vfloat32m1_t Time = vle_v_f32m1(pT, vl);
		vfloat32m1_t Time_mult_minus_int_rest = vfmul_vv_f32m1(Time, Minus_Int_rest, vl);
		vse_v_f32m1(expmas, Time_mult_minus_int_rest, vl);
		for (int i = 0; i < 4; i++)
		{
			expmas[i] = std::exp(expmas[i]);
		}
		vfloat32m1_t exp_Time_mult_minus_int_rest = vle_v_f32m1(expmas, vl);
		vfloat32m1_t exp_etc_mult_d2 = vfmul_vv_f32m1(exp_Time_mult_minus_int_rest, d2, vl);
		vfloat32m1_t so_mult_d1 = vfmul_vv_f32m1(so, d1, vl);
		vfloat32m1_t so_mult_d1_minus_exp_etc_mult_d2 = vfsub_vv_f32m1(so_mult_d1, exp_etc_mult_d2, vl);
		vse_v_f32m1(pC, so_mult_d1_minus_exp_etc_mult_d2, vl);

		//vse_v_f32m1(pC, so, vl);

		pS0 += vl;
		pK += vl;
		pT += vl;
		pC += vl;



	}
	
}
