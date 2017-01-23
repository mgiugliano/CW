//
// CWsingle - My own Interactive and Adaptive 
//            Morse trainer for Single Characters
//
// 22/1/2017 - (c) Michele Giugliano (mgiugliano@gmail.com)
// Compile with: gcc -o CWsingle CWsingle.c -O
//

#include "CWsingle.h"   
#define USAGE	"%s a b c d e... (list of letters/number/characters to include)"

int main(int argc, char *argv[]) { // main()
 double p_baseline;             // Baseline occurrence probability
 char cmd[40];                  // Will contain the command to play the mp3 files of each symbol
 char err[40];                  // Will contain the command to play the error mp3 sound
 int n;                         // Index of the current symbol to play and to be copied
 char current;                  // Current symbol to play and to be copied
 int made_an_error;             // Boolean variable to distinguish wrong from right.
 struct sigaction sigIntHandler;// To catch the CTRL-C event.

  srand((unsigned)time(NULL)); // Initialise seed of random n generator, with curren time
  rand();            // Discarded: the first rand() returns always the same value

  Nsyms = argc-1;   // Global var, containing the number of symbols entered by the user
  syms  = argv;     // Global var, containing the symbols entered by the user
  Ntot  = 0;        // Global var, containing the total number of "trials"
  Nerr  = 0;        // Global var, containing the total number of "errors"
  p_baseline = 1. / ((double) Nsyms); // Equal probabilities for each symbol (initially at least)   


 if (argc<2) {                // If called without argument, print the correct usage and 
	printf(USAGE, argv[0]);		  // exit silently. Otherwise the number of symbols
    return 0;					        // entered is (argc - 1) and the symbols are argv[1], argv[2], ...
  }
  system("clear");
  printf("Welcome to CWsingle! (c) 2017 Michele Giugliano\n");
  printf("Press any key to start and CTRL-C to terminate at any moment.\nSymbols for this session: ");
  for (n=0; n<Nsyms; n++) 
   printf("%c ", syms[n+1][0]);
  printf("\n\n");

  P    = (double *) calloc(Nsyms, sizeof(double));	// Memory allocation for P
  for (n=0; n<Nsyms; n++) 
  	P[n] = p_baseline;       // All probabilities are initialised and set equal.

  // The system command to play an error sound, in case of a mistake, is prepared here:
  sprintf(err, "afplay ./wrong.mp3"); // do NOT use & (so that the proper pause is granted)
  
  sigIntHandler.sa_handler = my_handler;  // Catching the interrupts and later free the memory
  sigemptyset(&sigIntHandler.sa_mask);    // Catching the interrupts and later free the memory
  sigIntHandler.sa_flags = 0;             // Catching the interrupts and later free the memory

  getch();
  while (1) { //-=-=-= INFINITE LOOP -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    sigaction(SIGINT, &sigIntHandler, NULL);    // Let's prepare to catch CTRL-C (to free memory)
    made_an_error = 0;					                // Boolean var initialised, to catch user's errors

    Ntot++;                       // The counter of the total number of trials is updated here

	  normaliseP(P, Nsyms);					// Normalise the symbols occurrence probabilities to sum 1
 	  n = rand_event(P, Nsyms); 		// One letter is generated "randomly" with probabilities P[]
    current = argv[n+1][0];       // The symbol is taken from the list of those provided by the user
    
 		// Prepare now for the command to play audio files (with some exceptions in the file names)
    if (current=='?')     sprintf(cmd, "afplay ./single/_.mp3 &"); 
	  else if (current=='/')sprintf(cmd, "afplay ./single/+.mp3 &");
    else                  sprintf(cmd, "afplay ./single/%c.mp3 &", current); 

    //printf("%s\n", cmd);
    system(cmd);  			             // Launch the command by a system shell call
 	  printf("?>    \r");  fflush(stdout);// Prints on screen a prompt

    while (getch()!=current) {  // Blocks execution until user press the right button on keyboard
        printf("X>    \r");  fflush(stdout); // Feedback is given to the user
        made_an_error = 1;      // If we are here, at least one error has been made.
        Nerr++;                 // The counter of the total number of mistakes is updated here
        system(err);            // The error sound is played to the user
  	    system(cmd);						// The symbol is played again to the user
  		  P[n] = P[n] * 2.;				// The corresponding occurrence probability is doubled (i.e. repetita iuvant)
  		  
	  } // end while (getch()!=current)
	  if (made_an_error==0)                         // If no error was made this time
	  P[n] = ((P[n]* 0.9) <= p_baseline) ? p_baseline : P[n] * 0.9; 
    // the prob is slowly decreased towards the baseline (i.e. uniform) value.
	} // end while (1)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
return 0;
} // end main


