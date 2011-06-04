

/* readfile.c  */  

/* Read a csv file.                             */   
/* This code is released to the public domain.  */ 
/* "Share and enjoy..."  ;)                     */ 


#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>



int main() { 

int linecount=0; 

char line[120];  

FILE *fp = fopen("data.csv", "r") ; 

while( fgets(line, sizeof(line), fp) != NULL) 
 { 
    linecount++;
    printf("%d %s", linecount, line);         
 }  /* EOF */  
   
fclose(fp);   
    
    
/* Now, read some text and print it, using sscanf  */  
char myline[] = "Steven Seagal;123 Cool St;" ; 
char name[20]; 
char address[12];

if (sscanf(myline,"%14[^;]s %12[^;]s", name, address) == 2 ) 
   {   
     printf("%s %s\n", name, address);         
   }   
   
                   
return 0;   
  
}








 

