/*
 * quick & dirty test of the strtok() fn
 */
#include <stdio.h>
#include <string.h>

int main() {
	char *string = "This is the year of our discontent\n";
	char *delim = " \t\v\a";
	char *ptr = string;
	char *word;

	do {
		word = strtok(ptr, delim);
		ptr = (char *) NULL;
		printf(":%s:\n", word);
	} while (word != (char *) NULL);

	return 0;
}
