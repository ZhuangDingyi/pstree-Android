/* 
StudentNumber:515020910156
Name:庄丁奕
*/
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
/*======Question====*/
/*
Generate a new process and output
“StudentIDParent” with PID, then generates its children process output “StudentIDChild” with PID. 

Use execl to execute ptree in the child process,show the relationship between above two process.
*/
int main(int argc, char *argv[]) {
	pid_t parent_pid = getpid();
	pid_t child_pid = fork();
	if (child_pid == 0) {
		printf("StudentID: 515020910156 Parent, %d\n", parent_pid);
		printf("StudentID: 515020910156 Child, %d\n", child_pid);	
		execl("/data/misc/Problem2ARM","Problem2ARM", (char*)0);//This process pid is bigger than parent_pid by 1
	}
	return 0;
}
