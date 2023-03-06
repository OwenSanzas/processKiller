#include "utils.h"


void writePROMPT() {
    write(STDERR_FILENO, prompt, PROMPTLEN);
}


void writeNewLine() {
    write(STDERR_FILENO, "\n", 1);
}


void writeCATCHPHRASE() {
    write(STDERR_FILENO, CATCHPHRASE, strlen(CATCHPHRASE));
}

