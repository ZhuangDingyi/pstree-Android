#Prj1-Problem3

'''
Generate a new process and output
“StudentIDParent” with PID, then generates its children process output “StudentIDChild” with PID. 

Use execl to execute ptree in the child process,show the relationship between above two process.
'''

In "Problem3/jni" folder, type commands in Linux shell:
$ ndk-build

change to "Problem3/libs/armeabi/"folder,type:
$ adb push test_problem3 /data/misc

In Android shell:
# chmod 777 test_problem3
# ./test_problem3


