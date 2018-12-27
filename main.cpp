/*
 * main.cpp
 *
 *  Created on: Dec 22, 2018
 *      Author: os
 */

#include "ATM.h"
//#include "bank.h"
// set argv and argc as arguments for main!!!!!!!!!!!!!
int main(int argc,char* argv[]){
	//cout << "started bank!"<< endl;
	//int argc= 3;
	//cout << "argc is "<<argc<< endl;
	//string input_file = "short_atm.txt";

	// less than 3 arguments means it was called incorrectly
		if (argc < 3)
		{
			cerr << "illegal arguments" << endl;
			exit(1);
		}
		// check if N matches number of text inputs
		int N = atoi(argv[1]);
		//cout << "number of atms "<< N << endl;

		if (argc != N + 2)
		{
			cerr << "illegal arguments" << endl;
			exit(1);
		}
		// seed the rand function
		srand(time(NULL));
		bank ourBank(N); // this is the main bank object
		bank* p_bank=&ourBank;
		vector<ATM*> ATMs; // allocate array of N ATMs

		for (int i = 0; i < N; i++)
		{
			string input_file=string(argv[i + 2]);
			ATM* newATM = new ATM(to_string(i+1),p_bank,input_file);
			ATMs.push_back(newATM); // initialize them
		}
		pthread_t *ATMs_th = new pthread_t[N]; // array of N threads for the ATMs
		for (int i = 0; i < N; i++)
		{
			//ATMs[i]->parse_input(&ATMs[i]); // pass text file to each ATM
			if (pthread_create(&ATMs_th[i], NULL, ATM::parse_input, (void*)ATMs[i])) // create threads- need add parsing and exe methods
			{
				cerr << "Error creating thread" << endl;
				exit(1);
			}
		}
		pthread_t Commission_th, PrintAccounts_th;
		// Commission collection thread
		if (pthread_create(&Commission_th, NULL, bank::comission, (void*)p_bank))// nedd to change and add the commosion method
		{
			cerr << "Error creating thread" << endl;
			exit(1);
		}
		// Print Accounts thread
		if (pthread_create(&PrintAccounts_th, NULL, bank::printAccounts, (void*)p_bank))//nedd to change and add the print method
		{
			cerr << "Error creating thread" << endl;
			exit(1);
		}
		// wait for all threads to finish
		for (int i = 0; i < N; i++) { // wait for ATMs
			if (pthread_join(ATMs_th[i], NULL))
			{
				cerr << "Error joining thread" << endl;
				exit(1);
			}
		}

		if (pthread_join(Commission_th, NULL)) // wait for commission thread
		{
			cerr << "Error joining thread" << endl;
			exit(1);
		}
		if (pthread_join(PrintAccounts_th, NULL)) // wait for print accounts threads
		{
			cerr << "Error joining thread" << endl;
			exit(1);
		}
		// free dynamically allocated memory
		delete[] ATMs_th;
		//delete[] ATMs;

		return 0;
}


