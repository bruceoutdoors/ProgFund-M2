#include "vendingmachine.h"
#include "adminpanel.h"
#include "stringnum.h"
#include "padder.h"
#include "round.h"
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <fstream>
#include <limits>

#define A 97

VendingMachine::VendingMachine(string moneyFile, string stockFile, string logFile)
{
	money = new CashContainer(moneyFile);
	drinks = new DrinksStock(stockFile);
	logger = new TransactionLogger(logFile);
	readSettings();
}

void VendingMachine::run()
{
	while (true) {
		cout << ">> Enter '*' to access admin panel <<" << endl << endl;
		cout << "The vending machine welcomes you customer! Please enter a selection:" << endl << endl;
		printMenu();
		getUserChoice();
		getQuantity();
		price = Round((*drinks)[choice].cost * quantity);
		purchaseMenu();
		if (performTransaction()) {
			// successful transaction! and decrement drink quantity
			(*drinks)[choice].quantity -= quantity;

			// write changes to disk:
			drinks->save();

			cout << endl << "Here is " << quantity  << " " << (*drinks)[choice].name << "!" << endl
				<< endl << "Would you like to buy another drink?";
		} else {
			cout << endl << "Would you like to try again?";
		}

		cout << " (y/n) => ";
		char input;
		cin >> input;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		if (input != 'y') {
			cout << endl << "Well, see you soon!" << endl;
			break;
		}

		system("CLS");
	}
}

VendingMachine::~VendingMachine()
{
	delete money;
	delete drinks;
	delete logger;
}

void VendingMachine::printMenu()
{
	char idx = A;
	for (size_t i = 0; i < drinks->getDrinkCount(); i++) {
		cout << idx++ << ") "
			<< addPadding((*drinks)[i].name, 25);
		cout << "RM" << (*drinks)[i].cost;

		if (drinks->hasSoldOut(i)) { cout << " [Sold Out]"; }

		cout << endl;
	}

	cout << endl << "Purchase (a to " << --idx << ") => ";
}

void VendingMachine::getUserChoice()
{
	char idx = A;
	char input = A;
	cin >> input;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	if (input == '*') {
		launchAdminPanel();
		system("CLS");
		run(); // restart the vending machine
	}

	for (size_t i = 0; i < drinks->getDrinkCount(); i++) {
		if (input == idx++) {
			if ((*drinks)[i].quantity <= 0) {
				cout << endl << "** " << (*drinks)[i].name
					<< " is sold out. ";
				break;
			}
			choice = i;
			return;
		}
	}
	cout << "Invalid selection! Please try again: ";

	return getUserChoice();
}

/*
* Returns the amount paid, 0 if failed. Function recurses until the
* user has input the sufficient amount for the drink.
*
* This method also stores the input amount to the temp money storage
*/
void VendingMachine::purchaseMenu(float amount)
{
	system("CLS");

	itemAmountDisplay(amount);

	cout << "Insert" << endl;
	char menuIdx = A;

	for (size_t i = 0; i < CashContainer::SIZE; i++) {
		cout << menuIdx++ << ") RM" << CashContainer::getValue(i) << endl;
	}

	cout << endl << "Pay (a to " << --menuIdx << ") => ";

	char input;
	menuIdx = A;
	cin >> input;

	// add the input amount:
	for (size_t i = 0; i < CashContainer::SIZE; i++) {
		if (input == menuIdx++) {
			inputAmount.increment(i);
			break;
		}
	}

	if (inputAmount.getTotal() < price) {
		// function recurses until the required amount is reached
		return purchaseMenu(inputAmount.getTotal());
	}
}

/*
* Displays the item the user wishes to purchases and the
* amount that the user currently has.
*/
void VendingMachine::itemAmountDisplay(float amount)
{
	cout << addPadding("Purchase of " + (*drinks)[choice].name + " x" + numToString(quantity), 38)
		<< "RM" << price
		<< endl
		<< addPadding("Amount paid", 38)
		<< "RM" << amount << endl << endl;
}

/*
* Dispenses change. Returns true and dispenses the change if there
* is change, returns false and returns back the user's cash if there
* is not enough change. If an exact amount is given it just returns true.
*/
bool VendingMachine::performTransaction()
{
	system("CLS");
	itemAmountDisplay(inputAmount.getTotal());

	float difference = Round(inputAmount.getTotal() - price);

	// if it is exact amount there'll be no change
	if (difference == 0) {
		*money += inputAmount;
		money->save();
		logTransaction(0);
		return true;
	}

	CashContainer change;

	// change is dispense if it can be dispensed,
	// otherwise the user's money is returned.
	try {
		// attempt to calculate the required change
		change = money->calcChange(difference);

		// we're not passing the exception by reference cause we're not
		// doing anything with it; setting it as reference gives compiler
		// warnings. Set it to reference should you require to use the exception.
	} catch (runtime_error e) {
		cout << "I'm sorry, I do not have enough change..." << endl << endl;
		cout << "Payment returned\t" << inputAmount.getTotal() << endl << endl;
		inputAmount.makeEmpty();

		for (size_t i = 0; i < CashContainer::SIZE; i++) {
			// Any denomination smaller than the required change that has a quantity 
			// count of 0 is marked as change that is unable to be dispensed.
			if (difference >= CashContainer::getValue(i) && (*money)[i] == 0)
				cout << "Unable to dispense\tRM" << CashContainer::getValue(i) << endl;
		}
		return false;
	}

	// display change:
	cout << "Change dispensed" << endl;
	for (size_t i = 0; i < CashContainer::SIZE; i++) {
		if (change[i] == 0) continue;
		cout << "RM" << CashContainer::getValue(i) << " x "
			<< change[i] << endl;
	}

	// dispense the change and accept input money
	*money -= change;
	*money += inputAmount;
	money->save();

	logTransaction(change.getTotal());

	inputAmount.makeEmpty();
	return true;
}

void VendingMachine::launchAdminPanel()
{
	AdminPanel admin(money, drinks, logger);
	admin.run();
	readSettings();
}

void VendingMachine::readSettings()
{
	ifstream file("data/settings");
	if (file.good()) file >> maxCans;
	else maxCans = 5;
}

void VendingMachine::getQuantity()
{
	cout << endl << "Enter your desired quantity. Maximum is " << maxCans << ": ";

	if (!(cin >> quantity)) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		system("CLS");
		run(); // restart to main menu
	}

	if (quantity < 0) {
		cout << "Negative values are not accepted." << endl;
	} else if (quantity > maxCans) {
		cout << "You cannot purchase more than " << maxCans << " cans." << endl;
	} else if (quantity > (*drinks)[choice].quantity) {
		cout << "I'm sorry, we only have " << (*drinks)[choice].quantity << " left" << endl;
	} else return;

	getQuantity();
}

void VendingMachine::logTransaction(const float change)
{
	// log transaction:
	Transaction trans = {
		(*drinks)[choice],
		quantity,
		inputAmount.getTotal(),
		change,
		*money
	};

	logger->logTransaction(trans);
}
