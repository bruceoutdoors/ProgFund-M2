#pragma once

#include <vector>
#include "csv.h"

using namespace std;

struct Drink {
	string name;
	float cost;
	int quantity;
};

class DrinksStock
{
public:
	DrinksStock(const string dir);
	~DrinksStock();
	void save();
	void addDrink(Drink drink);
	void removeDrink(size_t index);
	// Drink &get(size_t index) { return drinks[index]; }
	bool hasSoldOut(size_t index) { return drinks[index].quantity <= 0; }
	Drink &operator[](size_t index);

	size_t getDrinkCount() const { return drinks.size(); }

private:
	Table convertToTable();
	void readCsv(Csv *csvdat);
	vector<Drink> drinks;
	string fileDir;
	Csv *csvdat;
};

