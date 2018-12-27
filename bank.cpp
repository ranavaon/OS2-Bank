#include "bank.h"
#define BANK_INDEX 1

bank::bank(int ATM_num_):ATM_num(ATM_num_) {
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
	this->log_file.open("log_file.txt");
	Account* THE_BANK_ACC = new Account(BANK_INDEX,"1111",0,true);
	this->accounts[BANK_INDEX]=THE_BANK_ACC;
}

bank::~bank() {
	for(map<int, Account*>::iterator it = this->accounts.begin(); it != this->accounts.end(); ++it){
		delete ((*it).second);
	}
	this->log_file.close();
	pthread_mutex_destroy(&this->ATM_num_lock);
	pthread_mutex_destroy(&this->log_file_lock);
}
void bank::print_to_log(string to_print){
	pthread_mutex_lock(&this->log_file_lock);
	this->log_file<<to_print<<endl;
	pthread_mutex_unlock(&this->log_file_lock);

}
void* bank::comission(void* bank_){//need to be locked and unlocked
	bank* p_bank = (bank*)bank_;
	while(true){
				pthread_mutex_lock(&(p_bank->ATM_num_lock));
				if(p_bank->ATM_num<=0){
					pthread_mutex_unlock(&(p_bank->ATM_num_lock));
					break;
				}
				pthread_mutex_unlock(&(p_bank->ATM_num_lock));
				double percentage = MIN_PERCENTAGE + ((double)rand() / RAND_MAX) * (MAX_PERCENTAGE - MIN_PERCENTAGE); // generate random percentage
				for (map<int , Account*>::iterator it = p_bank->accounts.begin(); it != p_bank->accounts.end(); ++it)
				{
					(*it).second->lock(vip_,read_); // lock the vip status of this account
					if(!(*it).second->is_vip()){ //if(!p_bank->accounts[it._M_current]->is_vip())
						(*it).second->lock(balance_,write_);
						//cout << " account is write is locked" << endl;
						p_bank->accounts[BANK_INDEX]->lock(balance_,write_);// lock the bank account to add the commission

						//cout << "calculating commission" << endl;
						int theft = (int)(percentage*(atoi((*it).second->getBalance("bank","bank").c_str()) / 100) + 0.5); // calculate nearest int of percentage
						//cout << "taking commission" << endl;
						(*it).second->withdraw(theft,"bank_commission","",transfer_);
						//(*it)->unlock(balance_,read_);
						p_bank->accounts[BANK_INDEX]->deposit(theft,"transfer","");
						p_bank->print_to_log( "Bank: commissions of " + to_string(percentage) + " % were charged, the bank gained " + to_string(theft) + " $ from account " + to_string((*it).second->get_AccountNum()));
						p_bank->accounts[BANK_INDEX]->unlock(balance_,write_);//unlock the bank account
						(*it).second->unlock(balance_,write_); // unlock current account
					}
					(*it).second->unlock(vip_,read_);
				}
				//p_bank->Unlock(READ); // unlock map
				sleep(COMMISSION_PERIOD);
	}
			pthread_exit(NULL);//needed?
}

Account* bank::get_account(int accNum){
	if (accNum == BANK_INDEX )
		return NULL;
	for (map<int , Account*>::iterator it=this->accounts.begin(); it != this->accounts.end(); it++ ){
		if(((*it).second->get_AccountNum())==accNum){
			return &(*it->second);
		}

	}
	return NULL;
}
string bank::openAcc(int AccNum, string password,string atm_id, int initial_amount){
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
void* bank::printAccounts(void* bank_){

	bank* p_bank = (bank*)bank_;
	while(true){
		pthread_mutex_lock(&(p_bank->ATM_num_lock));
		if(p_bank->ATM_num<=0){
			pthread_mutex_unlock(&(p_bank->ATM_num_lock));
			break;
		}
		pthread_mutex_unlock(&(p_bank->ATM_num_lock));
		if(p_bank->accounts.size() > 1){
			p_bank->accounts[BANK_INDEX]->lock(balance_,read_);
			printf("\033[2J");
			printf("\033[1;1H");
			cout << "Current Bank Status" << endl;
			for (map<int, Account*>::iterator it = (p_bank->accounts.begin()); it != p_bank->accounts.end(); ++it)
			{
				if((*it).second->get_AccountNum() == BANK_INDEX)
					it++;
				(*it).second->lock(balance_,read_);
				(*it).second->lock(vip_,read_); // lock account-how to?
				(*it).second->print_status();
				//cout << "Account " << p_bank->accounts[it._M_current]->get_AccountNum() << ": Balance - " <<
					//	p_bank->accounts[it._M_current]->getBalance("bank","bank") << " $ , Account Password - " << p_bank->accounts[it._M_current]->getPassword() << endl;
				(*it).second->unlock(vip_,read_);
				(*it).second->unlock(balance_,read_); // unlock account
			}
			cout << "The Bank has " << p_bank->accounts[BANK_INDEX]->getBalance("bank","bank") << " $" << endl;
			p_bank->accounts[BANK_INDEX]->unlock(balance_,read_);
		}

		usleep(PRINT_ACCOUNTS_PERIOD);

	}
	pthread_exit(NULL);
}

void bank :: decrease_atm_num(){
	pthread_mutex_lock(&this->ATM_num_lock);
	ATM_num--;
	pthread_mutex_unlock(&this->ATM_num_lock);
}

