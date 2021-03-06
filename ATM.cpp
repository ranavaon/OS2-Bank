#include "ATM.h"



//********************************************
// function name: parse_input
// Description: parses the commands from the input file and executes them
// Parameters:  atm- pointer to the specific functioning atm
// Returns: nothing
//**************************************************************************************
void* ATM :: parse_input(void* atm){
	ATM* p_atm = (ATM*)atm;

	ifstream cmds;
	cmds.open(p_atm->input_path);
	string crnt_cmd;
	vector<string> tokens;
	while(getline(cmds, crnt_cmd)){
		p_atm->split(crnt_cmd, &tokens);
		if (!tokens.size()){
			continue;
		}// handle here all commands execution!
		if(!tokens[0].compare("O")){ // open account
			if(tokens.size() != 4){
				cerr << "Not enough arguments provided for oppening an account" << endl;
				exit(1);
			}
			if(p_atm->p_Bank->get_account(atoi(tokens[1].c_str())) != NULL){
				p_atm->p_Bank->print_to_log("Error " + p_atm->num+ ": Your transaction failed – account with the same id exists");
				continue;
			}
			string did_open=p_atm->p_Bank->openAcc(atoi(tokens[1].c_str()), tokens[2],p_atm->num, atoi(tokens[3].c_str()));
			p_atm->p_Bank->print_to_log(did_open);
		}
		else if (!tokens[0].compare("L")){ // make VIP
			if(tokens.size() != 3){
				cerr << "Not enough arguments provided for making an account vip" << endl;
				exit(1);
			}
			p_atm->makeVip(atoi(tokens[1].c_str()), tokens[2]);
		}
		else if(!tokens[0].compare("D")){ // deposit
			if(tokens.size() != 4){
				cerr << "Not enough arguments provided for deposit" << endl;
				exit(1);
			}
			p_atm->deposit(atoi(tokens[1].c_str()), tokens[2], atoi(tokens[3].c_str()));
		}
		else if(!tokens[0].compare("W")){ //withdraw
			if(tokens.size() != 4){
				cerr << "Not enough arguments provided for withdraw" << endl;
				exit(1);
			}
			p_atm->withdraw(atoi(tokens[1].c_str()), tokens[2], atoi(tokens[3].c_str()));
		}
		else if(!tokens[0].compare("B")){ //balance
			if(tokens.size() != 3){
				cerr << "Not enough arguments provided for balance" << endl;
				exit(1);
			}
			p_atm->balance(atoi(tokens[1].c_str()), tokens[2]);
		}
		else if(!tokens[0].compare("T")){ // transfer
			if(tokens.size() != 5){
				cerr << "Not enough arguments provided for transfer" << endl;
				exit(1);
			}
			p_atm->transfer(atoi(tokens[1].c_str()), tokens[2], atoi(tokens[3].c_str()), atoi(tokens[4].c_str()));
		}
		else
			cerr << "not a legal command!" << endl;
	}
	p_atm->p_Bank->decrease_atm_num();
	pthread_exit(NULL);
}


//********************************************
// function name: withdraw
// Description: sends the withdraw command to the specified account and prints the result
// Parameters:  accNum- the account to withdraw from, pass- the account's password, moneyOut- amount of money to withdraw
// Returns: nothing
//**************************************************************************************
void ATM :: withdraw(int accNum,string pass,int moneyOut){
	Account* pAcc = p_Bank->get_account(accNum); 
	if(pAcc == NULL){
		this->p_Bank->print_to_log ("Error " + this->num + ": Your transaction failed – account id " + to_string(accNum) + " does not exist");
		return;
	}
	pAcc->lock(balance_,write_);
	string result = pAcc->withdraw(moneyOut, pass, this->num, not_transfer);
	this->p_Bank->print_to_log(result);
	sleep(ATM_SLEEP_TIME);
	pAcc->unlock(balance_,write_);
}


//********************************************
// function name: deposit
// Description: sends the deposit command to the specified account and prints the result
// Parameters:  accNum- the account to deposit from, pass- the account's password, moneyOut- amount of money to deposit
// Returns: nothing
//**************************************************************************************
void ATM :: deposit(int accNum,string pass,int moneyIn){
	Account* pAcc = p_Bank->get_account(accNum);
	if(pAcc == NULL){
		this->p_Bank->print_to_log ("Error " + this->num + ": Your transaction failed – account id " + to_string(accNum) + " does not exist");
		return;
	}
	pAcc->lock(balance_,write_);
	string result = pAcc->deposit(moneyIn, pass, this->num);
	this->p_Bank->print_to_log(result);
	sleep(ATM_SLEEP_TIME);
	pAcc->unlock(balance_,write_);
}

