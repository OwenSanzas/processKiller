// head files
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_SIZE 4096

// methods I used
void setAlarm();
void alarm_handler();
void input_handler();
int wordCount();

// initialize process id
pid_t pid = 0;

/*
    This method sets an alarm signal by calling alarm()
    input: int time is the timeout we entered in the beginning
    output: None
*/
void setAlarm (int time) {
  if (signal(SIGALRM, alarm_handler) == SIG_ERR) {
    perror("Signal Error");
  }
  alarm(time);
}

/*
    This method handles the alarm signal
    input: int sig is the signal response got by signal()
    output: None
*/
void alarm_handler(int sig){

  if (pid > 0){
    const char catchphrase[] = "Bwahaha ... Tonight, I dine on turtle soup!\n";
    if (write(1, catchphrase, sizeof(catchphrase)) == -1){
      perror("Failed to write.");
    }
    if (kill(pid, SIGALRM) == -1) {
      perror("Fail to kill.");
    }
  }

}

/*
    This method counts the number of input worlds and returns the number of words.
    It will check all-space input in the beginning
    input: char buffer[] is the input str from the user
    input: int num is the length of the input captured by read()
    output: int wordNum is the number of words in the input
*/
int wordCount(char buffer[], int num) {
  int wordNum = 0;
  int sc = 1;
  //printf("the first element is:%clllllll", buffer[0]);
  for (int j = 0; j < num; j++) {
    if (buffer[j] != 0) {
      // printf("not space %c", buffer[j]);
      sc = 0;
    }
  }

  if (sc == 1) {
    // printf("space input");
    return 0;
  }

  for (int i = 0; i < num - 1; i++) {
    if (i == 0 && buffer[0] != ' ') {
      wordNum++;
    } else if (buffer[i] != ' ' && buffer[i - 1] == ' '){
      wordNum++;
    }
  }
  // printf("we read %i args.", wordNum);
  return wordNum;
 }


/*
    This method handles the input signal (ctrl + c)
    input: int sig is the signal response got by signal()
    output: None
*/
void input_handler (int sig) {

  if (pid > 0) {
    if (kill(pid, SIGKILL) == -1) {
      perror("Fail to kill.");
      exit(EXIT_FAILURE);
    }
    write(STDERR_FILENO, "\n", 1); 
  } else {
      write(STDERR_FILENO, "\n", 1);
      write(STDERR_FILENO, "penn-shredder#", 16);
    }
}

int main(int argc, char** argv) {
    // initialize signal registers
    signal(SIGALRM, alarm_handler);
    signal(SIGINT, input_handler);

    // initial time
    int time = 0;

    // C review:
    // argc: the number of arguments for main function
    // argv: an array of arguments:
    // if the input is ./penn-shredder 2, argv = {"./penn-shredder", "2"}
    // use atoi function to convert the time into int
    if (argc == 2) {
      time = atoi(argv[1]);
    }
    
    // if the input time is invalid: i.e. negative numbers or not digital values, set time to 0
    if (time <= 0) {
      const char invalidTimePrompt[] = "Invalid input! Running with 0 timeout\n";
      int size = strlen(invalidTimePrompt);
      write(STDERR_FILENO, invalidTimePrompt,  size);
      time = 0;
    }

   
    // the loop of this shell
    while (1) {
      // initialize waitstatus for wait()
      int waitStatus = 0;

      // reset process id for every loop
      pid = 0;

      // show PROMPT:
      const char shellPrompt[] = "penn-shredder#";
      int size = strlen(shellPrompt);

      // if the printing of shell prompt fails, show the error msg
      if (write(STDERR_FILENO, shellPrompt, size) == -1) {
        perror("Write Error");
      }

      // define input char[] and put 0s in it
      char buffer[INPUT_SIZE];
      for (int i = 0; i < INPUT_SIZE; i++) {
        buffer[i] = 0;
      }

      // inputRead is a response from read function and the number of bytes in the input char[]:
      // C review: 1 char = 1 byte
      int inputRead = read(STDIN_FILENO, buffer, INPUT_SIZE);
    
      // handle the end of file(EOF)
      if (inputRead == 0) {
        exit(0);
      } else if (inputRead == -1) {
        perror("Read Error");
      }

      // case 1: if the input is a Enter key:
      if (buffer[0] == '\n') {
        // go to the next loop again
        continue;
      }

      // case 2: if the input is nothing:
      if (buffer[0] == '\0') {
        continue;
      }

      // case 3 and 4: all-space input and normal input:
      int argNum = wordCount(buffer, inputRead);
      if (argNum == 0) {
        continue;
      }

      //printf("it has %i words", argNum);
      // the memory size depends on how many tokens we have.
      char **command = (char **)malloc((argNum + 1) * sizeof(char*));
      if (command == NULL) {
        perror("Malloc Error");
        exit(EXIT_FAILURE);
      }
      char **envVariable = {NULL};

      // the first token is command[0]
      command[0] = strtok(buffer, " \n\t");
      int i = 1;
      for (i = 1; i < argNum; i++) {
        command[i] = strtok(NULL, " \n\t");
       // printf("\nfinal command is: %s and ", command[i]);
      }
      command[i] = NULL;
          
      // now the buffer is the command we want to use, then we need to fork a child process
      setAlarm(time);
      pid = fork();
      
      if (pid == 0) {
        // execve(): if sleep 3
        // execve("/bin/sleep", {"/bin/sleep", "3", NULL}, NULL);
        int err = execve(command[0], command, envVariable);
        // free command
        free(command);
        if (err == -1) {
          perror ("Execve Error");
          exit(EXIT_FAILURE);
        }
        } else if (pid < 0) { // fork error, failed to create a child process
          perror("Fork Error");
          exit(EXIT_FAILURE);
        } else { // now the parent process is running
          pid_t pid1;
          do {
            pid1 = wait(&waitStatus);
            if (pid1 == -1) {
              perror("Wait Error");
              exit(EXIT_FAILURE);
            }

            if (WIFEXITED(waitStatus)) {
              alarm(0);
            }
          } while (!WIFEXITED(waitStatus) && !WIFSIGNALED(waitStatus));
          int waitResponse = wait(&waitStatus);
          if (waitResponse == -1) {
          } 
        }
        //free(buffer);
        //free(cleanedBuffer);
        free(command);
        alarm(0);
    }
}