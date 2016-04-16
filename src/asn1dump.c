/*
 *	asn1dump.c
 *
 *	show the structure of an ASN.1-file
 *
 *	written 03/93 by Andreas Kraft
 *
 *	Changes:
 *		10/94 (akr)  added support for various string types.
 * 
 *	Remember: TS=4
 */

# if defined(__TURBOC__) | defined(__WATCOMC__)
# include	<io.h>
# elif		solaris
# include	<sys/types.h>
# include	<unistd.h>
# elif		__svr4__
# include	<sys/types.h>
# include	<unistd.h>
# else
# include	<sys/file.h>
# endif		/* __TURBOC__ */

# include	<stdlib.h>
# include	<stdio.h>
# include	<ctype.h>
# include	<string.h>
# include	"getargs.h"
# include	"fileleng.h"
# include	"akasn1.h"



static long	 AnalyseTag (void);
static char	*Class2String (int);
static int	 Hexdump (char *);
static char	*Pc2String (int);
static void	 PrintIndent (void);
static long	 ShowValue (byte, byte, byte, long);
static char	*Tag2String (long, int);

int		 do_context   = 0;		/* Try to analyse context-tags			*/
int		 do_hexdump   = 0;		/* hexdump file only					*/
int		 do_octhex    = 0;		/* hexdump octet strings				*/
int		 offset       = 0;		/* offset in File						*/
int		 do_prtoffset = 0;		/* Print the current offset in the file */
int		 indent       = 0;		/* Number of indent-tabs				*/
long	 flength      = 0;

int
main(int argc, char *argv[]) {

	do_context   = is_arg ("-context", argc, argv);
	do_hexdump   = is_arg ("-dump", argc, argv);
	do_octhex    = is_arg ("-octhex", argc, argv);
	do_prtoffset = is_arg ("-prtoffset", argc, argv);
	offset       = intval ("-offset", argc, argv);

	if (getremain (argc) != 1) {
		fprintf (stderr, "\nasn1dump -- ");
		fprintf (stderr, "written by Andreas Kraft\n");
		fprintf (stderr,"\n");
		fprintf (stderr, "usage: asn1dump [Options] <filename>\n\n");
		fprintf (stderr, "       Options:\n\n");
		fprintf (stderr, "       -context      : try to show content of context tags\n");
		fprintf (stderr, "       -octhex       : hexdump octet strings\n");
		fprintf (stderr, "       -dump         : hexdump only\n");
		fprintf (stderr, "       -prtoffset    : Print the current offset\n");
		fprintf (stderr, "       -offset <pos> : start at byte offset 'pos'\n");
		fprintf (stderr, "\n");
		return 1;
	}

	if (do_hexdump) {		/* do hexdump only ! */
		Hexdump (argv[getindex()+1]);
		return 0;
	} /* if */


	/* Open ASN.1-file */
	if (asnOpenReadFile (argv[getindex()+1]) == -1) {
		fprintf (stderr, "asn1dump: can't open file '%s'\n",argv[getindex()+1]);
		return 1;
	}
	
	flength = filelength(asnfd);
	asnFseek ((long)offset);

	while (AnalyseTag () != -1)   /* empty */ ;

	/* Close ASN.1-file */
	asnCloseFile ();

	return 0;
}


/****************************************************************************/

