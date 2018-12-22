/*
 * bank.cpp
 *
 *  Created on: Dec 21, 2018
 *      Author: os
 */
/* STEEL NEED A BIT OF FIXING- LOCK UNLOCK AND deposit to the bank and withdraw from non vip accounts*/
#include "bank.h"

bank::bank() {
	// TODO Auto-generated constructor stub
	Account THE_BANK_ACC("1","1111",0,true);
	this->accounts.push_back(THE_BANK_ACC);
}

/*bank::~bank() {//needed?
	// TODO Auto-generated destructor stub
}*/
void* bank::comission(void* bank_){//need to be locked and unlocked
	bank* p_bank = (bank*)bank_;
			{
				double percentage = MIN_PERCENTAGE + ((double)rand() / RAND_MAX) * (MAX_PERCENTAGE - MIN_PERCENTAGE);; // generate random percentage

				//p_bank->Lock(READ); // collecting comm. doesn't change map strc. read lock

				for (vector<Account*>::iterator it = p_bank->accounts.begin(); it != p_bank->accounts.end(); ++it)
				{
					p_bank->accounts[it._M_current]->lock(vip_,read_); // write lock current account for collecting commission

					if(!p_bank->accounts[it._M_current]->is_vip()){
						p_bank->accounts[it._M_current]->lock(balance_,read_);
						int theft = (int)floor(percentage*(atoi(p_bank->accounts[it._M_current]->getCurrBalance().c_str()) / 100.0 + 0.5)); // calculate nearest int of percentage
						p_bank->accounts[it._M_current]->withdraw();

						p_bank->accounts[it._M_current]->unlock(balance_,read_); // unlock current account

						cout<< "Bank: commissions of " << percentage << " % were charged, the bank gained " << to_string(theft) << " $ from account " <<p_bank->accounts[it._M_current]->get_AccountNum() << endl;


						p_bank->accounts.front()->Deposit(theft);
					}
					p_bank->accounts[it._M_current]->unlock(vip_,read_);
				}

				//p_bank->Unlock(READ); // unlock map

				sleep(COMMISSION_PERIOD);
			}
//			pthread_exit(NULL);needed?
}

Account* bank::get_account(string accNum){
	if (accNum == "1")
		return NULL;
	for (vector<Account>::iterator it=this->accounts.begin(); it != this->accounts.end(); it++ ){
		if (this->accounts[it->AccountNum]==accNum){
			return this->accounts[it];
		}
	}
	return NULL;
}
string bank::openAcc(string AccNum, string password,string atm_id, int initial_amount){
	for (vector<Account>::iterator it=this->accounts.begin(); it != this->accounts.end(); it++ ){
		if (this->accounts[it->AccountNum]==AccNum){
			return "Error " << atm_id << ": Your transaction failed – account with the same id exists " ;
		}
	}
	if (initial_amount < 0 )
		return "Error"<<atm_id<< ": Your transaction failed – initial ammount is illigel ";
	Account newAccount(AccNum,password,initial_amount,false);
	this->accounts.push_back(newAccount);// check if need more
	return atm_id<<": New account id is"<< AccNum <<"with password" << password << "and initial balance"<< initial_amount;
}
void* bank::printAccounts(void* bank_){
	bank* p_bank = (bank*)bank_;
		printf("\033[2J");
		printf("\033[1;1H");
		cout << "Current Bank Status" << endl;
		for (vector<Account*>::iterator it = p_bank->accounts.begin(); it != p_bank->accounts.end(); ++it)
		{
			p_bank->accounts[it._M_current]->lock(balance_,read_);
			p_bank->accounts[it._M_current]->lock(vip_,read_); // lock account-how to?
			cout << "Account " << p_bank->accounts[it._M_current]->get_AccountNum() << ": Balance - " <<
					p_bank->accounts[it._M_current]->getCurrBalance() << " $ , Account Password - " << p_bank->accounts[it._M_current]->getPassword() << endl;
			p_bank->accounts[it._M_current]->unlock(balance_,read_); // unlock account
			p_bank->accounts[it._M_current]->unlock(vip_,read_);
		}

		cout << "The Bank has " << p_bank->accounts.front().getCurrBalance() << " $" << endl;

		usleep(PRINT_ACCOUNTS_PERIOD);

		//pthread_exit(NULL);neeeded?

}


