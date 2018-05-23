
//#include<bits/stdc++.h>   //包含c++所有头文件，但此不支持！
#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<map>

using namespace std;

int submitYear,submitMonth,submitDay;
unordered_map<string,vector<string>> umap;  //包含在<undered_map>头文件中

void splitString(const string &s,vector<string> &v,const string &c)
{
	string::size_type pos1,pos2;
	pos1=0;
	pos2=s.find(c);
	while(pos2!=string::npos)
	{
		v.push_back(s.substr(pos1,pos2-pos1));
		pos1=pos2+c.size();
		pos2=s.find(c,pos1);
	}
	if(pos1!=s.length())
	{
		v.push_back(s.substr(pos1));
	}
}

vector<int> getDate(string str,vector<string>& vstring)
{
	vector<int>res;
	splitString(str,vstring,"/");
	res.push_back(stoi(vstring[0]));
	res.push_back(stoi(vstring[1]));
	res.push_back(stoi(vstring[2]));

	return res;
}

void carsMaintain(string str)
{
	string carNumber;
	int produceYear,produceMonth,produceDay;
	int ageYear,ageMonth,ageDay;
	string company;
	int milesLength;
	bool isFixed=false;
	vector<string>vecString;
	vector<string>vecProduceDate;

	splitString(str,vecString,"|");
	splitString(vecString[1],vecProduceDate,"/");
	carNumber=vecString[0];
	produceYear=stoi(vecProduceDate[0]);
	produceMonth=stoi(vecProduceDate[1]);
	produceDay=stoi(vecProduceDate[2]);

	ageYear=submitYear-produceYear;
	ageMonth=submitMonth-produceMonth;
	ageDay=submitDay-produceDay;
	
	company=vecString[2];
	milesLength=stoi(vecString[3]);
	isFixed=vecString[4]=="T";

	//日期计算的不对！
	//write-off
	if(ageYear>=6||(ageYear>=3&&isFixed))
	{
		if(ageMonth<=1)
			umap["Write-Off"].push_back(str);
	}
	//distance related
	else if(10000-milesLength%10000<=500||milesLength%10000==0)
		umap["Distance-Related"].push_back(str);
	//time related
	else
	{
		if(isFixed)
		{
			if(ageYear>=1)
			{
				if((12-produceMonth+submitMonth)%3!=1)
					umap["Time-Related"].push_back(str);
			}
			else
			{
				if(ageMonth%3!=1)
					umap["Time-Related"].push_back(str);
			}
		}
		else if(ageYear>=3)
		{
			if((12-produceMonth+submitMonth)%6==0||(12-produceMonth+submitMonth)%6==5)
				umap["Time-Related"].push_back(str);
		}
		else
		{
			if((12-produceMonth+submitMonth)%12==0||(12-produceMonth+submitMonth)%12==11)
				umap["Time-Related"].push_back(str);
		}
	}
}

map<string, vector<string>> printHelper(vector<string>& vecString)
{
	map<string, vector<string>> mmap;
	for (auto str : vecString)
	{
		vector<string> vstr;
		splitString(str, vstr, "|");
		mmap[vstr[2]].push_back(vstr[0]);
	}
	return mmap;
}

void printFormat(vector<string> vstr)
{
	auto mmap = printHelper(vstr);
	for (auto item : mmap)
	{
		cout << item.first << ": " << item.second.size() << " ";
		for (int i = 0; i < item.second.size(); i++)
		{
			if (i == 0)
			{
				if (i == item.second.size() - 1)
					cout << "(" << item.second[i] << ")";
				else
					cout << "(" << item.second[i] << ",";
			}
			else if (i == item.second.size() - 1)
				cout << " " << item.second[i] << ")";
			else
				cout << " " << item.second[i] << ",";
		}
		cout << endl;
	}
}

void printOut()
{
	cout<<"Reminder"<<endl;
	cout<<"==============="<<endl;
	cout<<endl;
	cout<<"* Time-related maintenance coming soon..."<<endl;
	printFormat(umap["Time-Related"]);
	cout<<"* Distance-related maintenance coming soon..."<<endl;
	printFormat(umap["Distance-Related"]);
	cout<<"* Write-off coming soon..."<<endl;
	printFormat(umap["Write-Off"]);
}

int main()
{
	/*freopen("taxi_in.txt","r",stdin);  //输入/输出重定向，到文件
	freopen("taxi_out.txt","w",stdout);*/
	freopen("test2_in.txt", "r", stdin);  //输入/输出重定向，到文件
	freopen("test2_out.txt", "w", stdout);
	
	string firstLine,submitDate;
	vector<string> vstring;
	getline(cin,firstLine);
	submitDate=firstLine.substr(firstLine.find(' ')+1);
	vector<int> submitDateVec=getDate(submitDate,vstring);
	submitYear=submitDateVec[0];
	submitMonth=submitDateVec[1];
	submitDay=submitDateVec[2];

	string str;
	while(getline(cin,str))
	{
		carsMaintain(str);
	}
	
	printOut();

	return 0;
}