/*******************************************************************************
 *  The BYTE UNIX Benchmarks - Release 3
 *          Module: syscall.c   SID: 3.3 5/15/91 19:30:21
 *
 *******************************************************************************
 * Bug reports, patches, comments, suggestions should be sent to:
 *
 *	Ben Smith, Rick Grehan or Tom Yager at BYTE Magazine
 *	ben@bytepb.byte.com   rick_g@bytepb.byte.com   tyager@bytepb.byte.com
 *
 *******************************************************************************
 *  Modification Log:
 *  $Header: syscall.c,v 3.4 87/06/22 14:32:54 kjmcdonell Beta $
 *  August 29, 1990 - Modified timing routines
 *
 ******************************************************************************/
/*
 *  syscall  -- sit in a loop calling the system
 *
 */
char SCCSid[] = "@(#) @(#)syscall.c:3.3 -- 5/15/91 19:30:21";

#include <stdio.h>
#include "timeit.c"

unsigned long iter;

report()
{
	fprintf(stderr,"%ld loops\n", iter);
	exit(0);
}

main(argc, argv)
int	argc;
char	*argv[];
{
	int	duration;
	int	i;

	if (argc != 2) {
		printf("Usage: %s duration\n", argv[0]);
		exit(1);
	}

	duration = atoi(argv[1]);

	iter = 0;
	wake_me(duration, (void*) report);

	while (1) {
		close(dup(0));
		getpid();
		getuid();
		umask(022);
		iter++;
	}
}
