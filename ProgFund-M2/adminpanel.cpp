#include "adminpanel.h"
#include "cashcontainer.h"
#include "drinksstock.h"
#include "transactionlogger.h"
#include "padder.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <limits.h> // INT_MAX
#include <stdlib.h> // atoi
#include <ctype.h> // isdigit

const string settingsdir = "data/settings";
const string passworddir = "data/password";

AdminPanel::AdminPanel(CashContainer *money,
	DrinksStock *drinks,
	TransactionLogger *logger)
{
	this->money = money;
	this->drinks = drinks;
	this->logger = logger;

	ifstream file(settingsdir.c_str());
	if (file.good()) file >> maxCans;
	else maxCans = 5;
}


AdminPanel::~AdminPanel()
{
}

void AdminPanel::run()
{
	askForPassword();
	mainMenu();
}

void AdminPanel::mainMenu()
{
	system("CLS");
	char choice;
	cout << "** Welcome to adminpanel!! **" << endl
		<< endl
		<< "Select from the following options: " << endl
		<< endl
		<< "a) set the names, prices and the initial units in the of drink" << endl
		<< "b) set the max number of cans (now " << maxCans << ") which can be bought per transaction." << endl
		<< "c) set the initial amount in the machine for each denomination" << endl
		<< "d) clear transaction log to an empty state" << endl
		<< "e) change the admin password." << endl
		<< "f) return." << endl
		<< endl
		<< "Select (a to e) => ";
	cin >> choice;

	// NOTE: we can't use numeric_limits<streamsize>::max() because it clashes
	// with the min max macros from <windows.h>
	cin.ignore(INT_MAX, '\n');

	switch (choice) {
	case 'a':
		setupDrinks();
		break;
	case 'b':
		setMaxCans();
		break;
	case 'c':
		setInitMoney();
		break;
	case 'd':
		clearTransactionLog();
		break;
	case 'e':
		setPassword();
		break;
	default: return;
	}

	mainMenu();
}

void AdminPanel::setupDrinks()
{
	system("CLS");

	bool canAddDrink = false;

	if (drinks->getDrinkCount() < 10) canAddDrink = true;

	int dashlen = 60;
	cout << "Setup drinks stock:" << endl << endl;
	cout << string(dashlen, '-') << endl;
	cout << "| id | Name                     | Cost          | Quantity |" << endl;
	cout << string(dashlen, '-') << endl;
	for (size_t i = 0; i < drinks->getDrinkCount(); i++) {
		cout << "| " << i << "  | " << addPadding((*drinks)[i].name, 25) << "| "
			<< "RM" << (*drinks)[i].cost << "\t| "
			<< (*drinks)[i].quantity << "\t   |\n";
	}
	cout << string(dashlen, '-') << endl
		<< endl
		<< "Select the drink by id (0-" << drinks->getDrinkCount() - 1 << ")." << endl;

	if (canAddDrink) cout << "To add a drink, enter 'a' (Max 10 drinks)." << endl;

	cout << "To return back to main menu, enter 'b'." << endl
		<< "=> ";

	char input;
	cin >> input;
	cin.ignore(INT_MAX, '\n');

	if (!isdigit(input)) {
		if (input == 'a' && canAddDrink) {
			addNewDrink();
			setupDrinks();
		} else return;
	} 

	int id = atoi(&input);
	if (id < 0 || id >= (int)drinks->getDrinkCount()) return;

	string name = (*drinks)[id].name;
	cout << endl
		<< "What would like to change in " << name << "?" << endl
		<< "a) name" << endl
		<< "b) cost" << endl
		<< "c) quantity" << endl
		<< "d) remove drink" << endl
		<< endl
		<< "Select (a-d), other to cancel => ";

	char choice2;
	bool isSuccess = true;
	cin >> choice2;
	cin.ignore(INT_MAX, '\n');

	cout << endl;

	switch (choice2) {
	case 'a':
		cout << "Enter new name for " << name << ": " << endl
			<< "=> ";
		getline(cin, (*drinks)[id].name);
		break;
	case 'b':
		cout << "Enter new cost for " << name << ": " << endl
			<< "=> ";
		if (!(cin >> (*drinks)[id].cost)) isSuccess = false;
		break;
	case 'c':
		cout << "Enter new quantity for " << name << ": " << endl
			<< "=> ";
		if (!(cin >> (*drinks)[id].quantity)) isSuccess = false;
		break; 
	case 'd':
		cout << "Are you sure you want to remove " << name << "? (y/n)" << endl
			<< "=> ";
		cin >> input;
		cin.ignore(INT_MAX, '\n');
		if (input == 'y') drinks->removeDrink(id);
		else isSuccess = false;
		break;
	default:
		setupDrinks();
	}

	if (!isSuccess) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		setupDrinks();
		return;
	}
	
	drinks->save();

	cout << endl << "** Changes have successfully been saved!" << endl;
	Sleep(1700);
	setupDrinks();
}

