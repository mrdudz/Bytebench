/*******************************************************************************
 *  The BYTE UNIX Benchmarks - Release 3
 *          Module: hanoi.c   SID: 3.3 5/15/91 19:30:20
 *
 *******************************************************************************
 * Bug reports, patches, comments, suggestions should be sent to:
 *
 *	Ben Smith, Rick Grehan or Tom Yager
 *	ben@bytepb.byte.com   rick_g@bytepb.byte.com   tyager@bytepb.byte.com
 *
 *******************************************************************************
 *  Modification Log:
 *  $Header: hanoi.c,v 3.5 87/08/06 08:11:14 kenj Exp $
 *  August 28, 1990 - Modified timing routines (ty)
 *
 ******************************************************************************/

/* char SCCSid[] = "@(#) @(#)hanoi.c:3.3 -- 5/15/91 19:30:20"; */

#define VERBOSE
//#define USECMDLINE

#include <stdio.h>
#include <stdlib.h>
#include "timeit.c"

unsigned short iter = 0; /* number of iterations */
char num[4];
long cnt;

int disk=2,       /* default number of disks */
    duration=10;  /* default time to run test */

void mov(unsigned char n,unsigned char f,unsigned char t);

void report(void)
{
	fprintf(stderr,"%ld loops\n", iter);
	exit(0);
}

int main(int argc,char **argv)
{

	#ifdef USECMDLINE	 
	if (argc < 2) {
		printf("Usage: %s [duration] [disks]\n", argv[0]);
		exit(1);
	}
	else
	{
		if(argc > 1) duration = atoi(argv[1]);
		if(argc > 2) disk = atoi(argv[2]);
	}
	#endif
	
	printf("towers of hanoi\n\rdisks: %d\n\rduration: %d\n\r",disk,duration);
	
	num[1] = disk;

	wake_me(duration, (void*)report);

	while(num[3]<disk) {
		mov(disk,1,3);
		++iter;
	}

	report();
	
}

void mov(unsigned char n,unsigned char f,unsigned char t)
{
char o;
	
	if(n == 1) 
	{
		num[f]--;
		num[t]++;
	}
	else
	{
		o = (6-(f+t));
		mov(n-1,f,o);
		mov(1,f,t);
		mov(n-1,o,t);
	}
	
	#ifdef VERBOSE
   	printf("%2d: %2d %2d %2d %2d\n\r",
		(int)iter,(int)num[0],(int)num[1],(int)num[2],(int)num[3]);
	#endif	
	
}
