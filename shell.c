#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void* getFile(char* input)
{
	printf("Type command you would like to execute:\n ");
	scanf("%s", input);
} 



int main(int argc, char * argv[])
{
	while(1)
	{
		const char* input[20];
		getFile(input);
		printf(input);
	}

}