void AdminPanel::setMaxCans()
{
	cout << endl
		<< "set new max number of cans per transaction (max is 5)" << endl
		<< "Invalid values cancels the operation." << endl
		<< "=> ";
	int newmax;
	if (!(cin >> newmax)) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		return;
	}

	if (newmax < 1 || newmax > 5) return;

	maxCans = newmax;
	ofstream file(settingsdir.c_str(), ofstream::trunc);
	file << maxCans;

	cout << endl << "Success! Max cans has been set to "
		<< newmax << ".";
	Sleep(2000);
}

void AdminPanel::setInitMoney()
{
	system("CLS");
	const int dashlen = 25;
	cout << "Set the initial amount for each denomination: " << endl
		<< endl
		<< string(dashlen, '-') << endl
	    << "| id | denom    | val   |" << endl
	    << string(dashlen, '-') << endl;

	for (size_t i = 0; i < CashContainer::SIZE; i++) {
		cout << "| " << i << "  | RM" << money->getValue(i)
			 << "\t| " << (*money)[i] << "\t|" << endl
		     << string(dashlen, '-') << endl;
	}

	cout << endl << "Select by id (0-" << CashContainer::SIZE-1
		<< "), invalid numbers to go back" << endl
		<< "=> ";
	int choice;

	if (!(cin >> choice)) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		return;
	}

	if (choice < 0 || choice > (int)CashContainer::SIZE - 1) return;

	int newval;
	cout << endl << "Changing value of denomination RM"
		<< money->getValue(choice)
		<< " (negative values cancels operation)." << endl
		<< endl
		<< "Enter new value => ";
	cin >> newval;

	if (!(cin >> newval)) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		return;
	}

	if (newval < 0) return;

	money->set(choice, newval);
	money->save();
	setInitMoney();
}

void AdminPanel::clearTransactionLog()
{
	cout << endl
		<< "This will effectively wipe out all your transaction log data." << endl
		<< "Are you sure you want to do this? (y/n) " << endl
		<< " => ";
	char choice;
	cin >> choice;
	cin.ignore(INT_MAX, '\n');

	if (choice == 'y') {
		logger->clearLog();
		// display success message for 2 seconds
		cout << endl << "Success! All data in log file has been wiped out.";
		Sleep(2000);
	}
}

bool AdminPanel::askForPassword()
{
	// if file doesn't exist you get a free pass!
	ifstream file(passworddir.c_str());
	if (!file.good()) return true;

	string password, input;
	file >> password;

	cout << "Enter admin password to proceed: " << endl
		<< "=> ";


	while (true) {
		cin >> input;
		cin.ignore(INT_MAX, '\n');
		if (input == password) return true;
		cout << "Wrong!! Try again: ";
	}
}

void AdminPanel::setPassword()
{
	string password;
	cout << endl << endl
		<< "Set new password (spaces are not allowed): ";

	cin >> password;
	cin.ignore(INT_MAX, '\n');

	ofstream newfile(passworddir.c_str(), ofstream::trunc);
	newfile << password;

	cout << endl << "Success! Password has been set as \""
		<< password << "\"";
	Sleep(2000);
}

void AdminPanel::addNewDrink()
{
	cout << endl;
	Drink newdrink;
	cout << "Enter new name for new drink: " << endl
		<< "=> ";
	getline(cin, newdrink.name);
	cout << endl << "Enter new cost for " << newdrink.name << ": " << endl
		<< "=> ";
	if (!(cin >> newdrink.cost)) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		return;
	}
	cout << endl << "Enter quantity for " << newdrink.name << ": " << endl
		<< "=> ";
	if (!(cin >> newdrink.quantity)) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		return;
	}
	drinks->addDrink(newdrink);

	drinks->save();

	cout << endl << "** Successfully added a new drink: \"" 
		<< newdrink.name << "\"!" << endl;
	Sleep(1700);
}
