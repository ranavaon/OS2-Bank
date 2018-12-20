#ifndef _ATM_H
#define _ATM_H

#include "Accounts.h"
#include "Bank.h"


class ATM {
public:
	ATM(string num, Bank* p_bank){}//need to finish
	void set_input_file(const char* input);//RAN NAVON- open the file to each ATM
	static void* parse_input(void* ATM);//RAN NAVON-added parsing method
	void withdraw(string accNum,string pass,int moneyOut);
	void deposit(string accNum,string pass,int moneyIn);
	void balance(string accNum,string pass);
	void makeVip(string accNum,string pass);
	void transfer(string accFromNum,string pass,string accToNum,int amount);
private:
	string num; //RAN NAVON- changed it to int?- the string makes problems with the initilize
	Bank* p_bank;
	ifstream input_file;//RAN NAVON- changed beacause we needed open file
	


};
#endif
