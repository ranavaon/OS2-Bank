#include "Account.h"

//********************************************
// function name: Account constructor
// Description: build an instans of an Account
// Parameters: accNum- account id, pass- account's password, initMoney- initial balance, VIP_- initial VIP status
// Returns: pointer to Account
//**************************************************************************************
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


//********************************************
// function name: withdraw
// Description: substract the the indicated amount of money from the account's balance
// Parameters: amount- aomunt of money to withdraw, password- account's password, atm_id- the id of the atm which sent the transaction
// Returns: result string
//**************************************************************************************
string Account :: withdraw(int amount, string password, string atm_id, is_transfer is_transfer_){ 
	if(!PasswordCheck(password) && password.compare("Bank_commission"))
		return "Error " + atm_id + ": Your transaction failed – password for account id " + to_string(this->AccountNum) + " is incorrect "; 
	if(amount > this-> balance){
		return "Error " + atm_id + ": Your transaction failed – account id " + to_string(this->AccountNum) + " balance is lower than " + std::to_string(amount); 
	}
	this->balance-=amount;
	if(!is_transfer_){
		return atm_id + ": Account " + to_string(this->AccountNum)+ " new balance is " +  std::to_string(this-> balance) + " after " + std::to_string(amount) + " $ was withdrew";
	}
	return "transfer_withdraw_success";
}

//********************************************
// function name: deposit
// Description: add the indicated amount of money to the account's balance
// Parameters:  amount- aomunt of money to deposit, password- account's password, atm_id- the id of the atm which sent the transaction
// Returns: result string
//**************************************************************************************
string Account :: deposit(int amount, string password, string atm_id){ 
	if(!PasswordCheck(password) && password.compare("transfer"))
		return "Error " + atm_id + ": Your transaction failed – password for account id " + to_string(this->AccountNum) + " is incorrect "; 
	this->balance+=amount;
	if(password.compare("transfer"))
		return atm_id + ": Account " + to_string(this->AccountNum) + " new balance is " + std::to_string(this-> balance) + " after " + std::to_string(amount) + " $ was deposited"; 
	return "transfer_deposit_success";
}

void Account :: convert_to_vip(string password,string atm_id){
	if (!PasswordCheck(password))
		throw "Error " + atm_id + ": Your transaction failed – password for account id " + to_string(this->AccountNum) + " is incorrect ";
	VIP = true;
}

//********************************************
// function name: getBalance
// Description: returns the balance in fue formats
// Parameters:  password- account's password, atm_id- the id of the atm which sent the transaction
// Returns: result string
//**************************************************************************************
string Account :: getBalance(string password, string atm_id){
	if(!password.compare("Bank")){
		return std::to_string(this->balance);
	}
	if(!password.compare("transfer"))
			return  std::to_string((this->balance)) ;
	if (PasswordCheck(password)){
		return atm_id + ": Account " + to_string(this->AccountNum) + " balance is " + std::to_string(this->balance);
	}
	return "Error " + atm_id + ": Your transaction failed – password for account id " + to_string(this->AccountNum) + " is incorrect ";
}


//********************************************
// function name: print_status
// Description: prints the account status(balance and password)
// Parameters:  non
// Returns: void
//**************************************************************************************
void Account :: print_status(){ // need to add lock and unlock here!
	cout << AccountNum << " status is: balance:" << balance << ", passowrd: " << Password << endl;
}

//********************************************
// function name: lock
// Description: locks vip or balance fields for writers or readers 
// Parameters:  L- the lock we wish to lock(vip or balance), readWrite- kind of lock(read or write- for readers/writers mechanism)
// Returns: void
//**************************************************************************************
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



//********************************************
// function name: unlock
// Description: unlocks vip or balance fields for writers or readers 
// Parameters:  L- the lock we wish to unlock(vip or balance), readWrite- kind of unlock(read or write- for readers/writers mechanism)
// Returns: void
//**************************************************************************************
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

