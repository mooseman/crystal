

/* parser1.c  */  

/* A simple parser.                             */ 
/* This code is released to the public domain.  */ 
/* "Share and enjoy..."  ;)                     */ 


#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>
#include "parser1.h" 

                  
void select(char *tok) 
{ 
  char *token;   
  if (!strcmp(tok, "select")) 
     { 
        token = strtok(NULL, " ");   
        from(token);   
     } 
  else puts("Select error! \n");         
}           


void from(char *tok) 
{ 
  char *token;     
  if (!strcmp(tok, "from")) 
    { 
      token = strtok(NULL, " ");   
      where(token);     
    } 
  else puts("From error! \n");      
}


void where(char *tok) 
{   
  if (!strcmp(tok, "where"))  puts("Success! \n"); 
  else puts("Where error! \n");   
}



int main() { 

char *token ;   
char text[80] = "select from where" ;  

token = strtok(text, " ");     
printf("Token = %s\n", token);
select(token);            
   
char text2[80] = "select foo where" ;    
token = strtok(text2, " ");     
printf("Token = %s\n", token);
select(token);            
       
return 0;   
  
}








 

