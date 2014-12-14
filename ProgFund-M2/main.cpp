#include <iostream>
#include <cstdlib>

#include "csv.h"
#include "drinksstock.h"
#include "cashcontainer.h"
#include "vendingmachine.h"

using namespace std;

int main()
{
	// setup float precision:
	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(2);

	try {
		/*Csv csv("data/ip.csv");
		cout << csv[3][1] << endl;
		csv[3][1] = "OMG I'm so cool";
		cout << csv[3][1] << endl;
		csv.save();
		csv.deleteEverything();*/

		//csv.writeAppendRow({"hello", "new", "row"});
		/*DrinksStock d("data/drinks-stock.csv");
		d[0].quantity++;
		d[0].name = "Lychee Supremo";
		d.save();*/
		/*CashContainer *container = new CashContainer();
		container->set(0, 3);
		container->set(1, 5);
		container->set(2, 6);
		container->set(4, 1);
		container->set(6, 15);
		container->save("data/cashcontainer");*/

		/*CashContainer *container = new CashContainer("data/money");
		int p[] = { 0, 1, 2, 3, 4, 5, 6 };
		int q[] = { 0, 10, 21, 7, 12, 5, 6 };
		CashContainer c1(p);
		CashContainer c2(q);
		c2.getTotal();
		CashContainer c3 = c1 - c2;

		bool adaChangeKah = c2.hasChange(c1);
		delete container;*/

		//CashContainer c(115.86001f);
		string logFile = "data/transaction-log.csv";
		string moneyFile = "data/money";
		string stockFile = "data/drinks-stock.csv";
		VendingMachine machine(moneyFile, stockFile, logFile);

		/*char input;
		cout << "Enter 'a' to access admin panel, other inputs starts the vending machine" << endl << " >> ";
		cin >> input;

		if (input == 'a') machine.launchAdminPanel();
		
		system("CLS");*/
		machine.run();

	} catch (exception e) {
		cout << "ERROR: " << e.what() << endl;
	}
	

	return 0;
}
