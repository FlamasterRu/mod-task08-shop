#include "task.h"
#include <cmath>

Customer::Customer()
{
	t_start = std::chrono::system_clock::now();
}

int Customer::TimeQueue()
{
	std::chrono::duration<double> duration = std::chrono::system_clock::now() - t_start;
	return duration.count();
}

double fact(int N)
{
	if (N < 0) // ���� ������������ ���� ������������� �����
		return 0; // ���������� ����
	if (N == 0) // ���� ������������ ���� ����,
		return 1; // ���������� ��������� �� ���� - �� �����������, �� ��� 1 =)
	else // �� ���� ��������� �������
		return N * fact(N - 1); // ������ ��������.
}

Shop::Shop(int numThr, int numCustomer, int timeShop, int avNumBuy, int maxLenQueue)
{
	this->numThr = numThr;
	this->numCustomer = numCustomer;
	this->timeShop = timeShop;
	this->maxLenQueue = maxLenQueue;
	this->avNumBuy = avNumBuy;
	for (int i = 0; i < numThr; ++i)
	{
		threads.push_back(thread(&Shop::Work, this));
		threads.at(i).detach();
	}
}

void Shop::Start()
{
	srand(time(0));
	while (numAll < numCustomer)
	{
		int r = rand() % 1000;
		avTimeCust += r;
		this_thread::sleep_for(std::chrono::milliseconds(r));	// �������� �������� � �����
		m.lock();
		if (lenQueue > maxLenQueue)
		{
			//cout << "ignored" << endl;
			++numIgnored;
		}
		else
		{
			//cout << "in" << endl;
			cus.push_back(Customer());
			++lenQueue;
		}
		++numAll;
		m.unlock();
	}
	while (numIgnored + numSuccess < numCustomer)
	{
		this_thread::sleep_for(std::chrono::milliseconds(10));	// ���, ���� �������� ���������� ������
	}
	avTimeWaitQueue /= static_cast<double>(numSuccess);
	avTimeBuy /= static_cast<double>(numSuccess);
	avTimeWait /= static_cast<double>(numThr);
	avTimeWork /= static_cast<double>(numThr);
	cout << "����������� ����������: " << numSuccess << endl;
	cout << "������������� ����������: " << numIgnored << endl;
	cout << "������� ����� ���������� � �������: " << avTimeWaitQueue << endl;
	cout << "������� ����� �� �����: " << avTimeBuy << endl;
	cout << "������� ����� ������ �����: " << avTimeWork << endl;
	cout << "������� ����� ������� �����: " << avTimeWait << endl;
	double lambda = 1000. / avTimeCust;	// ��� ��� ������������
	double nu = 1000. / avTimeBuy;
	double ro = lambda / nu;
	double P0 = 1.;
	for (int i = 1; i <= numThr; ++i)
	{
		P0 += pow(ro,i) / fact(i);
	}
	for (int i = numThr + 1; i < numThr + maxLenQueue; ++i)
	{
		P0 += pow(ro, i) / (fact(numThr) * pow(numThr, i - numThr));
	}
	P0 = 1. / P0;
	double Prej = pow(ro, numThr + maxLenQueue) * P0 / (pow(numThr, maxLenQueue) * fact(numThr));
	cout << "����������� ������ �������������: " << Prej << endl;
	cout << "����������� ������ �����������: " << static_cast<double>(numIgnored) / static_cast<double>(numCustomer) << endl;
	double Q = 1. - Prej;
	cout << "������������� ���������� ����������� �������������: " << Q << endl;
	cout << "������������� ���������� ����������� �����������: " << static_cast<double>(numSuccess) / static_cast<double>(numCustomer) << endl;
	double A = lambda * Q;
	cout << "���������� ���������� ����������� �������������: " << A << endl;
	cout << "���������� ���������� ����������� �����������: " << static_cast<double>(numSuccess) * avTimeCust / static_cast<double>(numCustomer) << endl;
}

void Shop::Work()
{
	srand(time(0));
	while (numIgnored + numSuccess < numCustomer)
	{
		m.lock();
		if (lenQueue > 0)
		{
			//cout << "start" << endl;
			--lenQueue;
			avTimeWaitQueue += cus.front().TimeQueue();
			m.unlock();
		}
		else
		{
			avTimeWait += 10;
			m.unlock();
			this_thread::sleep_for(std::chrono::milliseconds(10));	// ����� �������� ������ 10 �����������
			continue;
		}
		// ������ ������������
		int numBuy = rand() % avNumBuy + avNumBuy;
		for (int i = 0; i < numBuy; ++i)
		{
			this_thread::sleep_for(std::chrono::milliseconds(timeShop));
		}
		m.lock();
		avTimeWork += numBuy * timeShop;
		avTimeBuy += numBuy * timeShop;
		//cout << "end" << endl;
		cout << numSuccess + numIgnored + 1 << endl;
		++numSuccess;
		m.unlock();
	}
}




