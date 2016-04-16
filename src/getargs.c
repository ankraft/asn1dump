/*
 *	getargs.c
 *
 * This set of routines lets you manipulate the command line
 * arguments conveniently.  is_arg(str,...) returns true if
 * string str exists in the argument list.  The other functions
 * intval(str...),floatval(str...) and stringval(str...) return
 * the command line entry following the string str, converted
 * to the appropriate data type.  So, for example you could use
 * -i to flag an input file, and access the filename using
 * stringval("-i",argc,argv)
 *
 *	These functions are originaly written by Steve Lehar. 
 *	A lot of changes and improvements were made by Andreas Kraft.
 */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "getargs.h"
# include "streq.h"
# include "stricmp.h"


static	int	maxindex = 0;
static	int	ignorecase = 0;

/*:>* getargs.c *************************************************************


Name
	is_arg

Chapter
	The Q Series / Command Line Parsing

Info
	Look for a command line argument

Syntax
	int is_arg (const char *argument, const int argc, char *argv[]);

Include
	getargs.h

Section
	Command line parsing

Description
	`is_arg()` determines whether a command line `argument` exists in the
	array `argv`. `argc` and `argv` are the two parameters of the `main()`
	function.$
	It interpretes the argument `--` as the end-of-argument delimiter.

Return value
	If the argument is found in the parameter list, `is_arg()` returns it's
	index in `argv` or 0 otherwise.

Example
	% int doVerbose;
	%
	% doVerbose= is_arg ("-verbose", argc, argv);

See also
	getindex
	getremain
	nextval
	stringval

**************************************************************************<:*/


int 
is_arg(const char str[], const int argc, char * argv[]) {

	int i,
		found=0;

	for (i = 1; i < argc && found == 0; i++) {
		if (streq(argv[i], ARGS_EOA))		/* stop at -- */
			break;
		if (ignorecase) {
			if (stricmp(argv[i], str) == 0) 
				found = i;
		} else {
			if (strcmp (argv[i], str)==0) 
				found=i;
		}
	}
	maxindex = (found > maxindex) ? found : maxindex; /** akr **/
	return found ;
}


/*:>* getargs.c *************************************************************

Name
	stringval

Chapter
	The Q Series / Command Line Parsing

Info
	Get a string value of a parameter

Syntax
	char *stringval (const char *argument, const int argc, char *argv[]);

Include
	getargs.h

Section
	Command line parsing

Description
	`stringval()` returns the string value of the argument given in 
	`argument`. If the `argument` is a Null pointer the value of the 
	`argument` parameter from the last call to `stringval()` is taken and
	the search for the parameter starts with the next index.$
	`argc` and `argv` are the two parameters of the `main()` function.$
	It interpretes the argument `--` as the end-of-argument delimiter.

Return value
	If the `argument` is found in the parameter list, `stringval()`
	returns a pointer to the string or 0 otherwise.$
	It interpretes the argument `--` as the end-of-argument delimiter.

Example
	% char *tempDir= "/tmp";
	%
	% if (is_arg ("-tempdir", argc, argv))
	%	tempDir= stringval ("-tempdir", argc, argv);

See also
	floatval
	intval
	is_arg
	nextval

**************************************************************************<:*/

char * 
stringval(const char * str, const int argc, char * argv[]) {

	static char * workstr = 0;
	static int	  lastindex = 0;
	int			  i, 
				  index = 0;

	if (str) {
		if (workstr)
			free(workstr);
		workstr = strdup(str);
		if (workstr == 0)
			return 0;
		lastindex = 0;
	}
	if (workstr == 0)
		return 0;

	for (i = lastindex + 1; i < argc && index == 0; i++) {
		if (streq(argv[i], ARGS_EOA))	/* Stop at -- */
			break;
		if (streq(argv[i], workstr)) 
			index = i;
	}

	/*
	 * Wenn keine passende Option gefunden wurde, suche eine korrekt
	 * beginnende Option und geben den Rest als stringval() zurueck.
	 */

	if (index == 0) {
		int len = strlen(workstr);
		for (i= lastindex + 1; i < argc && index == 0; i++) {
			if (streq(argv[i], ARGS_EOA))
				break;
			if (ignorecase) {
				if (strnicmp(argv[i],workstr,len)== 0)
					index = i;
			} else {
				if (strncmp(argv[i],workstr,(unsigned)len)== 0)
					index = i;
			}
		}
		lastindex = index;
		if (index)
			return argv[index]+len;
		return 0;
	}

	lastindex = index;
	if (index > 0 && argc > index+1) {
		maxindex = ((index+1)>maxindex) ? index+1 : maxindex; /** akr **/
		return argv[index+1];
	}
	return 0;
}


/*:>* getargs.c *************************************************************

Name
	intval

Chapter
	The Q Series / Command Line Parsing

Info
	Get an integer value of a parameter

Syntax
	int intval (const char *argument, const int argc, char *argv[]);

Include
	getargs.h

Section
	Command line parsing

Description
	`intval()` returns the integer value of the argument given in 
	`argument`. If the `argument` is a Null pointer the value of the 
	`argument` parameter from the last call to `intval()` is taken and
	the search for the parameter starts with the next index.$
	`argc` and `argv` are the two parameters of the `main()` function.$
	It interpretes the argument `--` as the end-of-argument delimiter.

Return value
	If `argument` is found in the parameter list, `intval()`
	returns the integer value or 0 otherwise.

Example
	% int length = 0;
	%
	% if (is_arg ("-length", argc, argv))
	%	length = intval ("-length", argc, argv);

See also
	floatval
	stringval
	is_arg
	nextval

**************************************************************************<:*/

