#include "Bank.h"

int main(int argc, char* argv[])
{
	// less than 3 arguments means it was called incorrectly
	if (argc < 3)
	{
		cerr << "illegal arguments" << endl;
		exit(1);
	}
	// check if N matches number of text inputs
	int N = atoi(argv[1]);
	if (argc != N + 2)
	{
		cerr << "illegal arguments" << endl;
		exit(1);
	}
	// seed the rand function
	srand(time(NULL));//????
	Bank ourBank; // this is the main bank object
	Bank* p_bank=&ourBank;
	//ATM* ATM_arr = new ATM[N]; // allocate array of N ATMs


	for (int i = 1; i <= N; i++)
	{
		ATM newATM = ATM(i,p_bank);
		ourBank.ATMs.push_back(newATM); // initialize them
	}
	/* till here i got-RAN NAVON */
	pthread_t *ATMs_th = new pthread_t[N]; // array of N threads for the ATMs
	for (int i = 0; i < N; i++)
	{
		ourBank.ATMs[i].setCmds(argv[i + 2]); // pass text file to each ATM
		if (pthread_create(&ATMs_th[i], NULL, ATM::ExeCmd, (void*)ourBank.ATMs[i])) // create threads- need add parsing and exe methods
		{
			cerr << "Error creating thread" << endl;
			exit(1);
		}
	}
	pthread_t Commission_th, PrintAccounts_th;
	// Commission collection thread
	if (pthread_create(&Commission_th, NULL, Bank, (void*)p_bank))// nedd to change and add the commosion method
	{
		cerr << "Error creating thread" << endl;
		exit(1);
	}
	// Print Accounts thread
	if (pthread_create(&PrintAccounts_th, NULL, Bank::PrintAccounts, (void*)p_bank))//nedd to change and add the print method
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
	//delete[] ATM_arr;

	return 0;
}
