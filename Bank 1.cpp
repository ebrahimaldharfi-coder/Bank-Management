#include <iostream>
#include <cstdio>
#include <algorithm>
#include <fstream>
#include <vector>
#include <random>
#include <string>
#include <cctype>  // isupper|lawer, isdiget
#include <iomanip> // setw()
#include <ctime>
using namespace std;

string filename = "clint.txt";

struct sClient
{
	string AcountName;
	string PinCode;
	string Name;
	string Phone;
	double Balanse;
	bool MarkToDelet = false;
};

vector<string> SplitString(string s1, string delim)
{
	vector <string> vstring;
	short pos = 0;
	string sword;

	while ((pos = s1.find(delim)) != std::string::npos)
	{
		sword = s1.substr(0, pos);

		if (sword != "")
		{
			vstring.push_back(sword);
		}

		s1.erase(0, pos + delim.length());
	}

	if (s1 != "")
	{
		vstring.push_back(s1);

	}

	return vstring;
}

sClient ConvertLineToRecord(string line, string dilem = "<>")
{
	sClient clin;
	vector<string>vclinet;

	vclinet = SplitString(line, dilem);

	clin.AcountName = vclinet[0];
	clin.PinCode = vclinet[1];
	clin.Name = vclinet[2];
	clin.Phone = vclinet[3];
	clin.Balanse = stod(vclinet[4]);

	return clin;

}

vector<sClient>LoadClintDataFromFile(string name)
{
	vector <sClient>vclinet;

	fstream myfile;
	myfile.open(name, ios::in);

	if (myfile.is_open())
	{
		string line;
		sClient client;

		while (getline(myfile, line))
		{
			client = ConvertLineToRecord(line);
			vclinet.push_back(client);
		}
		myfile.close();
	}
	return vclinet;
}

sClient ReadNewClinet()
{
	sClient clinet;

	cout << "Enter Acount Number? ";
	getline(cin >> ws, clinet.AcountName);

	cout << "Enter Pin-Code? ";
	getline(cin, clinet.PinCode);

	cout << "Enter Your Name? ";
	getline(cin, clinet.Name);

	cout << "Enter Phone-Number? ";
	getline(cin, clinet.Phone);

	cout << "Enter Balance? ";
	cin >> clinet.Balanse;

	return clinet;
}

string ConvertToLine(sClient clinet, string delem = "<>")
{
	string s1 = "";
	s1 += clinet.AcountName + delem;
	s1 += clinet.PinCode + delem;
	s1 += clinet.Name + delem;
	s1 += clinet.Phone + delem;
	s1 += to_string(clinet.Balanse);

	return s1;
}

void AddToFile(string name, string line)
{
	fstream myfile;

	myfile.open(name, ios::out | ios::app);

	if (myfile.is_open())
	{
		myfile << line << endl;

		myfile.close();
	}
}

void AddNewClinet()
{
	sClient clinet = ReadNewClinet();
	AddToFile(filename, ConvertToLine(clinet));
}

void AddClinet()
{
	char t = 'Y';
	do
	{
		AddNewClinet();

		cout << "Do You Want add more <Y>\n";
		cin >> t;

	} while (toupper(t) == 'Y');
}

bool FindClinetByName(string name, vector <sClient>vclinet, sClient& clinet)
{
	for (sClient& c : vclinet)
	{
		if (c.AcountName == name)
		{
			clinet = c;
			return true;
		}

	}

	return false;
}

bool MarkToDelet(string name, vector <sClient>& vclinet)
{
	for (sClient& c : vclinet)
	{
		if (c.AcountName == name)
		{
			c.MarkToDelet = true;
			return true;
		}
	}
	return false;
}

vector <sClient> SaveClintsToFile(string name, vector <sClient>& vclinet)
{
	string line;
	fstream myfile;
	myfile.open(name, ios::out);

	if (myfile.is_open())
	{
		for (sClient& c : vclinet)
		{
			if (c.MarkToDelet == false)
			{
				line = ConvertToLine(c);
				myfile << line << endl;
			}
		}
		myfile.close();
	}
	return vclinet;
}

