/*************************************
Program: cashcontainer.cpp
Course:  Programming Fundamentals
Year:    2014/10 Trimester 1
Name:    LEE ZHEN YONG
ID:      1122702848
Lecture: TC01
Lab:     TT01
Email:   bruceoutdoors@gmail.com
Phone:   016-3188854
*************************************/

#include "cashcontainer.h"
#include "round.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cassert>

const float CashContainer::values[] =
{ 20.00f, 10.00f, 5.00f, 1.00f, 0.50f, 0.20f, 0.10f };

CashContainer::CashContainer() : isFile(false), total(0)
{
	makeEmpty();
}

CashContainer::CashContainer(string dir) : isFile(true), fileDir(dir)
{
	ifstream file(dir.c_str());
	if (!file.good())
		throw runtime_error("I can't find \"" + dir + "\"");

	string line;
	getline(file, line);

	istringstream buffer(line);
	// assuming the file follows the format as prescribed:
	for (size_t i = 0; i < SIZE; i++) buffer >> container[i];

	updateTotal();
}

CashContainer::CashContainer(const int container[SIZE])
{
	for (size_t i = 0; i < SIZE; i++)
		this->container[i] = container[i];

	updateTotal();
}

/*
take a float amount and generate change from it
*/
CashContainer::CashContainer(float amount)
{
	makeEmpty();
	amount = Round(amount);

	for (size_t i = 0; i < SIZE; i++) {
		// add a small value to counter float precision problems
		while (values[i] <= amount) {
			amount -= values[i];
			amount = Round(amount);
			(*this)[i]++;
		}
	}

	updateTotal();
	assert(amount == 0);
}

/*
Copy constructor and assignment operator overloading:
*/
CashContainer::CashContainer(const CashContainer &c)
{
	*this = c;
}

CashContainer& CashContainer::operator=(const CashContainer& c)
{
	if (this != &c) {
		for (size_t i = 0; i < SIZE; i++) set(i, c[i]);

		this->total = c.total;
		this->fileDir = c.fileDir;
		this->isFile = c.isFile;
	}

	return *this;
}

CashContainer::~CashContainer()
{
}

int & CashContainer::operator[](size_t index)
{
	checkRange(index);
	return container[index];
}

int CashContainer::operator[](const size_t index) const
{
	checkRange(index);
	return container[index];
}

float CashContainer::getValue(int index)
{
	checkRange(index);
	return values[index];
}

void CashContainer::checkRange(int index)
{
	if (index < 0 || index > (int)SIZE)
		throw out_of_range(index + " is not a valid index of cash container!");
}

void CashContainer::makeEmpty()
{
	int i = SIZE;
	while (i--) container[i] = 0;
}

void CashContainer::save(string dir)
{
	ofstream file(dir.c_str());
	for (size_t i = 0; i < SIZE; i++)
		file << container[i] << " ";

	fileDir = dir;
	isFile = true;
}

void CashContainer::save()
{
	if (!isFile)
		throw runtime_error("You can't file save a CashContainer that is not attached to a file!");

	save(fileDir);
}

/*
assignment operators are tricky to implement for this scenario
(e.g. container[index] = 7), as integers are raw data types
without assignment operator overloads. You'll need a proxy integer
class that overloads the assignment operator, which I couldn't be
bothered to do :P Ergo a classic set function is used.
*/
void CashContainer::set(size_t index, int value)
{
	checkRange(index);
	container[index] = value;
	updateTotal();
}

// convenience function that increments instead of setting the value
void CashContainer::increment(size_t index, int value)
{
	set(index, container[index] + value);
}

CashContainer CashContainer::operator+(const CashContainer &c) const
{
	CashContainer result;
	for (size_t i = 0; i < SIZE; i++)
		result.set(i, (*this)[i] + c[i]);

	return result;
}

CashContainer CashContainer::operator-(const CashContainer &c) const
{
	CashContainer result;
	for (size_t i = 0; i < SIZE; i++)
		result.set(i, (*this)[i] - c[i]);

	return result;
}

/*
The following 2 methods do NOT return a copy. They will modify the
LHS directly. The LHS will retain will its properties after the
operation is completed.
*/
CashContainer &CashContainer::operator-=(const CashContainer &c)
{
	for (size_t i = 0; i < SIZE; i++)
		this->set(i, (*this)[i] - c[i]);

	return *this;
}

CashContainer & CashContainer::operator+=(const CashContainer &c)
{
	for (size_t i = 0; i < SIZE; i++)
		this->set(i, (*this)[i] + c[i]);

	return *this;
}

// will this container have change after minus for CashContainer other?
bool CashContainer::hasChange(const CashContainer &other) const
{
	CashContainer c = *this - other;

	for (size_t i = 0; i < SIZE; i++) {
		if (c[i] < 0) return false;
	}

	return true;
}

void CashContainer::updateTotal()
{
	total = 0;
	for (size_t i = 0; i < SIZE; i++) {
		total += container[i] * values[i];
	}
	total = Round(total);
}

CashContainer CashContainer::calcChange(float changeAmount) const
{
	float amount;
	CashContainer thistemp;
	CashContainer change;
	int ignoredIdx = -1; // >= 0 begins selective ignorance
	int attemptedQuantity;
	bool hasAttemptedQuantity = false;

	do {
		thistemp = *this; // copy this contents
		change.makeEmpty();
		amount = changeAmount;
		
		for (size_t i = 0; i < SIZE; i++) {
			int counter = 0; // counter to repeat "attemptedQuantity" times 
			while (amount >= values[i] && thistemp[i] > 0) {
				/** Test case: 
				 * - change for 2 given 3 0.5, 6 0.2, 0 0.1, the change should be 2 0.5 + 5 0.2.
				 * - change for 0.6 given 1 0.5, 3 0.2, 0 0.1, the change should be 3 0.2.
				 * - change for 0.1 given 0 0.1, the change cannot be output.
				 **/
				if (ignoredIdx != -1 && i == ignoredIdx) {
					// selectively choose an quantity to limit from a denomination 
					// for each denomination until change can be found.

					// being able to reach this enclosing condition meant we have
					// found an denomination we can selectively ignore that is less
					// than the change amount and has a quantity.
					if (!hasAttemptedQuantity) {
						// we can assert that we can never execute "attemptedQuantity" 
						// more than "thistemp[i]" (available quantity) because of the
						// condition in the enclosing while loop above ^
						hasAttemptedQuantity = true;
						attemptedQuantity = (int)(amount / values[i]);
					}
					
					if (counter == attemptedQuantity) {
						// once the counter is exhausted we skip to the next denomination
						i++;
						continue;
					} else counter++;
				}

				// in selective ignorance, the following lines are executed "attemptedQuantity" times:
				thistemp.increment(i, -1); // << decrement
				change.increment(i);
				amount = Round(amount - values[i]);
			}
		}

		if (amount == 0) break;

		// if there is no available attempted quantities to try, the 
		// following condition either starts the selective ignorance when 
		// it is -1, or repeats the loop with the next denomination since
		// we can't find an attempted quantity from it
		if (!hasAttemptedQuantity) {
			ignoredIdx++;
			continue;
		}

		if (attemptedQuantity < 0) {
			// if all attempts have failed, we move on to the next denomination
			// and ask for the next attempted quantity:
			ignoredIdx++;
			hasAttemptedQuantity = false;
		} else {
			// try again, with less
			attemptedQuantity--;
		}
	} while (ignoredIdx != (SIZE-1));

	if (amount > 0) throw runtime_error("Insufficient change!");

	return change;
}

