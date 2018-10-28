#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */
#include <pthread.h>

#define PROCESS_TABLE_SIZE 5

struct process {
    char * programName;
    int process_id;
    int parent_id;
};

struct process processTable[PROCESS_TABLE_SIZE];

void* getCommand(char* input)
{
	printf("Type command you would like to execute:\n ");
    fgets(input,20,stdin);


}

void* runProcess(void * programName) {

    char* fileName = (char*)programName;

    pid_t processID = getpid();
    pid_t parentID = getppid();

    printf("This is the child\n");
    printf("Process ID: %d\n", processID);
    printf("Finished printing id\n");
}

int main(int argc, char * argv[])
{
	//while(1)
	//
		const char* input[20];
		char* jobsCommand = "jobs";

		// read Input
		getCommand(input);
		printf("%s\n", input); //Debugging

		//If User typed in 'jobs', show job table
		if(strcmp(input, jobsCommand) == 0) {
            //showJobs(processTable);
            printf("Show Jobbios");
		} else {
            // Tokenize String so we know if there is an & or not
            char *token;
            int bg = 0;
            char *programName;
            token = strtok(input, " ");
            while(token != NULL) {
                if(strcmp(token, "&\n") == 0) {
                    bg = 1;
                } else {
                    strcpy(programName, token);
                }
                printf("token is %s\n", token, input);
                token = strtok(NULL, " ");
            }
            //Fork and get process ID
            pid_t pid = fork();

            //If less than 0, somethings wrong
            if(pid < 0) {
                printf("Error on forking\n");
            }
            //If id is not 0, we are the parent
            else if(pid > 0) {
                struct process childProcess;
                pid_t childID = pid;

                childProcess.process_id = childID;
                childProcess.programName = programName;

                // Enter process into processTable
                processTable[0] = childProcess;

                //If we are not running in the background and we are the parent
                if(!bg) {

                    // Wait till our current child process is done
                    int status;
                    waitpid(pid, &status,  0);


                    printf("This is the parent\n");
                    printf("Process ID: %d\n", getpid());
                    printf("%d\n, %s\n", processTable[0].process_id, processTable[0].programName);
                }
            }  //if it is 0, we are the child
            else {
                pthread_t thread1;
                // if we want to run process in background and we are the child
                if(bg) {
                    struct process currentProcess;

                    void* processPointer = &currentProcess;

                    // Create a thread that will do the actions for us
                    pthread_create(&thread1, NULL, runProcess, processPointer);

                    printf("Starting Thread\n");
                    pthread_join(thread1, NULL);
                    printf("Thread Done\n");
                } else {
                    // Run the process normally
                    runProcess((void*) processTable);
                }

            }
            // We now check  if bg = 1, if it is, use a pthread to fork the process, otherwise just call the method normally
            // Passing reference to process table so we modify the right thing




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
