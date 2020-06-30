// shell2.c
// simple shell example using execvp for a command processing
//
/*
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>*/
#include <iostream>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sysexits.h>
#include <unistd.h>
#include <fstream>
#include <stdio.h>
using namespace std;
int main()
{
   ifstream file;
   file.open("a3ping1Data.txt");
   string l; 
   char program[80];
   char *args[100];
   pid_t pid;
   int status;
   int i;
   string ping = "ping";
   string c = "-c";
   
   printf("Ready to exec()...\n");
   args[0]=(char *)ping.c_str();
   strcpy(program,ping.c_str());
   args[1]=(char *)c.c_str();
   args[4]=NULL;
   //args[3]="www.utdallas.edu";

   printf(" ** program starts ** \n"); 

   //args[2]="3";	// if you like to change -c n to be 04
    while(getline(file,l)){
       int fds[2];
       pipe(fds);
       string address = l.substr(0,l.find(" "));
       string n= l.substr(l.find(" ")+1);
       pid=fork();
       if (pid == 0) {
           // child process
	   close(fds[0]); // because this is unnecessary

           args[2] = (char *)n.c_str();
           args[3] = (char *)address.c_str();
           dup2(fds[1],1);
	   i=execvp(program,args);
           // if you see this, then execvp is failed to run
           printf("i=%d ... did it work?\n",i);

       }
       else if (pid > 0) {
           // parent waits for child
           
        if ((pid=waitpid(pid, &status, 0)) < 0){
            cout<<"its a error, kaboom";
	   }
	 //  cout<<"started the execution of the parent process \n";
	   close(fds[1]);
	   dup2(fds[0],0);
	   int c;

	   string out = "";
           while ((c = getc(stdin)) != EOF){
	            out+=(char)c;
	   	/*	if (putc(c, stdout) == EOF) {
				fprintf(stderr, "write error\n");
				return 1;
			}*/

			if (ferror(stdin)) {
			fprintf(stderr, "read error\n");
				return 1;
				}
			
	}   
	cout<<out.substr(out.find("---"));
        //cout<<"parent process is done executing\n";
	}
       
  }
   file.close();


   printf(" ** end of program run ** \n"); 

   return 0;
}
