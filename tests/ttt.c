
#ifdef TEST

#  include <stdio.h>

#  define ITEST(a,b)	if ( a == b ) printf("OK  ");\
			else {printf("FAIL");totresult++;}\
			printf(" %s = %d : %d\n", #a, a, b);
#  define RTEST(a,b)	if ( a == b ) printf("OK  ");\
			else {printf("FAIL");totresult++;}\
			printf(" %s = %g : %g\n", #a, a, b);
int main() {
long v=1, index=1, step=1, set=1;
long v1=1, v2=1, v3=1, v4=1;
double r1=1, r2=1, r3=1, r4=1;
char *text[] = {
		"v	=	-1",
		"# comment",
		"index	=	3",
		"r3=-3.05",
		"step=0x02",
		"	v1 = 017",
		"v2 = 0xFF",
		"r1=2.0 r2	=	  -0.5",
		" v3	=-0xFF,	v4=-027",
		};
int i, totresult = 0, num=sizeof(text)/sizeof(text[0]);

	printf("test input: %d lines\n",num);
	for (i=0; i < num; ++i) {
		printf("%s\n", text[i]);
	}
	printf("\n");

	for (i=0; i < num; ++i) {
		(void) NAMETOL(text[i], step);
		(void) NAMETOL(text[i], v);
		(void) NAMETOL(text[i], index);
		(void) NAMETOL(text[i], set);
		(void) NAMETOL(text[i], v1);
		(void) NAMETOL(text[i], v3);
		(void) NAMETOL(text[i], v2);
		(void) NAMETOL(text[i], v4);
		(void) NAMETOD(text[i], r2);
		(void) NAMETOD(text[i], r3);
		(void) NAMETOD(text[i], r1);
	}
/* compare to expected result */
	ITEST(v,-1);
	ITEST(index,3);
	ITEST(step,2);
	ITEST(set,1);
	ITEST(v1,15);
	ITEST(v2,255);
	ITEST(v3,-255);
	ITEST(v4,-23);
	RTEST(r1,2.0);
	RTEST(r2,-0.5);
	RTEST(r3,-3.05);
	RTEST(r4,1.0);
	fprintf(stderr,"\nThere were ");
	if (totresult == 0) fprintf(stderr,"NO");
	else fprintf(stderr,"%d",totresult);
	fprintf(stderr," nameread test failures\n");
	return totresult;
}
#endif /* TEST */
