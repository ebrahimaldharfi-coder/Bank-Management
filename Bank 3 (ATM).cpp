#include <iostream>
#include <conio.h>
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

const string ClientsFileName = "clint.txt";

void ShowAtmMainMenue();
void Login();
void ShowQuickWithDrowScreen();


struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

sClient CurrentClient;

vector<string> SplitString(string S1, string Delim)
{

    vector<string> vString;

    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;

}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{

    sClient Client;
    vector<string> vClientData;

    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double


    return Client;

}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);

            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return vClients;

}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;

}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vClients;

}

bool FindClientByUsernameAndPassword(string AccountNumber, string PinCode, sClient& User)
{

    vector <sClient> vClient = LoadCleintsDataFromFile(ClientsFileName);

    for (sClient U : vClient)
    {

        if (U.AccountNumber == AccountNumber && U.PinCode == PinCode)
        {
            User = U;
            return true;
        }

    }
    return false;

}

bool  LoadUserInfo(string AccountNumber, string PinCode)
{

    if (FindClientByUsernameAndPassword(AccountNumber, PinCode, CurrentClient))
        return true;
    else
        return false;

}

enum enAtmMenueOptions { eQuickWithdrawit = 1, eNormalWithdraw = 2, eDeposit = 3, eChickBalance = 4, eLogout};

short ReadTransactionsMenueOption()
{
    cout << "Choose what do you want to do? [1 to 5]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to ATM Menue Screen...";
    system("pause>0");
    ShowAtmMainMenue();
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount)
{
    vector<sClient>vClients = LoadCleintsDataFromFile(ClientsFileName);

    char Answer = 'n';
    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {


        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                CurrentClient = C;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;

                return true;
            }

        }


        return false;
    }

}

enum enQuickWithDrow { w20 = 1, w50 = 2, w100 = 3, w200 = 4, w400 = 5, w600 = 6, w800 = 7, w1000 = 8, wExit = 9 };

short ReadWithDrowOption()
{
    cout << "Choose what do you want to do? [1 to 9]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void GoBackToQuickWithDrow()
{
    cout << "Prees anykay to continue.....";
   
    system("pause>0");

    ShowQuickWithDrowScreen();

}

void ChickWithdrow(short Amount)
{
    if (Amount > CurrentClient.AccountBalance)
    {
        cout << " The Amount exceeds your balance, make another choice. " << endl;
        GoBackToQuickWithDrow();
    }

    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, Amount * -1);
}

void QuickWithdrow(enQuickWithDrow Choice)
{
    switch (Choice)
    {
    case enQuickWithDrow::w20 :
    {
        ChickWithdrow(20);
        GoBackToMainMenue();
        break;
    }

    case enQuickWithDrow::w50:
    {

        ChickWithdrow(50);
        GoBackToMainMenue();
        break;
    }


    case enQuickWithDrow::w100:
    {
        ChickWithdrow(100);
        GoBackToMainMenue();
        break;
    }


    case enQuickWithDrow::w200:
    {
        ChickWithdrow(200);
        GoBackToMainMenue();

        break;

    }


    case enQuickWithDrow::w400:
    {        

    ChickWithdrow(400);
    GoBackToMainMenue();

    break;
    }

    case enQuickWithDrow::w600:
    {

        ChickWithdrow(600);
        GoBackToMainMenue();

        break;
    }


    case enQuickWithDrow::w800:
    {

        ChickWithdrow(800);
        GoBackToMainMenue();

        break;
    }


    case enQuickWithDrow::w1000:
    {

        ChickWithdrow(1000);
        GoBackToMainMenue();

        break;

    }


    case enQuickWithDrow::wExit:
    {
        system("cls");

        ShowAtmMainMenue();
        break;

    }
    }

}

void ShowQuickWithDrowScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\ATM Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] 20.\n";
    cout << "\t[2] 50.\n";
    cout << "\t[3] 100.\n";
    cout << "\t[4] 200.\n";
    cout << "\t[5] 400.\n";
    cout << "\t[6] 600.\n";
    cout << "\t[7] 800.\n";
    cout << "\t[8] 1000.\n";
    cout << "\t[9] Exit.\n";
    cout << "===========================================\n";

    cout << "Your Balance is: " << CurrentClient.AccountBalance << endl;

    QuickWithdrow((enQuickWithDrow)ReadWithDrowOption());
    
}

void ShowNormalWithDrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";

    cout << "Your Balance is: " << CurrentClient.AccountBalance << endl;

        int Amount = 0;

    do
    {
        cout << "\nPlease enter an amount multiple 5's ? ";
        cin >> Amount;
    } while (Amount % 5 != 0);
    

    while (Amount > CurrentClient.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << CurrentClient.AccountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> Amount;
    }

     DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, Amount * -1);

}

void ShowDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";

    cout << "Your Balance is: " << CurrentClient.AccountBalance << endl;

    double Amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> Amount;

    while (Amount <= 0)
    {
        cout << "Please Enter apositive number! ";
        cin >> Amount;
    }

    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, Amount);

}

void ShowChickBalaneScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\Chick Balances Screen";
    cout << "\n-----------------------------------\n";

    cout << "Your Balance is: " << CurrentClient.AccountBalance << endl;

}

void PerfromMianMenueOption(enAtmMenueOptions AtmMenueOption)
{
    switch (AtmMenueOption)
    {
    case enAtmMenueOptions::eQuickWithdrawit:
    {
        system("cls");
        ShowQuickWithDrowScreen();
        GoBackToMainMenue();
        break;
    }

    case enAtmMenueOptions::eNormalWithdraw:
    {
        system("cls");
        ShowNormalWithDrawScreen();
        GoBackToMainMenue();
        break;
    }


    case enAtmMenueOptions::eDeposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToMainMenue();
        break;
    }


    case enAtmMenueOptions::eChickBalance:
    {
        system("cls");
        ShowChickBalaneScreen();
        GoBackToMainMenue();
        break;

    }


    case enAtmMenueOptions::eLogout:
    {
        Login();
        break;

    }
    }

}

void ShowAtmMainMenue()
{

    system("cls");
    cout << "===========================================\n";
    cout << "\t\ATM Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Chick Balance.\n";
    cout << "\t[5] Logout.\n";

    cout << "===========================================\n";
    PerfromMianMenueOption((enAtmMenueOptions)ReadTransactionsMenueOption());
}

void Login()
{
    bool LoginFaild = false;

    string AccountNumber, PinCode;
    do
    {
        system("cls");

        cout << "\n---------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n---------------------------------\n";

        if (LoginFaild)
        {
            cout << "Invlaid AccountNumber/PinCode!\n";
        }

        cout << "Enter AccountNumber? ";
        cin >> AccountNumber;

        cout << "Enter PinCode? ";
        cin >> PinCode;

        LoginFaild = !LoadUserInfo(AccountNumber, PinCode);

    } while (LoginFaild);

    ShowAtmMainMenue();

}

int main()
{
	Login();
}