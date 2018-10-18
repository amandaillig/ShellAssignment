#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

struct Commands {
    char programName[20];
    char pid[20]; //Should this be int?  I'm too lazy rn to care
    //Boolean for if & is included or not?
};

void* getCommand(char* input)
{
	printf("Type command you would like to execute:\n ");
    fgets(input,20,stdin);
} 

void* showJobs(struct Commands cmdArray[]) {
    printf("Here are the jobs being run:\n");
    //Check if array is empty, if not cycle through array and print out necessary information
}

struct Commands createStruct(char* input) {
    struct Commands cmdTemp;
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

int main(int argc, char * argv[])
{
    struct Commands arr_commands[5];
	//while(1)
	//
		const char* input[20];
		char* jobsCommand = "jobs";
		getCommand(input);
		printf("%s\n", input);

		if(strcmp(input, jobsCommand) == 0) {
            showJobs(arr_commands);
		} else {
            //TODO: Tokenize string, create a struct and add to command array, fork()

            //char inputTokenString[20];
            //strcpy(inputTokenString, input);

            struct Commands cmd = createStruct(input);
            //TODO: Add struct to array
            printf("Finished");
		}
	//}

}
