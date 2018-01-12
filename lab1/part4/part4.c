#include <ctype.h>

#include "part4.h"

void str_lower(char string[], char buffer[]) {
	for (int i = 0; string[i]; i++) {
		buffer[i] = tolower(string[i]);
	}
}

void str_lower_mutate(char string[]) {
	for (int i = 0; string[i]; i++) {
		string[i] = tolower(string[i]);
	}
}