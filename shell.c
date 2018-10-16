#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

char input[20];

char* getFile()
{
printf("Type command you would like to execute:\n ");
        scanf("%s", input);
	return input;
} 



int main(int argc, char * argv[])
{
//	while(1)
//	{
		input = getFile();		
		printf(input);
//	}

}
