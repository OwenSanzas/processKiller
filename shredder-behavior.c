#include "shredder-behavior.h"


/* ------------------------------------------------------------------------
------------------------- Shredder Behaviour Functions -----------------------
------------------------------------------------------------------------*/

/**
 * Function: getAndParseInput
 * --------------------
 * 
 * parse the input from the user
 * 
 * buffer: arguments count
 * argc[]: array of arguments
 * 
 * returns: int representing the running mode of the shell
 */
void cmdExecution(struct parsed_command *cmd, int time) {
    currentPid = fork();
    alarm(time);
    if (!currentPid) {
        // child process
        execve(cmd->commands[0][0], cmd->commands[0], NULL);
        perror(*cmd->commands[0]);
        exit(EXIT_FAILURE);
    } else {
        // parent process, wait the child
        int waitStatus;
        #ifdef EC_NOKILL
        do {
            waitpid(currentPid, &waitStatus, 0);
        } while (!WIFEXITED(waitStatus) && !WIFSIGNALED(waitStatus));

        #else
        waitpid(currentPid, &waitStatus, 0);
        #endif

        alarm(0);
        
    }
}




/**
 * Function: timeParser
 * --------------------
 * 
 * parse the input from the user
 * 
 * argc: same as the parameters of the main function, number of args
 * argv[]: args string
 * 
 * returns: parsed stop time of the shredder
 */
int timeParser(int argc, char *argv[]) {
    // corner case: if there is only one arg, set the time to 0
    if (argc != 2) {
        printf("No time input! The shredder will run with 0 stop time.\n");
        return 0;
    }

    int time = atoi(argv[1]);

    if (time > 0) {
        return time;
    } else {
        printf("Invalid input! The shredder will run with 0 stop time.\n");
        return 0;
    }

    
}








/* ------------------------------------------------------------------------
---------------------------- Helper Functions -----------------------------
------------------------------------------------------------------------*/
/**
 * Function: sigHandler
 * --------------------
 * 
 * Signal handler helper function
 * 
 * sig: recived signal



 * Function: sigRegister
 * --------------------
 * 
 * Signal register helper function for SIGINT and SIGALRM


 * Function: initBuffer
 * --------------------
 * 
 * Signal register helper function for SIGINT and SIGALRM
 


 */



void signalRegister() {
    if (signal(SIGINT, signalHandler) == SIG_ERR) {
        perror("signal error");
    }

    if (signal(SIGALRM, signalHandler) == SIG_ERR) {
        perror("signal error");
    }
}

void signalHandler(int signal) {
    // for parent process (shell), ctrl+c will kill the child process but not the shell
    // if the current process is the child, kill it
    #ifdef EC_NOKILL
    // TODO: MAC part: do not use kill in this section
    if (currentPid) {
            // the child pid is taken from the parent
            if (signal == SIGALRM) {
                writeCATCHPHRASE();
                return;
            }
        }
    #else
        if (currentPid) {
            // the child pid is taken from the parent
            if (signal == SIGALRM) {
                writeCATCHPHRASE();
                kill(currentPid, SIGKILL);
                return;
            }

            if (signal == SIGINT) {
                if (kill(currentPid, SIGKILL) == -1) {
                perror("Kill error");
                }
            }
        }
    #endif
    writeNewLine();
    writePROMPT();
}
