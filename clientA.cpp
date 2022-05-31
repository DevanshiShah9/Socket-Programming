#include "miscellaneous.h"



int clientSetupA()
{	
	int sockA,connectA;
	sockA = socket(PF_INET, SOCK_STREAM, 0);
	socketError(sockA);
  	struct sockaddr_in clientA;
	clientA.sin_family = AF_INET;
	clientA.sin_port = TCP_PORTA;
	memset(clientA.sin_zero, '\0', sizeof (clientA.sin_zero));
	inet_pton(AF_INET, localhost, &(clientA.sin_addr));
	connectA=connect(sockA, (struct sockaddr*)&clientA, sizeof(clientA));
	connectError(connectA);

	return sockA;
}

void checkWallet(int sockA, string username)
{
	int sendA,len,recvA;
	char msg[MAXDATASIZE]; 
	bzero(msg, MAXDATASIZE);
	string msg_string= CHECK_WALLET+ (" " + username);
  	strcpy(msg, msg_string.c_str());
	len=strlen(msg);
	sendA= send(sockA,msg,len,0);
	sendError(sendA);
	cout<< username << " sent a balance enquiry request to the main server." << endl;
	bzero(msg, MAXDATASIZE);
	recvA=recv(sockA, msg, MAXDATASIZE, 0);
	recvError(recvA);
	cout<<"The current balance of " << username << " is :" << msg << " alicoins."<<endl;
	close(sockA);
}

void txCoins(int sockA, string username1, string username2, int transfer_amount)
{
	int sendA,len,recvA;
	char msg[MAXDATASIZE]; 
	bzero(msg, MAXDATASIZE);
	string msg_string= TXCOINS+ (" " + username1 + " " + username2 + " " + to_string(transfer_amount));
  	strcpy(msg, msg_string.c_str());
	len=strlen(msg);
	sendA= send(sockA,msg,len,0);
	sendError(sendA);
	cout<< username1 << " has requested to transfer " << transfer_amount << " coins to " << username2 << "." << endl;
	bzero(msg, MAXDATASIZE);
	recvA=recv(sockA, msg, MAXDATASIZE, 0);
	recvError(recvA);
	string s= msg;
	string possibility ("Insufficient Balance");
	if ((s.substr(0,possibility.length()).compare(possibility))==0)
	{
		cout<< username1 << " was unable to transfer " << transfer_amount << " alicoins to " << username2 << " because of insufficient balance." << endl;
		cout<<"The current balance of " << username1 << " is : " << s.substr(possibility.length()+1) << " alicoins." << endl;
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
		cout<< username1 << " successfully transferred " << transfer_amount << " alicoins to " << username2 << "." << endl;
		cout<<"The current balance of " << username1 << " is : " << msg << " alicoins." << endl;
	}
	close(sockA);

}

int txList(int sockA)
{
	int sendA,len;
	char msg[MAXDATASIZE]; 
	bzero(msg, MAXDATASIZE);
  	strcpy(msg, TXLIST);
	len=strlen(msg);
	sendA= send(sockA,msg,len,0);
	sendError(sendA);
	cout<<"ClientA sent a sorted list request to the main server."<<endl;	
	close(sockA);
	return 0;
}

int computeStats(int sockA, string username)
{
	int sendA,len,recvA;
	char msg[MAXDATASIZE]; 
	bzero(msg, MAXDATASIZE);
	string msg_string=STATS+(" "+ username);
  	strcpy(msg, msg_string.c_str());
	len=strlen(msg);
	sendA= send(sockA,msg,len,0);
	sendError(sendA);
	cout<<username+" sent a statistics enquiry request to the main server."<<endl;
	recvA=recv(sockA, msg, MAXDATASIZE, 0);
	recvError(recvA);
	cout<<username+" statistics are the following.:"<<endl;
	cout<< "Rank Username NumofTransacions Total"<<endl;
	char *token = strtok(msg, ",");
	while (token != NULL)
	{
		cout<<token<<endl;
		token = strtok(NULL, ",");
	}
	close(sockA);
	return 0;
}

int main (int argc, char *argv[])
{
	cout<< "The client A is up and running." << endl;
	int sockA;
	sockA=clientSetupA();
	if (argc == 2) 
	{	
		if(strcmp(argv[1],"TXLIST")==0)
		{
			txList(sockA);	
		}
		else
		{
			checkWallet(sockA,argv[1]);
		}
	}
	else if(argc==3 && (strcmp(argv[2],"stats")==0))
	{
		computeStats(sockA, argv[1]);
	}

	else if (argc==4)
	{
		txCoins(sockA,argv[1],argv[2],stoi(argv[3]));
	}
	
}




