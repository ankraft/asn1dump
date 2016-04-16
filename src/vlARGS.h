/*
 *	vlARGS.h
 */

#ifndef	_VLargs_H_
#define	_VLargs_H_

#ifndef vlARGS

#if defined(ANSI) || defined(__ANSI__) || defined(__STDC__)  || defined(hpux)
#define USE_VLARGS
#endif

#if defined(aix) || defined(__BORLANDC__) || defined(__WATCOMC__) || defined(_MSC_VER)
#define USE_VLARGS
#endif



#if defined(USE_VLARGS)
#define	vlARGS(x)	x
#define	vlNOARGS	(void)
#define	vlHASPROT	1
#else
#define	vlARGS(x)	()
#define	vlNOARGS	()
#define	vlHASNOPROT	1
#endif
#endif /* vlARGS */

#ifndef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif
#endif

#endif	/* _VLargs_H_ */
