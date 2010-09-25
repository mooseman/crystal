

/*  binary.c  */ 

#include <stdio.h>
binary(word, table, n)
char *word;
int	table[];
int n;{
	int low, high, mid, cond;
	low = 0;
	high = n - 1;
	while (low <= high){
		mid = (low + high) / 2;
		if ((cond = strcmp(word, table[mid])) < 0)
			high = mid - 1;
		else if (cond > 0)
			low = mid + 1;
		else
			return (mid);
		}
	return (-1);
	}





