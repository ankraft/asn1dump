/*
 *	getargs.h
 *
 *	Includefile for getargs.c
 *
 *	added by Andreas Kraft
 */

# ifndef	GETARGS_H
# define	GETARGS_H

# include	"vlARGS.h"
# include	"QBool.h"

# define	ARGS_EOA	"--"

EXTERN float	 floatval(const char *, const int, char **);
EXTERN int		 intval(const char *, const int, char **);
EXTERN int		 getindex(void);
EXTERN int		 getremain(const int);
EXTERN int		 is_arg(const char *, const int, char **);
EXTERN void		 newval(void);
EXTERN char		*nextval(const int, char **);
EXTERN char		*stringval(const char *, const int, char **);
EXTERN void		 skiparg(const char *, const int, const int, char **);
EXTERN void		 argsIgnoreCase(const int);

# define ASSIGNSTRVAL(s,opt,argc,argv)	{ char *tmp=stringval(opt,argc,argv); if (tmp) s= tmp; }

/****************************************************************************
 *
 *	C++ Klassendefinition
 */

# ifdef __cplusplus

class Getargs {

private:
	int		  argc;
	char	**argv;
	bool	  selfAlloc;

public:
	Getargs ();
	Getargs(const int ac, char **av);
	Getargs(const char *);
	virtual ~Getargs();

	Getargs	&setArgs(const int, char **);

	int		 isarg(const char *arg);			// Check for a commandline switch

	int		 intval(const char *arg = 0);		// Get integer value, default get the
												// value from the last call
	char	*stringval(const char *arg = 0);	// Get string value, default: get the
												// value from the last call
	float	 floatval(const char *arg);			// Get float value, default: get the
												// value from the last call
	char	*nextval();							// Get the next argument

	int		 getindex() const;					// Get actual index
	int		 getremain() const;					// Get number of arguments remaining
	void	 skiparg(const char *, const int = 0);	// Skip an argument
	void	 reset();							// Reset the internal counter 
	int		 count() const;						// Get number of arguments
	void	 ignorecase(const bool);			// Ignore the case of the parameter
};

# endif /* __cplusplus */

# endif		/* GETARGS_H */
