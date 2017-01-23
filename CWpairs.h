//
// CWPairs - My own Interactive and Adaptive 
//            Morse trainer for Single Characters
//
// 22/1/2017 - (c) Michele Giugliano (mgiugliano@gmail.com)
//
// CWpairs.h --> support file (gathering all include and functions)
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
double **P;             // Global var, containing the presentation occurrence probabilities
char A[1];              // Global var, containing the first of the two chars entered by the user
char B[1];              // Global var, containing the first of the two chars entered by the user

//---------------------------------------------------------------------------
// rand_event2():
// Given an array of N positive numbers (representing probabilites, i.e. numbers in [0 ; 1], 
// whose total sum is 1) P[i] i=1...N, this routine returns an integer i corresponding to 
// the occurrence of a pseudo-random event with prob P[i].
//
// Imagine all P[i] as segments on a line of lenght 1, depicted as below
// |----P1----+--P2--+----P3----+--------P4--------|
// 
// If you now consider a pseudo random number r, uniformly distributed in the range [0 ; 1],
// probability that a realisation will be "falling" in the interval with length P[i] is exactly P[i].
// Therefore the following algorithm simulates the occurrence of an event of out N possible, with 
// probabilites described explicitly.
// 
// Note: I know P is a 2-dimensional array in this case, but it is regarded as if it is a series of 
// probabilities, each corresponding to an event (identified by two index)

//---------------------------------------------------------------------------
int rand_event2(double **P, int N) {
    const double eps = 1e-9;               // This is my definition of "zero"
    int k;                                 // Running index on all the segments
    double r=((double) rand())/(RAND_MAX); // The random number is drawn here: its instance is "r"
    for (k=1; k<=N*N; k++) {               // (0 < r < P1) happens with probability P1 
        r -= P[(k-1)/N][(k-1)%N];          // (P1 < r < (P1+P2)) happens with probability P2 
        if (r < eps) return k-1;           // etc. etc.
    }
    return -1; // -1 is an end error code
} // end rand_event()


// normaliseP2():
// Given a 2-dim array of N*N positive numbers (representing probabilites, i.e. numbers in [0 ; 1], 
// whose total sum is 1) P[i][j] i,j=1...N, this routine re-normalise the individual values so that
// their sum is 1 (so they are real probabilities and can be fed into rand_event().
//
void normaliseP2(double **P, int N) {
 int i, j;
 double sumP = 0.;                      // Temporary accumulator
 for (i=0; i<N; i++) for (j=0; j<N; j++) sumP += P[i][j];      // for summing each element 
 for (i=0; i<N; i++) for (j=0; j<N; j++) P[i][j] /= sumP;      // And then each element is divided by such a sum,
 return;                                // guaranteeing that the new values will sum to 1.
} // end normaliseP()


// getch2():
// This is "blocking" the execution of the program and it returns the **two** key pressed by the user in sequence.
// However, it does not require the user to press enter. I took it from the web while looking for the OSX 
// version of keyboard hit - kbhit - available under windows. 
//
void getch2(void) {
 struct termios oldstate, newstate;     // The current state of the user terminal must be altered to
                                        // allow for the desired trick.
 tcgetattr(0, &oldstate);               // Current attributes are acquired to be restored later
 newstate = oldstate;                   // The current state is modified
 newstate.c_lflag &= ~ICANON;           // according to this and 
 newstate.c_lflag &= ~ECHO;             // this flags.
 tcsetattr(0, TCSANOW, &newstate);      // The new state is set (temporarily)
 read(0, &A, 1);                        // The (first) character is finally acquired, by a low-level I/O read
 read(0, &B, 1);                        // The (second) character is finally acquired, by a low-level I/O read
 tcsetattr(0, TCSANOW, &oldstate);      // The previous state is restored

 return;                                // The selected chars are returned via A and B
} // end getch2()


// print_stats2():
// Prints the statistics and performance
void print_stats2(void) {
    int i,j;
    normaliseP2(P, Nsyms);
    printf("%%%f correct (on %d trials)\n\n", 100.*(double)(Ntot-Nerr)/Ntot, Ntot); // Statistics is displayed
    for (i=0; i<Nsyms; i++)
     for (j=0; j<Nsyms; j++) 
      printf("%c%c[%f]\n", syms[i+1][0], syms[j+1][0], P[i][j]);               // Statistics is displayed
    printf("Higher values mean they have been more difficult for you in this session.\n\n");
    return;
} // end print_stats2()


// my_handler():
// This is handling the user interrupt, particularly for terminating the execution of the program safely
//
void my_handler(int s){
    //printf("Caught signal %d\n",s);
    if (s==2) {                            // "2" corresponds to CTRL-C
        system("reset");                   // A system call to reset the terminal is issued (maybe not required)
        print_stats2();                     // Print statistics
        free(P);                           // Memory allocated for P is released here
    }
    exit(1); 
} // end my_handler()

// compare():
// This is handling the comparison between the current pair of character and the user entries
// 
int compare(char a, char b) {
 getch2();
//printf("a: %c, b: %c, A: %c, B: %c\n",a,b,A[0],B[0]);
 return ((a==A[0]) && (b==B[0]));
} // end compare()

// getch():
// This is "blocking" the execution of the program and it returns the key pressed by the user.
// However, it does not require the user to press enter after pressing the button. I took it 
// from the web while looking for the OSX version of keyboard hit - kbhit - available under windows. 
//
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
 tcsetattr(0, TCSANOW, &oldstate);      // The previous state is restored

 return chbuf[0];                       // The selected char is returned as output of the function
} // end getch()

