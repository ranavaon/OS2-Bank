#ifndef _BANK_H
#define _BANK_H

#include "ATM.h"
#include <vector>
class Bank{

public:
	Bank();
	void comission();


private:
	vector<ATM> ATMs;
	vector<Account> accounts;

};

#endif
