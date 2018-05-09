/* 
StudentNumber:515020910156
Name:庄丁奕
*/
#include "Problem2.h"
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXNUM 500
#define __NR_ptree 287
/*======Question====*/
/*
Test your new system call
Write a simple C program which calls ptree
Print the entire process tree (in DFS order) using tabs to indent children with respect to their parents.
*/
struct prinfo {
	pid_t parent_pid;       /* process id of parent */
	pid_t pid;              /* process id */
	pid_t first_child_pid;  /* pid of youngest child */
	pid_t next_sibling_pid; /* pid of older sibling */
	long state;             /* current state of process */
	long uid;               /* user id of process owner */
	char comm[64];          /* name of program executed */
	int depth;		/* To identify relationship in dfs*/
};
int main(int argc, char *argv[]){
	//Initialization
	int nr=0;
	struct prinfo buf[MAXNUM];
	struct prinfo p;
	//Run module
	syscall(__NR_ptree,buf,&nr);
	//Begin print
	printf("The number of process is %d.\n",nr);
	int i=0,j=0;
	for (i=0;i<nr;++i){
		p=buf[i];
		for (j=0;j<p.depth;++j)
			printf("\t");
		printf("%s,%d,%ld,%d,%d,%d,%ld\n",p.comm,p.pid,p.state,p.parent_pid,p.first_child_pid,p.next_sibling_pid,p.uid);
	}
	return 0;

}
