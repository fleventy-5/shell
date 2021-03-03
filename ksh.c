#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <editline/readline.h>


char **get_input(char *input){
	char **command = malloc(8 *sizeof(char *));
	char *seperator = " ";
	char *parsed;
	
	int index = 0;
	parsed = strtok(input, seperator);
	while(parsed!=NULL){
		command[index] = parsed;
		index++;
				
		parsed = strtok(NULL,seperator);
	}

	command[index] = NULL;
	return command;
}


int cd(char *path){
	return chdir(path);
}


int main(){
	printf("Fluffy Shell version 0.0  CTRL+ C to exit\n");
	char **command;
	char *input;
	pid_t child_pid;
	int stat_loc;

	char *clr[] = {"clear",NULL};
	child_pid = fork();
	if(child_pid ==0){
		execvp(clr[0],clr);
	} else {
		waitpid(child_pid, &stat_loc,WUNTRACED);
	}

	while(1){
		input = readline("ksh>$");
		command = get_input(input);
		
		if(strcmp(command[0],"cd")==0){
			if(cd(command[1])<0){
				perror(command[1]);
			}
			continue;
		}
		
		child_pid  = fork();
		if(child_pid == 0){
			execvp(command[0],command);
			printf("Command does not exist\n");
			
		} else {
			waitpid(child_pid, &stat_loc, WUNTRACED);
		}
		free(input);
		free(command);
	}
	return 0;
}
