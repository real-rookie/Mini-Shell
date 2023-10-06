#include "shell.h"

void shell::exec_shell(std::vector<std::string> cmd){
    int pid = fork();
    if(pid == 0){
        // child

        for(std::vector<std::string>::size_type i = 0; i < cmd.size(); ++i){
            if(cmd[i][0] == '<'){
                int fd_in = open(cmd[i].substr(1).c_str(), O_RDONLY);
                dup2(fd_in, STDIN_FILENO);
                cmd[i].clear();
            } else if(cmd[i][0] == '>'){
                int fd_out = open(cmd[i].substr(1).c_str(), O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
                dup2(fd_out, STDOUT_FILENO);
                cmd[i].clear();
            } else if(cmd[i][0] == '&'){
                cmd[i].clear();
            }
        }
        char tokens[8][21];
        char *token_ptrs[9];
        int temp_token_idx = 0;
        for(auto s : cmd){
            if(!s.empty()){
                strcpy(tokens[temp_token_idx], s.c_str());
                token_ptrs[temp_token_idx] = tokens[temp_token_idx];
                ++temp_token_idx;
            }
        }
        token_ptrs[temp_token_idx] = nullptr;        
        if(execvp(token_ptrs[0], token_ptrs) == -1){
            perror("execve() failed!");
            exit(EXIT_FAILURE);
        }
    } else if(pid == -1){
        // fork failed

        perror("fork() failed!");
        return;
    } else {
        // parent
        
        if(cmd[cmd.size() - 1] != "&"){
            // foreground
            waitpid(pid, nullptr, 0);
            return;
        }

        // background

        std::string cmd_str;
        for(auto s : cmd){
            cmd_str.append(s + " ");
        }
        pcb.emplace(pid, process(pid, "R", cmd_str));
    }
}

// returns user time and sys time
std::pair<int, int> shell::get_user_sys_times(){
    rusage usage;
    int who = RUSAGE_CHILDREN;
    if(getrusage(who, &usage) == -1){
        perror("sys call getrusage() failed!");
    }
    return {usage.ru_utime.tv_sec, usage.ru_stime.tv_sec};
}

int shell::get_real_time(int pid){
    std::string ps_cmd = "ps --pid " + std::to_string(pid);
    FILE* pipe = popen(ps_cmd.c_str(), "r");
    char buf[255];
    fgets(buf, 255, pipe); // throws away the first line
    fgets(buf, 255, pipe);
    if(feof(pipe)){
        pclose(pipe);
        return -1;
    }
    pclose(pipe);
    std::istringstream iss(buf);
    std::string token;
    for(int i = 0; i < 3; ++i){
        iss >> token;
    }
    // assumes program runs within 99 hours
    int sec = std::stoi(token.substr(0, 2)) * 3600 + 
              std::stoi(token.substr(3, 2)) * 60 +
              std::stoi(token.substr(6, 2));
    return sec;
}

void shell::jobs_shell(){
    std::ostringstream msg;
    msg << "Running processes:" << std::endl;
    msg << "#\tPID\tS\tSEC\tCOMMAND" << std::endl;
    int i = 0;
    for(auto p : pcb){
        msg << i << ":\t" << p.first << '\t' << p.second.state
            << '\t' << shell::get_real_time(p.first) << '\t' << p.second.cmd << std::endl;
    }
    msg << "Processes =\t" << pcb.size() << "\tactive" << std::endl;
    msg << "Completed processes:" << std::endl;
    std::pair<int, int> times = get_user_sys_times();
    msg << "User time =\t" << times.first << "\tseconds" << std::endl;
    msg << "System time =\t" << times.second << "\tseconds" << std::endl;
    std::cout << msg.str() << std::endl;
}