int 
intval(const char * str, const int argc, char * argv[]) {
	char	*value;

	if ((value= stringval (str, argc, argv))== 0)
		return 0;
	return atoi (value);
}

/*:>* getargs.c *************************************************************

Name
	floatval

Chapter
	The Q Series / Command Line Parsing

Info
	Get a float value of a parameter

Syntax
	float floatval (const char *argument, const int argc, char *argv[]);

Include
	getargs.h

Section
	Command line parsing

Description
	`stringval()` returns the floating point value of the argument given in 
	`argument`. If the `argument` is a Null pointer the value of the 
	`argument` parameter from the last call to `intval()` is taken and
	the search for the parameter starts with the next index.$
	`argc` and `argv` are the two parameters of the `main()` function.$
	It interpretes the argument `--` as the end-of-argument delimiter.

Return value
	If `argument` is found in the parameter list, `floatval()`
	returns the integer value or 0 otherwise.

Example
	% float length = 0;
	%
	% if (is_arg ("-length", argc, argv))
	%	length = intval ("-length", argc, argv);

See also
	intval
	stringval
	is_arg
	nextval

**************************************************************************<:*/

float 
floatval(const char * str, const int argc, char * argv[]) {
	char	*value;

	if ((value= stringval (str, argc, argv))== 0)
		/*
		 *	The following cast is necessary for MSVC++ 4.2
		 */
		return (float)0;

	return (float)atof (value);
}

/*:>* getargs.c *************************************************************

Name
	newval

Chapter
	The Q Series / Command Line Parsing

Info
	Start command line parsing all over again

Syntax
	void newval (void);
	float floatval (const char *argument, const int argc, char *argv[]);

Include
	getargs.h

Section
	Command line parsing

Description
	`newval` resets the internal index to the last used parameter to zero. 

Return value
	None

See also
	is_arg
	getindex
	getremain

**************************************************************************<:*/

void 
newval() {
	maxindex = 0;
}


/*:>* getargs.c *************************************************************

Name
	getindex

Chapter
	The Q Series / Command Line Parsing

Info
	Get last index found so far

Syntax
	int getindex(void);

Include
	getargs.h

Section
	Command line parsing

Description
	This function returns the largest index where an argument has been
	found so far.

Return value
	The function returns an integer value.

Example
	%	lastIndex = getindex();

See also
	is_arg
	newval
	getremain

**************************************************************************<:*/

int
getindex() {
	return maxindex;
}

/*:>* getargs.c *************************************************************

Name
	getremain

Chapter
	The Q Series / Command Line Parsing

Info
	Get the number of remaining arguments

Syntax
	int getremain(const int argc);

Include
	getargs.h

Section
	Command line parsing

Description
	This function returns the number of remaining arguments in the list of
	arguments which haven't been accessed so far..
	`argc` is the number of parameters given to the main()-function.

Return value
	This function returns the number of remaining arguments as an integer.

Example
	%	while (getremain(argc))
	%		printf("%s\n", nextval(argc, argv);

See also
	is_arg
	newval
	getindex
	nextval

**************************************************************************<:*/


int 
getremain(const int argc) {
	return argc-(maxindex+1);
}


/*:>* getargs.c *************************************************************

Name
	nextval

Chapter
	The Q Series / Command Line Parsing

Info
	Get the next argument from the argument list

Syntax
	char *nextval (const int argc, const char *argv[]);

Include
	getargs.h

Section
	Command line parsing

Description
	This function returns the next command line argument as a string.
	`argc` and `argv` are the two parameters of the main()-function.$
	It interpretes the argument `--` as the end-of-argument delimiter.

Return value
	If there is a next argument in the parameter list `nextval()` returns
	a char pointer to it or 0 otherwise.

Example
	%	while (getremain(argc))
	%		printf("%s\n", nextval(argc, argv);

See also
	is_arg
	newval
	getindex
	getremain

**************************************************************************<:*/


char *
nextval(const int argc, char * argv[]) {

	char * x;

	maxindex++;
	if (maxindex < argc) {
		while (streq(argv[maxindex], ARGS_EOA)) {
			maxindex++;
			if (maxindex == argc) {
				maxindex--;
				return 0;
			}
		}
		x = argv[maxindex];
	} else {
		maxindex--;
		x = 0;
	}
	return x;
}

/*:>* getargs.c *************************************************************

Name
	skipval

Chapter
	The Q Series / Command Line Parsing

Info
	Skip an argument

Syntax
	void skiparg(const char * str, const int par, const int argc, char * argv[]);

Include
	getargs.h

Section
	Command line parsing

Description
	Skip the argument matching `str`. If `par` is set to 0, then it is
	assumed, that the skipped argument has no parameter.$
	It interpretes the argument `--` as the end-of-argument delimiter.

Return value
	None

Example
	%	skiparg("-help", 0, argc, argv);

See also
	is_arg
	getremain

**************************************************************************<:*/

    
void 
skiparg(const char * str, const int par, const int argc, char * argv[]) {
	if (par)
		stringval(str, argc, argv);
	else
		is_arg(str, argc, argv);
}



/*:>* getargs.c *************************************************************

Name
	argsIgnoreCase

Chapter
	The Q Series / Command Line Parsing

Info
	Toggle the case matching of arguments

Syntax
	void argsIgnoreCase(const int value);

Include
	getargs.h

Section
	Command line parsing

Description
	Set whether the case of argument switches shall be ignored.  0 does not
	ignore, all other value do.

Return value
	None

Example
	%	argsIgnoreCase(1);

See also
	is_arg

**************************************************************************<:*/

void 
argsIgnoreCase(const int value) {
	ignorecase = value;
}
