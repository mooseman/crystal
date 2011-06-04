

#include "globals.h"
#include "util.h"
#include "scan.h"


typedef enum
   { START,INASSIGN,INCOMMENT,INNUM,INOVER,INENDCOMMENT,INID,INLTEQ,INBGEQ,INNEQ,DONE }
   StateType;


char tokenString[MAXTOKENLEN+1];

#define BUFLEN 256

static char lineBuf[BUFLEN]; /* holds the current line */
static int linepos = 0; /* current position in LineBuf */
static int bufsize = 0; /* current size of buffer string */
static int EOF_flag = FALSE; /* corrects ungetNextChar behavior on EOF */


static int getNextChar(void)
{ if (!(linepos < bufsize))
  { lineno++;
    if (fgets(lineBuf,BUFLEN-1,source))
    {if (EchoSource) fprintf(listing,"%4d: %s",lineno,lineBuf);


      bufsize = strlen(lineBuf);
      linepos = 0;

      return lineBuf[linepos++];
    }
    else
    { EOF_flag = TRUE;
      return EOF;
    }
  }
  else 
{


return lineBuf[linepos++];
}
}


static void ungetNextChar(void)
{ if (!EOF_flag) linepos-- ;}

/* lookup table of reserved words */
static struct
    { char* str;
      TokenType tok;
    } reservedWords[MAXRESERVED]
   = {{"if",IF},{"else",ELSE},
      {"int",INT},{"return",RETURN},{"void",VOID},
      {"while",WHILE},{"input",INPUT},{"output",OUTPUT},{"main",MAIN}};


static TokenType reservedLookup (char * s)
{ int i;
  for (i=0;i<MAXRESERVED;i++)
    if (!strcmp(s,reservedWords[i].str))
      return reservedWords[i].tok;
  return ID;
}


TokenType getToken(void)
{  
   int tokenStringIndex = 0;
  
   TokenType currentToken;
   
   StateType state = START;

   int save;
//printf("DDDDDD");
   while (state != DONE)
{

 int c = getNextChar();
 save = TRUE;
     switch (state)
     { case START:
         if (isdigit(c))
           state = INNUM;
         else if (isalpha(c))
           state = INID;
         else if (c == '=')
           state = INASSIGN;
         else if (c == '<')
           state = INLTEQ;
         else if (c == '>')
           state = INBGEQ;
         else if (c == '!')
           state = INNEQ;
         else if ((c == ' ') || (c == '\t') || (c == '\n'))
           save = FALSE;
         else if (c == '/')
          {
           save=FALSE;
           state = INOVER;
          }
         else
         { state = DONE;
           switch (c)
           { 
             case EOF:
               save = FALSE;
               currentToken = ENDFILE;
               break;
             case ':':
               currentToken=ERROR;
               break;
             case '{':
               currentToken = LCIR;
               break;
             case '}':
               currentToken = RCIR;
               break;
             case '[':
               currentToken = LQUATA;
               break;
             case ']':
               currentToken = RQUATA;
               break;
             case '+':
               currentToken = PLUS;
               break;
             case '-':
               currentToken = MINUS;
               break;
             case '*':
               currentToken = TIMES;
               break;
             case '(':
               currentToken = LPAREN;
               break;
             case ')':
               currentToken = RPAREN;
               break;
             case ';':
               currentToken = SEMI;
               break;	
             case ',':
               currentToken = COMOS;
               break;
             default:
               currentToken = ERROR;
               break;
           }
         }
         break;
       case INLTEQ:
            state=DONE;
            if(c=='=')
            currentToken=LTEQ;
            else
            {
              ungetNextChar();
              currentToken=LT;
            }
            break;
       case INBGEQ:
            state=DONE;
            if(c=='=')
            currentToken=BGEQ;
            else
            {
              ungetNextChar();
              currentToken=BG;
            }
            break;    
       case INNEQ:
            state=DONE;
            if(c=='=')
            currentToken=NEQ;
            else
            {
               ungetNextChar();
               currentToken=ERROR;
            }
            break;

       case INOVER:
           save = FALSE;
           if(c=='*')
            state=INCOMMENT;
           else
             {
                ungetNextChar();
                 state=DONE;
                 tokenString[tokenStringIndex++] = '/';
                   currentToken=OVER;
           }
          break;
       case INCOMMENT:
         save = FALSE;
         if (c == EOF)
         { 
           state = DONE;
           currentToken = ENDFILE;
         }
         else if (c == '*') 
              state = INENDCOMMENT;
         break;
       case INENDCOMMENT:
         save = FALSE;
         if (c = '/')
         state = START;
         else
         state = INCOMMENT;
         break;
       case INASSIGN:
         state = DONE;
         if (c == '=')
           currentToken = EQ;
         else
         { 
           ungetNextChar();
           //save = FALSE;
           currentToken = ASSIGN;
         }
         break;
       case INNUM:
         if (!isdigit(c))
         { 
           ungetNextChar();
           save = FALSE;
           state = DONE;
           currentToken = NUM;
         }
         break;
       case INID:
         if (!isalpha(c))
         { 
           ungetNextChar();
           save = FALSE;
           state = DONE;
           currentToken = ID;
         }
         break;
       case DONE:
       default: 
         fprintf(listing,"Scanner Bug: state= %d\n",state);
         state = DONE;
         currentToken = ERROR;
         break;
     }
     if ((save) && (tokenStringIndex <= MAXTOKENLEN))
       tokenString[tokenStringIndex++] = (char) c;
     if (state == DONE)
     { tokenString[tokenStringIndex] = '\0';
       if (currentToken == ID)
         currentToken = reservedLookup(tokenString);
     }
   }
   if (TraceScan) {
     fprintf(listing,"\t%d: ",lineno);
     printToken(currentToken,tokenString);
   }
   return currentToken;
} /* end getToken */

