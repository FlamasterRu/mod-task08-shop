#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <ctime>
#include <chrono>
#include <mutex>

using namespace std;

class Customer
{
public:
	std::chrono::system_clock::time_point t_start;
	int timeShop = 0;
	int timeQueue = 0;

	Customer();
	int TimeQueue();
};

class Shop
{
public:
	int numThr;	// количество касс
	int numCustomer;	// количество покупателей
	int timeShop;	// время обслуживания в миллисекундах
	int avNumBuy;	// среднее количество покупок
	int maxLenQueue;	// длина очереди максимальная

	int numIgnored = 0;	// количество необслуженных покупателей
	int numSuccess = 0;	// количество обслуженных покупателей
	int numAll = 0;	// количество поставленных в очередь
	int avLenQueue = 0;	// средняя длина очереди
	double avTimeWaitQueue = 0;	// среднее время ожидания в очереди
	double avTimeBuy = 0;	// среднее время обслуживания на кассе
	double avTimeWork = 0;	// среднее время работы кассы
	double avTimeWait = 0;	// среднее время простоя кассы
	double avTimeCust = 0;	// среднне время появления нового покупателя в очереди
	vector<thread> threads;
	int lenQueue = 0;	// длина очереди
	vector<Customer> cus;
	mutex m;


	Shop(int numThr, int numCustomer, int timeShop, int avNumBuy, int maxLenQueue);
	void Start();
	void Work();
};


