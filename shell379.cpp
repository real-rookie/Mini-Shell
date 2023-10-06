#include <vector>
#include <string>
#include <sstream>
#include "shell.h"

int main(){
    shell shell_instance;
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