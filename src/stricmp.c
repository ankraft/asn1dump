/*
 *	stricmp.c
 */

# include <ctype.h>
# include "stricmp.h"

int stricmp(const char *s1, const char *s2)
{
	if(s1 == s2)
		return(0);
	while(tolower(*s1) == tolower(*s2++))
		if(*s1++ == '\0')
			return(0);
	return(tolower(*s1) - tolower(*--s2));
}

int strcmpi(const char *s1, const char *s2)
{
	return stricmp(s1,s2);
}

int strnicmp(const char *s1, const char *s2, const int pos) 
{
	int n = pos;
	n++;
	if(s1 == s2)
		return(0);
	while(--n > 0 && tolower(*s1) == tolower(*s2++))
		if(*s1++ == '\0')
			return(0);
	return((n == 0)? 0: (tolower(*s1) - tolower(*--s2)));
}

