#include <stdio.h>

#include "part4.h"

int main(void) {
	char string[] = "Hello, World!";
	char buffer[13];
	str_lower(string, buffer);
	printf("%s\n", buffer);
	str_lower_mutate(string);
	printf("%s\n", string);
	
	return 0;
}