//********************************************
// function name: balance
// Description: prints the balance of a specific account
// Parameters:  accNum- the account to print its status from, pass- the account's password
// Returns: nothing
//**************************************************************************************
void ATM :: balance(int accNum,string pass){
	Account* pAcc = p_Bank->get_account(accNum);
	if(pAcc == NULL){
		this->p_Bank->print_to_log ("Error " + this->num + ": Your transaction failed – account id " + to_string(accNum) + " does not exist");
		return;
	}
	pAcc->lock(balance_,read_);
	string result = pAcc->getBalance(pass, this->num);
	this->p_Bank->print_to_log(result);
	sleep(ATM_SLEEP_TIME);
	pAcc->unlock(balance_,read_);
}

//********************************************
// function name: makeVip
// Description: activates a specific acount's convert_to_vip method
// Parameters:  accNum- the account to convert, pass- the account's password
// Returns: nothing
//**************************************************************************************
void ATM :: makeVip(int accNum,string pass){
	Account* pAcc = p_Bank->get_account(accNum);
	if(pAcc == NULL){
		this->p_Bank->print_to_log ("Error " + this->num + ": Your transaction failed – account id " + to_string(accNum) + " does not exist");
		return;
	}
	pAcc->lock(vip_,write_);
	try{
		pAcc->convert_to_vip(pass, this->num);
	}
	catch(string &e){
		this->p_Bank->print_to_log(e);
	}
	sleep(ATM_SLEEP_TIME);
	pAcc->unlock(vip_,write_);
}

//********************************************
// function name: transfer
// Description: transfers money between accounts
// Parameters:  accFromNum- the account to withdraw money from, pass- the account's password, accToNum- the account to pass the money to, amount- the amount of money to transfer
// Returns: nothing
//**************************************************************************************
void ATM :: transfer(int accFromNum,string pass,int accToNum,int amount){
	Account* pAcc_from = p_Bank->get_account(accFromNum);
	Account* pAcc_to = p_Bank->get_account(accToNum);
	string result_from;
	string result_to;
	if(pAcc_from == NULL){
		this->p_Bank->print_to_log ("Error " + this->num + ": Your transaction failed – account id " + to_string(accFromNum) + " does not exist");
		sleep(ATM_SLEEP_TIME);
		return;
	}
	if(pAcc_to == NULL){
		this->p_Bank->print_to_log ("Error " + this->num + ": Your transaction failed – account id " + to_string(accToNum) + " does not exist");
		sleep(ATM_SLEEP_TIME);
		return;
	}
	Account* lock_first;
	Account* lock_second;
	if(accFromNum>accToNum){
		lock_first = pAcc_from;
		lock_second = pAcc_to;
	}
	else{
		lock_first = pAcc_to;
		lock_second = pAcc_from;
	}
	lock_first->lock(balance_,write_);
	lock_second->lock(balance_,write_);
	result_from = pAcc_from->withdraw(amount, pass, this->num, transfer_);
	if(result_from.compare("transfer_withdraw_success")){
		this->p_Bank->print_to_log(result_from);
		lock_second->unlock(balance_,write_);
		lock_first->unlock(balance_,write_);
		return;
	}
	result_to = pAcc_to->deposit(amount, "transfer", this->num);
	if(result_to.compare("transfer_deposit_success")){
		this->p_Bank->print_to_log(result_to);
		lock_second->unlock(balance_,write_);
		lock_first->unlock(balance_,write_);
		return;
	}
	this->p_Bank->print_to_log( this->num + ": Transfer " +to_string(amount) + " from account " + to_string(accFromNum) +" to account " + to_string(accToNum) + " new account balance is " + pAcc_from->getBalance("transfer", num) + " new target account balance is " + (pAcc_to->getBalance("transfer", num)));
	sleep(ATM_SLEEP_TIME);
	lock_second->unlock(balance_,write_);
	lock_first->unlock(balance_,write_);
}

// splits a string- for parser use
void ATM :: split(string toSplit,  vector<string>* dest){
	string temp;
	stringstream strStrm(toSplit);
	dest->clear();
	while(strStrm >> temp){
		dest->push_back(temp);
	}
}









