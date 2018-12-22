
 #ifndef _ATM_H
#define _ATM_H

#include "Account.h"
#include "bank.h"


class ATM {
public:
	ATM(string num, bank* p_bank, string input_path):num(num),p_bank(p_bank){parse_input(input_path);}
	void* parse_input(void* input_path);//changed it for the p_thread creat
	void withdraw(string accNum,string pass,int moneyOut);
	void deposit(string accNum,string pass,int moneyIn);
	void balance(string accNum,string pass);
	void makeVip(string accNum,string pass);
	void transfer(string accFromNum,string pass,string accToNum,int amount);
private:
	string num;
	bank* p_bank;



};
#endif
