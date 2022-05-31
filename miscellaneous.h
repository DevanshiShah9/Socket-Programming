#ifndef MISCELLANEOUS_H
#define MISCELLANEOUS_H

#include<iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h> 
#include <string.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <sys/wait.h>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

#define localhost "127.0.0.1"
#define BACKLOG 10
#define MAXDATASIZE 1024000
#define CHECK_WALLET "1"
#define TXCOINS "2"
#define TXLIST "3"
#define STATS "4"
#define UDP_PORTA 21190 //21000+ last 3 digits of 6298935190 = 21000+190=21190
#define UDP_PORTB 22190 //22000+ last 3 digits of 6298935190 = 21000+190=21190
#define UDP_PORTC 23190 //23000+ last 3 digits of 6298935190 = 21000+190=21190
#define UDP_PORTM 24190 //24000+ last 3 digits of 6298935190 = 21000+190=21190
#define TCP_PORTA 25190 //25000+ last 3 digits of 6298935190 = 25000+190=25190
#define TCP_PORTB 26190 //26000+ last 3 digits of 6298935190 = 26000+190=26190


int socketError(int sock);
int bindError(int bind);
int listenError(int listen);
int connectError(int connect);
int acceptError(int acc);
int sendError(int send);
int recvError(int recv);

int check_wallet(string username,string filename);
vector<string> tx_coins(string username1, string username2, int transaction_amount, string filename);
void tx_list(char *msg, string filename);
void compute_stats(char *msg,string username,string filename);

#endif



