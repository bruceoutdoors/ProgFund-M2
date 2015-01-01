/*************************************
Program: transactionlogger.h
Course:  Programming Fundamentals
Year:    2014/10 Trimester 1
Name:    LEE ZHEN YONG
ID:      1122702848
Lecture: TC01
Lab:     TT01
Email:   bruceoutdoors@gmail.com
Phone:   016-3188854
*************************************/



#pragma once

#include <string>
#include "csv.h"
#include "drinksstock.h"
#include "cashcontainer.h"
#include "stringnum.h"

using namespace std;

struct Transaction
{
	Drink drink;
	int numCans;
	float paidAmount;
	float changeDispensed;
	CashContainer moneyLeft;
};

class TransactionLogger
{
public:
	TransactionLogger(const string &dir);
	~TransactionLogger();

	void logTransaction(const Transaction &trans);
	void clearLog();

private:
	void writeHeaderRow();

	string fileDir;
	Csv *csvdat;
};