static long AnalyseTag (void) {
	byte	 tbuf[10];
	byte	 cl,
			 pc,
			 tag;
	long	 l,
			 length,
			 glen,
			 lpos,
			 showlen;
	int		 taglen,
			 lengthlen;
	int		 i;
	byte	 buffer;

	if ((taglen= asnReadTag (tbuf)) == -1) 
		return -1;

	asnDecodeTag (&cl, &pc, &tag, tbuf);
	lpos = asnFtell();
	lengthlen= asnReadLength (&length);
/* fprintf (stderr, "length=%ld----Restlength=%ld\n", length, flength-asnFtell()); */

	if (length<-1 || length>(flength-asnFtell())) {
		printf ("at position %ld: unexpected length (%ld) encountered\n", lpos, length);
		exit (1);
	} /* if */

	if (tag == asnNOTHING && cl == asnUNIVERSAL && length==0)
		return asnENDCONTENTS;
		

	PrintIndent ();
	printf ("%s (taglength= %d length= %ld) (%s/%s)", 
				Tag2String (tag, cl), 
				taglen,
				length,
				Class2String (cl),
				Pc2String (pc)
			);

	glen= (long)taglen + (long)lengthlen + length;

	if ((showlen= ShowValue (tag, cl, pc, length)) == -1)
		return -1;

	length-= showlen;

	if ( cl!=asnCONTEXT || 
	    (cl==asnCONTEXT && pc==asnCONSTRUCTED) ||
		(cl==asnAPPLICATION && pc==asnCONSTRUCTED) ||
		(cl==asnCONTEXT && pc==asnPRIMITIVE && do_context)) {

		if (pc==asnCONSTRUCTED || tag==asnEXTERNAL)	{
			indent++;
			while (length>0 || length==-1) {
				if ((l= AnalyseTag ()) == -1)	/* Recursion !! */
					return -1;
				if (l == asnENDCONTENTS) {
					indent--;
					return asnENDCONTENTS;
				}
				length-= (length != -1) ? l : 0;
			} /* while */
			indent--;
		} /* if */
	} /* if */
	if (cl==asnCONTEXT && !do_context && pc!=asnCONSTRUCTED) {
		indent++;
		PrintIndent ();
		printf ("(skipping %ld Bytes: ", length);
		//asnSkipFile (length);

		for (i = 0; i < length; i++) {
			if (asnRead(&buffer, 1) == -1)
				return -1;
			if (isprint(buffer) && buffer != '\t')
				printf ("%c", buffer);
			else
				printf ("{%02X}", buffer);
		}
		printf(")\n");
		indent--;
		return glen;
	}

	return glen;
}


/*
 * display the coded value
 */

static long ShowValue (byte tag, byte cl, byte pc, long length) {
	long	 l, ll,
			 longvalue;
	byte	 buffer[256],
			 bytevalue;
	int		 fl;

	switch ((int)cl) {

		case asnUNIVERSAL:	
			printf ("\n");
			indent++;
			switch ((int)tag) {
				case asnBOOLEAN:
					if (asnReadBoolean (&bytevalue, (int)length) == -1)
						return -1l;
					PrintIndent ();
					printf ("::= %s\n", (bytevalue==asnTRUE)?"TRUE":"FALSE");
					indent--;
					return length;

				case asnINTEGER:
					if (asnReadInteger (&longvalue, (int)length) == -1)
						return -1l;
					PrintIndent ();
					printf ("::= %ld\n", longvalue);
					indent--;
					return length;

				case asnENUMERATED:
					if (asnReadEnum (&longvalue, (int)length) == -1)
						return -1l;
					PrintIndent ();
					printf ("::= %ld\n", longvalue);
					indent--;
					return length;

				case asnOCTETSTRING:
				case asnNUMERICSTRING:
				case asnPRINTABLESTRING:
				case asnTELETEXSTRING:
				case asnVIDEOTEXSTRING:
				case asnIA5STRING:
				case asnGRAPHICSTRING:
				case asnVISIBLESTRING:
				case asnGENERALSTRING:
					if ((l= asnReadOctet (buffer, (int)length, 
										  sizeof(buffer)-1, pc))==-1)
						return -1l;
					PrintIndent ();
					printf ("::= ");
					fl= 0;
					for (ll=0; ll<l; ll++) {
						if (do_octhex)
							printf ("%02X ", buffer[ll]);
						else 
							if ((isspace(buffer[ll]) && buffer[ll] != ' ')|| !isprint(buffer[ll])) {
								if (fl) {
									printf ("\" ");
									fl= 0;
								} /* if */
								printf ("%02X ", buffer[ll]);
							} else {
								if (!fl) {
									printf ("\"");
									fl= 1;
								} /* if */
								printf ("%c", buffer[ll]);
							} /* else */
					} /* for */
					if (fl)
						printf ("\"");
					printf ("\n");
					indent--;
					return length;

				case asnOBJECTID:
					if (asnReadOid ((char *)buffer, (int)length) == -1)
						return -1l;
					PrintIndent ();
					printf ("::= %s\n",buffer);
					indent--;
					return length;

				case asnEXTERNAL:
					indent--;
					return 0;

				case asnSEQUENCE:
					indent--;
					return 0;

				case asnSET:
					indent--;
					return 0;

				case asnNULLVALUE:
					indent--;
					return 0;
				default:
					indent--;
					break;
			}
			asnSkipFile (length);
			return length;

		case asnAPPLICATION:
		case asnPRIVATE:
		case asnCONTEXT:
			printf ("\n");
			l= 0;
			if (do_context || pc==asnCONSTRUCTED) {
				indent++;
				if ((l= AnalyseTag ()) == -1)	/* Recursion !! */
					return -1l;
				indent--;
			}
			return l;

# ifdef NIE
		case asnAPPLICATION:
		case asnPRIVATE:	
			printf (": unknown. %ld bytes skipped\n", length);
			asnSkipFile (length);
			return length;
# endif

	} /* switch */

	return 0;
}


