#pragma once

/*
It should be assumed that the amount the vending machine takes is in
RM20, RM10, RM5, RM1, 50cents, 20cents, 10cents. This is fixed and
cannot be changed. Therefore we store change and transactions in a
int[7] array, where the index of array corresponds to the
quantity of each change in its respective order as listed above.
*/

#include <string>

using namespace std;

class CashContainer
{
public:
	static const size_t SIZE = 7;

	CashContainer();
	CashContainer(float amount);
	CashContainer(const int container[SIZE]);
	CashContainer(string dir);
	CashContainer(const CashContainer &c); // copy constructor
	CashContainer& operator=(const CashContainer& c); // assignment operator overload
	~CashContainer();

	static float getValue(int index);
	void updateTotal();
	float getTotal() { return total; }
	int &operator[](const size_t index);
	int operator[](const size_t index) const;
	CashContainer operator+(const CashContainer &c) const;
	CashContainer operator-(const CashContainer &c) const;
	CashContainer &operator-=(const CashContainer &c);
	CashContainer &operator+=(const CashContainer &c);
	bool hasChange(const CashContainer &other) const;
	CashContainer calcChange(float changeAmount) const;

	void makeEmpty();
	void set(size_t index, int value);
	void increment(size_t index, int value = 1);
	void save(string dir);
	void save();
	bool hasFile() const { return isFile; }


private:
	static void checkRange(int index);

	static const float values[SIZE];
	int container[SIZE];
	bool isFile;
	string fileDir;
	float total;
};

