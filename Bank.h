#ifndef _BANK_H
#define _BANK_H

#include "ATM.h"
#include <vector>
class Bank{

public:
	Bank();
	void open_Account(string accNum,string pass,int firstMoney);//RAN NAVON added a new account- need to be writen in bank.cpp
	static void* printAllAcounts(void*);//nRAN NAVONeed to be added
	static void* comission(void*);//RAN NAVON need to be added
	Account* get_account(string account_num);

private:
	vector<ATM> ATMs;
	vector<Account> accounts;

};

#endif
