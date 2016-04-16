/*:>* fileleng.c ************************************************************

Name
	filelength

Info
	Get the length of an open file

Syntax
	long filelength (int fd);

Include
	QCore.h

Description
	`filelength()` can be used to get the length of an open file `fd`.

Return value
	The function returns the length of `fd`.

Example
	% long length;
	%
	% length= filelength (myFd);

See also
	filelengthFP
	getFileSize
	tell

**************************************************************************<:*/

# include	<assert.h>
# include	<stdio.h>

# ifndef _MSC_VER
# include	<unistd.h>
# else
# include	<io.h>
# endif

# include	"./fileleng.h"

long filelength (int fd) {
	long	oldPosition,
			newPosition;

	assert (fd != -1);

	oldPosition= lseek(fd, 0, SEEK_CUR);
	newPosition= lseek (fd, 0, SEEK_END);
	lseek(fd, oldPosition, SEEK_SET);

	return newPosition;
}
