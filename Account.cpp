#include "Account.h"

		string Password;
		int balance;
		bool VIP;
Account :: Account(int accNum,string pass,int initMoney,bool VIP_): AccountNum(accNum),Password(pass),balance(initMoney),VIP(VIP_){
	read_count_vip =0;
	read_count_balance = 0;
	if(pthread_mutex_init(&this->vip_read_lock, NULL)) // init read lock mutex
	{
		cerr << "Error initializing mutex" << endl;
		exit(1);
	}
	if (pthread_mutex_init(&this->vip_write_lock, NULL)) // init write lock mutex
	{
		cerr << "Error initializing mutex" << endl;
		exit(1);
	}
	if (pthread_mutex_init(&this->balance_write_lock, NULL)) // init write lock mutex
	{
		cerr << "Error initializing mutex" << endl;
		exit(1);
	}
	if (pthread_mutex_init(&this->balance_read_lock, NULL)) // init write lock mutex
	{
		cerr << "Error initializing mutex" << endl;
		exit(1);
	}
	sleep(SLEEP_TIME);
}


string Account :: withdraw(int amount, string password, string atm_id, is_transfer is_transfer_){ // lock and unlock should be used in ATM method!! or if not possible, make a wraper method for this one, to encapsulate it between lock and unlock
	if(!PasswordCheck(password) && password.compare("bank_commission"))
		return "Error " + atm_id + ": Your transaction failed – password for account id " + to_string(this->AccountNum) + " is incorrect "; // "Error <ATM ID>" should be added in front of this string in the function which called this method
	//this->lock(&this->balance_write_lock,&this->balance_read_lock, write_);
	if(amount > this-> balance){
		//this->unlock(&this->balance_write_lock,&this->balance_read_lock, write_);
		return "Error " + atm_id + ": Your transaction failed – account id " + to_string(this->AccountNum) + " balance is lower than " + std::to_string(amount); // "Error <ATM ID>" should be added in front of this string in the function which called this method
	}
	this->balance-=amount;
	//this->unlock(&(this->balance_write_lock),&(this->balance_read_lock), write_);
	if(!is_transfer_){
		return atm_id + ": Account " + to_string(this->AccountNum)+ " new balance is " +  std::to_string(this-> balance) + " after " + std::to_string(amount) + " $ was withdrew"; // <ATM ID> should be added in front of this string in the function which called this method
	}
	return "transfer_withdraw_success";
}


string Account :: deposit(int amount, string password, string atm_id){ // lock and unlock should be used in ATM method! or if not possible, make a wraper method for this one, to encapsulate it between lock and unlock
	if(!PasswordCheck(password) && password.compare("transfer"))
		return "Error " + atm_id + ": Your transaction failed – password for account id " + to_string(this->AccountNum) + " is incorrect "; // "Error <ATM ID>" should be added in front of this string in the function which called this method
	this->balance+=amount;
	if(password.compare("transfer"))
		return atm_id + ": Account " + to_string(this->AccountNum) + " new balance is " + std::to_string(this-> balance) + " after " + std::to_string(amount) + " $ was deposited"; // <ATM ID> should be added in front of this string in the function which called this method
	return "transfer_deposit_success";
}

void Account :: convert_to_vip(string password,string atm_id){
	if (!PasswordCheck(password))
		throw "Error " + atm_id + ": Your transaction failed – password for account id " + to_string(this->AccountNum) + " is incorrect ";
	VIP = true;
}


string Account :: getBalance(string password, string atm_id){
	if(!password.compare("bank")){
		return std::to_string(this->balance);
	}
	if(!password.compare("transfer"))
			return  std::to_string((this->balance)) ;
	if (PasswordCheck(password)){
		return atm_id + ": Account " + to_string(this->AccountNum) + " balance is " + std::to_string(this->balance);
	}
	return "Error " + atm_id + ": Your transaction failed – password for account id " + to_string(this->AccountNum) + " is incorrect ";
}

void Account :: print_status(){ // need to add lock and unlock here!
	cout << AccountNum << " status is: balance:" << balance << ", passowrd: " << Password << endl;
}


void Account :: lock(SpecificLock L, ReadWrite readWrite){
	pthread_mutex_t* wrLock;
	pthread_mutex_t* rdLock;
	int* read_count;

	switch (L){
		case vip_:
			wrLock =  &(this->vip_write_lock);
			rdLock = &(this->vip_read_lock);
			read_count = &(this->read_count_vip);
			break;
		case balance_:
			wrLock =  &this->balance_write_lock;
			rdLock = &this->balance_read_lock;
			read_count = &(this->read_count_balance);
			break;
	}
	switch (readWrite){
		case read_:
			if (pthread_mutex_lock(rdLock))
			{
				cerr << "Error locking mutex" << endl;
				exit(1);
			}
			*read_count++;
			if (*read_count == 1)
				if (pthread_mutex_lock(wrLock))
				{
					cerr << "Error unlocking mutex" << endl;
					exit(1);
				}
			if (pthread_mutex_unlock(rdLock))
			{
				cerr << "Error unlocking mutex" << endl;
				exit(1);
			}
		break;
		case write_:
			if (pthread_mutex_lock(wrLock))
			{
				cerr << "Error unlocking mutex" << endl;
				exit(1);
			}
		break;
	}
}



void Account :: unlock(SpecificLock L, ReadWrite readWrite){
	pthread_mutex_t* wrLock;
	pthread_mutex_t* rdLock;
	int* read_count;
	switch (L){
		case vip_:
			wrLock =  &(this->vip_write_lock);
			rdLock = &(this->vip_read_lock);
			read_count = &(this->read_count_vip);
			break;
		case balance_:
			wrLock =  &this->balance_write_lock;
			rdLock = &this->balance_read_lock;
			read_count = &(this->read_count_balance);
			break;
	}
	switch (readWrite){
		case read_:
			if (pthread_mutex_lock(rdLock))
			{
				cerr << "Error locking mutex" << endl;
				exit(1);
			}
			*read_count--;
			if (*read_count == 0)
				if (pthread_mutex_unlock(wrLock))
				{
					cerr << "Error unlocking mutex" << endl;
					exit(1);
				}
			if (pthread_mutex_unlock(rdLock))
			{
				cerr << "Error unlocking mutex" << endl;
				exit(1);
			}
		break;
		case write_:
			if (pthread_mutex_unlock(wrLock))
			{
				cerr << "Error unlocking mutex" << endl;
				exit(1);
			}
		break;
	}
}

