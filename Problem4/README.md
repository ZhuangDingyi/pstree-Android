#Prj1-Problem4

'''
Please develop a Caesar Encryption Server, which receives plaintext from clients and
sends the corresponding ciphertext to clients.
Only the letters need to be encrypted, e.g. How are you? → Krz duh brx?
The Server can serve at most 2 clients concurrently, more clients coming have to wait.
The server-side program must be concurrent multi-threaded.
Client input :q to end the service.
For simplicity, you can execute one server and multiple clients in one host.
'''

In current folder, type command in Linux shell:
$ gcc -o client client.c
$ gcc -o server server.c -lpthread

In one terminal:
$ ./server 

In several independent terminals:
$ ./client



