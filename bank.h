/*
 * bank.h
 *
 *  Created on: Dec 23, 2018
 *      Author: os
 */
#ifndef _BANK_H
#define _BANK_H

#include "Account.h"
#include <vector>
#include <map>
#define SLEEP_TIME 1
#define PRINT_ACCOUNTS_PERIOD 500000
#define COMMISSION_PERIOD 3
#define MAX_PERCENTAGE 4
#define MIN_PERCENTAGE 2
#define ATM_SLEEP_TIME 1

class bank{

public:
	bank(int ATM_num_);
	static void* comission(void* bank_);
	string openAcc(int AccNum, string password,string atm_id, int initial_amount); //------------ need to check that the password is of 4 digits if not- print: "illegal arguments" as in the instructions!!!!!!!!!!1
	static void* printAccounts(void* bank_);
	//void increase_atm_num(){ATM_num++;}
	void decrease_atm_num();
	Account* get_account(int accNum); // in this method- check if the iterator points to: accounts.end(), if it does it means that the specified account does not exist
// if it does not eaxist, return null
	~bank();
	void print_to_log(string to_print);
private:
	int ATM_num; // functioning atms
	map<int,Account*> accounts;
	ofstream	log_file;
	pthread_mutex_t ATM_num_lock;
	pthread_mutex_t log_file_lock;


};

#endif
