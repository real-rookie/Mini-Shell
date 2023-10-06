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
        if(execve(token_ptrs[0], token_ptrs + 1, nullptr) == -1){
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
        pcb.emplace_back(process(pid, "R", cmd_str));
        // FIXME child process becomes a zombie process
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

void shell::update_pcb(){
    pcb.erase(std::remove_if(pcb.begin(), pcb.end(),
        [](process p){
            return shell::get_real_time(p.pid) == -1;
        }
    ), pcb.end());
}

int shell::get_real_time(int pid){
    std::string ps_cmd = "ps --pid " + std::to_string(pid);
    FILE* pipe = popen(ps_cmd.c_str(), "r");
    char buf[255];
    fgets(buf, 255, pipe); // throws away the first line
    fgets(buf, 255, pipe);
    if(feof(pipe)){
        // there is only one line
        // returns -1 to indicate the process has exited
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
    update_pcb();
    for(std::vector<process>::size_type i = 0; i < pcb.size(); ++i){
        process p = pcb[i];
        msg << i << ":\t" << p.pid << '\t' << p.state
            << '\t' << shell::get_real_time(p.pid) << '\t' << p.cmd << std::endl;
    }
    msg << "Processes =\t" << pcb.size() << "\tactive" << std::endl;
    msg << "Completed processes:" << std::endl;
    std::pair<int, int> times = get_user_sys_times();
    msg << "User time =\t" << times.first << "\tseconds" << std::endl;
    msg << "System time =\t" << times.second << "\tseconds" << std::endl;
    std::cout << msg.str() << std::endl;
}