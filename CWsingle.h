//
// CWsingle -   My own Interactive and Adaptive
//            Morse trainer for Single Characters
//
// 22/1/2017 - (c) Michele Giugliano (mgiugliano@gmail.com)
//
// CWsingle.h --> support file (gathering all include and functions)
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>       // Required for initialising randomly the seeds of random()
#include <math.h>
#include <signal.h>     // Required to capture the CTRL-C to stop (safely) the program

#include <termios.h>    // Required by the function getch()
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/uio.h>

int Ntot;               // Global var, containing the total number of "trials"
int Nerr;               // Global var, containing the total number of "errors"
int Nsyms;              // Global var, containing the number of symbols entered by the user
char **syms;            // Global var, containing the symbols entered by the user
double *P;              // Global var, containing the presentation occurrence probabilities

//---------------------------------------------------------------------------
// rand_event():
// Receives an array P of numbers in [0 ; 1] (whose sum is 1), and returns i.
// 'i' corresponds to the occurrence of the pseudo-random event with prob P[i]
//
// Algorithm:
// Imagine all P[i] as segments on a line of total lenght 1, as depicted
// |----P1----+--P2--+----P3----+--------P4--------|
//
// Drawing a pseudo random number r, unif. distributed in [0 ; 1], the probability
// that r will be "falling" in the interval with length P[i] is exactly P[i].
// Thus, the occurrence of an event of out N possible, with known probabilites,
// can be simulated by drawing a random number from a uniform distribution.
//
int rand_event(double *P, int N) {      // pointer to P, length of P[]
	const double eps = 1e-9;               // This is my definition of "zero"
	int k;                                 // Running index on all the segments
    double r=((double) rand())/(RAND_MAX); // The random number is drawn here: its instance is "r"
    for (k=1; k<=N; k++) {                 // (0 < r < P1) happens with probability P1
    	r -= P[k-1];                       // (P1 < r < (P1+P2)) happens with probability P2
    	if (r < eps) return k-1;           // etc. etc.
    }
    return -1; // -1 is an end error code
} // end rand_event()

// normaliseP():
// Receives an array P of no in [0 ; 1] and renormalises it so that they sum to 1,
// so they are real probabilities and can be fed into rand_event().
//
void normaliseP(double *P, int N) {
 int i;
 double sumP = 0.;                      // Temporary accumulator
 for (i=0; i<N; i++) sumP += P[i];      // for summing each element
 for (i=0; i<N; i++) P[i] /= sumP;      // And then each element is divided by such a sum,
 return;                                // guaranteeing that the new values will sum to 1.
} // end normaliseP()


// getch():
// This "blocks" the execution of the program and returns the key pressed, but
// does not require the user to press enter after pressing the key. I took it
// from the web while looking for the macOS version of keyboard hit - kbhit,
// available under Windows.
char getch(void) {
 char chbuf[1];                         // Buffer for storing the (single) char selected by the user
 struct termios oldstate, newstate;     // The current state of the user terminal must be altered to
                                        // allow for the desired trick.
 tcgetattr(0, &oldstate);               // Current attributes are acquired to be restored later
 newstate = oldstate;                   // The current state is modified
 newstate.c_lflag &= ~ICANON;           // according to this and
 newstate.c_lflag &= ~ECHO;             // this flags.
 tcsetattr(0, TCSANOW, &newstate);      // The new state is set (temporarily)
 read(0, &chbuf, 1);                    // The character is finally acquired, by a low-level I/O read
 tcsetattr(0, TCSANOW, &oldstate);      // The previous state is restored.
 return chbuf[0];                       // The selected char is returned.
} // end getch()


// print_stats():
// Prints the statistics and performance
void print_stats(void) {
    int i;
    double p_baseline;
    p_baseline = 1. / ((double) Nsyms);
    normaliseP(P, Nsyms);
    printf("%%%f correct (on %d trials)\n\n", 100.*(double)(Ntot-Nerr)/Ntot, Ntot); // Statistics is displayed

    printf("Difficult characters for you, in this session:\n");
    for (i=0; i<Nsyms; i++)
      if (P[i]>p_baseline) printf("%c ", syms[i+1][0]);               // Statistics is displayed
    printf("\n\n\n");
//    for (i=0; i<Nsyms; i++)
//      printf("%c[%f]\n", syms[i+1][0], P[i]);               // Statistics is displayed
//    printf("Higher values mean they have been more difficult for you in this session.\n\n");
    return;
} // end print_stats()


void print_probs(void) {
    int i;
    for (i=0; i<Nsyms; i++)
      printf("%f  ", P[i]);  // Print each element
    printf("\n");
//    for (i=0; i<Nsyms; i++)
//      printf("%c[%f]\n", syms[i+1][0], P[i]);               // Statistics is displayed
//    printf("Higher values mean they have been more difficult for you in this session.\n\n");
    return;
} // end print_stats()


// my_handler():
// This is handling the user interrupt, particularly for terminating the execution of the program safely
//
void my_handler(int s){
    //printf("Caught signal %d\n",s);
	if (s==2) {                            // "2" corresponds to CTRL-C
        system("reset");                   // A system call to reset the terminal is issued (maybe not required)
        print_stats();                     // Print statistics
		free(P);                           // Memory allocated for P is released here
	}
    exit(1);
} // end my_handler()
