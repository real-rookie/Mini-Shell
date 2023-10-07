#include <vector>
#include <string>
#include <sstream>
#include "shell.h"

shell shell_instance;

void on_child_terminating(int signum){
    int status;
    int pid;
    while((pid = waitpid(-1, &status, WNOHANG)) > 0){
        // normally terminated or terminated by a signal
        if(WIFEXITED(status) || WIFSIGNALED(status)){
            shell_instance.pcb.erase(pid);
        }
    }
}

int main(){
    signal(SIGCHLD, on_child_terminating);
    while(1){
        // save cmd in vector
        std::cout << "\nshell379:  " << std::flush;
        std::string line;
        std::vector<std::string> cmd;
        getline(std::cin, line);
        std::istringstream iss(line);
        std::string token;
        while(iss >> token){
            cmd.push_back(token);
        }
        if(cmd.size() == 0){
            continue;
        }

        if(cmd[0] == "exit"){
            shell_instance.exit_shell();
        }else if(cmd[0] == "jobs"){
            shell_instance.jobs_shell();
        }else if(cmd[0] == "kill"){
            shell_instance.kill_shell(cmd);
        }else if(cmd[0] == "resume"){
            shell_instance.resume_shell(cmd);
        }else if(cmd[0] == "sleep"){

        }else if(cmd[0] == "suspend"){
            shell_instance.suspend_shell(cmd);
        }else if(cmd[0] == "wait"){
            shell_instance.wait_shell(cmd);
        }else{
            shell_instance.exec_shell(cmd);
        }
    }
}