/*
 * print indent
 */

static void PrintIndent (void) {
	int	i;

	if (do_prtoffset) 
		printf("%08ld - ", asnFtell());
	for (i = 0; i < indent; i++)
		printf("   ");
}
	

/*****************************************************************************/
/*
 * Convert tagnum to string
 */
static char *tagstrings[]= {  0,
		  /*    1                      2                  3           */
			"Boolean",	 			"Integer",			"Bitstring",
		  /*    4          	           5                  6           */
			"Octetstring",			"Null",				"Objectidentifier",
		  /*    7                      8                  9           */
			"Objectdescriptor",		"External",			"Real",
		  /*   10                     11                 12           */
			"Enumerated",			"<reserved>",		"<reserved>",
		  /*   13                     14                 15           */
			"<reserved>",			"<reserved>",		"<reserved>",
		  /*   16                     17                 18           */
			"Sequence",				"Set",				"NumString",
		  /*   19              		  20                 21           */
			"Printable String",		"Teletex",			"Videotex",
		  /*   22            	  	  23                 24           */
			"IA5 String",			"UCTtime",			"Time",
		  /*   25           		  26                 27           */
			"Graphic String",		"Visible String",	"General String",
				0
			};

static char *Tag2String (long tag, int cl) {
	static char	buffer[20];

	switch (cl) {
		case asnPRIVATE:
			sprintf (buffer, "P[%ld]", tag);
			return buffer;
		case asnAPPLICATION:
			sprintf (buffer, "A[%ld]", tag);
			return buffer;
		case asnCONTEXT:
			sprintf (buffer, "C[%ld]", tag);
			return buffer;
		case asnUNIVERSAL:
			return (tag<0 || tag>27) ? " " : tagstrings[(int)tag];
		default:
			return "<=>";
	} /* switch */
	/* never reached */
}


/*
 * Convert class to string
 */

static char *Class2String (int cl) {
	switch (cl) {
		case	asnUNIVERSAL:		return "UNIV";
		case	asnAPPLICATION:		return "APPL";
		case	asnCONTEXT:			return "CONT";
		case	asnPRIVATE:			return "PRIV";
	}
	/* Hopefully never reached */
	return "unknown class";
}


/*
 * Convert pc to string
 */

static char *Pc2String (int pc) {
	switch (pc) {
		case	asnPRIMITIVE:		return "PRIM";
		case	asnCONSTRUCTED:		return "CONST";
	}
	/* Hopefully never reached */
	return "unknown pc";
}

/*****************************************************************************
 *
 * Do a hexdump of a file
 */

static int Hexdump (char *fn) {
	FILE	*fp;
	int		 count, c;
	long	 gescount;
	char	 buffer[20];

	if ((fp= fopen (fn, "rb")) == NULL) {
		fprintf (stderr, "can't open file %s\n", fn);
		return -1;
	}
	count= 0;
	gescount= 0;
	memset (buffer, 0, 20);

	printf ("%08lX  ", gescount);
	while ((c= getc(fp)) != EOF) {
		printf ("%02X ", c);
		buffer[count++]= isgraph(c) ? c : '.';
		gescount++;

		if (count == 8)
			printf (" ");
		if (count == 16) {
			printf ("  %.8s %s\n", buffer, &buffer[8]);
			printf ("%08lX  ", gescount);
			count= 0;
			memset (buffer, 0, 20);
		}
	}
	for ( /**/; count && count < 17; count++) {
		printf ("   ");
		if (count == 7)
			printf (" ");
		if (count == 15) 
			printf ("  %.8s %s\n", buffer, &buffer[8]);
	}
	fclose (fp);
	return 0;
}
