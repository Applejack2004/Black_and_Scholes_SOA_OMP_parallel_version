#pragma once
#include <cmath>
#include <vector>
#include <random>
#include <omp.h>
#include <iostream>
class Option
{
private:
	int N;
	std::vector<float> T;//время
	std::vector<float> S0;//начальная цена акции
	std::vector<float> K;// цена исполнения или страйк
	std::vector<float> C;//цена опциона
public:

	const float Volatility = 0.2f;
	const float Interest_rest = 0.13f;
	Option()
	{
		N = 10;

#pragma omp parallel num_threads(12)
		{
#pragma omp for simd
			for (int i = 0; i < N; i++)
			{
				T.push_back(0.0f);
				K.push_back(0.0f);
				C.push_back(0.0f);
				S0.push_back(0.0f);
			}
		}

	}

	Option(int _N)
	{
		N = _N;
		T.reserve(N);
		K.reserve(N);
		C.reserve(N);
		S0.reserve(N);
#pragma omp parallel num_threads(12)
		{
#pragma omp for simd
			for (int i = 0; i < N; i++)
			{
				T.push_back(0.0f);
				K.push_back(0.0f);
				C.push_back(0.0f);
				S0.push_back(0.0f);
			}
		}


	}
	void random_datas()
	{
#pragma omp parallel num_threads(12)
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
#pragma omp parallel num_threads(12)
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
