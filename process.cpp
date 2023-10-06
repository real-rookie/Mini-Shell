#include "process.h"

process::process(int pid, std::string state, std::string cmd):
    pid(pid), state(state), cmd(cmd){
    ;
}
