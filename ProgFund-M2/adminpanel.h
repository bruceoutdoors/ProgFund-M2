#pragma once

#include <string>

class CashContainer;
class DrinksStock;
class TransactionLogger;

using namespace std;

class AdminPanel
{
public:
	AdminPanel(CashContainer *money, DrinksStock *drinks, TransactionLogger *logger);
	~AdminPanel();

	void run();

private:
	void mainMenu();
	void setupDrinks(const string &change = "");
	void setMaxCans();
	void setInitMoney();
	void clearTransactionLog();
	void setPassword();
	bool askForPassword();

	CashContainer *money;
	DrinksStock *drinks;
	TransactionLogger *logger;
	int maxCans;
};

