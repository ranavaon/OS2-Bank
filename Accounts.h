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

typedef enum{read_, write_} ReadWrite;
typedef enum{vip_, balance_} SpecificLock;
typedef enum{not_transfer, transfer} is_transfer;

using namespace std;

class Account{
public:
	Account(string accNum,string pass,int initMoney,bool VIP_);
	string get_AccountNum();
	string withdraw(int amount, string password, string atm_id, is_transfer is_transfer_);// lock and unlock should be used in ATM method! or if not possible, make a wraper method for this one, to encapsulate it between lock and unlock (already inside the new tread made for the ATM)
	string deposit(int amount, string password, string atm_id);// lock and unlock should be used in ATM method! or if not possible, make a wraper method for this one, to encapsulate it between lock and unlock
	string convert_to_vip(string password,string atm_id);// lock and unlock should be used in ATM method! or if not possible, make a wraper method for this one, to encapsulate it between lock and unlock
	string getBalance(string password,string atm_id);// lock and unlock should be used in ATM method! or if not possible, make a wraper method for this one, to encapsulate it between lock and unlock
	bool is_vip(){return VIP;}// lock and unlock should be used in ATM method! or if not possible, make a wraper method for this one, to encapsulate it between lock and unlock
	bool PasswordCheck(string pass){return !(pass.compare(Password));}
	void lock(SpecificLock L, ReadWrite readWrite); // should be used in ATM methods!
	void unlock(SpecificLock L, ReadWrite readWrite); // should be used in ATM methods!


private:
		int read_count;
		string AccountNum;
		string Password;
		int balance;
		bool VIP;
		pthread_mutex_t vip_write_lock;
		pthread_mutex_t vip_read_lock;
		pthread_mutex_t balance_write_lock;
		pthread_mutex_t balance_read_lock;

};
#endif
