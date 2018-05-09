/* 
StudentNumber:515020910156
Name:庄丁奕
*/
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/sched.h>
#include<linux/unistd.h>
#include<linux/syscalls.h>
#include<linux/string.h>
#include<linux/list.h>
#include<linux/slab.h>          //  memory allocation
#include<linux/uaccess.h>       //  copy to user
MODULE_LICENSE("Dual BSD/GPL");
#define __NR_ptree 287 //391 does not work out!!!
#define MAXNUM 500

/*======Question====*/
/*
Write a new system call in Linux.
The system call you write should take two arguments
and return the process tree information in a depth-
first-search (DFS) order.

Each system call must be assigned a number. Your
system call should be assigned number 391.
*/
/*======Notification====*/
/* depth-first-search should be fulfilled */
/* system call should be 391 but it would not work out, change it as 287 as the demo*/
/*buf points to a buffer for the process data, and nr points to the size of this buffer (number ofentries) 
The system call copies as many entries of the process tree data to the buffer as possible, and stores the number of entries actually copied in nr*/
/* remember to use tasklist_lock when trannversal */


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

struct prinfo tasks_store[MAXNUM]; // Used to store the tasks generated in dfs
// Basic infomation copying operation
void copyTaskInfo(struct prinfo *buf, struct task_struct *task2copy,int *nr,int depth){
	buf->pid = task2copy->pid;
	buf->parent_pid =task2copy->parent->pid; //Instead of real_parent
	buf->state=task2copy->state;
	buf->uid=task2copy->cred->uid;
	buf->depth=depth;
	get_task_comm(buf->comm,task2copy);
	
	++*nr; 

	// Get pid of first child process,which is list_head type
	if (list_empty(&task2copy->children)){
		buf->first_child_pid=0;
	}
	else{
		struct task_struct *tmp_tk;
		tmp_tk=list_entry((&task2copy->children)->next,struct task_struct,sibling); // Use sibling instead of children field, get youngest children
		if (tmp_tk==NULL)
			buf->first_child_pid=0;
		else
			buf->first_child_pid=tmp_tk->pid;
		
	}
	// Find a older/next sibling if exists
	if (list_empty(&task2copy->sibling)){
		buf->next_sibling_pid=0;
	}
	else{
		struct list_head *tmp_ls;
		struct task_struct *tmp_tk;
		list_for_each(tmp_ls,&(task2copy->children)){
			tmp_tk=list_entry(tmp_ls,struct task_struct,sibling);
			if (tmp_tk->pid==buf->pid){
				buf->next_sibling_pid=list_entry(tmp_ls->next,struct task_struct, sibling)->pid; 
				break;
			}
		}
	}
	printk("Copy success:comm: %s, pid: %d",buf->comm, buf->pid);//Used for testing the kernel condition

}

//*nr copies as many entries as possible, including parents and children
void dfs(struct task_struct *init_tasks, int *nr, int depth){
	struct task_struct *tasks_erg;
	struct list_head *subtask_list;	
	if (init_tasks==NULL)
		{printk("DFS NULL!");
		return;}
//Store infomation of each entried process
	copyTaskInfo(&tasks_store[*nr],init_tasks,nr,depth);
	
	list_for_each(subtask_list,&init_tasks->children){
		tasks_erg=list_entry(subtask_list,struct task_struct,sibling);
		dfs(tasks_erg,nr,depth+1); //Traversal
	}
}
static int (*oldcall)(void);
static int ptree(struct prinfo *buf, int *nr)
{
    int i = 0;
    printk("begin to ptree!!\n");
    read_lock(&tasklist_lock);
    // begin to dfs from init_task
    dfs(&init_task,nr, 0);
    read_unlock(&tasklist_lock);
    printk("ptree complete!!\n");
    
    // copy from tasks[] to buf[]
    for (i = 0; i < *nr; ++i)
    {
	strcpy(buf[i].comm, tasks_store[i].comm);
        buf[i].pid = tasks_store[i].pid;
        buf[i].state = tasks_store[i].state;
	buf[i].parent_pid = tasks_store[i].parent_pid;
	buf[i].first_child_pid = tasks_store[i].first_child_pid;
	buf[i].next_sibling_pid = tasks_store[i].next_sibling_pid;
	buf[i].uid = tasks_store[i].uid;
	buf[i].depth = tasks_store[i].depth;
    }
    return 0;
}
/*========Preservation================*/

static int addsyscall_init(void){
	long *syscall = (long*)0xc000d8c4;
	oldcall = (int(*)(void))(syscall[__NR_ptree]);
	syscall[__NR_ptree] = (unsigned long)ptree;
	printk(KERN_INFO "module load!\n");
	return 0;
}

static void addsyscall_exit(void){
	long *syscall = (long*)0xc000d8c4;
	syscall[__NR_ptree] = (unsigned long)oldcall;
	printk(KERN_INFO "module exit!\n");
}

module_init(addsyscall_init);
module_exit(addsyscall_exit);
