/*************************************
Program: main.cpp
Course:  Programming Fundamentals
Year:    2014/10 Trimester 1
Name:    LEE ZHEN YONG
ID:      1122702848
Lecture: TC01
Lab:     TT01
Email:   bruceoutdoors@gmail.com
Phone:   016-3188854
*************************************/

#include <iostream>

#include "vendingmachine.h"

using namespace std;

int main()
{
	// setup float precision:
	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(2);

	try {
		string logFile = "data/transaction-log.csv";
		string moneyFile = "data/money";
		string stockFile = "data/drinks-stock.csv";
		VendingMachine machine(moneyFile, stockFile, logFile);

		machine.run();

	} catch (exception &e) {
		cout << "ERROR: " << e.what() << endl;
		cin.get();
	}


	return 0;
}
