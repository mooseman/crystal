

/*  data.c  */  

#include <stdio.h>
#include "defs.h"

/* storage words */

char	symtab[SYMTBSZ];
char	*glbptr, *rglbptr, *locptr;
int	ws[WSTABSZ];
int	*wsptr;
int	swstcase[SWSTSZ];
int	swstlab[SWSTSZ];
int	swstp;
char	litq[LITABSZ];
int	litptr;
char	macq[MACQSIZE];
int	macptr;
char	line[LINESIZE];
char	mline[LINESIZE];
int	lptr, mptr;

/* miscellaneous storage */

int	nxtlab,
	litlab,
	stkp,
	argstk,
	ncmp,
	errcnt,
	glbflag,
	ctext,
	cmode,
	lastst;

FILE	*input, *input2, *output;
FILE	*inclstk[INCLSIZ];
int	inclsp;
char	fname[20];

char	quote[2];
char	*cptr;
int	*iptr;
int	fexitlab;
int	iflevel, skiplevel;
int	errfile;
int	sflag;
int	cflag;
int	errs;
int	aflag;


