#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
using namespace std;

int bgFlag;

void parse_cmd(char *cmd, char **argv){
	int idx = 0;
	bgFlag = 0;

	argv[idx] = strtok(cmd, " \t\n");
	while(argv[idx] != NULL){
		idx++;
		argv[idx] = strtok(NULL, " \t\n");
	}

	if (idx>0 && !strcmp(argv[idx-1], "&")){		//background process
		bgFlag = 1;
		argv[idx-1] = NULL;
	}
}

int main(){
  char command[512];
	char  *argv[64];              //command line argument

  while(1){
    cout << "command: " << flush;
    fgets(command, 512, stdin);              //read in the command line
		if(strlen(command) <= 1)
			continue;
		parse_cmd(command, argv);
		if (!strcmp(argv[0], "exit")){			//no arg no service
      return 0;
    } else {
      pid_t returnedValue = fork();
      if (returnedValue < 0) {
        perror("error forking");
        return -1;
      } else if (returnedValue == 0){
        //child code
				execvp(argv[0], argv);     //execute the command
       	perror (command);
	      return -1;
      } else {
        //parent
				if(!bgFlag){
	        if(waitpid(returnedValue, 0,0) < 0){
  	        perror("error waiting for child");
    	      return -1;
      	  }
				}
      }
    }
  }
}
