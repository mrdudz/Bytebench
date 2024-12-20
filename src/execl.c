/*******************************************************************************
 *  The BYTE UNIX Benchmarks - Release 3
 *          Module: execl.c   SID: 3.3 5/15/91 19:30:19
 *
 *******************************************************************************
 * Bug reports, patches, comments, suggestions should be sent to:
 *
 *	Ben Smith, Rick Grehan or Tom Yager
 *	ben@bytepb.byte.com   rick_g@bytepb.byte.com   tyager@bytepb.byte.com
 *
 *******************************************************************************
 *  Modification Log:
 *  $Header: execl.c,v 3.5 87/06/22 15:37:08 kjmcdonell Beta $
 *  August 28, 1990 - Modified timing routines
 *
 ******************************************************************************/
/*
 *  Execing
 *
 */
char SCCSid[] = "@(#) @(#)execl.c:3.3 -- 5/15/91 19:30:19";

#include <stdio.h>
//#include <sys/types.h>
#include <ctype.h>

char	bss[8*1024];	/* something worthwhile */

#define main dummy

#include "big.c"        /* some real code */

#undef main

/* added by BYTE */
char *getenv();


main(argc, argv)	/* the real program */
int	argc;
char	*argv[];
{
        unsigned long iter = 0;
	char *ptr;
	char *fullpath;
	int 	duration;
	char	count_str[6], start_str[12], path_str[81], *dur_str;
	long	start_time, this_time;

#ifdef DEBUG
	int count;
	for(count = 0; count < argc; ++ count)
		printf("%s ",argv[count]);
		printf("\n");
#endif
	if (argc < 2)
		{
		printf("Usage: %s duration\n", argv[0]);
		exit(1);
		}


	duration = atoi(argv[1]);
	if (duration > 0)
		/* the first invocation */
		{
		dur_str = argv[1];
		if((ptr = getenv("BINDIR")) != NULL)
			sprintf(path_str,"%s/execl",ptr);
		fullpath=path_str;
		time(&start_time);
		}
	else  /* one of those execl'd invocations */
		{
		/* real duration follow the phoney null duration */
		duration = atoi(argv[2]);
		dur_str = argv[2];
		iter = (unsigned long)atoi(argv[3]); /* where are we now ? */
		sscanf(argv[4], "%ld", &start_time);
		fullpath = argv[0];
		}

	sprintf(count_str, "%d", ++iter); /* increment the execl counter */
	sprintf(start_str, "%ld", start_time);
	time(&this_time);
	if (this_time - start_time >= duration) { /* time has run out */
		fprintf(stderr, "%ld loops\n", iter);
		exit(0);
		}
	execl(fullpath, fullpath, "0", dur_str, count_str, start_str, 0);
	printf("Exec failed at iteration %d\n", iter);
	perror("Reason");
	exit(1);
}
