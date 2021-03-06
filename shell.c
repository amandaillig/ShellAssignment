#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */
#include <pthread.h>

/*
 * Intro To Operating Systems
 * Assignment 2: Shell
 *
 * Authors: Matthew Muenzberg, Mandy Illig, Maxwell Hicks
 */

#define PROCESS_TABLE_SIZE 5

struct process {
    char programName[20];
    int process_id;
    int active;
};

struct process processTable[PROCESS_TABLE_SIZE];

void* runProcess(void * programName) {
    char* fileName = (char*) programName;

    // Replace process with execve system call
    if(execve(fileName, NULL, NULL) == -1) {
        printf("There was an error processing your request.  Wrong file name?\n");

    }
}

int findNextEmptyIndex() {
    for(int i = 0; i < PROCESS_TABLE_SIZE; i++) {
        if(!processTable[i].active) {
            return i;
        }
    }
    return -1;
}

void cleanUpProcessTable() {
    for (int i = 0; i < PROCESS_TABLE_SIZE; i++) {
        if (processTable[i].active) {
            int status;
            pid_t pid = processTable[i].process_id;
            waitpid(pid, &status, WNOHANG);

            if (WIFEXITED(status)) {
                processTable[i].active = 0;
            }
        }
    }
}

void getCommand(char input[20]) {
    printf("\nbw: ");
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

void showJobs() {
    cleanUpProcessTable();
    printf("PID\t\tProgram\n");
    for(int i = 0; i < PROCESS_TABLE_SIZE; i++) {
        if(processTable[i].active) {
            struct process p = processTable[i];
            printf("%d\t\t%s\n", p.process_id, p.programName);
        }
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

        childProcess.process_id = pid;
        strcpy(childProcess.programName, programName);
        childProcess.active = 1;

        int status;

        //If we are not running in the background and we are the parent
        if(bg) {
            // ** ENTER PROCESS INTO TABLE **
            processTable[index] = childProcess;

            waitpid(pid, &status,  WNOHANG);
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
            // Create a thread that will do the actions for us
            pthread_create(&thread1, NULL, runProcess, (void*)programName);

            pthread_join(thread1, NULL);

        } else {
            // Run the process normally
            runProcess((void*) programName);
        }
    }
}

int main(int argc, char * argv[])
{
    printf("Shellinator v9000\n");
    printf("Authors: Matthew Muenzberg, Mandy Illig, Maxwell Hicks\n");
    printf("Type 'jobs' to see a list of jobs, type 'q' or 'quit' to exit shell");
    int stopLoop = 1;
    while(stopLoop) {

        cleanUpProcessTable();

        char input[20];
        char *jobsCommand = "jobs\n";

        // ** READ USER INPUT **
        getCommand(input);

        // ** QUIT COMMAND **
        if((strcmp(input, "quit\n") == 0) || (strcmp(input, "q\n") == 0)) {
            stopLoop = 0;
        }
            // ** SHOW JOBS **
        else if(strcmp(input, jobsCommand) == 0) {

            showJobs();
        }
            // ** START EXECUTING COMMAND **
        else {
            // ** TOKENIZING STRING **
            int bg = 0;
            char *programName[20];
            tokenizeString(input, programName, &bg);

            // ** FORKING **

            // Check if our process table is already full
            int index;
            index = findNextEmptyIndex();

            // If there is an empty spot
            if(index != -1) {
                startFork(bg, programName, &stopLoop, index);
            } else {
                printf("Too many processes running\n");
            }
        }
    }
}
