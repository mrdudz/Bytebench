/*******************************************************************************
 *  The BYTE UNIX Benchmarks - Release 3
 *          Module: fstime.c   SID: 3.5 5/15/91 19:30:19
 *
 *******************************************************************************
 * Bug reports, patches, comments, suggestions should be sent to:
 *
 *	Ben Smith, Rick Grehan or Tom Yager
 *	ben@bytepb.byte.com   rick_g@bytepb.byte.com   tyager@bytepb.byte.com
 *
 *******************************************************************************
 *  Modification Log:
 * $Header: fstime.c,v 3.4 87/06/22 14:23:05 kjmcdonell Beta $
 * 10/19/89 - rewrote timing calcs and added clock check (Ben Smith)
 * 10/26/90 - simplify timing, change defaults (Tom Yager)
 * 11/16/90 - added better error handling and changed output format (Ben Smith)
 * 11/17/90 - changed the whole thing around (Ben Smith)
 * 2/22/91 - change a few style elements and improved error handling (Ben Smith)
 * 4/17/91 - incorporated suggestions from Seckin Unlu (seckin@sumac.intel.com)
 * 4/17/91 - limited size of file, will rewind when reaches end of file
 ******************************************************************************/
char SCCSid[] = "@(#) @(#)fstime.c:3.5 -- 5/15/91 19:30:19";

#include <stdio.h>
#include <signal.h>
#include <errno.h>

#define SECONDS 2
#define BUFF_SIZE 1024
#define MAX_BLOCKS 2000   /* max number of BUFF_SIZE blocks in file */
#define FNAME0	"dummy0"
#define FNAME1	"dummy1"

/****************** GLOBALS ***************************/
char buf[BUFF_SIZE];
int			seconds = SECONDS;
int			f;
int			g;
int			i;
int			stop_count();
int			clean_up();
int			sigalarm = 0;

/******************** MAIN ****************************/

main(argc, argv)
char **argv;
{

/**** initialize ****/
    if (argc > 1)
	seconds = atoi(argv[1]);
    if (argc == 3 && chdir(argv[2]) == -1)
	    {
	    perror("fstime: chdir");
	    exit(1);
	    }
    if((f = creat(FNAME0, 0600)) != -1)
            close(f);
    else
	    {
	    perror("fstime: creat");
	    exit(1);
	    }
    if((g = creat(FNAME1, 0600)) != -1)
            close(g);
    else
	    {
	    perror("fstime: creat");
	    exit(1);
	    }
    if( (f = open(FNAME0, 2)) == -1)
	    {
	    perror("fstime: open");
	    exit(1);
	    }
    if( ( g = open(FNAME1, 2)) == -1 )
	    {
	    perror("fstime: open");
	    exit(1);
	    }
    /* fill buffer */
    for (i = 0; i < BUFF_SIZE; i++)
	buf[i] = i & 0177;
    /*** run the tests ****/
    signal(SIGKILL,clean_up);
    if(w_test() || r_test() || c_test())
	{
	clean_up();
	exit(1);
	}
    /* else */
    clean_up();
    exit(0);
}

w_test()
/* write test */
{
    long n_blocks = 0L;
    int f_blocks;
#ifndef __CC65__
    extern int sigalarm;
#endif

    sync();
    sleep(5); /* to insure the sync */

    signal(SIGALRM,stop_count);
    sigalarm = 0; /* reset alarm flag */
    alarm(seconds);
    while(!sigalarm)
	{
	for(f_blocks=0; f_blocks < MAX_BLOCKS; ++f_blocks)
		{
		if (write(f, buf, BUFF_SIZE) < 0)
		    {
		    if (errno != EINTR) {
			perror("fstime: write");
			return(-1);
			} else stop_count();
		    }
		++ n_blocks;
		}
        lseek(f, 0L, 0); /* rewind */
	}
    /* stop clock */
    fprintf(stderr, "%d second sample\n", seconds);
    fprintf(stderr, "%ld Kbytes/sec write\n",
         (long) n_blocks / (long) seconds);
return(0);
}

#ifdef __CC65__
//extern int sigalarm;
//extern int errno;
#endif

r_test()
/* read test */
{
    long n_blocks = 0L;
#ifndef __CC65__
    extern int sigalarm;
    extern int errno;
#endif

    /* rewind */
    sync();
    sleep(10+seconds/2);
    errno = 0;
    lseek(f, 0L, 0);

    signal(SIGALRM,stop_count);
    sigalarm = 0; /* reset alarm flag */
    alarm(seconds);
    while(!sigalarm)
	{
	if (read(f, buf, BUFF_SIZE) < 0) /* read while checking for an error */
	    switch(errno)
	    {
	    case 0:
            case EINVAL:
                lseek(f, 0L, 0);  /* rewind at end of file */
		continue;
		break;
            case EINTR:
		stop_count();
		break;
            default:
                perror("fstime: read");
	        return(-1);
		break;
            }
	++ n_blocks;
        }
    /* stop clock */
    fprintf(stderr, "%d second sample\n", seconds);
    fprintf(stderr, "%ld Kbytes/sec read\n",
         (long) n_blocks / (long) seconds);
return(0);
}


c_test()
/* copy test */
{
    long n_blocks = 0L;
#ifndef __CC65__
    extern int sigalarm;
#endif

    /* rewind */
    sync();
    sleep(10+seconds/2); /* to insure the sync */
    errno - 0;
    lseek(f, 0L, 0);

    signal(SIGALRM,stop_count);
    sigalarm = 0; /* reset alarm flag */
    alarm(seconds);
    while(!sigalarm)
	{
	if (read(f, buf, BUFF_SIZE) < 0)
	    switch(errno)
	    {
	    case 0:
            case EINVAL:
                lseek(f, 0L, 0);  /* rewind at end of file */
                lseek(g, 0L, 0);  /* rewind the output as well */
		continue;
		break;
            case EINTR:
		stop_count();
		break;
            default:
                perror("fstime: copy read");
	        return(-1);
		break;
            }
	if (write(g, buf, BUFF_SIZE) < 0)
	    {
            if (errno != EINTR) {
	        perror("fstime: copy write");
	        return(-1);
                } else stop_count();
	    }
	++ n_blocks;
        }
    /* stop clock */
    fprintf(stderr, "%d second sample\n", seconds);
    fprintf(stderr, "%ld Kbytes/sec copy \n",
         (long) n_blocks / (long) seconds);
return(0);
}

stop_count()
{
#ifndef __CC65__
extern int sigalarm;
#endif
sigalarm = 1;
return(0);
}

clean_up()
{
if( unlink(FNAME0) || unlink(FNAME1))
	return(-1);
else
	return(0);
}
