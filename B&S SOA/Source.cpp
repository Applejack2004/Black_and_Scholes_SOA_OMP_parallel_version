#include<iostream>
#include"Option.h"
#include <chrono>
int N = 0;;
float price = 0.0f;


int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		std::cout << "no arguments!" << std::endl;
	}
	else
	{
		for (int i = 0; i < argc; i++)
		{
			std::cout << argv[i]<<" ";
		}
		N = std::atoi(argv[1]);
	}
	/*std::cout << "Enter the number of shares:" << std::endl;
	std::cin >> N;*/
	Option Obj(N);
	Obj.random_datas();
	const std::chrono::time_point<std::chrono::system_clock> t1 = std::chrono::system_clock::now();
	Obj.Get_option_price();
	const std::chrono::time_point<std::chrono::system_clock> t2 = std::chrono::system_clock::now();
	auto time = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << " The option price has been calculated!" << std::endl;
	price = Obj.Get_out_price();
	std::cout << "Fair option price =" << price << "$"<<std::endl;
	std::cout << "Elapsed time:" << time << std::endl;

}