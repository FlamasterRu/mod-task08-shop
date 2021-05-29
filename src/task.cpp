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
	if (N < 0) // если пользователь ввел отрицательное число
		return 0; // возвращаем ноль
	if (N == 0) // если пользователь ввел ноль,
		return 1; // возвращаем факториал от нул€ - не удивл€етесь, но это 1 =)
	else // ¬о всех остальных случа€х
		return N * fact(N - 1); // делаем рекурсию.
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
		this_thread::sleep_for(std::chrono::milliseconds(r));	// случайно подходит к кассе
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
		this_thread::sleep_for(std::chrono::milliseconds(10));	// ждЄт, пока работают оставшиес€ потоки
	}
	avTimeWaitQueue /= static_cast<double>(numSuccess);
	avTimeBuy /= static_cast<double>(numSuccess);
	avTimeWait /= static_cast<double>(numThr);
	avTimeWork /= static_cast<double>(numThr);
	cout << "ќбслуженные покупатели: " << numSuccess << endl;
	cout << "Ќеобслуженные покупатели: " << numIgnored << endl;
	cout << "—реднее врем€ нахождени€ в очереди: " << avTimeWaitQueue << endl;
	cout << "—реднее врем€ на кассе: " << avTimeBuy << endl;
	cout << "—реднее врем€ работы кассы: " << avTimeWork << endl;
	cout << "—реднее врем€ просто€ кассы: " << avTimeWait << endl;
	double lambda = 1000. / avTimeCust;	// так как миллисекунды
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
	cout << "¬еро€тность отказа теоретическа€: " << Prej << endl;
	cout << "¬еро€тность отказа фактическа€: " << static_cast<double>(numIgnored) / static_cast<double>(numCustomer) << endl;
	double Q = 1. - Prej;
	cout << "ќтносительна€ пропускна€ способность теоретическа€: " << Q << endl;
	cout << "ќтносительна€ пропускна€ способность фактическа€: " << static_cast<double>(numSuccess) / static_cast<double>(numCustomer) << endl;
	double A = lambda * Q;
	cout << "јбсолютна€ пропускна€ способность теоретическа€: " << A << endl;
	cout << "јбсолютна€ пропускна€ способность фактическа€: " << static_cast<double>(numSuccess) * avTimeCust / static_cast<double>(numCustomer) << endl;
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
			this_thread::sleep_for(std::chrono::milliseconds(10));	// чтобы провер€л каждые 10 миллисекунд
			continue;
		}
		// начали обслуживание
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




