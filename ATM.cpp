#include "ATM.h"
#include <vector>


void* ATM :: parse_input(void* input_path){
	string input_file_name=(string*)input_path;
	ifstream cmds.open(input_file_name);
	string crnt_cmd;
	vector<string> tokens;
	while(get_line(cmds, crnt_cmd)){
		boost::split(tokens, crnt_cmd, boost::is_any_of(" "));
		if (!tokens.size())
			continue;
		// handle here all commands execution!
		switch(tokens[0]){
			case "O": // open account
				if(tokens.size() != 4){
					cerr << "Not enough arguments provided for oppening an account" << endl;
					exit(1);
				}
				if(this->p_bank->get_account(tokens[1]) != NULL){
					cout << "Error " << num << ": Your transaction failed – account with the same id exists" << endl;
					continue;
				}
				this->p_bank->openAcc(tokens[1], tokens[2], atoi(tokens[3].c_str()));
				cout << num << ": New account id is " << tokens[1] <<" with password " << tokens[2] << " and initial balance " << tokens[3] << endl;
			break;
			case "L": // make VIP
				if(tokens.size() != 3){
					cerr << "Not enough arguments provided for making an account vip" << endl;
					exit(1);
				}
				this->makeVip(tokens[1], tokens[2]);
			break;
			case "D": // deposit
				if(tokens.size() != 4){
					cerr << "Not enough arguments provided for deposit" << endl;
					exit(1);
				}
				this->deposit(tokens[1], tokens[2], atoi(tokens[3].c_str()));
			break;
			case "W": //withdraw
				if(tokens.size() != 4){
					cerr << "Not enough arguments provided for withdraw" << endl;
					exit(1);
				}
				this->withdraw(tokens[1], tokens[2], atoi(tokens[3].c_str()));
			break;
			case "B": //balance
				if(tokens.size() != 3){
					cerr << "Not enough arguments provided for balance" << endl;
					exit(1);
				}
				this->balance(tokens[1], tokens[2]);
			break;
			case "T": // transfer
				if(tokens.size() != 5){
					cerr << "Not enough arguments provided for transfer" << endl;
					exit(1);
				}
				this->transfer(tokens[1], tokens[2], tokens[3], atoi(tokens[4].c_str()));
			break;
		}
	}
	pthread_exit(NULL);
}


void ATM :: withdraw(string accNum,string pass,int moneyOut){
	Account* pAcc = p_bank->get_account(accNum); // in
	if(pAcc == NULL){
		cout << "Error " << num << ": Your transaction failed – account id " << accNum << " does not exist" <<endl;
		return;
	}
	pAcc->lock(balance_,write_);
	string result = pAcc->withdraw(moneyOut, pass, this->num, not_transfer);
	cout << result << endl;
	pAcc->unlock(balance_,write_);
}


void ATM :: deposit(string accNum,string pass,int moneyIn){
	Account* pAcc = p_bank->get_account(accNum);
	if(pAcc == NULL){
		cout << "Error " << num << ": Your transaction failed – account id " << accNum << " does not exist" <<endl;
		return;
	}
	pAcc->lock(balance_,write_);
	string result = pAcc->deposit(moneyIn, pass, this->num);
	cout << result << endl;
	pAcc->unlock(balance_,write_);
}


void ATM :: balance(string accNum,string pass){
	Account* pAcc = p_bank->get_account(accNum);
	if(pAcc == NULL){
		cout << "Error " << num << ": Your transaction failed – account id " << accNum << " does not exist" <<endl;
		return;
	}
	pAcc->lock(balance_,read_);
	string result = pAcc->getBalance(pass, this->num);
	cout << result << endl;
	pAcc->unlock(balance_,read_);
}


void ATM :: makeVip(string accNum,string pass){
	Account* pAcc = p_bank->get_account(accNum);
	if(pAcc == NULL){
		cout << "Error " << num << ": Your transaction failed – account id " << accNum << " does not exist" <<endl;
		return;
	}
	pAcc->lock(vip_,write_);
	try{
		string result = pAcc->getBalance(pass, this->num);
	}
	catch(string &e){
		cout << e << endl;
	}
	pAcc->unlock(vip_,write_);
}


void ATM :: transfer(string accFromNum,string pass,string accToNum,int amount){
	Account* pAcc_from = p_bank->get_account(accFromNum);
	Account* pAcc_to = p_bank->get_account(accToNum);
	string result_from;
	string result_to;
	if(pAcc_from == NULL){
		cout << "Error " << num << ": Your transaction failed – account id " << accFromNum << " does not exist" <<endl;
		return;
	}
	if(pAcc_to == NULL){
		cout << "Error " << num << ": Your transaction failed – account id " << accToNum << " does not exist" <<endl;
		return;
	}
	Account* lock_first;
	Account* lock_second;
	if(accFromNum.compare(accToNum)>0){
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
		cout << result_from << endl;
		lock_first->unlock(balance_,write_);
		lock_second->unlock(balance_,write_);
		return;
	}
	result_to = pAcc_to->deposit(amount, "transfer", this->num);
	if(result_to.compare("transfer_deposit_success")){
		cout << result_to << endl;
		lock_first->unlock(balance_,write_);
		lock_second->unlock(balance_,write_);
		return;
	}
	cout << num << ": Transfer " << amount << " from account " << accFromNum << " to account " << accToNum << " new account balance is " << pAcc_from->getBalance("transfer", num) << " new target account balance is " << pAcc_to->getBalance("transfer", num) << endl;
	lock_first->unlock(balance_,write_);
	lock_second->unlock(balance_,write_);
}

