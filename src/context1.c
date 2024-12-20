
/*******************************************************************************
 *  The BYTE UNIX Benchmarks - Release 3
 *          Module: context1.c   SID: 3.3 5/15/91 19:30:18
 *
 *******************************************************************************
 * Bug reports, patches, comments, suggestions should be sent to:
 *
 *	Ben Smith, Rick Grehan or Tom Yager
 *	ben@bytepb.byte.com   rick_g@bytepb.byte.com   tyager@bytepb.byte.com
 *
 *******************************************************************************
 *  Modification Log:
 *  $Header: context1.c,v 3.4 87/06/22 14:22:59 kjmcdonell Beta $
 *  August 28, 1990 - changed timing routines--now returns total number of
 *                    iterations in specified time period
 *
 ******************************************************************************/
char SCCSid[] = "@(#) @(#)context1.c:3.3 -- 5/15/91 19:30:18";
/*
 *  Context switching via synchronized unbuffered pipe i/o
 *
 */

#include <stdio.h>
#include "timeit.c"
#include <errno.h>

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
	int duration;
	int	check;
	int	p1[2], p2[2];

	if (argc != 2) {
		printf("Usage: context duration\n");
		exit(1);
	}

	duration = atoi(argv[1]);

	/* set up alarm call */
	iter = 0;
	wake_me(duration, (void*)report);

	if (pipe(p1) || pipe(p2)) {
		perror("pipe create failed");
		exit(1);
	}

	if (fork()) {	/* parent process */
		/* master, write p1 & read p2 */
		close(p1[0]); close(p2[1]);
		while (1) {
			if (write(p1[1], (char *)&iter, sizeof(iter)) != sizeof(iter)) {
				if ((errno != 0) && (errno != EINTR))
					perror("master write failed");
				exit(1);
			}
			if (read(p2[0], (char *)&check, sizeof(check)) != sizeof(check)) {
				if ((errno != 0) && (errno != EINTR))
					perror("master read failed");
				exit(1);
			}
			if (check != iter) {
				printf("Master sync error: expect %d, got %d\n",
					iter, check);
				exit(2);
			}
			iter++;
		}
	}
	else { /* child process */
		unsigned long iter1;

		iter1 = 0;
		/* slave, read p1 & write p2 */
		close(p1[1]); close(p2[0]);
		while (1) {
			if (read(p1[0], (char *)&check, sizeof(check)) != sizeof(check)) {
				if ((errno != 0) && (errno != EINTR))
					perror("slave read failed");
				exit(1);
			}
			if (check != iter1) {
				printf("Slave sync error: expect %d, got %d\n",
					iter, check);
				exit(2);
			}
			if (write(p2[1], (char *)&iter1, sizeof(iter1)) != sizeof(check)) {
				if ((errno != 0) && (errno != EINTR))
					perror("slave write failed");
				exit(1);
			}
			iter1++;
		}
	}
}
