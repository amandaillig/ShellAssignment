#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */
#include <pthread.h>

#define PROCESS_TABLE_SIZE 5

typedef struct process {
    char * programName;
    int process_id;
};

struct process* processTable[PROCESS_TABLE_SIZE] = {0};

void* runProcess(void * programName) {
    char* fileName = (char*) programName;

    // Replace process with execve system call
    if(execve(fileName, NULL, NULL) == -1) {
        printf("There was an error processing your request.  Wrong file name?\n");
        printf("File name: %s.", fileName);
    }
}

int findNextEmptyIndex() {
    for(int i = 0; i < PROCESS_TABLE_SIZE; i++) {
        if(processTable[i] == NULL) {
            return i;
        }
    }
    return -1;
}

void getCommand(char * input) {
    printf("\nType command you would like to execute:\n");
    fgets(input,20,stdin);
}

void tokenizeString(char * input, char * programName, int * bg) {
    char *token;

    token = strtok(input, " ");
    while(token != NULL) {
        if(strcmp(token, "&\n") == 0) {
            *bg = 1;
        } else {
            strcpy(programName, token);
        }
        token = strtok(NULL, " ");
    }

    if(!(*bg)) {
        programName[strlen(programName) - 1] = '\0';
    }
}

void startFork(int bg, char * programName, int * stopLoop, int index) {
    pid_t pid = fork();

    //  ** ERROR **
    if(pid < 0) {
        printf("Error on forking\n");
    }
        // **PARENT PROCESS**
    else if(pid > 0) {
        struct process childProcess;

        pid_t childID = pid;

        childProcess.process_id = childID;
        childProcess.programName = programName;

        // ** ENTER PROCESS INTO TABLE **
        processTable[index] = &childProcess;

        int status;
        pid_t returnPid;

        //If we are not running in the background and we are the parent
        if(bg) {
            returnPid = waitpid(pid, &status,  WNOHANG);
            // Check if child process has ended
        } else {
            // Wait till our current child process is done
            waitpid(pid, &status,  0);
        }
    }
        // **CHILD PROCESS**
    else {
        // Stop the loop in the child so we don't have infinite children asking for command #plannedparenthood
        *stopLoop = 0;
        pthread_t thread1;
        // if we want to run process in background and we are the child
        if(bg) {
            // I THINK this is where the pthreads should go
            // Run pthread on runprocess() with a parameter of (void*)programName


        } else {
            // Run the process normally
            runProcess((void*) programName);
        }

    }
}

int main(int argc, char * argv[])
{
    int stopLoop = 1;
    while(stopLoop) {

        char *input[20];
        char *jobsCommand = "jobs\n";

        // ** READ USER INPUT **
        getCommand(&input);

        // ** QUIT COMMAND **
        if((strcmp(input, "quit\n") == 0) || (strcmp(input, "q\n") == 0)) {
            stopLoop = 0;
        }

            // ** SHOW JOBS **
        else if(strcmp(input, jobsCommand) == 0) {
            //showJobs(processTable);
            printf("Show Jobbios\n");
        }
            // ** START EXECUTING COMMAND **
        else {
            // ** TOKENIZING STRING **
            int bg = 0;
            char *programName[20];
            tokenizeString(&input, &programName, &bg);

            // ** FORKING **

            // Check if our process table is already full
            int index;
            index = findNextEmptyIndex();

            // If there is an empty spot
            if(index != -1) {
                startFork(bg, &programName, &stopLoop, index);
            } else {
                printf("Too many processes running\n");
            }
        }
    }
}
