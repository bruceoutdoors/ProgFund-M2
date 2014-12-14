#include "transactionlogger.h"
#include <fstream>

TransactionLogger::TransactionLogger(const string &dir) : fileDir(dir)
{
	// create file if it doesn't exist
	ifstream file(dir);
	if (!file.good()) ofstream file(dir);

	csvdat = new Csv(dir);
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
	row.push_back(numToString(csvdat->getRowCount() + 1));
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