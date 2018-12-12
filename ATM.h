#ifndef _ATM_H
#define _ATM_H

#include "Accounts.h"
#include "Bank.h"


class ATM {
public:
	ATM(string num, Bank* p_bank):num(num),p_bank(p_bank){};
	void openAccount(string accNum,string pass,int initMoney);
	void withdraw(string accNum,string pass,int moneyOut);
	void deposit(string accNum,string pass,int moneyIn);
	void balanc(string accNum,string pass);
	void makeVip(string accNum,string pass);
	void transfer(string accFromNum,string pass,string accToNum,int money);
private:
	string num;
	Bank* p_bank;
	


};
#endif
