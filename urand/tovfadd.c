#include <stdio.h>

/* check whether addition allows overflows and/or sets the sign bit */

int main() {
	int i = 7, j;
	int isize = (int) 8*sizeof(int);
	for (j = 0; j < 2*isize; ++j) {
		printf("%d : %d\n", j, i);
		i += i;
		if (j%3 == 0) ++i;
	}
	return 0;
}
