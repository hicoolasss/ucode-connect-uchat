#include "../inc/server.h"
void daemon_server()
{
    pid_t process_id = 0;
    pid_t sid = 0;
    
    process_id = fork();
    if (process_id < 0) {
        exit(1);
    }
    if (process_id > 0) {
        printf("server pid = %d\n", process_id);
        exit(0);
    }
    umask(0);
    sid = setsid();
    
    if (sid < 0)
        exit(1);
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}
