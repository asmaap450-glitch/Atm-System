#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>
using namespace std;
const string ClientsFileName = "clint.txt";
struct sClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;
};
sClient Client;
enum  eAtmMenue { deposit = 1, normalwithdraw = 2, quickwithdraw = 3,checkbalance = 4, Exit = 5};

void PrintFinalCard();

void NormaleWithdrawScreen();

void QuickWithDrawScreen();

short ReadMainMenueOption() {
	short chois = 0;
	cout << "chois what you want to do? [1 to 5]?";
	cin >> chois;
	return chois;
}

short ReadQuickWithdrawOption() {
	short chois = 0;
	while (chois < 1 || chois > 9)
	{
		cout << "\nchoose what you want to do? [1 to 9]?";
		cin >> chois;
	}
	return chois;
}

string ReadClientAccountNumber()
{
	string AccountNumber = "";
	cout << "\nPlease enter AccountNumber? ";
	cin >> AccountNumber;
	return AccountNumber;
}

sClient ChangeClientRecord(string AccountNumber)
{
	sClient Client;
	Client.AccountNumber = AccountNumber;
	cout << "\n\nEnter PinCode? ";
	getline(cin >> ws, Client.PinCode);
	return Client;
}

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
		S1.erase(0, pos + Delim.length()); /* erase() until
		positon and move to next word. */
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

vector <sClient> SaveCleintsDataToFile(string FileName, vector<sClient> vClients)
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

