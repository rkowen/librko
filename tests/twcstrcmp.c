#include <stdio.h>
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#include "librko.h"

int main() {
	char test[][3][20] = {
	/*	str,		wc str,			match */
		{"abcdefgh",	"abcdefgh",		1	},
		{"abcdefgh",	"abcdefgi",		0	},
		{"abcdefgh",	"abcdef",		0	},
		{"abcdefgh",	"ab*gh",		1	},
		{"abcdefgh",	"ab*gi",		0	},
		{"abcdefgh",	"ab*ghi",		0	},
		{"abcdefgh",	"ab*g",			0	},
		{"abcdefgh",	"a??d??gh",		1	},
		{"abcdefgh",	"a??d??gi",		0	},
		{"abcdefgh",	"a??d??ghi",		0	},
		{"abcdefgh",	"a??d??g",		0	},
		{"abcdefgh",	"a??d*gh",		1	},
		{"abcdefgh",	"a??d*gi",		0	},
		{"abcdefgh",	"a??d*ghi",		0	},
		{"abcdefgh",	"a??d*g",		0	},
		{"abcdefgh",	"a??d*h",		1	},
		{"abcdefgh",	"a??d*i",		0	},
		{"abcdefgh",	"ab*",			1	},
		{"abcdefgh",	"*gh",			1	},
		{"abcdefgh",	"*",			1	},
		{"abcdefgh",	"????????",		1	},
		{"abcdefgh",	"???????",		0	},
		{"abcdefgh",	"?????????",		0	},
		{"ab*defgh",	"ab\\**gh",		1	},
		{"abcdefgh",	"ab\\**gh",		0	},
		{"ab*defgh",	"ab\\*d*h",		1	},
		{"ab*d?fgh",	"ab\\*d\\?*h",		1	},
		{"ab*defgh",	"ab\\*d\\?*h",		0	},
		{"abcabcgh",	"a*cgh",		1	},
		{"abcdefgh",	"*def*",		1	},
		{"abcdefgh",	"*defgh*",		1	},
		{"abcdefgh",	"abc*defgh",		1	},
	};
	int i, result, totresult = 0, num = sizeof(test)/sizeof(test[0]);
	for (i = 0; i < num; ++i) {
		result = wcstrcmp(test[i][0], test[i][1]);
		if (result != 0 ) {
			if (*test[i][2] == 0) printf("OK   :");
			else {
				totresult++;
				printf("FAIL :");
			}
		} else {
			if (*test[i][2] == 1) printf("OK   :");
			else {
				totresult++;
				printf("FAIL :");
			}
		}
		printf("%15.15s %15.15s : %d\n", test[i][0], test[i][1],
			result);
	}
	fprintf(stderr,"\nThere were ");
	if (totresult == 0) fprintf(stderr,"NO");
	else fprintf(stderr,"%d", totresult);
	fprintf(stderr," wildcard test failures.\n\n");
	return totresult;
}
