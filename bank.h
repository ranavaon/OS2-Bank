#ifndef _BANK_H
#define _BANK_H

#include "ATM.h"
#include <vector>
class Bank{

public:
	Bank();
	void comission();
	void openAcc(string AccNum, string password, int initial_amount); //------------ need to check that the password is of 4 digits if not- print: "illegal arguments" as in the instructions!!!!!!!!!!1
	void printAccounts();
	Account* get_account(string accNum); // in this method- check if the iterator points to: accounts.end(), if it does it means that the specified account does not exist
// if it does not eaxist, return null

private:
	vector<ATM> ATMs;
	vector<Account> accounts;

};

#endif
