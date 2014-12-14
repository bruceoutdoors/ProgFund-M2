#include "drinksstock.h"
#include "stringnum.h"
#include <sstream>

DrinksStock::DrinksStock(const string dir) : fileDir(dir)
{
	csvdat = new Csv(dir);
	readCsv(csvdat);
}


DrinksStock::~DrinksStock()
{
	delete csvdat;
}

void DrinksStock::readCsv(Csv *csvdat)
{
	for (size_t i = 0; i < csvdat->getRowCount(); i++) {
		Drink drink;
		drink.name = (*csvdat)[i][0];
		istringstream((*csvdat)[i][1]) >> drink.cost;
		istringstream((*csvdat)[i][2]) >> drink.quantity;

		drinks.push_back(drink);
	}
}

Drink & DrinksStock::operator[](size_t index)
{
	return drinks[index];
}

void DrinksStock::save()
{
	delete csvdat;
	csvdat = new Csv(convertToTable());
	csvdat->setDirectory(fileDir);
	csvdat->save();
}

Table DrinksStock::convertToTable()
{
	Table table;

	for (size_t i = 0; i < drinks.size(); i++) {
		Row row;
		row.push_back(drinks[i].name);
		row.push_back(numToString(drinks[i].cost));
		row.push_back(numToString(drinks[i].quantity));
		table.push_back(row);
	}

	return table;
}

void DrinksStock::addDrink(Drink drink)
{
	drinks.push_back(drink);
}

void DrinksStock::removeDrink(size_t index)
{
	drinks.erase(drinks.begin() + index);
}
