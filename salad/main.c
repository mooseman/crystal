

#include "globals.h"


#define NO_PARSE FALSE

#define NO_ANALYZE FALSE

#define NO_CODE FALSE

#include "util.h"
#if NO_PARSE
#include "scan.h"
#else
#include "parse.h"
#if !NO_ANALYZE
#include "analyze.h"
#if !NO_CODE
#include "cgen.h"
#endif
#endif
#endif
char * func[20];
/* allocate global variables */
int lineno = 0;
FILE * source;
FILE * listing;
FILE * code;
//struct my_functable
//{
//char * p;
//int type;
//}my_functable[10];
/* allocate and set tracing flags */
int EchoSource = FALSE;
int TraceScan = FALSE;
int TraceParse = FALSE;
int TraceAnalyze = FALSE;
int TraceCode = FALSE;

int Error = FALSE;

main( int argc, char * argv[] )
{ 
int i=0;
listing = stdout;
TreeNode * syntaxTree;
  char pgm[120]; /* source code file name */
  if ((argc<2)||(argc>6))
    { fprintf(stderr,"usage: %s [-S] [-s] [-p] [-t] <filename>\n",argv[0]);
      exit(1);
    }

if((argc==2)&&!strcmp(argv[1],"-h"))
{
fprintf(listing,"\n salad: the compiler for c- language.\n");
fprintf(listing,"          -S: Echo the source code\n");
fprintf(listing,"          -s: Show the scan progress\n");
fprintf(listing,"          -p: Show the parse process\n");
fprintf(listing,"          -t: Show the analysis process\n");
return 0;
}  
else if((argc==2)&&!strcmp(argv[1],"-v"))
{
fprintf(listing,"\n salad: the compiler for c- language.\n");
fprintf(listing,"\n        now the version 1.0 by saladjonk\n");
fprintf(listing,"\n        you can touch me by saladjonk@gmail.com\n");
fprintf(listing,"\n <*I wish there is some to make this compiler support the string IO*>\n\n");
return 0;
}
for(i=1;i<argc;i++)
{
if(!strcmp(argv[i],"-S"))
EchoSource=TRUE;
else if(!strcmp(argv[i],"-s"))
TraceScan=TRUE;
else if(!strcmp(argv[i],"-p"))
TraceParse=TRUE;
else if(!strcmp(argv[i],"-t"))
TraceAnalyze=TRUE;
}







  strcpy(pgm,argv[argc-1]) ;
  if (strchr (pgm, '.') == NULL)
     strcat(pgm,".sal");
  source = fopen(pgm,"r");
  if (source==NULL)
  { fprintf(stderr,"File %s not found\n",pgm);
    exit(1);
  }
  /* send listing to screen */

  fprintf(listing,"\nBegining c- compilation: %s ......\n",pgm);

#if NO_PARSE
  while (getToken()!=ENDFILE);

#else
  syntaxTree = parse();
// fprintf(listing,"\nSuccessfully constructed the syntax tree for the source file .\n");
  if (TraceParse) {
    fprintf(listing,"\nSyntax tree:\n");
    printTree(syntaxTree);
//printf("%s",syntaxTree->sibling->child[1]->sibling->attr.name);
  }

#if !NO_ANALYZE
  if (! Error)
  { if (TraceAnalyze) fprintf(listing,"\nBuilding Symbol Table...\n");
    buildSymtab(syntaxTree);
 // fprintf(listing,"\nSuccessfully builded the synbol table for the source file .\n");
  
}


#if !NO_CODE
  if (! Error)
  { char * codefile;
    int fnlen = strcspn(pgm,".");
   codefile = (char *) calloc(fnlen+4, sizeof(char));
    strncpy(codefile,pgm,fnlen);
    strcat(codefile,".tm");
    code = fopen(codefile,"w");
    if (code == NULL)
    { printf("Unable to open %s\n",codefile);
      exit(1);
    }
    codeGen(syntaxTree,codefile);
    fclose(code);
  }
#endif

#endif

#endif
fprintf(listing,"\nSuccessfully compiled.\n");
fprintf(listing,"\nNow you can run it with the TM visual machine .\n");

  fclose(source);
  return 0;
}

