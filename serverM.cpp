#include "miscellaneous.h"

int checkWallet(int child_sock, int sockABC, string s, string client)
{	
	struct sockaddr_in wallet_sock;
	int  send_output, recv_output;
	socklen_t wallet_sock_size;
	int wallet=1000;
	char msg[MAXDATASIZE]; 
	wallet_sock.sin_family = AF_INET;
	memset(wallet_sock.sin_zero, '\0', sizeof (wallet_sock.sin_zero));
	inet_pton(AF_INET, localhost, &(wallet_sock.sin_addr));
	string server_arr[] = {"A","B","C"};
	int arr[] = { UDP_PORTA, UDP_PORTB, UDP_PORTC };
	for (int i=0; i< 3; i++)
	{
		int PORT = arr[i];
		wallet_sock.sin_port = PORT;
		bzero(msg, MAXDATASIZE);
		strcpy(msg,s.c_str());
		wallet_sock_size=sizeof(wallet_sock);
		int len=strlen(msg);
		send_output=sendto(sockABC,msg,len,0,(struct sockaddr *) &wallet_sock, wallet_sock_size);
		sendError(send_output);
		cout<<"The main server sent a request to server " << server_arr[i] <<"."<< endl;
		bzero(msg, MAXDATASIZE);
		recv_output=recvfrom(sockABC, msg, MAXDATASIZE , 0,(struct sockaddr *) &wallet_sock, &wallet_sock_size);
		recvError(recv_output);
		cout<<"The main server received transactions from Server " << server_arr[i] << " using UDP over port " << to_string(arr[i])<< "." << endl;
		char *token = strtok(msg, " ");
		while (token != NULL)
		{
			wallet+=stoi(token);
			token = strtok(NULL, " ");
		}
	}
	bzero(msg, MAXDATASIZE);
	strcpy(msg,to_string(wallet).c_str());
	int len=strlen(msg);
	send_output= send(child_sock,msg,len,0);
	sendError(send_output);
	cout<<"The main server sent the current balance to client "<< client <<"." << endl;
	close(child_sock);
	return 0;
}

int txCoins(int child_sock, int sockABC, string s,string username1, string username2, int transaction_amount, string client)
{	
	struct sockaddr_in txcoins;
	int  send_output, recv_output;
	int username1_wallet=0;
	int sequence_number=-1;
	socklen_t txcoins_sock_size;
	char msg[MAXDATASIZE]; 
	txcoins.sin_family = AF_INET;
	memset(txcoins.sin_zero, '\0', sizeof (txcoins.sin_zero));
	inet_pton(AF_INET, localhost, &(txcoins.sin_addr));
	string server_arr[] = {"A","B","C"};
	int arr[] = { UDP_PORTA, UDP_PORTB, UDP_PORTC };
	int username1_count=0;
	int username2_count=0;
	for (int i=0; i< 3; i++)
	{
		int PORT = arr[i];
		txcoins.sin_port = PORT;
		bzero(msg, MAXDATASIZE);
		strcpy(msg,s.substr(2).c_str());
		txcoins_sock_size=sizeof(txcoins);
		int len=strlen(msg);
		send_output=sendto(sockABC,msg,len,0,(struct sockaddr *) &txcoins, txcoins_sock_size);
		sendError(send_output);
		cout<<"The main server sent a request to server " << server_arr[i] <<"."<< endl;
		bzero(msg, MAXDATASIZE);
		recv_output=recvfrom(sockABC, msg, MAXDATASIZE , 0,(struct sockaddr *) &txcoins, &txcoins_sock_size);
		recvError(recv_output);
		cout<<"The main server received feedback from Server" << server_arr[i] << "using UDP over port " << to_string(arr[i])<< "." << endl;

		vector<string> vect;
		char *token = strtok(msg, " ");
		while (token != NULL)
		{
			vect.push_back(token);
			token = strtok(NULL, " ");
		}
		username1_wallet+=stoi(vect[0]);
		username1_count+=stoi(vect[1]);
		username2_count+=stoi(vect[2]);
		if (sequence_number<stoi(vect[3]))
		{
			sequence_number=stoi(vect[3]);
		}
	}
		
	bzero(msg, MAXDATASIZE);

	if (username1_count==0 && username2_count==0)
	{
		strcpy(msg,"Both");
	}

	else if (username1_count==0)
	{
		strcpy(msg,username1.c_str());
	}

	else if (username2_count==0)
	{
		strcpy(msg,username2.c_str());
	}

	else 
	{
		username1_wallet+=1000;
		if (username1_wallet<transaction_amount)
		{
			string s= "Insufficient Balance" + (" " + to_string(username1_wallet));
			strcpy(msg,s.c_str());
		}
		
		else
		{
			string s= to_string(sequence_number+1) + " " + username1 + " " + username2 + " " + to_string(transaction_amount);
			strcpy(msg,s.c_str());
			int random_index = rand() % 3; //generates a random number between 0 and 3
			int port = arr[random_index];
			txcoins.sin_family = AF_INET;
			memset(txcoins.sin_zero, '\0', sizeof (txcoins.sin_zero));
			inet_pton(AF_INET, localhost, &(txcoins.sin_addr));
			txcoins.sin_port = port;
			txcoins_sock_size=sizeof(txcoins);
			int len=strlen(msg);
			send_output=sendto(sockABC,msg,len,0,(struct sockaddr *) &txcoins, txcoins_sock_size);
			sendError(send_output);
			bzero(msg, MAXDATASIZE);
			username1_wallet-=transaction_amount;
			strcpy(msg, to_string(username1_wallet).c_str());
			}

	}

	int len=strlen(msg);
	send_output= send(child_sock,msg,len,0);
	sendError(send_output);
	cout<<"The main server sent the result of the transaction to client "<< client <<"." << endl;
	close(child_sock);
	return 0;
}

