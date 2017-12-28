# CW
My own command line utilities to learn and practice CW (Morse Code)

22/1/2017 - Michele Giugliano (mgiugliano@gmail.com)
28/12/2017 - Bug fix and improvements

CWSingle and CWPairs

I am learning CW (Morse Code) by CW Academy's excellent beginner classes (http://www.cwops.org/cwacademy.html). My instructor closely follows the series of training sessions as presented in the website http://morsecode.scphillips.com/trainer.html, which is also recommended for individual study. 
Unfortunately, although of extremely high quality, such a website offers much less interactivity than http://lcwo.net, which I have been enjoying in my (earlier) attempt at getting more comfortable with CW. There, I particularly enjoyed the "Morse Machine" operation (see http://lcwo.net/forum/423) and its idea of automatically presenting random letters/symbols, and wait for the user response as (the right) key pressed by the user. Unfortunately, in the website recommended by CW Academy, I must click with the mouse button and/or interact with the navigation widgets on a webpage, each time I want to listen to a (new) letter/symbol. I am getting old and perhaps I am old fashioned, but I do enjoy the command line.

The order of presentation of (new) letters, during my current CW Academy training sessions, differs from the lcwo.net (Koch's method) so I cannot use it yet. Until I become more proficient and explicitly knowlegable in all CW symbols (so that I can use lwco.net), I am going to use my own software for single and pairs of characters.

These are admittedly quick-and-dirty (command line) utilities, conceived exclusively for myself and running currently only under OSX - although I am pretty sure they can be easily adapted easily for Windows and Linux. These software make explcit use of two existing utilities:
	- ebook2cw (https://fkurz.net/ham/ebook2cw.html)
	- afplay (https://developer.apple.com/legacy/library/documentation/Darwin/Reference/ManPages/man1/afplay.1.html)

The first is a **fantastic** (command line) utility that generates a mp3 audio file, containing the CW version of the text specified as the input. I decided to use it to generate a simple library of high quality audio files, one for each CW symbol, so that my own software can simply play and present them to me in random order and with the desired level of interactivity.
Assuming that you already have a working installation of ebook2cw, from a terminal session you should invoke the bash script I provide. It is called generate_mp3_files.sh and requires the ebook2cw executable to be in the same current directory. Over a couple of minutes, such a bash script will create and populate the content of two subdirs, "single/" and "pairs/", containing a series of mp3 files, each for single or pairs of letters e.g.:

	- single/f.mp3 			(out of 38 total elements, total 100 KByte)
	- pairs/vh.mp3 			(out of 1445 total elements, i.e. 38 * 38, total ~6 MByte)

The second utility is a native command with OSX, useful for reproducing audio files by the command line. Under Linux/Windows maybe one could look at https://www.mpg123.de/download.shtml. The ANSI C code must be modified accordingly replacing "afplay" with "mpg123" and perhaps alter other input parameters in the C code.
If venturing in this direction and not particularly familiar with C programming, simply google the "system()" command (in ANSI C), which is used to have a C program calling an existing software.

In order to compile the two utilities, simply invoke from the command line:

> gcc -o CWSingle CWSingle.c -O

> gcc -o CWPairs CWPairs.c -O

Note that the most convenient way to put GCC on your Mac is to get the "Command Line Tools" dmg from https://developer.apple.com/downloads/ (~120 MB). You must create a (free) developer account in order to access that website. Or you could download the entire Xcode8 installer (~4 GB). 


