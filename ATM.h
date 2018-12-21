
 #ifndef _ATM_H
#define _ATM_H

#include "Account.h"
#include "bank.h"


class ATM {
public:
	ATM(string num, Bank* p_bank, string input_path):num(num),p_bank(p_bank){parse_input(input_path);}
	void parse_input(string input_path);
	void withdraw(string accNum,string pass,int moneyOut);
	void deposit(string accNum,string pass,int moneyIn);
	void balance(string accNum,string pass);
	void makeVip(string accNum,string pass);
	void transfer(string accFromNum,string pass,string accToNum,int amount);
private:
	string num;
	Bank* p_bank;



};
#endif
