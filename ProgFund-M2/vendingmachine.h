#pragma once

#include <string>
#include "cashcontainer.h"
#include "drinksstock.h"
#include "transactionlogger.h"

using namespace std;

class VendingMachine
{
public:
	VendingMachine(string moneyFile, string stockFile, string logFile);
	virtual ~VendingMachine();
	void run();
	void launchAdminPanel();

private:
	void printMenu();
	void getUserChoice();
	void getQuantity();
	void purchaseMenu(float amount = 0);
	void itemAmountDisplay(float amount);
	bool performTransaction();
	void readSettings();
	void logTransaction(const float change);

	CashContainer inputAmount;
	CashContainer *money;
	DrinksStock *drinks;
	TransactionLogger *logger;
	int choice;
	int maxCans;
	int quantity;
	float price;
};
