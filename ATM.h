/*
 * ATM.h
 *
 *  Created on: Dec 22, 2018
 *      Author: os
 */

 #ifndef _ATM_H
#define _ATM_H

#include "Account.h"
#include "bank.h"


class ATM {
public:
	ATM(string num_, Bank* p_Bank_, string input_path_):num(num_),p_Bank(p_Bank_),input_path(input_path_){/*parse_input(this);*/}
	static void* parse_input(void*);//);
	void withdraw(int accNum,string pass,int moneyOut);
	void deposit(int accNum,string pass,int moneyIn);
	void balance(int accNum,string pass);
	void makeVip(int accNum,string pass);
	void transfer(int accFromNum,string pass,int accToNum,int amount);
	void split(string toSplit,  vector<string>* dest);
private:
	string input_path;
	string num;
	Bank* p_Bank;
};
#endif
