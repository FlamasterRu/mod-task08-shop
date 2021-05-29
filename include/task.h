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
	int numThr;	// ���������� ����
	int numCustomer;	// ���������� �����������
	int timeShop;	// ����� ������������ � �������������
	int avNumBuy;	// ������� ���������� �������
	int maxLenQueue;	// ����� ������� ������������

	int numIgnored = 0;	// ���������� ������������� �����������
	int numSuccess = 0;	// ���������� ����������� �����������
	int numAll = 0;	// ���������� ������������ � �������
	int avLenQueue = 0;	// ������� ����� �������
	double avTimeWaitQueue = 0;	// ������� ����� �������� � �������
	double avTimeBuy = 0;	// ������� ����� ������������ �� �����
	double avTimeWork = 0;	// ������� ����� ������ �����
	double avTimeWait = 0;	// ������� ����� ������� �����
	double avTimeCust = 0;	// ������� ����� ��������� ������ ���������� � �������
	vector<thread> threads;
	int lenQueue = 0;	// ����� �������
	vector<Customer> cus;
	mutex m;


	Shop(int numThr, int numCustomer, int timeShop, int avNumBuy, int maxLenQueue);
	void Start();
	void Work();
};


