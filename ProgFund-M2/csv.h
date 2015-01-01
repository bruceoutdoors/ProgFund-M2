/*************************************
Program: csv.h
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

#include <string>
#include <vector>

using namespace std;

typedef vector<string> Row;
typedef vector<Row> Table;

class Csv
{
public:
	Csv(const string &dir);
	Csv(const Table &table);
	void read(const string &dir);
	void appendRow(Row row);
	void save();
	void writeAppendRow(const Row row);
	void deleteEverything();

	Row &operator[](size_t index);
	virtual ~Csv();
	size_t getRowCount() const { return table.size(); }
	string getDirectory() const { return directory; }
	void setDirectory(string val) { directory = val; }

private:
	void writeRow(const Row row, ofstream &file);

	string directory;
	Table table;
};

