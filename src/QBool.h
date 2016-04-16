/*
 *	QBool.h
 */

# ifndef	_qbOOL_H_
# define	_qbOOL_H_


/* Special treatment for GCC */
# ifdef __GNUC__
	# define	HAS_BOOL
# endif

# if __BORLANDC__ >= 0x500
	# define	HAS_BOOL
# endif

# if	__WATCOMC__ >= 1100
	# define	HAS_BOOL
# endif

# if	_MSC_VER
	# define	HAS_BOOL
# endif


# ifndef HAS_BOOL
	# ifndef bool
		typedef	int	bool;
		# define	true	1
		# define	false	0
	# endif  /* bool */
# endif  /* HAS_BOOL */

# endif	/* _qbOOL_H_ */
