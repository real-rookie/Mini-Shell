#ifndef SHELL_H
#define SHELL_H

#include <vector>
#include <unordered_map>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sys/wait.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <sstream>
#include <algorithm>
#include "process.h"

class shell{
public:
    std::unordered_map<int, process> pcb;

public:
    void exit_shell();
    void jobs_shell();
    void kill_shell(std::vector<std::string> cmd);
    void resume_shell(std::vector<std::string> cmd);
    void sleep_shell();
    void suspend_shell(std::vector<std::string> cmd);
    void wait_shell(std::vector<std::string> cmd);
    void exec_shell(std::vector<std::string> cmd);

private:
    std::pair<int, int> get_user_sys_times();
    static int get_real_time(int pid);
};

#endif