void merge(string arr[], int l, int r, int m) 
{ 
	//l=left, m=middle, r=right
	//i=index of subarray 1, j=index of subarray 2, k=index of merged original array
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 
    string L[n1], R[n2]; 

    for (i = 0; i < n1; i++) 
        L[i] = arr[l + i]; 
    for (j = 0; j < n2; j++) 
        R[j] = arr[m + 1+ j]; 
  
    i = 0; 
    j = 0; 
    k = l; 
    while (i < n1 && j < n2) 
    { 
		//picking up the numeric sequence number from strings stored at ith index of L array and jth index of R array respectively
        string seq1,seq2;
        stringstream s1(L[i]);
        stringstream s2(R[j]);
        s1>>seq1;
        s2>>seq2;

        if (stoi(seq1)<=stoi(seq2))
        { 
            arr[k] = L[i]; 
            i++; 
        } 
        else
        { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
  

    while (i < n1) 
    { 
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 

    while (j < n2) 
    { 
        arr[k] = R[j]; 
        j++; 
        k++; 
    } 
} 
  

void mergeSort(string arr[], int l, int r) 
{ 
    if (l < r) 
    { 
        int m = l+(r-l)/2; 

        mergeSort(arr, l, m); 
        mergeSort(arr, m+1, r); 
  
        merge(arr, l, r, m); 
    } 
}

int txList(int child_sock,int sockABC)
{
	struct sockaddr_in list_sock;
	int  send_output, recv_output;
	socklen_t list_sock_size;
	char msg[MAXDATASIZE]; 
	list_sock.sin_family = AF_INET;
	memset(list_sock.sin_zero, '\0', sizeof (list_sock.sin_zero));
	inet_pton(AF_INET, localhost, &(list_sock.sin_addr));
	int arr[] = { UDP_PORTA, UDP_PORTB, UDP_PORTC };
	vector<string> all_transactions;
	for (int i=0; i< 3; i++)
	{
		int PORT = arr[i];
		list_sock.sin_port = PORT;
		bzero(msg, MAXDATASIZE);
		strcpy(msg,TXLIST);
		list_sock_size=sizeof(list_sock);
		int len=strlen(msg);
		send_output=sendto(sockABC,msg,len,0,(struct sockaddr *) &list_sock, list_sock_size);
		sendError(send_output);
		bzero(msg, MAXDATASIZE);
		recv_output=recvfrom(sockABC, msg, MAXDATASIZE , 0,(struct sockaddr *) &list_sock, &list_sock_size);
		recvError(recv_output);
		
		char *token = strtok(msg, ",");
		while (token != NULL)
		{
			all_transactions.push_back(token);
			token = strtok(NULL, ",");
		}
	}
	string all_transactions_arr[all_transactions.size()];
    copy(all_transactions.begin(),all_transactions.end(),all_transactions_arr);
    int all_transactions_arr_size = all_transactions.size();
	mergeSort(all_transactions_arr, 0, all_transactions_arr_size - 1); 

	fstream myFile;
	myFile.open("alichain.txt",fstream::out);
	if (myFile)
	{
		for (int i=0;i<all_transactions_arr_size;i++)
		{
			myFile << all_transactions_arr[i]<<endl;
		}
		myFile.close();
	}
	else
	{
		fstream createFile("alichain.txt");
		createFile.open ("alichain.txt",fstream::app|fstream::out);
		for (int i=0;i<all_transactions_arr_size;i++)
		{
			createFile << all_transactions_arr[i]<<endl;
		}
		createFile.close();
	}
	
	cout<<"The sorted file is up and ready."<<endl;
	close(child_sock);
	return 0;
}

void mergeForStats(string arr[], int l, int r, int m) 
{ 
	//l=left, m=middle, r=right
	//i=index of subarray 1, j=index of subarray 2, k=index of merged original array
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 
    string L[n1], R[n2]; 

    for (i = 0; i < n1; i++) 
        L[i] = arr[l + i]; 
    for (j = 0; j < n2; j++) 
        R[j] = arr[m + 1+ j]; 
  
    i = 0; 
    j = 0; 
    k = l; 
    while (i < n1 && j < n2) 
    { 
		//picking up the numeric sequence number from strings stored at ith index of L array and jth index of R array respectively
        string seq1,seq2;
        stringstream s1(L[i]);
        stringstream s2(R[j]);
		for(int i=0;i<2;i++)
		{
			s1>>seq1;
        	s2>>seq2;
		}
        

        if (stoi(seq1)>=stoi(seq2))
        { 
            arr[k] = L[i]; 
            i++; 
        } 
        else
        { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
  

    while (i < n1) 
    { 
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 

    while (j < n2) 
    { 
        arr[k] = R[j]; 
        j++; 
        k++; 
    } 
} 
  

void mergeSortForStats(string arr[], int l, int r) 
{ 
    if (l < r) 
    { 
        int m = l+(r-l)/2; 

        mergeSortForStats(arr, l, m); 
        mergeSortForStats(arr, m+1, r); 
  
        mergeForStats(arr, l, r, m); 
    } 
}


int computeStats(int child_sock, int sockABC, string username)
{
	struct sockaddr_in list_sock;
	int  send_output, recv_output;
	socklen_t list_sock_size;
	char msg[MAXDATASIZE]; 
	list_sock.sin_family = AF_INET;
	memset(list_sock.sin_zero, '\0', sizeof (list_sock.sin_zero));
	inet_pton(AF_INET, localhost, &(list_sock.sin_addr));
	int arr[] = { UDP_PORTA, UDP_PORTB, UDP_PORTC };
	vector<string> all_transactions;
	vector<string>unique_usernames;
	vector<string> one_transaction;
	vector<string> stats_table;
	for (int i=0; i< 3; i++)
	{
		int PORT = arr[i];
		list_sock.sin_port = PORT;
		bzero(msg, MAXDATASIZE);
		string msg_string=STATS+(" "+username);
		strcpy(msg,msg_string.c_str());
		list_sock_size=sizeof(list_sock);
		int len=strlen(msg);
		send_output=sendto(sockABC,msg,len,0,(struct sockaddr *) &list_sock, list_sock_size);
		sendError(send_output);
		bzero(msg, MAXDATASIZE);
		recv_output=recvfrom(sockABC, msg, MAXDATASIZE , 0,(struct sockaddr *) &list_sock, &list_sock_size);
		recvError(recv_output);
		char *token = strtok(msg, ",");
		while (token != NULL)
		{
			all_transactions.push_back(token);
			string transaction=token;
			string word;
			stringstream s(transaction);
			while (s >> word)
			{
				one_transaction.push_back(word);
			}

			if(one_transaction[1]!=username)
			{
				bool unique=true;
				int unique_usernames_length= unique_usernames.size();
				for(int i=0;i<unique_usernames_length;i++)
				{
					if(unique_usernames[i].compare(one_transaction[1])==0)
					{
						unique=false;
						break;
					}
				}
				if(unique)
				{
					unique_usernames.push_back(one_transaction[1]);
				}
			}

			else if (one_transaction[2]!=username)
			{
				bool unique=true;
				int unique_usernames_length= unique_usernames.size();
				for(int i=0;i<unique_usernames_length;i++)
				{
					if(unique_usernames[i].compare(one_transaction[2])==0)
					{
						unique=false;
						break;
					}
				}
				if(unique)
				{
					unique_usernames.push_back(one_transaction[2]);
				}	
			}
			one_transaction.clear();
			token = strtok(NULL, ",");
		}
	}

	int unique_usernames_length= unique_usernames.size();
	for(int i=0;i<unique_usernames_length;i++)
	{
		int count=0;
		int balance=0;
		one_transaction.clear();
		int all_transactions_length=all_transactions.size();
		for(int j=0;j<all_transactions_length;j++)
		{
			char temp[MAXDATASIZE];
			strcpy(temp,all_transactions[j].c_str());
			char *transact = strtok(temp," ");
			while(transact!=NULL)
			{
				one_transaction.push_back(transact);
				transact=strtok(NULL," ");
			}
			delete(transact);
			if(one_transaction[1]==username && one_transaction[2]==unique_usernames[i])
			{
				count++;
				balance-=stoi(one_transaction[3]);
			}
			else if(one_transaction[1]==unique_usernames[i] && one_transaction[2]==username)
			{
				count++;
				balance+=stoi(one_transaction[3]);
			}
			one_transaction.clear();
			
		}
		string tx=unique_usernames[i]+" "+to_string(count)+" "+to_string(balance);
		stats_table.push_back(tx);
	}
	string stats_table_arr[stats_table.size()];
    copy(stats_table.begin(),stats_table.end(),stats_table_arr);
    int stats_table_arr_size = stats_table.size();
	mergeSortForStats(stats_table_arr, 0, stats_table_arr_size - 1); 
	string msg_string="1"+(" "+stats_table_arr[0]);
	for (int i=1;i<stats_table_arr_size;i++)
	{
		msg_string=msg_string+","+to_string(i+1)+(" "+stats_table_arr[i]);
	}
	bzero(msg, MAXDATASIZE);
	strcpy(msg,msg_string.c_str());
	int len=strlen(msg);
	send_output= send(child_sock,msg,len,0);
	sendError(send_output);
	close(child_sock);
	return 0;
}

int serverSetupForClientA(int sockABC)
{	
	int portClientA = TCP_PORTA;
	struct sockaddr_in serverMClientA;
	int sockA, listenA, bindA;
	serverMClientA.sin_family = AF_INET;
	serverMClientA.sin_port = portClientA;
	memset(serverMClientA.sin_zero, '\0', sizeof (serverMClientA.sin_zero));
	inet_pton(AF_INET, localhost, &(serverMClientA.sin_addr));
	//cout << inet_ntoa(serverMClientA.sin_addr); 

	sockA = socket(PF_INET, SOCK_STREAM, 0);
	socketError(sockA);

	bindA=::bind(sockA, (struct sockaddr *)&serverMClientA, sizeof(serverMClientA));
	bindError(bindA);

	listenA=listen(sockA, BACKLOG);
	listenError(listenA);

	return sockA;
}

int serverSetupForClientB(int sockABC)
{	
	int portClientB = TCP_PORTB;
	struct sockaddr_in serverMClientB;
	int sockB, bindB, listenB;
	serverMClientB.sin_family = AF_INET;
	serverMClientB.sin_port = portClientB;
	memset(serverMClientB.sin_zero, '\0', sizeof (serverMClientB.sin_zero));
	inet_pton(AF_INET, localhost, &(serverMClientB.sin_addr));
	//cout << inet_ntoa(serverMClientB.sin_addr); 

	sockB = socket(PF_INET, SOCK_STREAM, 0);
	socketError(sockB);

	bindB=::bind(sockB, (struct sockaddr *)&serverMClientB, sizeof(serverMClientB));
	bindError(bindB);

	listenB=listen(sockB, BACKLOG);
	listenError(listenB);	

	return sockB;
}

int infiniteLoopFunc(int sockA,int sockB, int sockABC)
{
	socklen_t addr_sizeA,addr_sizeB;
	struct sockaddr_in clientA,clientB;
	int child_sockA,child_sockB,recvA,recvB;
	int portClientA = TCP_PORTA;
	int portClientB = TCP_PORTB;
	while(true)
	{
		char msgA[MAXDATASIZE]; 
		bzero(msgA, MAXDATASIZE);
		addr_sizeA=sizeof(clientA);
		child_sockA = accept(sockA, (struct sockaddr *)&clientA,&addr_sizeA);
		acceptError(child_sockA);
		recvA=recv(child_sockA, msgA, MAXDATASIZE, 0);
		recvError(recvA);
		string sA= msgA;
		if(sA.length()!=0)
		{
				if ((sA.substr(0,1).compare(CHECK_WALLET)==0))
			{
				cout<<"The main server received input= "<<sA.substr(2)<<" from the client using TCP over port "<<to_string(portClientA)<<"."<<endl;
				checkWallet(child_sockA,sockABC,sA.substr(2),"A");
			}

			else if ((sA.substr(0,1).compare(TXCOINS)==0))
			{	
				int transaction_amount;
				string username1,username2;
				char s_input[MAXDATASIZE];
				strcpy(s_input,sA.substr(2).c_str());
				char *input = strtok(s_input, " ");
				vector<string> values;
				while (input != NULL)
					{
						values.push_back(input);
						input = strtok(NULL, " ");
					}
				username1=values[0];
				username2=values[1];
				transaction_amount=stoi(values[2]);
				cout<<"The main server received from "<<username1<<" to transfer "<< to_string(transaction_amount) << " coints to " << username2 << " using TCP over port "<<to_string(portClientA)<< "."<< endl;
				txCoins(child_sockA,sockABC,sA,username1,username2,transaction_amount,"A");
			}

			else if ((sA.substr(0,1).compare(TXLIST)==0))
			{
				cout<<"A TXLIST request has been received from A."<<endl;
				txList(child_sockA,sockABC);
			}

			else if ((sA.substr(0,1).compare(STATS)==0))
			{
				cout<<"A STATS request has been received from A."<<endl;
				computeStats(child_sockA, sockABC, sA.substr(2));
			}
			bzero(msgA,MAXDATASIZE);
			string sA= msgA;
		}
		
		char msgB[MAXDATASIZE]; 
		bzero(msgB, MAXDATASIZE);
		addr_sizeB=sizeof(clientB);
		child_sockB = accept(sockB, (struct sockaddr *)&clientB,&addr_sizeB);
		acceptError(child_sockB);
		recvB=recv(child_sockB, msgB, MAXDATASIZE, 0);
		recvError(recvB);
		string sB= msgB;
		if(sB.length()!=0)
		{
				if ((sB.substr(0,1).compare(CHECK_WALLET)==0))
			{
				cout<<"The main server received input= "<<sB.substr(2)<<" from the client using TCP over port "<<to_string(portClientB)<<"."<<endl;
				checkWallet(child_sockB,sockABC,sB.substr(2),"B");
			}

			else if ((sB.substr(0,1).compare(TXCOINS)==0))
			{
				int transaction_amount;
				string username1,username2;
				char s_input[MAXDATASIZE];
				strcpy(s_input,sB.substr(2).c_str());
				char *input = strtok(s_input, " ");
				vector<string> values;
				while (input != NULL)
					{
						values.push_back(input);
						input = strtok(NULL, " ");
					}
				username1=values[0];
				username2=values[1];
				transaction_amount=stoi(values[2]);
				cout<<"The main server received from "<<username1<<" to transfer "<< to_string(transaction_amount) << " coints to " << username2 << "using TCP over port "<<to_string(portClientB)<< "."<< endl;
				txCoins(child_sockB,sockABC,sB, username1, username2, transaction_amount,"B");
			}

			else if ((sB.substr(0,1).compare(TXLIST)==0))
			{
				cout<<"A TXLIST request has been received from B."<<endl;
				txList(child_sockB,sockABC);
			}

			else if ((sB.substr(0,1).compare(STATS)==0))
			{
				cout<<"A STATS request has been received from B."<<endl;
				computeStats(child_sockB, sockABC, sB.substr(2));
			}
			bzero(msgB,MAXDATASIZE);
			string sB= msgB;
		}
		
	}
	return 0;
}

int serverSetupForBackendServersABC()
{	
	int port = UDP_PORTM;
	struct sockaddr_in serverMBackendServersABC;
	int sockABC, bindABC;
	serverMBackendServersABC.sin_family = AF_INET;
	serverMBackendServersABC.sin_port = port;
	memset(serverMBackendServersABC.sin_zero, '\0', sizeof (serverMBackendServersABC.sin_zero));
	inet_pton(AF_INET, localhost, &(serverMBackendServersABC.sin_addr));
	//cout << inet_ntoa(serverMBackendServersABC.sin_addr); 

	sockABC = socket(PF_INET, SOCK_DGRAM, 0);
	socketError(sockABC);

	bindABC=::bind(sockABC, (struct sockaddr *)&serverMBackendServersABC, sizeof(serverMBackendServersABC));
	bindError(bindABC);

	return sockABC;
}



int main ()
{
	cout<<"The main server is up and running."<<endl;
	int sockABC=serverSetupForBackendServersABC();
	int sockA=serverSetupForClientA(sockABC);
	int sockB=serverSetupForClientB(sockABC);
	infiniteLoopFunc(sockA,sockB,sockABC);
	
}




