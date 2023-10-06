#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class process{
public:
    int pid;
    std::string state;
    std::string cmd;
    process(int pid, std::string state, std::string cmd);
};

#endif