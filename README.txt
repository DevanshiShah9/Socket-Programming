Student Name: Devanshi Krishna Shah
Student ID: 6298935190

Operations executed in the project:
1. Check Wallet
2. TXCOINS
3. TXLIST
4. stats (completed the optional part as well)

Code Files:
1. miscellaneous.cpp, miscellaneous.h - This is a header file common to all the other .cpp files.
   It contains import statements, definitions of constants used throughout the project, socket error detection 
   functions, and 4 functions common for computation of 4 operations to the backend servers A,B,C.

2. serverM.cpp - This file is for serverM and its computations for the 4 operations.

3. serverA.cpp - This file is for serverA and its computations for the 4 operations.

4. serverB.cpp - This file is for serverB and its computations for the 4 operations.

5. serverC.cpp - This file is for serverC and its computations for the 4 operations.

6. clientA.cpp - This file is for clientA and its computations for the 4 operations.

7. clientB.cpp - This file is for clientB and its computations for the 4 operations.

Order of calls:
- All .cpp files include miscellaneous.h for import statements, constant definitions such as PORT numbers 
  and socket error detection functions.
- When an operation is called for clientA, it connects and sends request to the TCP socket in Server M 
  for ClientA, server M then sends requests to UDP sockets of backend servers A,B,C; receives the inofmration 
  from servers A,B,C performs further computation if any and sends the result as a response to clientA.
- The same procedure as above is followed for clientB.
- For every operation the required messages at every step are printed to the terminal in the required format
  for the user to observe.

No idiosyncrasy observed in the project.
No code is resued from anywhere.

Note: In the given block files, block1.txt had 2 '\n' characters at the end, so I manually cleared one and checked the code, the output came as expected. In addition, I have handled both the cases where '\n' is the last character in the file and one where it isn't, by checking if the last character is '\n' or not.
