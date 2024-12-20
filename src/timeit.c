/*******************************************************************************
 *
 *  The BYTE UNIX Benchmarks - Release 3
 *          Module: timeit.c   SID: 3.3 5/15/91 19:30:21
 *******************************************************************************
 * Bug reports, patches, comments, suggestions should be sent to:
 *
 *	Ben Smith, Rick Grehan or Tom Yager
 *	ben@bytepb.byte.com   rick_g@bytepb.byte.com   tyager@bytepb.byte.com
 *
 *******************************************************************************
 *  Modification Log:
 *  May 12, 1989 - modified empty loops to avoid nullifying by optimizing
 *                 compilers
 *  August 28, 1990 - changed timing relationship--now returns total number
 *	                  of iterations (ty)
 *
 ******************************************************************************/

/* this module is #included in other modules--no separate SCCS ID */

/*
 *  Timing routine
 *
 */

#include <signal.h>

#define SIGALRM    1

void alarmirq(void)
{
    raise(SIGALRM);
}

void alarm(int seconds)
{
}

void wake_me(int seconds, __sigfunc func)
{
	/* set up the signal handler */
	signal(SIGALRM, func);
	/* get the clock running */
	alarm(seconds);
}

