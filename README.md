My time shredder
Made by Ze Sheng

1. In this assignment I implemented a shell named penn-shredder that will restrict the runtime of executed processes.

2. This shell will read input from the user and execute it as a new process, but if the process exceeds a timeout, it will be terminated. 

3. The code only has a specified set of Linux system calls and standard C library functions to process.


How to start?

./shredder 4

where 4 means the timeout is 4, every child process runs over 4 secs will be killed


Here is an example of sleep command:
/bin/sleep 3
this means the shell will sleep for 3 secs

/bin/sleep 5
this will be terminatied by the shell because the timeout is 4
 