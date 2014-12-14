#pragma once

#include <string>

class CashContainer;
class DrinksStock;

using namespace std;

class AdminPanel
{
public:
	AdminPanel(CashContainer *money, DrinksStock *drinks);
	~AdminPanel();

	void run();

private:
	void mainMenu();
	void setupDrinks(const string &change = "");
	void setMaxCans();
	void setInitMoney();

	CashContainer *money;
	DrinksStock *drinks;
	int maxCans;
};

