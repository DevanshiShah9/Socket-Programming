#include "miscellaneous.h"


using namespace std;

int serverSetupForBackendServerC()
{	
	string filename="block3.txt";
	int port = UDP_PORTC;
	struct sockaddr_in serverMBackendServerC, serverM;
	socklen_t serverM_size;
	int sockC, bindC, sendC, recvC;
	serverMBackendServerC.sin_family = AF_INET;
	serverMBackendServerC.sin_port = port;
	memset(serverMBackendServerC.sin_zero, '\0', sizeof (serverMBackendServerC.sin_zero));
	inet_pton(AF_INET, localhost, &(serverMBackendServerC.sin_addr));
	//cout << inet_ntoa(serverMBackendServerC.sin_addr); 

	sockC = socket(PF_INET, SOCK_DGRAM, 0);
	socketError(sockC);

	bindC=::bind(sockC, (struct sockaddr *)&serverMBackendServerC, sizeof(serverMBackendServerC));
	bindError(bindC);

    while(true)
    {
        char msg[MAXDATASIZE]; 
        serverM.sin_family = AF_INET;
        memset(serverM.sin_zero, '\0', sizeof (serverM.sin_zero));
        inet_pton(AF_INET, localhost, &(serverM.sin_addr));
        int PORT = UDP_PORTM;
        serverM.sin_port = PORT;
        bzero(msg, MAXDATASIZE);
        serverM_size=sizeof(serverM);
        recvC=recvfrom(sockC, msg, MAXDATASIZE , 0,(struct sockaddr *) &serverM, &serverM_size);
        recvError(recvC);
        cout<<"The ServerC received a request from the Main Server."<<endl;

        vector<string> vect;
        char *token = strtok(msg, " ");
        while (token != NULL)
        {
            vect.push_back(token);
            token = strtok(NULL, " ");
        }
        if (vect.size()==1)
        {
            if(vect[0].compare(TXLIST)==0)
            {
                bzero(msg, MAXDATASIZE);
                tx_list(msg,filename);
            }
            else
            {
                int wallet=check_wallet(vect[0],filename);
                bzero(msg, MAXDATASIZE);
                strcpy(msg,to_string(wallet).c_str());
            }
            int len=strlen(msg);
            sendC=sendto(sockC,msg,len,0,(struct sockaddr *) &serverM, serverM_size);
            sendError(sendC);
            
        }
        else if (vect.size()==2)
        {	
            if(vect[0].compare(STATS)==0)
            {
                bzero(msg, MAXDATASIZE);
                compute_stats(msg,vect[1],filename);
                int len=strlen(msg);
                sendC=sendto(sockC,msg,len,0,(struct sockaddr *) &serverM, serverM_size);
                sendError(sendC);
            }
            
        }

        else if (vect.size()==3)
        {	
            bzero(msg, MAXDATASIZE);
			vector <string> msg_vect;
			msg_vect= tx_coins(vect[0],vect[1],stoi(vect[2]),filename);
			string msg_string=msg_vect[0]+ " " + msg_vect[1]+ " " + msg_vect[2]+" "+msg_vect[3];
			strcpy(msg,msg_string.c_str());
			int len=strlen(msg);
            sendC=sendto(sockC,msg,len,0,(struct sockaddr *) &serverM, serverM_size);
            sendError(sendC);
        }

        else if (vect.size()==4)
        {	
			ifstream input(filename);
            char last_char;
            while(!input.eof())
            {
                input.get(last_char);
            }
            input.close();
            fstream myfile;
            myfile.open (filename,fstream::app);
            string msg_to_append= vect[0]+ " " + vect[1]+ " " + vect[2]+ " "+ vect[3];
            if(last_char=='\n')
            {
                myfile <<msg_to_append<<endl;
            }
            else
            {
                myfile <<endl<<msg_to_append<<endl;
            }
            
            
            myfile.close();
        }
        cout<<"The ServerC finished sending the response to the Main Server."<<endl;
    }
	return 0;
}




int main ()
{
	cout<<"The ServerC is up and running using UDP on port "<< to_string(UDP_PORTC)<<"."<<endl;
	serverSetupForBackendServerC();
	
}





