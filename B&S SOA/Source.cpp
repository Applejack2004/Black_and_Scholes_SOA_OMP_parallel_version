#include<iostream>
#include"Option.h"
#include <chrono>
int N;
float price = 0.0f;


int main(int argc, char* argv[])
{
	//if (argc == 1)
	//{
	//	std::cout << "no arguments!" << std::endl;
	//}
	//else
	//{
	//	for (int i = 0; i < argc; i++)
	//	{
	//		std::cout << argv[i];
	//	}
	//	N = std::atoi(argv[1]);
	//}
	std::cout << "Enter the number of shares:" << std::endl;
	std::cin >> N;
	Option Obj(N);
	Obj.random_datas();
	auto start = std::chrono::high_resolution_clock::now();
	Obj.Get_option_price();
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float>elapsed = end - start;
	std::cout << " The option price has been calculated!" << std::endl;
	price = Obj.Get_out_price();
	std::cout << "Fair option price =" << price << "$"<<std::endl;
	std::cout << "Elapsed time:" << elapsed.count() << std::endl;

}