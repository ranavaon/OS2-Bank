#ifndef _Bank_H
#define _Bank_H

#include "Account.h"
#include <vector>
#include <map>
#define SLEEP_TIME 1
#define PRINT_ACCOUNTS_PERIOD 500000
#define COMMISSION_PERIOD 3
#define MAX_PERCENTAGE 4
#define MIN_PERCENTAGE 2
#define ATM_SLEEP_TIME 1

class Bank{

public:
	Bank(int ATM_num_);
	static void* comission(void* Bank_);
	string openAcc(int AccNum, string password,string atm_id, int initial_amount);
	static void* printAccounts(void* Bank_);
	void decrease_atm_num();
	Account* get_account(int accNum);
	~Bank();
	void print_to_log(string to_print);
private:
	int ATM_num; // functioning atms
	map<int,Account*> accounts;
	ofstream	log_file;
	pthread_mutex_t ATM_num_lock;
	pthread_mutex_t log_file_lock;


};

#endif
