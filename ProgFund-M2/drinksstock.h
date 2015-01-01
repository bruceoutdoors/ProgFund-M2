/*************************************
Program: drinksstock.h
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

