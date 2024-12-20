/*******************************************************************************
 *  The BYTE UNIX Benchmarks - Release 3
 *          Module: pipe.c   SID: 3.3 5/15/91 19:30:20
 *
 *******************************************************************************
 * Bug reports, patches, comments, suggestions should be sent to:
 *
 *	Ben Smith, Rick Grehan or Tom Yager
 *	ben@bytepb.byte.com   rick_g@bytepb.byte.com   tyager@bytepb.byte.com
 *
 *******************************************************************************
 *  Modification Log:
 *  $Header: pipe.c,v 3.5 87/06/22 14:32:36 kjmcdonell Beta $
 *  August 29, 1990 - modified timing routines (ty)
 *
 ******************************************************************************/
char SCCSid[] = "@(#) @(#)pipe.c:3.3 -- 5/15/91 19:30:20";
/*
 *  pipe  -- test single process pipe throughput (no context switching)
 *
 */

#include <stdio.h>
#include <errno.h>
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
	char	buf[512];
	int		pvec[2], duration;

	if (argc != 2) {
		printf("Usage: %s duration\n", argv[0]);
		exit(1);
		}

	duration = atoi(argv[1]);

	pipe(pvec);

	wake_me(duration, (void*) report);
	iter = 0;

	while (1) {
		if (write(pvec[1], buf, sizeof(buf)) != sizeof(buf)) {
			if ((errno != EINTR) && (errno != 0))
				printf("write failed, error %d\n", errno);
			}
		if (read(pvec[0], buf, sizeof(buf)) != sizeof(buf)) {
			if ((errno != EINTR) && (errno != 0))
				printf("read failed, error %d\n", errno);
			}
		iter++;
	}
}
