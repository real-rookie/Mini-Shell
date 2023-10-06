#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main(){
    int fd;
    char text[] = "runner runner runner\n";
    for(int i = 0; i < 20000; ++i){
        fd = open("test.txt", O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR| S_IWUSR);
        write(fd, text, strlen(text));
        close(fd);
    }
    cout << "input to runner: " << text << endl;
}
