

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <ctime>
#include <signal.h>
#include <vector>
using namespace std;

	char program[80];
	char *args[100];

void forker(int nprocesses, string website, string count, int fds[]) {
	pid_t pid;
	if(nprocesses > 0) {
		if((pid = fork()) == 0) {
			printf("\tChild process %d to ping %s %s times\n", nprocesses, website.c_str(), count.c_str());	
			args[2] = (char *)count.c_str();
			args[3] = (char *)website.c_str();
	
			close(fds[0]);
			dup2(fds[1], 1);

			int i = execvp(program, args); 
			
			cout << "i=" << i << "... execvp failed :(\n";
		} else if(pid >0) {
			forker(nprocesses-1, website, count, fds);
		} else {
			cout << "fork machine broke\n";
		}
	}

}
int main()
{
	int status;
	pid_t my_pid = getpid();


	ifstream infile("a3ping2Data.txt");
	string line;

	string arg_zero = "ping";
	string arg_one = "-c";

	strcpy(program, arg_zero.c_str());
	args[0] = (char *)arg_zero.c_str();
	args[1] = (char *)arg_one.c_str();
	args[4] = NULL;

	pid_t pid;

	while(getline(infile, line) ){
		if(line == "") { continue; }
		string website = line.substr(0, line.find(' '));
		string count = line.substr(1 + line.find(' '), line.rfind(' ')-line.find(' ')-1);
		int num_child = atoi(line.substr(1 +line.rfind(' ')).c_str());
		
		printf("Input data is website :%s: with :%s: packets and :%d: children\n", website.c_str(), count.c_str(), num_child);
		int fds[2];
		pipe(fds);

		vector<int> children(num_child);
		forker(num_child, website, count, fds);
			
		if(getpid() != my_pid) {
			children.push_back(getpid());
			kill(getpid(), SIGTERM);
		}
		for(int i = 0; i < children.size(); i++) {

			waitpid(children[i], &status, 0);
		}

			close(fds[1]);
			dup2(fds[0], 0);

			int c;
			string output = "";
			printf("Processing results for %s\n", website.c_str());

			while((c = getc(stdin)) != EOF) {
				output += (char)c;
				if((char)c == '\n') {
					if(output.find("---") != -1 || output.find("packets") != -1 || output.find("rtt") != -1) {
					cout << output;

					}
					output = "";
				}
				if(ferror(stdin))
				{ fprintf(stderr, "read error\n"); return -1; }
			}

			

	}
	return 0;
	infile.close();
}