void DeleteClinet(vector<sClient>vclinet)
{
	sClient clinet;
	char answr = 'N';
	string name = "";

	cout << "Right The Acounet-Number \n";
	cin >> name;

	if (FindClinetByName(name, vclinet, clinet))
	{
		cout << "Are you sure";
		cin >> answr;

		if (toupper(answr) == 'Y')
		{
			MarkToDelet(name, vclinet);
			SaveClintsToFile(filename, vclinet);
			cout << "\nDone " << endl;
		}

	}
	else
		cout << "The Acount-Name is infaled\n";
}

void PrintClint(sClient clint)
{
	cout << clint.AcountName << "   ";
	cout << clint.PinCode << "   ";
	cout << clint.Name << "   ";
	cout << clint.Phone << "   ";
	cout << clint.Balanse << "   ";
}

void PrintClinetFile(vector <sClient> vclint)
{
	for (sClient& c : vclint)
	{
		PrintClint(c);
		cout << endl;
		cout << "__________________________________________\n";
	}
}

void UpdateInfrmation(sClient& clinet)
{
	cout << "Enter Pin-Code? ";
	getline(cin >> ws, clinet.PinCode);

	cout << "Enter Your Name? ";
	getline(cin, clinet.Name);

	cout << "Enter Phone-Number? ";
	getline(cin, clinet.Phone);

	cout << "Enter Balance? ";
	cin >> clinet.Balanse;
}

void MarkToUpdate(string name, vector<sClient>& vclinet)
{
	for (sClient& c : vclinet)
	{
		if (c.AcountName == name)
		{
			UpdateInfrmation(c);
			SaveClintsToFile(filename, vclinet);
			cout << "\nDone " << endl;
		}
	}
}

void UpdateClint(vector <sClient>& vclint)
{
	sClient clinet;
	string AcountNumber;
	char answr = 'N';

	cout << "Enter AcountNumber? ";
	cin >> AcountNumber;

	if (FindClinetByName(AcountNumber, vclint, clinet))
	{
		PrintClint(clinet);
		cout << "\nAre you sure? ";
		cin >> answr;

		if (toupper(answr) == 'Y')
		{
			MarkToUpdate(AcountNumber, vclint);
		}

	}
	else
		cout << "The Acount-Name is infaled\n";
}

void FindClinet(vector<sClient>vclinet)
{
	sClient c;
	string AN;

	cout << "Enter Acount Number? " << endl;
	cin >> AN;

	if (FindClinetByName(AN, vclinet, c))
		PrintClint(c);
}

void Print_BankName(vector <sClient>vclinet) {
	int choice;

	cout << "\n========================================\n";
	cout << "          Welcome to Smart Bank  \n";
	cout << "========================================\n\n";

	cout << "\n====================================" << endl;
	cout << "           MANAGER MENU             " << endl;
	cout << "====================================" << endl;
	cout << "1. Add New User" << endl;
	cout << "2. Delete User" << endl;
	cout << "3. View System Reports" << endl;
	cout << "4. Update Inventory" << endl;
	cout << "5. Find Clinet" << endl;
	cout << "6. Logout" << endl;
	cout << "------------------------------------" << endl;


	cout << "Enter your choice: ";
	cin >> choice;

	switch (choice) {
	case 1:
		cout << "\n[Action] Navigating to Add User module..." << endl;
		AddClinet();
		break;
	case 2:
		cout << "\n[Action] Navigating to Delete User module..." << endl;
		DeleteClinet(vclinet);
		break;
	case 3:
		cout << "\n[Action] Displaying system reports..." << endl;
		PrintClinetFile(vclinet);
		break;
	case 4:
		cout << "\n[Action] Opening Update Status..." << endl;
		UpdateClint(vclinet);
		break;
	case 5:
		cout << "\n[Action] Opening Find Clinet..." << endl;
		FindClinet(vclinet);
		break;
	case 6:
		cout << "Thanks " << endl;
		break;
	default:
		cout << "\nInvalid choice! Please try again." << endl;
	}
}

void start(vector <sClient>vclinet)
{
	Print_BankName(vclinet);
}

int main() {

	vector <sClient>vclinet = LoadClintDataFromFile(filename);
	start(vclinet);

	system("pause>0");
	return 0;
}