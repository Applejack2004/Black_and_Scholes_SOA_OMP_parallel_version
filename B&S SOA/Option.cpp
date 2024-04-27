#include "Option.h"
void printArrayToFile(const float arr[], int size, const std::string& filename) {
	std::ofstream file(filename, std::ios::app); // �������� ����� � ������ ����������    
	if (file.is_open()) {
		for (int i = 0; i < size; i++) {
			file << arr[i] << " ";
		}
		file.close();

	}
	else {
		// std::cout << "Ошибка открытия файла" << std::endl;
	}
}
void FLOAT2INT(vfloat32m4_t f, vfloat32m4_t rf, vuint32m4_t fint, int _vl)
{
	vfloat32m4_t tmp = vfadd_vf_f32m4(f, 12582912.0f, _vl);
	rf = vfsub_vf_f32m4(tmp, 12582912.0f, _vl);
	fint = vreinterpret_v_f32m4_u32m4(tmp);

	fint = vand_vx_u32m4(fint, 0xFFFF, _vl);
}



vfloat32m4_t my_exp0(vfloat32m4_t x, int vl) {
	vfloat32m4_t y, kf;
	vuint32m4_t ki;


	float Log2 = (float)0x1.62e43p-1;
	float Log2h = (float)0xb.17200p-4;
	float Log2l = (float)0x1.7f7d1cf8p-20;
	float InvLog2 = (float)0x1.715476p0;

	// Here should be the tests for exceptional cases
	vfloat32m4_t x_mult_InvLog2 = vfmul_vf_f32m4(x, InvLog2, vl);
	FLOAT2INT(x_mult_InvLog2, kf, ki, vl);

	//y = (x - kf*Log2h) - kf*Log2l;
	vfloat32m4_t kf_mult_Log2h = vfmul_vf_f32m4(kf, Log2h, vl);
	vfloat32m4_t kf_mult_Log2l = vfmul_vf_f32m4(kf, Log2l, vl);
	vfloat32m4_t  x_sub_kflog2h = vfsub_vv_f32m4(x, kf_mult_Log2h, vl);
	y = vfsub_vv_f32m4(x_sub_kflog2h, kf_mult_Log2l, vl);

	vfloat32m4_t result;
	result = vfmul_vf_f32m4(y, (float)0x1.6850e4p-10, vl);
	result = vfadd_vf_f32m4(result, (float)0x1.123bccp-7, vl);
	result = vfmul_vv_f32m4(y, result, vl);
	result = vfadd_vf_f32m4(result, (float)0x1.555b98p-5, vl);
	result = vfmul_vv_f32m4(y, result, vl);
	result = vfadd_vf_f32m4(result, (float)0x1.55548ep-3, vl);
	result = vfmul_vv_f32m4(y, result, vl);
	result = vfadd_vf_f32m4(result, (float)0x1.fffff8p-2, vl);
	result = vfmul_vv_f32m4(y, result, vl);
	result = vfadd_vf_f32m4(result, (float)0x1p0, vl);
	result = vfmul_vv_f32m4(y, result, vl);
	result = vfadd_vf_f32m4(result, (float)0x1p0, vl);

	/* r.f = (float)0x1p0 + y * ((float)0x1p0
						+ y * ((float)0x1.fffff8p-2
						+ y * ((float)0x1.55548ep-3
						+ y * ((float)0x1.555b98p-5
						+ y * ((float)0x1.123bccp-7
						+ y *  (float)0x1.6850e4p-10)))));*/

						//  r.i16[HI] += k << 7; //Exponent update
	vuint32m4_t resint = vreinterpret_v_f32m4_u32m4(result);

	vuint32m4_t tmp_exp = vand_vx_u32m4(resint, 0x7F800000u, vl);//получаем текущую экспоненту за два действия
	vuint32m4_t tmp_exp_srl = vsrl_vx_u32m4(tmp_exp, 23, vl);

	tmp_exp_srl = vadd_vv_u32m4(tmp_exp_srl, ki, vl); //увеличиваем экспоненту
	vuint32m4_t tmp_exp_sll = vsll_vx_u32m4(tmp_exp_srl, 23, vl);//установка обновленной экспоненты за 3 действия
	vuint32m4_t resint_and_mas = vand_vx_u32m4(resint, 0x807FFFFFu, vl);
	resint = vor_vv_u32m4(resint_and_mas, tmp_exp_sll, vl);

	// vfloat32m4_t exp_vect = vreinterpret_v_u32m4_f32m4(resint);

	 //return exp_vect;
	return result;
}
vfloat32m4_t my_erf0(vfloat32m4_t x, int vl)
{
	float a1 = (float)0.254829592;
	float a2 = (float)-0.284496736;
	float a3 = (float)1.421413741;
	float a4 = (float)-1.453152027;
	float a5 = (float)1.061405429;
	float p = (float)0.3275911;
	vint32m4_t sign;
	vint32m4_t floatcast = vreinterpret_v_f32m4_i32m4(x);
	sign = vsra_vx_i32m4(floatcast, 31, vl);
	sign = vor_vx_i32m4(sign, 1, vl);
	vfloat32m4_t float_sign = vfcvt_f_x_v_f32m4(sign, vl);

	vfloat32m4_t x_abs = vfmul_vv_f32m4(float_sign, x, vl);

	vfloat32m4_t oper1 = vfmul_vf_f32m4(x_abs, p, vl);
	oper1 = vfadd_vf_f32m4(oper1, 1.0f, vl);
	vfloat32m4_t vect1 = vfmv_v_f_f32m4(1.0f, vl);
	vfloat32m4_t t = vfdiv_vv_f32m4(vect1, oper1, vl);

	vfloat32m4_t oper2 = vfmul_vf_f32m4(t, a5, vl);
	oper2 = vfadd_vf_f32m4(oper2, a4, vl);
	oper2 = vfmul_vv_f32m4(oper2, t, vl);
	oper2 = vfadd_vf_f32m4(oper2, a3, vl);
	oper2 = vfmul_vv_f32m4(oper2, t, vl);
	oper2 = vfadd_vf_f32m4(oper2, a2, vl);
	oper2 = vfmul_vv_f32m4(oper2, t, vl);
	oper2 = vfadd_vf_f32m4(oper2, a1, vl);
	oper2 = vfmul_vv_f32m4(oper2, t, vl);

	vfloat32m4_t minus_x = vfmul_vf_f32m4(x, -1.0f, vl);
	vfloat32m4_t minus_x_mult_x = vfmul_vv_f32m4(x, minus_x, vl);
	vfloat32m4_t exp_x_x = my_exp0(minus_x_mult_x, vl);

	oper2 = vfmul_vv_f32m4(oper2, exp_x_x, vl);
	vfloat32m4_t y = vfsub_vv_f32m4(vect1, oper2, vl);


	vfloat32m4_t y_mult_sign = vfmul_vv_f32m4(y, float_sign, vl);
	return y_mult_sign;

}
vfloat32m4_t my_log0(vfloat32m4_t x, int vl)
{

	//fast_log abs(rel) : avgError = 2.85911e-06(3.32628e-08), MSE = 4.67298e-06(5.31012e-08), maxError = 1.52588e-05(1.7611e-07)
	float s_log_C0 = -19.645704f;
	float s_log_C1 = 0.767002f;
	float s_log_C2 = 0.3717479f;
	float s_log_C3 = 5.2653985f;
	float s_log_2 = 0.6931472f;
	float s_log_C4 = -(1.0f + s_log_C0) * (1.0f + s_log_C1) / ((1.0f + s_log_C2) * (1.0f + s_log_C3)); //ensures that log(1) == 0


	// int e = static_cast<int>(ux - 0x3f800000) >> 23;
	vint32m4_t intx = vreinterpret_v_f32m4_i32m4(x);
	vint32m4_t exp = vsub_vx_i32m4(intx, 0x3f800000, vl);
	exp = vsra_vx_i32m4(exp, 23, vl);
	// ux |= 0x3f800000;
	// ux &= 0x3fffffff; // 1 <= x < 2  after replacing the exponent field
	intx = vor_vx_i32m4(intx, 0x3f800000, vl);
	intx = vand_vx_i32m4(intx, 0x3fffffff, vl);

	// x = reinterpret_cast<float&>(ux);
	vfloat32m4_t y = vreinterpret_v_i32m4_f32m4(intx);

	//float a = (x + s_log_C0) * (x + s_log_C1);
	vfloat32m4_t a = vfadd_vf_f32m4(y, s_log_C0, vl);
	vfloat32m4_t tmp = vfadd_vf_f32m4(y, s_log_C1, vl);
	a = vfmul_vv_f32m4(a, tmp, vl);
	// float b = (x + s_log_C2) * (x + s_log_C3);

	vfloat32m4_t b = vfadd_vf_f32m4(y, s_log_C2, vl);
	vfloat32m4_t tmp2 = vfadd_vf_f32m4(y, s_log_C3, vl);
	b = vfmul_vv_f32m4(b, tmp2, vl);
	//float c = (float(e) + s_log_C4);

	vfloat32m4_t e = vfcvt_f_x_v_f32m4(exp, vl);
	vfloat32m4_t c = vfadd_vf_f32m4(e, s_log_C4, vl);

	// float d = a / b;
	vfloat32m4_t d = vfdiv_vv_f32m4(a, b, vl);


	//(c + d)* s_log_2;
	vfloat32m4_t c_plus_d = vfadd_vv_f32m4(c, d, vl);
	vfloat32m4_t result = vfmul_vf_f32m4(c_plus_d, s_log_2, vl);
	return result;
}
void Option::Get_option_price()
{


	//int vl = vsetvlmax_e32m4();
	//vfloat32m4_t Int_rest = vfmv_v_f_f32m4(Interest_rest, vl);
	//vfloat32m4_t Volat = vfmv_v_f_f32m4(Volatility, vl);
	float constant1 = Interest_rest + (Volatility * Volatility * 0.5f);
	float constant2 = Interest_rest - (Volatility * Volatility * 0.5f);
	float Minus_Int_rest = Interest_rest * (-1.0f);

	//float logmas[16], expmas[16], sqrtmas[16],erf1mas[16],erf2mas[16];
	//float* pC = C, * pK = K, * pS0 = S0, * pT = T;
	int n = N; int block_size = 0;

	//printf("!!!!!!!!!!!!!!!\%d", (int)N);
	if (n < omp_get_max_threads())
	{
		block_size = n;
		std::cout << "test" << std::endl;
	}
	else {
		block_size = n / omp_get_max_threads();
		std::cout << "test2" << std::endl;
	}
	int block_number = 0; int extra_elements;
	if (n % block_size == 0)
	{
		block_number = n / block_size;
	}
	else {
		block_number = n / block_size;
		extra_elements = n - block_size * block_number;
	}
	std::cout << "block_size=" << block_size << " " << "block_number=" << block_number << " " << "extra_elements=" << extra_elements << std::endl;
#pragma omp parallel 
	{
		float* pC, * pK, * pS0, * pT;
#pragma omp for

		for (int block_index = 0; block_index < block_number; block_index++)
		{
			int block_start = block_index * block_size;
			int block_end = block_start + block_size;
			int last_block_size = 0;
			if (block_number - block_index == 1 && extra_elements != 0)
			{
				block_end = n;

				std::cout << "last_block_size=" << block_end - block_start << " ";
			}

			pC = C + block_start, pK = K + block_start, pS0 = S0 + block_start, pT = T + block_start;
			while (block_start < block_end)
			{
				int vl = vsetvl_e32m4(block_end - block_start);

				vfloat32m4_t so = vle_v_f32m4(pS0, vl);
				vfloat32m4_t k = vle_v_f32m4(pK, vl);
				vfloat32m4_t so_div_k = vfdiv_vv_f32m4(so, k, vl);

				vfloat32m4_t log_div_sok = my_log0(so_div_k, vl);
				vfloat32m4_t t = vle_v_f32m4(pT, vl);
				vfloat32m4_t const_mult_T = vfmul_vf_f32m4(t, constant1, vl);

				vfloat32m4_t sqrt_t = vfsqrt_v_f32m4(t, vl);

				vfloat32m4_t vol_mult_sqrt_t = vfmul_vf_f32m4(sqrt_t, Volatility, vl);
				vfloat32m4_t const_mult_t1_add_log_div_sok = vfadd_vv_f32m4(const_mult_T, log_div_sok, vl);
				vfloat32m4_t d1 = vfdiv_vv_f32m4(const_mult_t1_add_log_div_sok, vol_mult_sqrt_t, vl);

				//vfloat32m4_t const_mult_T2 = vfmul_vf_f32m4(t, constant2, vl);
				//vfloat32m4_t const_mult_T_div_vol_mult_sqrt_t2 = vfdiv_vv_f32m4(const_mult_T2, vol_mult_sqrt_t, vl);
				//vfloat32m4_t  d2 = vfadd_vv_f32m4(const_mult_T_div_vol_mult_sqrt_t2, log_div_sok,vl);
				vfloat32m4_t  d2 = vfsub_vv_f32m4(d1, vol_mult_sqrt_t, vl);

				vfloat32m4_t d1_div_sqrt2 = vfdiv_vf_f32m4(d1, sqrt(2.0f), vl);

				vfloat32m4_t erfd1_div_sqrt = my_erf0(d1_div_sqrt2, vl);

				vfloat32m4_t erf_div_sqrt_div_2 = vfmul_vf_f32m4(erfd1_div_sqrt, 0.5f, vl);
				vfloat32m4_t erf1 = vfadd_vf_f32m4(erf_div_sqrt_div_2, 0.5f, vl);


				vfloat32m4_t d2_div_sqrt2 = vfdiv_vf_f32m4(d2, sqrt(2.0f), vl);

				vfloat32m4_t erfd2_div_sqrt = my_erf0(d2_div_sqrt2, vl);
				vfloat32m4_t erfd2_div_sqrt_div_2 = vfmul_vf_f32m4(erfd2_div_sqrt, 0.5f, vl);
				vfloat32m4_t erf2 = vfadd_vf_f32m4(erfd2_div_sqrt_div_2, 0.5f, vl);



				vfloat32m4_t Time = vle_v_f32m4(pT, vl);
				vfloat32m4_t Time_mult_int_rest = vfmul_vf_f32m4(Time, Interest_rest, vl);

				vfloat32m4_t exp_Time_mult_int_rest = my_exp0(Time_mult_int_rest, vl);

				vfloat32m4_t one_vect = vfmv_v_f_f32m4(1.0f, vl);
				vfloat32m4_t minus_exp = vfdiv_vv_f32m4(one_vect, exp_Time_mult_int_rest, vl);

				vfloat32m4_t exp_etc_mult_erf2 = vfmul_vv_f32m4(minus_exp, erf2, vl); 
				vfloat32m4_t exp_etc_mult_erf2_mult_k = vfmul_vv_f32m4(exp_etc_mult_erf2, k, vl);
				vfloat32m4_t so_mult_erf1 = vfmul_vv_f32m4(so, erf1, vl);
				vfloat32m4_t res = vfsub_vv_f32m4(so_mult_erf1, exp_etc_mult_erf2_mult_k, vl);
				vse_v_f32m4(pC, res, vl);

				//vse_v_f32m4(pC, so, vl);

				pS0 += vl;
				pK += vl;
				pT += vl;
				pC += vl;
				block_start += vl;


			}


		}

	}
	/*    for (int i=0;i<n;i++){
	 std::cout<< C[i]<<" ";
   }*/
}