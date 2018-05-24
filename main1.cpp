
#include<iostream>
#include<string>
#include<vector>
#include<stdbool.h>
#include<time.h>
#include<stdlib.h>
#include<math.h>
#include<map>
using namespace std;

struct Date{
	int year;
	int month;
	int day;
};

struct Car{
	string number;
	struct Date purchase_date;
	string brand;
	int cur_miles;
	char fix;
};

struct Car extract_info(string & info);
struct Date extract_date(string & date);
time_t convert(int year, int month, int day);
void judge(struct Date _SubmitDate, struct Car _car);
bool is_write_off(struct Date _SubmitDate, struct Car _car);
bool is_write_off_reminder(struct Date _SubmitDate, struct Car _car);
bool is_distance_related(struct Date _SubmitDate, struct Car _car);
bool is_time_related(struct Date _SubmitDate, struct Car _car);
void show();
void show1(multimap<string, string> &_related);

multimap<string, string> Time_related, Distance_related, Write_off;

int main()
{
	string submitdate;
	cout << "SubmitDate:";
	cin >> submitdate;
	cin.get();

	struct Date SubmitDate;
	SubmitDate = extract_date(submitdate);

	struct Car cartmp;
	vector<struct Car> cars;
	string str;
	while (getline(cin, str))
	{
		cartmp = extract_info(str);
		judge(SubmitDate, cartmp);
	}
	show();

	return 0;
}

struct Car extract_info(string &str)
{
	struct Car _cartmp;
	int pos1, pos2;
	pos1 = str.find('|');
	_cartmp.number = str.substr(0, pos1);
	pos2 = str.find('|', pos1 + 1);
	string datetmp;
	datetmp = str.substr(pos1 + 1, pos2 - pos1 - 1);
	_cartmp.purchase_date = extract_date(datetmp);
	pos1 = pos2;
	pos2 = str.find('|', pos1 + 1);
	_cartmp.brand = str.substr(pos1 + 1, pos2 - pos1 - 1);
	pos1 = pos2;
	pos2 = str.find('|', pos1 + 1);
	_cartmp.cur_miles = stoi(str.substr(pos1 + 1, pos2 - pos1 - 1));
	_cartmp.fix = str[str.size() - 1];

	return _cartmp;
}

struct Date extract_date(string & date)
{
	struct Date _date;
	_date.year = stoi(date.substr(0, 4));
	_date.month = stoi(date.substr(5, 2));
	_date.day = stoi(date.substr(8, 2));
	return _date;
}

time_t convert(int year, int month, int day)
{
	tm info = { 0 };
	info.tm_year = year - 1900;
	info.tm_mon = month - 1;
	info.tm_mday = day - 1;
	return mktime(&info);
}

void judge(struct Date _SubmitDate, struct Car _car)
{
	if (!is_write_off(_SubmitDate, _car))
	{
		if (is_write_off_reminder(_SubmitDate, _car))
			Write_off.insert(make_pair(_car.brand, _car.number));
		else if (is_distance_related(_SubmitDate, _car))
			Distance_related.insert(make_pair(_car.brand, _car.number));
		else if (is_time_related(_SubmitDate, _car))
			Time_related.insert(make_pair(_car.brand, _car.number));
	}
	//return;
}

bool is_write_off(struct Date _SubmitDate, struct Car _car)
{
	long long to_second = (long long)convert(_SubmitDate.year, _SubmitDate.month, _SubmitDate.day);
	long long from_second = (long long)convert(_car.purchase_date.year, _car.purchase_date.month, _car.purchase_date.day);
	long long days_interval = (to_second - from_second) / 24 / 3600;
	if (_car.fix == 'F'&&days_interval >= 2190)
		return true;
	else if (_car.fix == 'T'&&days_interval >= 1095)
		return true;
	else
		return false;
}

bool is_write_off_reminder(struct Date _SubmitDate, struct Car _car)
{
	//报废提醒
	long long to_second = (long long)convert(_SubmitDate.year, _SubmitDate.month, _SubmitDate.day);
	long long from_second = (long long)convert(_car.purchase_date.year, _car.purchase_date.month, _car.purchase_date.day);
	long long days_interval = (to_second - from_second) / 24 / 3600;

	if (_car.fix == 'F'&&days_interval<2190)
	{
		time_t to_second1 = from_second + 2190 * 86400;
		tm writeoff;
		localtime_s(&writeoff, &to_second1);

		if ((_SubmitDate.year == writeoff.tm_year + 1900 && _SubmitDate.month == (writeoff.tm_mon + 1) - 1) || (_SubmitDate.year == writeoff.tm_year + 1900 && _SubmitDate.month == (writeoff.tm_mon + 1) && _SubmitDate.day < (writeoff.tm_mday + 1)))
			return true;
		else
			return false;
	}
	else if (_car.fix == 'T'&&days_interval < 1095)
	{
		time_t to_second1 = from_second + 1095 * 86400;
		tm writeoff;
		localtime_s(&writeoff, &to_second1);
		if ((_SubmitDate.year == writeoff.tm_year + 1900 && _SubmitDate.month == (writeoff.tm_mon + 1) - 1) || (_SubmitDate.year == writeoff.tm_year + 1900 && _SubmitDate.month == (writeoff.tm_mon + 1) && _SubmitDate.day < (writeoff.tm_mday + 1)))
			return true;
		else
			return false;
	}
	else
		return false;
}

bool is_distance_related(struct Date _SubmitDate, struct Car _car)
{
	//10000公里提醒
	if (_car.cur_miles % 10000 == 0 || _car.cur_miles % 10000 >= 9500)
		return true;
	else
		return false;
}

bool is_time_related(struct Date _SubmitDate, struct Car _car)
{

	//定期保养提醒
	int year_interval = _SubmitDate.year - _car.purchase_date.year;
	int month_interval = (_SubmitDate.year - _car.purchase_date.year - 1) * 12 + _SubmitDate.month + 12 - _car.purchase_date.month;

	if (_car.fix == 'T')
	{
		if ((month_interval % 3 == 0 && _SubmitDate.day <= _car.purchase_date.day) || (month_interval + 1) % 3 == 0)
			return true;
		else
			return false;
	}
	else if (_car.fix == 'F' && year_interval<3)
	{
		if ((month_interval % 12 == 0 && _SubmitDate.day <= _car.purchase_date.day) || (month_interval + 1) % 12 == 0)
			return true;
		else
			return false;
	}
	else if (_car.fix == 'F' && year_interval >= 3 && year_interval <= 6)
	{
		if ((month_interval % 6 == 0 && _SubmitDate.day <= _car.purchase_date.day) || (month_interval + 1) % 6 == 0)
			return true;
		else
			return false;
	}
	else
		return false;
}

void show()
{
	cout << "Reminder" << endl;
	cout << "********************" << endl;
	cout << endl;

	cout << "Time-related maintenance coming soon..." << endl;
	show1(Time_related);
	cout << endl;

	cout << "Distance-related maintenance coming soon..." << endl;
	show1(Distance_related);
	cout << endl;

	cout << "Write-off comming soon..." << endl;
	show1(Write_off);
	cout << endl;
}

void show1(multimap<string, string> & _related)
{
	multimap<string, string>::iterator iter;
	for (iter = _related.begin(); iter != _related.end(); ++iter)
	{
		string strFind = (*iter).first;
		unsigned int count = _related.count(strFind);
		cout << (*iter).first << ":" << count << " (";
		for (unsigned int j = 0; j < count - 1; ++j)
		{
			cout << (*iter).second << ",";
			iter++;
		}
		cout << (*iter).second << ")" << endl;
	}
}
