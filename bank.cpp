#include "bank.h"
#define Bank_INDEX -1
//********************************************
// function name: Bank constructor
// Description: build an instans of a Bank
// Parameters: ATM_num-how many ATMs it have
// Returns: Bank
//**************************************************************************************
Bank::Bank(int ATM_num_):ATM_num(ATM_num_) {
	if(pthread_mutex_init(&this->ATM_num_lock, NULL)) // init read lock mutex
	{
			cerr << "Error initializing mutex" << endl;
			exit(1);
	}
	if(pthread_mutex_init(&this->log_file_lock, NULL)) // init read lock mutex
	{
			cerr << "Error initializing mutex" << endl;
			exit(1);
	}
	this->log_file.open("log.txt");
	Account* THE_Bank_ACC = new Account(Bank_INDEX,"1111",0,true);
	this->accounts[Bank_INDEX]=THE_Bank_ACC;
}
//********************************************
// function name: ~Bank
// Description: distruactor for Bank elements
// Parameters: none
// Returns: none
//**************************************************************************************
Bank::~Bank() {
	for(map<int, Account*>::iterator it = this->accounts.begin(); it != this->accounts.end(); ++it){
		delete ((*it).second);
	}
	this->log_file.close();
	pthread_mutex_destroy(&this->ATM_num_lock);
	pthread_mutex_destroy(&this->log_file_lock);
}
//********************************************
// function name: print_to_log
// Description: prints messeges to the log file with mutex defense
// Parameters: string to_print
// Returns: none
//**************************************************************************************
void Bank::print_to_log(string to_print){
	pthread_mutex_lock(&this->log_file_lock);
	this->log_file<<to_print<<endl;
	pthread_mutex_unlock(&this->log_file_lock);

}
//********************************************
// function name: commision
// Description: takes the Bank's commision from all the accounts
// Parameters: void* Bank_
// Returns: none
//**************************************************************************************
void* Bank::comission(void* Bank_){//need to be locked and unlocked
	Bank* p_Bank = (Bank*)Bank_;
	while(true){
				pthread_mutex_lock(&(p_Bank->ATM_num_lock));
				if(p_Bank->ATM_num<=0){
					pthread_mutex_unlock(&(p_Bank->ATM_num_lock));
					break;
				}
				pthread_mutex_unlock(&(p_Bank->ATM_num_lock));
				double percentage = MIN_PERCENTAGE + ((double)rand() / RAND_MAX) * (MAX_PERCENTAGE - MIN_PERCENTAGE); // generate random percentage
				for (map<int , Account*>::iterator it = p_Bank->accounts.begin(); it != p_Bank->accounts.end(); ++it)
				{
					(*it).second->lock(vip_,read_); // lock the vip status of this account
					if(!(*it).second->is_vip()){ //if(!p_Bank->accounts[it._M_current]->is_vip())
						(*it).second->lock(balance_,write_);
						//cout << " account is write is locked" << endl;
						p_Bank->accounts[Bank_INDEX]->lock(balance_,write_);// lock the Bank account to add the commission

						//cout << "calculating commission" << endl;
						int theft = (int)(percentage*(atoi((*it).second->getBalance("Bank","Bank").c_str()) / 100) + 0.5); // calculate nearest int of percentage
						//cout << "taking commission" << endl;
						(*it).second->withdraw(theft,"Bank_commission","",transfer_);
						//(*it)->unlock(balance_,read_);
						p_Bank->accounts[Bank_INDEX]->deposit(theft,"transfer","");
						p_Bank->print_to_log( "Bank: commissions of " + to_string(percentage) + " % were charged, the Bank gained " + to_string(theft) + " $ from account " + to_string((*it).second->get_AccountNum()));
						p_Bank->accounts[Bank_INDEX]->unlock(balance_,write_);//unlock the Bank account
						(*it).second->unlock(balance_,write_); // unlock current account
					}
					(*it).second->unlock(vip_,read_);
				}
				//p_Bank->Unlock(READ); // unlock map
				sleep(COMMISSION_PERIOD);
	}
			pthread_exit(NULL);//needed?
}
//********************************************
// function name: get_account
// Description: bring a pointer to an account by his account number
// Parameters: int accNum-acoount number
// Returns: Account pointer
//**************************************************************************************
Account* Bank::get_account(int accNum){
	if (accNum == Bank_INDEX )
		return NULL;
	for (map<int , Account*>::iterator it=this->accounts.begin(); it != this->accounts.end(); it++ ){
		if(((*it).second->get_AccountNum())==accNum){
			return &(*it->second);
		}

	}
	return NULL;
}
//********************************************
// function name: openACC
// Description: open a new account if it doesn't exists already
// Parameters: int AccNum-account number, string password ,string atm_id-the sending ATM, int initial_amount
// Returns: string to print for sucess/fail
//**************************************************************************************
string Bank::openAcc(int AccNum, string password,string atm_id, int initial_amount){
	for (map<int ,Account*>::iterator it=this->accounts.begin(); it != this->accounts.end(); it++ ){
		if(((*it).second->get_AccountNum())==AccNum){
			return "Error " + atm_id + ": Your transaction failed – account with the same id exists " ;
		}
	}
	if (initial_amount < 0 )
		return "Error" + atm_id + ": Your transaction failed – initial ammount is illigel ";
	Account* newAccount = new Account(AccNum,password,initial_amount,false);
	this->accounts[newAccount->get_AccountNum()]=newAccount;// check if need more
	return atm_id + ": New account id is " + to_string(AccNum) + " with password " + password + " and initial balance "+ std::to_string(initial_amount);
}
//********************************************
// function name: printAccounts
// Description: prints all the accounts status every time period
// Parameters: pointer Bank_
// Returns: none
//**************************************************************************************
void* Bank::printAccounts(void* Bank_){

	Bank* p_Bank = (Bank*)Bank_;
	while(true){
		pthread_mutex_lock(&(p_Bank->ATM_num_lock));
		if(p_Bank->ATM_num<=0){
			pthread_mutex_unlock(&(p_Bank->ATM_num_lock));
			break;
		}
		pthread_mutex_unlock(&(p_Bank->ATM_num_lock));
		if(p_Bank->accounts.size() > 1){
			p_Bank->accounts[Bank_INDEX]->lock(balance_,read_);
			printf("\033[2J");
			printf("\033[1;1H");
			cout << "Current Bank Status" << endl;
			for (map<int, Account*>::iterator it = (p_Bank->accounts.begin()); it != p_Bank->accounts.end(); ++it)
			{
				if((*it).second->get_AccountNum() == Bank_INDEX)
					it++;
				(*it).second->lock(balance_,read_);
				(*it).second->lock(vip_,read_); // lock account-how to?
				(*it).second->print_status();
				(*it).second->unlock(vip_,read_);
				(*it).second->unlock(balance_,read_); // unlock account
			}
			cout << "The Bank has " << p_Bank->accounts[Bank_INDEX]->getBalance("Bank","Bank") << " $" << endl;
			p_Bank->accounts[Bank_INDEX]->unlock(balance_,read_);
		}

		usleep(PRINT_ACCOUNTS_PERIOD);

	}
	pthread_exit(NULL);
}
//********************************************
// function name: decrease_atm_num
// Description: a method to handle the number of active ATMs
// Parameters: none
// Returns: none
//**************************************************************************************
void Bank :: decrease_atm_num(){
	pthread_mutex_lock(&this->ATM_num_lock);
	ATM_num--;
	pthread_mutex_unlock(&this->ATM_num_lock);
}

