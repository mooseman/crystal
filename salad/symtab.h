

#ifndef _SYMTAB_H_
#define _SYMTAB_H_


void st_insert( char * name, int lineno, int loc,int pos );


int st_lookup ( char * name,int pos );



void printSymTab(FILE * listing,int pos);

#endif
