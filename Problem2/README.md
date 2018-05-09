#Prj1-Problem2

'''
Test your new system call
Write a simple C program which calls ptree
Print the entire process tree (in DFS order) using tabs to indent children with respect to their parents.
'''

In "Problem2/jni" folder, type commands in Linux shell:
$ ndk-build

change to "Problem2/libs/armeabi/"folder,type:
$ adb push runPtree_problem2 /data/misc

In Android shell:
# chmod 777 runPtree_problem2
# ./runPtree_problem2


