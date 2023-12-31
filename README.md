# Description
**This repo implements a mini shell that is capable of executing several commands**\
Supported commands:
* `exit` \- Exit from the mini shell after all the child processes are done
* `jobs` \- Display the info of all the child processes, including their pids, status, running time, commands issued, etc.
* `kill` \- Terminate a running process
* `suspend <pid>` \- Suspend the process \<pid\>
* `resume <pid>` \- Resume the process \<pid\>. This undoes suspend
* `sleep <int>` \- Stop the mini shell from accepting new commands for \<int\> seconds
* `wait <pid>` \- Stop the mini shell from accepting new commands until process \<pid\> is done
* `<cmd> <arg>` \- Run external \<cmd\> with arguments \<arg\>. E.g., `ls -la`
* I/O redirection\(`< >`\) and running processes in the background\(`&`\) are also supported
# To run the program
```
$ make
$ ./shell379
shell379:  jobs
shell379:  ls -la
```
