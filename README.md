# Project: Socket Programming (USC EE 450 - Introduction to Computer Networks)

## Name: Devanshi Krishna Shah

### Operations Executed in the Project
1. Check Wallet
2. TXCOINS
3. TXLIST
4. stats (completed the optional part as well)

### Code Files Overview
1. miscellaneous.cpp, miscellaneous.h - This is a header file common to all the other .cpp files.
   It contains import statements, definitions of constants used throughout the project, socket error detection 
   functions, and 4 functions common for computation of 4 operations to the backend servers A,B,C.

2. serverM.cpp - This file is for serverM and its computations for the 4 operations.

3. serverA.cpp - This file is for serverA and its computations for the 4 operations.

4. serverB.cpp - This file is for serverB and its computations for the 4 operations.

5. serverC.cpp - This file is for serverC and its computations for the 4 operations.

6. clientA.cpp - This file is for clientA and its computations for the 4 operations.

7. clientB.cpp - This file is for clientB and its computations for the 4 operations.

### Order of Calls
- All .cpp files include miscellaneous.h for import statements, constant definitions such as PORT numbers 
  and socket error detection functions.
- When an operation is called for clientA, it connects and sends request to the TCP socket in Server M 
  for ClientA, server M then sends requests to UDP sockets of backend servers A,B,C; receives the inofmration 
  from servers A,B,C performs further computation if any and sends the result as a response to clientA.
- The same procedure as above is followed for clientB.
- For every operation the required messages at every step are printed to the terminal in the required format
  for the user to observe.

### Project Notes
- No idiosyncrasy observed in the project.
- No code is reused from any external sources.

