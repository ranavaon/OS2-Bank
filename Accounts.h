#ifndef _ACCOUNT_H
#define _ACCOUNT_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

class Account{
public:
	void Account();
	string withdraw();
	string deposit();
	string getBalance();
	/*Mutex needed */

private:
		string AccountNum;
		string Password;
		int balance;
		bool VIP;
		vip mutex
		balance mutex

};
#endif
