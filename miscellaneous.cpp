#include "miscellaneous.h"

//error functions
int socketError(int sock)
{
    if (sock < 0){
		perror("[-]Socket error");
		exit(1);
	}
    //cout << "[+]Server socket created." << endl;
	return 0;
}
int bindError(int bind)
{
    if (bind < 0){
		perror("[-]Bind error");
		exit(1);
	}
    //cout << "[+]Bind to the port number:" << endl;
	return 0;
}

int listenError(int listen)
{
    if (listen < 0){
		perror("[-]Listen error");
		exit(1);
	}
    //cout << "Listening..." << endl;
	return 0;
}

int connectError(int connect)
{
    if (connect < 0){
		perror("[-]Connect error");
		exit(1);
	}
    //cout << "Connected to the server." << endl;
	return 0;
}

int acceptError(int acc)
{
    if (acc < 0){
		perror("[-]Accept error");
		exit(1);
	}
    //cout << "Accepting..." << endl;
	return 0;
}

int sendError(int send)
{
    if (send < 0){
		perror("[-]Send error");
		exit(1);
	}
    //cout << "Sending..." << endl;
	return 0;
}

int recvError(int recv)
{
    if (recv < 0){
		perror("[-]Receive error");
		exit(1);
	}
    //cout << "Receiving..." << endl;
	return 0;
}


//backend server functions
int check_wallet(string username,string filename)
{
	int wallet=0;
	string transaction, word;
	vector <string> vect;
	fstream MyReadFile(filename);
	while (getline (MyReadFile, transaction)) {
		stringstream s(transaction);
		while (s >> word)
		{
			vect.push_back(word);
		}
		if (vect[1].compare(username)==0)
		{
			wallet-=stoi(vect[3]);
		}

		else if (vect[2].compare(username)==0)
		{
			wallet+=stoi(vect[3]);
		}
		vect.clear();
	}
	MyReadFile.close();
	return wallet;		
}

vector<string> tx_coins(string username1, string username2, int transaction_amount, string filename)
{
	int wallet1=0;
	int count1=0;
	int count2=0;
	int max_sequence_number=-1;
	string transaction, word;
	vector <string> vect;
	fstream MyReadFile(filename);
	while (getline (MyReadFile, transaction)) {
		stringstream s(transaction);
		while (s >> word)
		{
			vect.push_back(word);
		}
		if(vect.size()==0)
		{
			break;
		}
		
		if (vect[1].compare(username1)==0)
		{
			wallet1-=stoi(vect[3]);
			count1+=1;
		}

		if (vect[2].compare(username1)==0)
		{
			wallet1+=stoi(vect[3]);
			count1+=1;
		}

		if(vect[1].compare(username2)==0)
		{
			count2+=1;
		}

		if(vect[2].compare(username2)==0)
		{
			count2+=1;
		}

		if (stoi(vect[0])>max_sequence_number)
		{
			max_sequence_number=stoi(vect[0]);
		}
		vect.clear();
	}
	MyReadFile.close();

	vect.push_back(to_string(wallet1));
	vect.push_back(to_string(count1));
	vect.push_back(to_string(count2));
	vect.push_back(to_string(max_sequence_number));
	
	return vect;		
}

void tx_list(char *msg, string filename)
{
	string transaction, word;
	fstream MyReadFile(filename);
	char tx[MAXDATASIZE];
	int i=0;
	while (getline (MyReadFile, transaction)) {
		if(i!=0)
		{
			transaction=","+transaction;
		}
		else
		{
			i+=1;
		}

		strcpy(tx,transaction.c_str());
		strcat(msg,tx);		
	}
	MyReadFile.close();		
}

void compute_stats(char *msg,string username,string filename)
{
	vector <string> vect;
	string transaction, word;
	fstream MyReadFile(filename);
	int i=0;
	while (getline (MyReadFile, transaction)) {
		stringstream s(transaction);
		while (s >> word)
		{
			vect.push_back(word);
		}

		if (vect[1].compare(username)==0 || vect[2].compare(username)==0)
		{
			char tx[MAXDATASIZE];
			if(i!=0)
			{
				transaction=","+transaction;
			}
			else
			{
				i+=1;
			}
			strcpy(tx,transaction.c_str());
			strcat(msg,tx);		
		}
		vect.clear();
	}
	MyReadFile.close();

}