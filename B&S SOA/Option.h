#pragma once
#include <cmath>
#include <random>
#include <omp.h>
#include <iostream>
class Option
{
private:
	int N;
	float* T;//время
	float* S0;//начальная цена акции
	float* K;// цена исполнения или страйк
	float* C;//цена опциона
public:

	const float Volatility = 0.2f;
	const float Interest_rest = 0.13f;
	Option()
	{
		N = 10;
		T = new float[N];
		K = new float[N];
		C = new float[N];
		S0 = new float[N];
#pragma omp parallel 
		{
#pragma omp for simd
			for (int i = 0; i < N; i++)
			{
				T[i] = 0.0f;
				K[i] = 0.0f;
				C[i] = 0.0f;
				S0[i] = 0.0f;
			}
		}

	}
	~Option()
	{
		delete[] T;
		delete[] K;
		delete[] C;
		delete[] S0;
	}
	Option(int _N)
	{
		N = _N;
		T = new float[N];
		K = new float[N];
		C = new float[N];
		S0 = new float[N];
#pragma omp parallel
		{
#pragma omp for simd
			for (int i = 0; i < N; i++)
			{
				T[i] = 0.0f;
				K[i] = 0.0f;
				C[i] = 0.0f;
				S0[i] = 0.0f;
			}
		}
	}
	void random_datas()
	{
		
		//std::default_random_engine rd(0);//генератор случайных чисел
		//std::uniform_real_distribution<float> dist1(10.0f, 100.0f);
		//std::uniform_real_distribution<float> dist2(5.0f, 30.0f);
#pragma omp parallel
		{
			std::default_random_engine rd(0);//генератор случайных чисел
			std::uniform_real_distribution<float> dist1(10.0f, 100.0f);
			std::uniform_real_distribution<float> dist2(5.0f, 30.0f);
#pragma omp for simd
			for (int i = 0; i < N; i++)
			{

				S0[i] = dist1(rd);
				K[i] = S0[i] + dist2(rd);
				T[i] = dist2(rd);


			}
		}
	}
	void Get_option_price();
	float Get_out_price()
	{
		float price = 0.0f;
#pragma omp parallel 
		{
#pragma omp for simd reduction(+:price)//потоки мешать не будут, каждый поток работает со своей копией price
		
			for (int i = 0; i < N; i++)
			{
             /*  #pragma omp atomic*/
				price += C[i];
			}
		}
	
		return price;



	}



};
