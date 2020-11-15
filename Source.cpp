#include <string>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <map>
#include <set>

using namespace std;

class Date {
public:
	int GetYear() const {
		return year;
	}
	int GetMonth() const {
		return month;
	}
	int GetDay() const {
		return day; 
	}
	void SetYear(int new_year) {
		if (new_year < 0) { year = -1 * new_year; }
		else 
		year = new_year;
	}
	void SetMonth(int new_month) {
		month = new_month;
	}
	void SetDay(int new_day) {
		day = new_day;
	}
private: 
	int year;
	int month;
	int day; 
};

bool operator<(const Date& lhs, const Date& rhs)
{
	if (lhs.GetYear() == rhs.GetYear()){
		if (lhs.GetMonth() == rhs.GetMonth())	{
			return lhs.GetDay() < rhs.GetDay(); 
		}
		else {
			return lhs.GetMonth() < rhs.GetMonth();
		}
	}
	else return lhs.GetYear() < rhs.GetYear(); 
}
ostream & operator << (ostream & stream, const Date & date)
{
	stream << setw(4)<<setfill('0')<<date.GetYear() << "-" << setw(2) << setfill('0') << date.GetMonth() <<
		"-" << setw(2) << setfill('0') << date.GetDay();
	return stream; 
}
class Database {
public:
	void AddEvent(const Date& date, const string& event) {
		database[date].insert(event); // теперь всё хорошо	
	}
	bool DeleteEvent(const Date& date, const string& event) {
		if (database.count(date) == 0)
		{
			return false; 
		}
else if (database.at(date).count(event)>0)
		{
			database.at(date).erase(event);
			return true;
		}
		else
			return false; 
	}
	int  DeleteDate(const Date& date) {
		if (database.count(date)==0)
		{
			return 0; 
		}
		else {
			int N = database.at(date).size();
			database.erase(date);
			return N;
		}
	}

	void Find(const Date& date) const {
		if (database.count(date)>0 && !database.at(date).empty()) {
			for (auto & item : database.at(date))
			{
				cout << item << endl;
			}
		}
		
	}	
	void Print() const {
		for ( const auto&  item : database)
		{
			for (const auto &item2: item.second)
			{
				 cout <<item.first<< " " << item2<<endl;
			}		
		}
	}
private:
	map <Date, set <string>> database; 
};
void CheckFormat(  string & yearmonthday)
{
	stringstream SS(yearmonthday); 
	int year, month, day; char def1, def2; 
	SS >> year >> def1 >> month >> def2;
	SS>>day;
	if (SS.fail() || SS.peek() != EOF || def1 != '-' || def2 != '-')
	{
		string error = "Wrong date format: " + yearmonthday;
		throw runtime_error(error.c_str());
	}
}

void ParseDate(Date & date, string &yearmonthday)
{
	CheckFormat(yearmonthday);
	stringstream SS(yearmonthday); int year, month, day; 
	SS >> year; SS.ignore(1);
	date.SetYear(year);
	SS >> month; SS.ignore(1);
	string error = "Month value is invalid: " + to_string(month); 
	if (month > 12 || month < 1) {
		throw runtime_error(error.c_str());
	}
	date.SetMonth(month);
	SS >> day; 
	string error1 = "Day value is invalid: " + to_string(day);
	if (day > 31 || day < 1) {
		throw runtime_error(error1.c_str());
	}
	date.SetDay(day);	
}

int main() {
	Database db;
	string command;
	while (getline(cin, command)) {

			string localcommand; 
			Date date;
			string yearmonthday;
			stringstream SS(command);
			SS >> localcommand; 
			if (localcommand == "Add")
			{
				SS >> yearmonthday;
				try {
					ParseDate(date, yearmonthday);
					string evenT;
					SS >> evenT;
					db.AddEvent(date, evenT);
				}
				catch (exception & e) {
					cout << e.what() << endl;
				}
			}
			else if (localcommand == "Find")
			{
				SS >> yearmonthday;
				try {
					ParseDate(date, yearmonthday);
					db.Find(date);
				}
				catch (exception &e) {
					cout << e.what() << endl;
				}
			}
			else if (localcommand == "Print")
			{
				db.Print();
			}
			else if (localcommand == "Del")
			{
				string evenT;
				SS >> yearmonthday;
				try {
					ParseDate(date, yearmonthday);

					SS >> evenT;
					if (evenT.empty())
					{
						cout << "Deleted " << db.DeleteDate(date) << " events" << endl;
					}
					else
					{
						if (db.DeleteEvent(date, evenT))
						{
							cout << "Deleted successfully" << endl;
						}
						else
							cout << "Event not found" << endl;
					}
				}
				catch (exception &e) {
					cout << e.what() << endl;
				}
			}
			else if (!localcommand.empty())
				cout << "Unknown command: " << localcommand << endl;
		// Считайте команды с потока ввода и обработайте каждую
	}
	return 0;
}
