
 #ifndef _ACCOUNT_H
#define _ACCOUNT_H
#define SLEEP_TIME 1

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <cstring>
#include <vector>


typedef enum{read_, write_} ReadWrite;
typedef enum{vip_, balance_} SpecificLock;
typedef enum{not_transfer, transfer_} is_transfer;

using namespace std;

class Account{
public:
Account(int accNum,string pass,int initMoney,bool VIP_);
	int get_AccountNum(){return AccountNum;}
	string withdraw(int amount, string password, string atm_id, is_transfer is_transfer_);
	string deposit(int amount, string password, string atm_id);
	void convert_to_vip(string password,string atm_id);
	string getBalance(string password,string atm_id);
	bool is_vip(){return VIP;}
	bool PasswordCheck(string pass){return !(pass.compare(Password));}
	void print_status();
	void lock(SpecificLock L, ReadWrite readWrite); // should be used in ATM methods!
	void unlock(SpecificLock L, ReadWrite readWrite); // should be used in ATM methods!


private:
		int read_count_vip;
		int read_count_balance;
		int AccountNum;
		string Password;
		int balance;
		bool VIP;
		pthread_mutex_t vip_write_lock;
		pthread_mutex_t vip_read_lock;
		pthread_mutex_t balance_write_lock;
		pthread_mutex_t balance_read_lock;

};
#endif
