#include <ctype.h>

void str_lower(char string[], char buffer[]) {
	int len = sizeof(string) / sizeof(string[0]);
	for (int i; i < len; i++) {
		buffer[i] = tolower(string[i]);
	}
	buffer
}

void str_lower_mutate(char string[]) {
	int len = sizeof(string) / sizeof(string[0]);
	for (int i; i < len; i++) {
		string[i] = tolower(string[i]);
	}
}