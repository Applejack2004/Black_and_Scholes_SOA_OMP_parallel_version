#pragma once
#include <cmath>
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
		for (int i = 0; i < N; i++)
		{
			T[i] = 0.0f;
			K[i] = 0.0f;
			C[i] = 0.0f;
			S0[i] = 0.0f;
		}

	}
	Option(int _N)
	{
		N = _N;
		T = new float[N];
		K = new float[N];
		C = new float[N];
		S0 = new float[N];
		for (int i = 0; i < N; i++)
		{
			T[i] = 0.0f;
			K[i] = 0.0f;
			C[i] = 0.0f;
			S0[i] = 0.0f;
		}
	}
	void random_datas()
	{
		for (int i = 0; i < N; i++)
		{
			S0[i] = 500.0f * std::rand() / (float)RAND_MAX;
			K[i] = 500.0f * std::rand() / (float)RAND_MAX;
			T[i] = 500.0f * std::rand() / (float)RAND_MAX;
			C[i] = 0.0f;
		}
	}
	void Get_option_price();
	float Get_out_price()
	{
		float price = 0.0f;
		for (int i = 0; i < N; i++)
			price += C[i];
		return price;



	}



};
