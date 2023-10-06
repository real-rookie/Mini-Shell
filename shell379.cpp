#include <vector>
#include <string>
#include <sstream>
#include "shell.h"

shell shell_instance;

void on_child_terminating(int signum){
    int status;
    int pid;
    while((pid = waitpid(-1, &status, WNOHANG)) > 0){
        if(WIFEXITED(status)){
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

        }else if(cmd[0] == "jobs"){
            shell_instance.jobs_shell();
        }else if(cmd[0] == "kill"){

        }else if(cmd[0] == "resume"){

        }else if(cmd[0] == "sleep"){

        }else if(cmd[0] == "suspend"){

        }else if(cmd[0] == "wait"){

        }else{
            shell_instance.exec_shell(cmd);
        }
    }
}