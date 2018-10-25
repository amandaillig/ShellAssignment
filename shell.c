#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */

#define PROCESS_TABLE_SIZE 5

struct process {
    char programName[20];
    int process_id;
    int parent_id;
};

struct processTableEntry {
    struct process currentProcess;
};

void* getCommand(char* input)
{
	printf("Type command you would like to execute:\n ");
    fgets(input,20,stdin);
}

void* forkProcess(struct process processTable[PROCESS_TABLE_SIZE]) {
    //Fork and get process ID
    pid_t pid = fork();

    //If -1, somethings wrong
    if(pid == -1) {
        printf("Error on forking\n");
    }
        //If id is not 0, we are the parent
    else if(pid > 0) {
        int status;
        waitpid(pid, &status,  0);
        printf("This is the parent\n");
        printf("Process ID: %d\n", getpid());
    }  //if it is 0, we are the child
    else {
        pid_t processID = getpid();
        pid_t parentID = getppid();

        struct process currentProcess;

        currentProcess.process_id = processID;
        currentProcess.parent_id = parentID;

        processTable[0] = currentProcess;

        printf("This is the child\n");
        printf("Process ID: %d\n", processID);
        printf("Finished printing id\n");
    }
}

int main(int argc, char * argv[])
{
    struct process processTable[PROCESS_TABLE_SIZE];
	//while(1)
	//
		const char* input[20];
		char* jobsCommand = "jobs";
		getCommand(input);
		printf("%s\n", input);

		//If User typed in 'jobs', show job table
		if(strcmp(input, jobsCommand) == 0) {
            //showJobs(processTable);
            printf("Show Jobbios");
		} else {
            //TODO: Tokenize string, create a struct and add to command array, fork()
            // Passing reference to process table so we modify the right thing
            forkProcess(&processTable);

            printf("%d\n", processTable[0].process_id);

            printf("Finished");
		}
	//}


    /*
void* showJobs(struct Commands cmdArray[]) {
    printf("Here are the jobs being run:\n");
    //Check if array is empty, if not cycle through array and print out necessary information
}

struct processTableEntry createStruct(char* input) {
    struct processTableEntry processTable[5];

    char *token;
    token = strtok(input, " ");
    while(token != NULL) {

        if(strcmp(token, "&") == 0) {
            // Background process logic
            // We might need to move this outside of this method, we probably don't want to have the threads be created in here
        } else {
            strcpy(cmdTemp.programName, token);
        }
        printf("token is %s\n", token, input);
        token = strtok(NULL, " ");
    }

    return cmdTemp;
}

void* executeProgram(struct Commands cmd) {
    pid_t pid=fork();
    if (pid==0) {
        execv(cmd.programName,NULL);
        exit(127);
    }
    else {
        waitpid(pid,0,0);
    }
}
*/
}
