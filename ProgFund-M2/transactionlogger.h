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
	string fileDir;
	Csv *csvdat;
};


