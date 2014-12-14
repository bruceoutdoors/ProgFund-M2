#include "adminpanel.h"
#include "cashcontainer.h"
#include "drinksstock.h"
#include "padder.h"
#include <cstdlib>
#include <iostream>
#include <fstream>

const string settingsdir = "data/settings";

AdminPanel::AdminPanel(CashContainer *money, DrinksStock *drinks)
{
	this->money = money;
	this->drinks = drinks;

	ifstream file(settingsdir);
	if (file.good()) file >> maxCans;
	else maxCans = 5;
}


AdminPanel::~AdminPanel()
{
}

void AdminPanel::run()
{
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
	//choice = 'c';
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
		break;
	case 'e': 
		break;
	default: return;
	}

	mainMenu();
}

void AdminPanel::setupDrinks(const string &change)
{
	system("CLS");

	if (change != "") cout << "** " << change << endl << endl;

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
		<< "Select the drink by id (0-" << drinks->getDrinkCount()-1 << ")." << endl
		<< "To return back to main menu, type invalid numbers" << endl
		<< "=> ";

	int id;
	cin >> id;
	if (id < 0 || id >= (int)drinks->getDrinkCount()) return;

	string name = (*drinks)[id].name;
	cout << endl 
		<< "What would like to change in " << name << "?" << endl
		<< "a) name" << endl
		<< "b) cost" << endl
		<< "c) quantity" << endl
		<< endl
		<< "Select (a-c), other to cancel => ";

	char choice2;
	cin >> choice2;

	cout << endl;

	switch (choice2) {
	case 'a':
		cout << "Enter new name for " << name << ": " << endl
			<< "=> ";
		cin >> (*drinks)[id].name;
		break;
	case 'b':
		cout << "Enter new cost for " << name << ": " << endl
			<< "=> ";
		cin >> (*drinks)[id].cost;
		break;
	case 'c':
		cout << "Enter new quantity for " << name << ": " << endl
			<< "=> ";
		cin >> (*drinks)[id].quantity;
		break;
	default:
		setupDrinks();
	}

	drinks->save();
	setupDrinks("Changes have successfully been saved!");
}

void AdminPanel::setMaxCans()
{
	cout << endl
		<< "set new max number of cans per transaction (max is 5)" << endl
		<< "Invalid values cancels the operation." << endl
		<< "=> ";
	int newmax;
	cin >> newmax;
	if (newmax < 0 || newmax > 5) {
		return;
	}
	
	maxCans = newmax;
	ofstream file(settingsdir, ofstream::trunc);
	file << maxCans;
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
	cin >> choice;

	if (choice < 0 || choice > CashContainer::SIZE - 1) return;

	int newval;
	cout << endl << "Changing value of denomination RM" 
		<< money->getValue(choice) 
		<< " (negative values cancels operation)." << endl 
		<< endl
		<< "Enter new value => ";
	cin >> newval;

	if (newval < 0) return;

	money->set(choice, newval);
	money->save();
	setInitMoney();
}
