#Prj1-Problem1

'''
Write a new system call in Linux.

The system call you write should take two arguments
and return the process tree information in a depth-
first-search (DFS) order.

Each system call must be assigned a number. Your
system call should be assigned number 391.
'''

In the "Problem1" folder, type the command in Linux shell:
$make clean
$make
$adb push ptree_problem1.ko /data/misc
$adb shell

In the Android shell:
# cd /data/misc
# ls
# insmod ptree_problem1.ko
