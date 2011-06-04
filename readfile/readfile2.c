

/* readfile.c  */  

/* Read a csv file.                             */   
/* This code is released to the public domain.  */ 
/* "Share and enjoy..."  ;)                     */ 


#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>

char headings[40][6] ; 

struct data { 
     int obsnum; 
     char *lastname; 
     char *firstnames; 
     char *city;
     int empid ; 
     int salary ; 
} ;        

struct data mydata[4]; 



int main() { 

int i ;  

FILE *fp = fopen("data.csv", "r") ; 

 if ( fp != NULL ) 
 { 
    for(i=0; i<4; i++)  
    {           
         while(i == 0) 
           {  
             headings[40][0] = "Obs" ; 
             headings[40][1] = strtok(fp, ",");  
             headings[40][2] = strtok(NULL, ",");  
             headings[40][3] = strtok(NULL, ",");  
             headings[40][4] = strtok(NULL, ",");  
             headings[40][5] = strtok(NULL, "\n");               
             i++;      
           }   
                  
   mydata->obsnum = i ;        
   mydata->firstnames = strtok(NULL, ",");  
   mydata->lastname = strtok(NULL, ",");  
   mydata->city = strtok(NULL, ",");  
   mydata->empid = strtok(NULL, ",");  
   mydata->salary = strtok(NULL, "\n");  
   i++;       
   }  /* EOF */  
   
}  /* File exists */    
   

/*  Print the file */ 
printf("%s, %s, %s, %s, %s, %s \n", headings[40][0], 
   headings[40][1], headings[40][2], headings[40][3], 
   headings[40][4], headings[40][5] );  
  
int j; 
  
for (j=0; j<4; j++)    
  { 
    printf("%d %s %s %s %d %d", 
           mydata[j].obsnum, mydata[j].firstnames, 
           mydata[j].lastname, mydata[j].city, 
           mydata[j].empid, mydata[j].salary );   
  }  
          
return 0;   
  
}








 

