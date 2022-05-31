#include "miscellaneous.h"



int clientSetupB()
{	
	int sockB,connectB;
	sockB = socket(PF_INET, SOCK_STREAM, 0);
	socketError(sockB);
  	struct sockaddr_in clientB;
	clientB.sin_family = AF_INET;
	clientB.sin_port = TCP_PORTB;
	memset(clientB.sin_zero, '\0', sizeof (clientB.sin_zero));
	inet_pton(AF_INET, localhost, &(clientB.sin_addr));
	connectB=connect(sockB, (struct sockaddr*)&clientB, sizeof(clientB));
	connectError(connectB);

	return sockB;
}

void checkWallet(int sockB, string username)
{
	int sendB,len,recvB;
	char msg[MAXDATASIZE]; 
	bzero(msg, MAXDATASIZE);
	string msg_string= CHECK_WALLET+ (" " + username);
  	strcpy(msg, msg_string.c_str());
	len=strlen(msg);
	sendB= send(sockB,msg,len,0);
	sendError(sendB);
	cout<< username << " sent a balance enquiry request to the main server." << endl;
	bzero(msg, MAXDATASIZE);
	recvB=recv(sockB, msg, MAXDATASIZE, 0);
	recvError(recvB);
	cout<<"The current balance of " << username << " is : " << msg << " alicoins."<<endl;
	close(sockB);
}

void txCoins(int sockB, string username1, string username2, int transfer_amount)
{
	int sendB,len,recvB;
	char msg[MAXDATASIZE]; 
	bzero(msg, MAXDATASIZE);
	string msg_string= TXCOINS+ (" " + username1 + " " + username2 + " " + to_string(transfer_amount));
  	strcpy(msg, msg_string.c_str());
	len=strlen(msg);
	sendB= send(sockB,msg,len,0);
	sendError(sendB);
	cout<< username1 << " has requested to transfer " << transfer_amount << " coins to " << username2 << endl;
	bzero(msg, MAXDATASIZE);
	recvB=recv(sockB, msg, MAXDATASIZE, 0);
	recvError(recvB);
	string s= msg;
	string possibility ("Insufficient Balance");
	if ((s.substr(0,possibility.length()).compare(possibility))==0)
	{
		cout<< username1 << " was unable to transfer " << transfer_amount << " alicoins to " << username2 << " because of insufficient balance." << endl;
		cout<<"The current balance of " << username1 << " is : " << s.substr(possibility.length()+2) << " alicoins." << endl;
	}

	else if ((s.substr(0,4).compare("Both"))==0)
	{
		cout << "Unable to proceed with the transaction as " << username1 << " and " << username2  << " are not part of the network." << endl;
	}

	else if ((s.compare(username1)==0))
	{
		cout<< "Unable to proceed with the transaction as " << username1 << " is not part of the network." << endl;
	}

	else if ((s.compare(username2)==0))
	{
		cout<< "Unable to proceed with the transaction as " << username2 << " is not part of the network." << endl;
	}

	else
	{
		cout<< username1 << " successfully transferred " << transfer_amount << " alicoins to " << username2 << endl;
		cout<<"The current balance of " << username1 << " is : " << msg << " alicoins." << endl;
	}
	close(sockB);

}

int txList(int sockB)
{
	int sendB,len;
	char msg[MAXDATASIZE]; 
	bzero(msg, MAXDATASIZE);
  	strcpy(msg, TXLIST);
	len=strlen(msg);
	sendB= send(sockB,msg,len,0);
	sendError(sendB);
	cout<<"ClientB sent a sorted list request to the main server."<<endl;
	close(sockB);	
	return 0;
	
}

int computeStats(int sockB, string username)
{
	int sendB,len,recvB;
	char msg[MAXDATASIZE]; 
	bzero(msg, MAXDATASIZE);
	string msg_string=STATS+(" "+ username);
  	strcpy(msg, msg_string.c_str());
	len=strlen(msg);
	sendB= send(sockB,msg,len,0);
	sendError(sendB);
	cout<<username+" sent a statistics enquiry request to the main server."<<endl;
	recvB=recv(sockB, msg, MAXDATASIZE, 0);
	recvError(recvB);
	cout<<username+" statistics are the following.:"<<endl;
	cout<< "Rank Username NumofTransacions Total"<<endl;
	char *token = strtok(msg, ",");
	while (token != NULL)
	{
		cout<<token<<endl;
		token = strtok(NULL, ",");
	}
	close(sockB);
	return 0;
}

int main (int argc, char *argv[])
{
	cout<< "The client B is up and running." << endl;
	int sockB;
	sockB=clientSetupB();
	if (argc == 2) 
	{
		if(strcmp(argv[1],"TXLIST")==0)
		{
			txList(sockB);	
		}
		else
		{
			checkWallet(sockB,argv[1]);
		}
	}
	else if(argc==3 && strcmp(argv[2],"stats")==0)
	{
		computeStats(sockB, argv[1]);
	}

	else if (argc==4)
	{
		txCoins(sockB,argv[1],argv[2],stoi(argv[3]));
	}
	
}




