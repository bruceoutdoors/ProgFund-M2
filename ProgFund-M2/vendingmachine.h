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
	void launchAdminPanel();

private:
	void printMenu();
	void getUserChoice();
	void getQuantity();
	void purchaseMenu(float amount = 0);
	void itemAmountDisplay(float amount);
	bool performTransaction();
	void readSettings();

	CashContainer inputAmount;
	CashContainer *money;
	DrinksStock *drinks;
	int choice;
	int maxCans;
	int quantity;
	float price;
};
