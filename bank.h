#ifndef _BANK_H
#define _BANK_H

#include "ATM.h"
#include <vector>
#define SLEEP_TIME 1
#define PRINT_ACCOUNTS_PERIOD 500000
#define COMMISSION_PERIOD 3
#define MAX_PERCENTAGE 4
#define MIN_PERCENTAGE 2
#define ATM_SLEEP_TIME 100000

class bank{

public:
	bank();
	void* comission(void* bank_);
	string openAcc(string AccNum, string password,string atm_id, int initial_amount); //------------ need to check that the password is of 4 digits if not- print: "illegal arguments" as in the instructions!!!!!!!!!!1
	void* printAccounts(void* bank_);
	Account* get_account(string accNum); // in this method- check if the iterator points to: accounts.end(), if it does it means that the specified account does not exist
// if it does not eaxist, return null

private:
	vector<ATM> ATMs;
	vector<Account> accounts;
	Account the_bank_acc;

};

#endif
