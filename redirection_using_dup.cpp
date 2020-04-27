/* This is to learn the usage of dup system call used to implement i/o redirection
Performs ls >> sample_out.txt */
#define MAX_SIZE_NAME 100

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream> 
#include <dirent.h>
#include <string> 
#include <cstring> 
#include <stdlib.h>
#include <errno.h>
using namespace std;

int main(void)
{
    pid_t proc_id = fork();
    if (proc_id == 0)
    {
        /* Do execution in this branch */
        char *current_dir_name = (char *)malloc(MAX_SIZE_NAME * sizeof(char));
        getcwd(current_dir_name, MAX_SIZE_NAME);

        DIR *curr_dir = opendir(current_dir_name);
        if (curr_dir != NULL)
        {
            int outfile_fd = open("ls_out.txt",  O_CREAT | O_WRONLY, S_IRWXU);
            if (outfile_fd >= 0)
            {
                int outfile_fd_stdout = dup2(outfile_fd, STDOUT_FILENO);
                if (outfile_fd_stdout == STDOUT_FILENO)
                {
                    struct dirent *curr_file = readdir(curr_dir);
                    while (curr_file != NULL)
                    {
                        cout  << curr_file -> d_name << endl;
                        curr_file = readdir(curr_dir);
                    }
                }
                close(outfile_fd_stdout);
            }

            else
            {
                cout << "Encountered error number " << strerror(errno) << endl;
            }
            
            close(outfile_fd);
            closedir(curr_dir);
            return 0;
        }

        else
        {
            return 0;
            cout << "Encountered error number " << strerror(errno) << endl;
        }
    }

    else if (proc_id > 0)
    {
        wait(NULL);
        cout << "Command executed" << endl;
    }

    else
    {
        cout << "Fork failed" << endl;
        cout << "Encoutered " << strerror(errno) << endl;
    }
}