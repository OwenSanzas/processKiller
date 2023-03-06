#include "shredder-behavior.h"

// this is the shell's pid
pid_t currentPid = -1;
// #define DEBUG 1


int main(int argc, char *argv[]) {
    // signal register 
    signalRegister();
    
    // initialize time:
    int time = 0;

    // parse time:
    time = timeParser(argc, argv);

    #ifdef DEBUG
        printf("the time is: %d \n", time);
    #endif
    
    while (1) {
        
        // print the prompt
        writePROMPT();

        // init buffer:
        char *buffer = malloc(INPUT_SIZE);
        for (int i = 0; i < INPUT_SIZE; i++) {
            buffer[i] = 0;
        }

        // get input:
        int inputNum = read(STDIN_FILENO, buffer, INPUT_SIZE);
        if (inputNum == -1) perror("Read Error");

        // case 1: EOF
        if (inputNum == 0) {
            free(buffer);
            exit(0);
        }

        // case 2: '\n'
        if (buffer[0] == '\n') {
            #ifdef DEBUG
                printf("empty input!\n");
            #endif
            free(buffer);
            continue;
        }

        
        struct parsed_command *cmd;
        int res = parse_command(buffer, &cmd);
        if (res < 0){
            perror ("Parser Error");
        }
        if (res > 0) {
            perror ("Invalid Input!");
        }

        free(buffer);

        #ifdef DEBUG
            print_parsed_command(cmd);
        #endif
        

        // case 3: all spaces
        if (cmd->num_commands == 0) {
            free(cmd);
            continue;
        }

        if (cmd == NULL) {
            printf("the command is null \n");
        }
        
        // now the input is valid
        cmdExecution(cmd, time);
        free(cmd);
        currentPid = -1;
    }
}   

