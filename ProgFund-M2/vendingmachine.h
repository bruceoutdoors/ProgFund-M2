#pragma once

#include <string>
#include "cashcontainer.h"
#include "drinksstock.h"

using namespace std;

class VendingMachine
{
public:
	VendingMachine(string moneyFile, string stockFile, string logFile);
	virtual ~VendingMachine();
	void run();

private:
	void printMenu();
	string addPadding(const string &str, size_t padding);
	int getUserChoice();
	void purchaseMenu(float amount = 0);
	void itemAmountDisplay(float amount);
	bool changeDispense();

	CashContainer inputAmount;
	CashContainer *money;
	DrinksStock *drinks;
	int choice;
};
