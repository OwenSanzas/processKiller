#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "utils.h"

#define INPUT_SIZE 4096


extern pid_t currentPid;

void signalRegister();
void signalHandler(int signal);
int timeParser(int argc, char *argv[]);
// struct parsed_command *getAndParseInput(char *buffer, struct parsed_command *cmd);
void cmdExecution(struct parsed_command *cmd, int time);