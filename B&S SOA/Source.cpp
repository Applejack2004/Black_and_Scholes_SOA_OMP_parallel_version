#include<iostream>
#include"Option.h"
#include <chrono>
int N = 0;;
float price = 0.0f;


int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "RUS");
    if (argc == 1)
    {
	   std::cout << "no arguments!" << std::endl;
	}
	else
	{
		for (int i = 0; i < argc; i++)
		{
			std::cout << argv[i] << std::endl;
		}
		N = std::atoi(argv[1]);
	}
	const char* num_threads_str = std::getenv("OMP_NUM_THREADS");//возвращает указатель на переменную окружения.
	int num_threads;
	if (num_threads_str != nullptr) {
		num_threads = std::atoi(num_threads_str);
		std::cout << "Значение переменной окружения OMP_NUM_THREADS: " << num_threads << std::endl;
	}
	else {
		std::cout << "Переменная окружения OMP_NUM_THREADS не установлена" << std::endl;
	}
	Option Obj(N);
	Obj.random_datas();
	const std::chrono::time_point<std::chrono::system_clock> t1 = std::chrono::system_clock::now();
	Obj.Get_option_price();
	const std::chrono::time_point<std::chrono::system_clock> t2 = std::chrono::system_clock::now();
	auto time = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << " Цена опциона вычислена!" << std::endl;
	price = Obj.Get_out_price();
	std::cout << "Справедливая цена опциона = " << price << "$" << std::endl;
	std::cout << "На вычисление затрачено:" << time << " ms" << std::endl;
	
}