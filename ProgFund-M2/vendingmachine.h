/*************************************
Program: vendingmachine.h
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
