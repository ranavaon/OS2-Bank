#include "ATM.h"
ATM::ATM(string num, Bank* p_bank){
	this->num=num;
	this->p_bank=p_bank;
}
void ATM::set_input_file(const char* input){
	this->input_file.open(input);
}

void* ATM :: parse_input(void* atm){
	// RAN NAVON- still need to handel mutexs if needed
	ATM* to_handel_ATM = (ATM*)atm;
	string cmd;
	vector<string> tokens;
	string buf;
	stringstream ss(cmd);


	while (getline(to_handel_ATM->input_file, cmd)) // read a line from text file
	{
		tokens.clear();
		while (ss >> buf) // this tokens by spaces
			tokens.push_back(buf); // tokenize command
		if (!tokens.size())
			continue;
		if (tokens[0].compare("O") == 0) // Open command
			to_handel_ATM->p_bank->open_Account(tokens[1],tokens[2],atoi(tokens[3].c_str()));
		else if (tokens[0].compare("D") == 0) // Deposit command
			to_handel_ATM-deposit(tokens[1],tokens[2],atoi(tokens[3].c_str()));
		else if (tokens[0].compare("W") == 0) // Withdraw command
			to_handel_ATM->withdraw(tokens[1],tokens[2],atoi(tokens[3].c_str()));
		else if (tokens[0].compare("B") == 0) // Balance command
			to_handel_ATM->balance(tokens[1],tokens[2]);
		else if (tokens[0].compare("T") == 0) // Transfer command
			to_handel_ATM->transfer(tokens[1],tokens[2],tokens[3],atoi(tokens[4].c_str()));
		else if (tokens[0].compare("L") == 0) // Transfer command
			to_handel_ATM->makeVip(tokens[1],tokens[2]);
		else
			cerr << "Unknown command" << endl;
		usleep(ATM_SLEEP_TIME);
	}

	/*
		ATM operations have ended
	*/
	//pthread_mutex_lock(&to_handel_ATM->_bank->getATMsCntLock()); // lock active atms counter
	//Bank::ActiveATMs--;
	//pthread_mutex_unlock(&to_handel_ATM->_bank->getATMsCntLock()); // unlock

	//pthread_exit(NULL);

}


void ATM :: withdraw(string accNum,string pass,int moneyOut){
	Account* pAcc = p_bank->get_account(accNum);
	pAcc->lock(balance_,write_);
	string result = pAcc->withdraw(moneyOut, pass, this->num, not_transfer);
	cout << result << endl;
	pAcc->unlock(balance_,write_);
}


void ATM :: deposit(string accNum,string pass,int moneyIn){
	Account* pAcc = p_bank->get_account(accNum);
	pAcc->lock(balance_,write_);
	string result = pAcc->deposit(moneyOut, pass, this->num);
	cout << result << endl;
	pAcc->unlock(balance_,write_);
}


void ATM :: balance(string accNum,string pass){
	Account* pAcc = p_bank->get_account(accNum);
	pAcc->lock(balance_,read_);
	string result = pAcc->getBalance(pass, this->num);
	cout << result << endl;
	pAcc->unlock(balance_,read_);
}


void ATM :: makeVip(string accNum,string pass){
	Account* pAcc = p_bank->get_account(accNum);
	pAcc->lock(vip_,write_);
	try{
		string result = pAcc->getBalance(pass, this->num);
	}
	catch(string e){
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
		cout << "Error "+num+": Your transaction failed – account id "+accFromNum+" does not exist" <<endl; 
		return;
	}
	if(pAcc_to == NULL){
		cout << "Error "+num+": Your transaction failed – account id "+accToNum+" does not exist" <<endl;
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
	result_from = pAcc_from->withdraw(amount, pass, this->num, transfer);
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
	cout << num+": Transfer "+amount+" from account "+accFromNum+" to account "+accToNum+" new account balance is "+pAcc_from->get_balance("transfer", num)+" new target account balance is "+pAcc_to->get_balance("transfer", num) << endl;
	lock_first->unlock(balance_,write_);
	lock_second->unlock(balance_,write_);
}
