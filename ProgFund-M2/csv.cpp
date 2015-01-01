/*************************************
Program: csv.cpp
Course:  Programming Fundamentals
Year:    2014/10 Trimester 1
Name:    LEE ZHEN YONG
ID:      1122702848
Lecture: TC01
Lab:     TT01
Email:   bruceoutdoors@gmail.com
Phone:   016-3188854
*************************************/

#include "csv.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

Csv::Csv(const string &dir)
{
	directory = dir;
	read(dir);
}

Csv::Csv(const Table &table)
{
	this->table = table;
}

void Csv::read(const string &dir)
{
	ifstream file(dir.c_str());
	if (!file.good())
		throw runtime_error("I can't find \"" + dir + "\"");

	string line, cell;

	while (getline(file, line)) {
		stringstream lineStream(line);
		Row r;
		while (std::getline(lineStream, cell, ',')) {
			r.push_back(cell);
		}
		// ignore empty rows
		if (r.size() <= 0) continue;
		table.push_back(r);
	}
}

Csv::~Csv()
{

}

Row &Csv::operator[](size_t index)
{
	return table[index];
}

void Csv::appendRow(const Row row)
{
	table.push_back(row);
}

void Csv::save()
{
	ofstream file(directory.c_str());
	for (Table::iterator row = table.begin(); row < table.end(); row++)
		writeRow(*row, file);
}

void Csv::writeRow(const Row row, ofstream &file)
{
	for (size_t i = 0; i < row.size(); i++) {
		file << row[i];
		if (i < row.size() - 1) file << ',';
	}
	file << '\n';
}

void Csv::writeAppendRow(const Row row)
{
	ofstream file(directory.c_str(), ofstream::app);
	appendRow(row);
	writeRow(row, file);
}

// wipe out the contents of the file
void Csv::deleteEverything()
{
	ofstream file(directory.c_str(), ofstream::trunc);
}
