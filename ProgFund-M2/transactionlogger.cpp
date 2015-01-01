/*************************************
Program: transactionlogger.cpp
Course:  Programming Fundamentals
Year:    2014/10 Trimester 1
Name:    LEE ZHEN YONG
ID:      1122702848
Lecture: TC01
Lab:     TT01
Email:   bruceoutdoors@gmail.com
Phone:   016-3188854
*************************************/

#include "transactionlogger.h"
#include <fstream>

TransactionLogger::TransactionLogger(const string &dir) : fileDir(dir)
{
	bool firstTime = false;

	// create file if it doesn't exist
	ifstream file(dir.c_str());
	if (!file.good()) {
		ofstream file(dir.c_str());
		firstTime = true;
	}

	csvdat = new Csv(dir);

	if (firstTime) writeHeaderRow();
}


TransactionLogger::~TransactionLogger()
{
	delete csvdat;
}


void TransactionLogger::clearLog()
{
	csvdat->deleteEverything();
}


void TransactionLogger::logTransaction(const Transaction &trans)
{
	Row row;
	row.push_back(numToString(csvdat->getRowCount()));
	row.push_back(trans.drink.name);
	row.push_back(numToString(trans.drink.cost));
	row.push_back(numToString(trans.numCans));
	row.push_back(numToString(trans.paidAmount));
	row.push_back(numToString(trans.changeDispensed));

	for (size_t i = 0; i < CashContainer::SIZE; i++) {
		row.push_back(numToString(trans.moneyLeft[i]));
	}

	csvdat->writeAppendRow(row);
}

void TransactionLogger::writeHeaderRow()
{
	Row row;
	row.push_back("Id");
	row.push_back("Name");
	row.push_back("Unit Price");
	row.push_back("Quantity");
	row.push_back("Paid Amount");
	row.push_back("Change Dispensed");
	row.push_back("RM20 left");
	row.push_back("RM10 left");
	row.push_back("RM5 left");
	row.push_back("RM1 left");
	row.push_back("RM0.5 left");
	row.push_back("RM0.2 left");
	row.push_back("RM0.1 left");
	csvdat->writeAppendRow(row);
}
