/*************************************
Program: adminpanel.h
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
	void addNewDrink();
	void setupDrinks();
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

