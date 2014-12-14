#include "vendingmachine.h"
#include "round.h"
#include <iostream>

#define A 97

VendingMachine::VendingMachine(string moneyFile, string stockFile, string logFile)
{
	money = new CashContainer(moneyFile);
	drinks = new DrinksStock(stockFile);
}

void VendingMachine::run()
{
	while (true) {
		printMenu();
		choice = getUserChoice();
		purchaseMenu();
		if (changeDispense()) {
			// successful transaction! accept money and decrement 
			// drink quantity
			*money += inputAmount;
			--(*drinks)[choice].quantity;

			// write changes:
			money->save();
			drinks->save();

			cout << endl << "There is your " << (*drinks)[choice].name << "!" << endl
				<< endl << "Would you like to buy another drink?";
		} else {
			cout << endl << "Would you like to try again?";
		}

		cout << "(y/n) => ";
		char input;
		cin >> input;
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

string VendingMachine::addPadding(const string &str, size_t padding)
{
	string result = str;
	int pads = padding - str.size();
	while (pads-- > 0) result.append(" ");

	return result;
}

int VendingMachine::getUserChoice()
{
	char idx = A;
	char input = A;
	cin >> input;

	for (size_t i = 0; i < drinks->getDrinkCount(); i++) {
		if (input == idx++) {
			if ((*drinks)[i].quantity <= 0) {
				cout << endl << "** " << (*drinks)[i].name
					<< " is sold out. ";
				break;
			}
			return i;
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

	for (int i = 0; i < CashContainer::SIZE; i++) {
		cout << menuIdx++ << ") RM" << CashContainer::getValue(i) << endl;
	}

	cout << endl << "Pay (a to " << --menuIdx << ") => ";

	char input;
	menuIdx = A;
	cin >> input;

	// add the input amount to the amount and temporary storage:
	for (int i = 0; i < CashContainer::SIZE; i++) {
		if (input == menuIdx++) {
			inputAmount.increment(i);
			break;
		}
	}

	// added a small value to amount to counter IEEE floating point
	// precision problem
	if (inputAmount.getTotal() < (*drinks)[choice].cost) {
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
	cout << addPadding("Purchase of " + (*drinks)[choice].name, 38)
	 	 << "RM" << (*drinks)[choice].cost 
		 << endl
	     << addPadding("Amount paid", 38)
	     << "RM" << amount << endl << endl;
}

/*
* Dispenses change. Returns true and dispenses the change if there
* is change, returns false and returns back the user's cash if there
* is not enough change. If an exact amount is given it just returns true.
*
* If change is dispensed, the money struct itself is modified.
*/
bool VendingMachine::changeDispense()
{
	system("CLS");
	itemAmountDisplay(inputAmount.getTotal());

	float difference = Round(inputAmount.getTotal() - (*drinks)[choice].cost);

	// return if exact amount immediately if it is 0
	if (difference == 0) return true;

	// calculate the required change
	CashContainer change(difference);

	// change is dispense if it can be dispensed,
	// otherwise the user's money is returned.
	if (money->hasChange(change)) {
		// display change:
		cout << "Change dispensed" << endl;
		for (int i = 0; i < CashContainer::SIZE; i++) {
			if (change[i] == 0) continue;
			cout << "RM" << CashContainer::getValue(i) << " x "
				<< change[i] << endl;
		}
		// dispense the change from the cash container
		*money -= change;
		return true;
	} else {
		cout << "Payment returned\t" << inputAmount.getTotal() << endl << endl;
		inputAmount.makeEmpty();

		for (int i = 0; i < CashContainer::SIZE; i++) {
			if (change[i] == 0) continue;

			cout << "Unable to dispense\tRM" << CashContainer::getValue(i) << endl;
		}
		return false;
	}
}
