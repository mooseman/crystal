

/* readfile.c  */  

/* Read a csv file.                             */   
/* This code is released to the public domain.  */ 
/* "Share and enjoy..."  ;)                     */ 


#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>


struct col { 
    char heading[10]; 
    int data[4];
};        


struct col mycol[5]; 


int main() { 

int i ;  

/* Set the "obs" column heading.  */  
/* mycol[0].heading = "Obs" ;  */ 

FILE *fp = fopen("data2.csv", "r") ; 

if ( fp != NULL ) 
 { 
    fscanf(fp, "%5s, %5s, %5s, %7s, %7s \n", 
       mycol[0].heading, mycol[1].heading, mycol[2].heading, 
       mycol[3].heading, mycol[4].heading ) ;   
                  
    for(i=1; i<5; i++)  
    {                                             
       fscanf(fp, "%2d, %2d, %2d, %4d, %4d \n", 
          &mycol[0].data[i], &mycol[1].data[i], 
          &mycol[2].data[i], &mycol[3].data[i], 
          &mycol[4].data[i] );             
          /* i++; */        
    }  /* EOF */  
   
  fclose(fp);   
   
}  /* File exists */    
   

/*  Print the file */ 

printf("%5s %5s %5s %7s %7s \n", 
   mycol[0].heading, mycol[1].heading, 
   mycol[2].heading, mycol[3].heading,     
   mycol[4].heading );        
 
 
  
int j; 
  
for (j=1; j<5; j++)    
  { 
    printf("%2d %2d %2d %4d %4d \n", 
       mycol[0].data[j], mycol[1].data[j], 
       mycol[2].data[j], mycol[3].data[j],     
       mycol[4].data[j] );                   
  }  
      
          
return 0;   
  
}








 