bool FindClientByAccountNumber(string AccountNumber,string pincode , sClient& Client)
{
	vector<sClient> vClients =
		LoadCleintsDataFromFile(ClientsFileName);
	for (sClient C : vClients)
	{
		if (C.AccountNumber == AccountNumber && C.PinCode==pincode)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

bool LoadUserInfo(string accountnum, string pincode) {
	if (FindClientByAccountNumber(accountnum, pincode, Client))
		return true;
	else
		return false;
}

void GoToAtmMenue() {
	cout << "\nPress any key to go back to main menue...";
	system("pause>0");
	PrintFinalCard();
}

void LogoutScreen() {
	cout << "\n---------------------------------------------\n";
	cout << "\t\t\t Program End :)\t\t\t\t";
	cout << "\n----------------------------------------------\n";
}

bool DepositBalanceToClientByAccountnumber(string accountnum,double amount,vector<sClient>& vClient) {
	char Answer= 'n';
	double depos = 0;
	double balance = 0;
	cout << "\nAre you sure perform this transaction? y/n ?";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		for (sClient& c : vClient)
		{
			if (c.AccountNumber == accountnum)
			{
				c.AccountBalance += amount;
		        SaveCleintsDataToFile(ClientsFileName, vClient);
		        cout<< "\nDone successfully.New balance is : "<<c.AccountBalance;
				return true;
			}
		}
		return false;
	}
}

void DepositScreen() {
	double amount = 0;
	cout << "\n============================================\n";
	cout << "\tDeposit Screen";
	cout << "\n============================================\n";
cout << "\nPlease enter a Postive Deposit Amount?";
	cin >> amount;
	while (amount <= 0)
	{
		cout << "\nPlease enter a Postive Deposit Amount?";
		cin >> amount;
	}
	vector<sClient>vClient = LoadCleintsDataFromFile(ClientsFileName);
	DepositBalanceToClientByAccountnumber(Client.AccountNumber,amount,vClient);
	Client.AccountBalance += amount;
}

void NormaleWithdraw() {
	int amount ;
	cout << "\nPlease enter withdraw amount multiple of 5 's?";
	cin >> amount;
	while (amount % 5 != 0)
	{
		cout << "\nThe amount exceeds your balance, make another chois?";
		cin >> amount;
	}
		if (Client.AccountBalance < amount)
		{
			cout << "\nThe amount exceeds your balance , make another choise.\n";
			cout << "\nPress any key to Continue...";
			system("pause>0");
			PrintFinalCard();
			return;
		}
		vector<sClient>vClient = LoadCleintsDataFromFile(ClientsFileName);
		DepositBalanceToClientByAccountnumber(Client.AccountNumber, amount * -1, vClient);
			Client.AccountBalance -= amount;
	}

void NormaleWithdrawScreen() {
	system("cls");
	cout << "===================================================\n";
	cout << "\t\tNormale Withdraw\n";
	cout << "===================================================\n";
	NormaleWithdraw();
}

short GetQuickWithdrawAmount(short QuickWithdraw) {
	switch (QuickWithdraw)
	{
	case 1:
		return 20;
	case 2:
		return 50;
	case 3:
		return 100;
	case 4:
		return 200;
	case 5:
		return 400;
	case 6:
		return 600;
	case 7:
		return 800;
	case 8:
		return 1000;
	default:
		return 0;
	}
}

void PreformQuickWithdrawOption(short amount ) {
	if (amount == 9)
		return;
	short WithdrawBalance = GetQuickWithdrawAmount(amount);
	if (WithdrawBalance > Client.AccountBalance)
	{
		cout << "\nThe amount exceeds your balance , make another choise.\n";
		cout << "\nPress any key to Continue...";
		system("pause>0");
		QuickWithDrawScreen();
	}
        vector<sClient>vClient = 
	    LoadCleintsDataFromFile(ClientsFileName);
		DepositBalanceToClientByAccountnumber(Client.AccountNumber, WithdrawBalance * -1, vClient);
        Client.AccountBalance -= WithdrawBalance;
}

void QuickWithDrawScreen() {
		system("cls");
		cout << "===================================================\n";
		cout << "\t\tQuick Withdraw\n";
		cout << "===================================================\n";
		cout << "\t[1] 20"<<setw(14)<<"[2] 50\n";
		cout << "\t[3] 100"<< setw(14)<< "[4] 200\n";
		cout << "\t[5] 400"<< setw(14)<< "[6] 600\n";
		cout << "\t[7] 800"<< setw(14)<< "[8] 1000\n";
		cout << "\t[9] Exit\n";
		cout << "===================================================\n";
		cout << "\nYour Balance is : " << Client.AccountBalance;
		PreformQuickWithdrawOption(ReadQuickWithdrawOption());
}

void CheckBalanceScreen() {
	cout << "===================================================\n";
	cout << "\t\tCheck Balance Screen\n";
	cout << "===================================================\n";
	cout << "your balanse is " << Client.AccountBalance;
}

void ReadAtmMenue(eAtmMenue chois) {
	switch (chois)
	{	
	case eAtmMenue::deposit:
	{
		system("cls");
		DepositScreen();
		GoToAtmMenue();
		break;
	}

	case eAtmMenue::normalwithdraw:
	{
		system("cls");
		NormaleWithdrawScreen();
		GoToAtmMenue();
		break;
	}

	case eAtmMenue::quickwithdraw:
	{
		system("cls");
		QuickWithDrawScreen();
		GoToAtmMenue();
		break;
	}

	case eAtmMenue::checkbalance:
	{
		system("cls");
		CheckBalanceScreen();
		GoToAtmMenue();
		break;
	}

	case eAtmMenue::Exit:
	{
		system("cls");
		LogoutScreen();
		break;
	}
  }
}

void PrintFinalCard() {
	system("cls");
	cout << "===================================================\n";
	cout << "\t\tAtm Main Menue Screen\n";
	cout << "===================================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Normale Withdraw.\n";
	cout << "\t[3] Quick Withdraw.\n";
	cout << "\t[4] Check Balance.\n";
	cout << "\t[5] Logout.\n";
	cout << "===================================================\n";
	ReadAtmMenue((eAtmMenue)ReadMainMenueOption());
}

void PrintLoginScreen() {
	string accountnum, pincode;
	bool loginfail = false;
	do {
		system("cls");
		cout << "\n=========================================\n";
		cout << "\tLogin Screen";
		cout << "\n=========================================\n";
		if (loginfail)
		{
			cout << "Invlaid Username/Password!\n";
		}
		cout << "Enter AccountNumber\n";
		cin >> accountnum;
		cout << "Enter Pincode\n";
		cin >> pincode;
		loginfail = !LoadUserInfo(accountnum, pincode);

	} while (loginfail);
	PrintFinalCard();
}

int main() {
	PrintLoginScreen();
	return 0;
}