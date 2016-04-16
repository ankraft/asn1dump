/*
 *	streq.c
 */

# include <assert.h>
# include <string.h>
# include "streq.h"

int streq( const char *s1, const char *s2 )
{
	assert(s1);
	assert(s2);
	return (strcmp(s1,s2)== 0);
}
