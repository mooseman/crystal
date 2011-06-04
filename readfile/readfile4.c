

/* readfile.c  */  

/* Read a csv file.                             */   
/* This code is released to the public domain.  */ 
/* "Share and enjoy..."  ;)                     */ 


#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>

char headings[12][6] ; 

struct data { 
     int obsnum; 
     char lastname[10]; 
     char firstnames[20];
     char city[20];
     int empid ; 
     int salary ; 
} ;        

struct data mydata[4]; 


int main() { 

int i ;  

headings[12][0] = "Obs" ;     

FILE *fp = fopen("data.csv", "r") ; 

if ( fp != NULL ) 
 { 
    fscanf(fp, "%s, %s, %s, %s, %s \n", 
       headings[12][1], headings[12][2], headings[12][3], 
       headings[12][4], headings[12][5] ) ;   
                  
    for(i=0; i<5; i++)  
    {                                      
       mydata[i].obsnum = i ;       
       fscanf(fp, "%12s, %22s, %22s, %d, %d \n", 
          mydata[i].firstnames, mydata[i].lastname, mydata[i].city, 
          &mydata[i].empid, &mydata[i].salary );             
          /* i++; */        
    }  /* EOF */  
   
  fclose(fp);   
   
}  /* File exists */    
   

/*  Print the file */ 

printf("%s, %s, %s, %s, %s, %s \n", 
   headings[12][0], headings[12][1], 
   headings[12][2], headings[12][3], 
   headings[12][4], headings[12][5] );  
  
int j; 
  
for (j=0; j<5; j++)    
  { 
    printf("%d, %15s, %15s, %20s, %d, %d \n", 
           mydata[j].obsnum, mydata[j].firstnames, 
           mydata[j].lastname, mydata[j].city, 
           mydata[j].empid, mydata[j].salary );   
   /* j++; */              
  }  
          
return 0;   
  
}








 

