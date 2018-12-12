#ifndef _ACCOUNT_H
#define _ACCOUNT_H

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

using namespace std;

class Account{
public:
Account(string accNum,string pass,int initMoney);
	string withdraw(string accNum,string pass,int money);
	string deposit(string accNum,string pass,int money);
	int getBalance(){return balance;}
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
