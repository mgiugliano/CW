#!/bin/bash
# generate_mp3_files.sh (command line utility)
# (a working ebook2cw installation - in the current working directory - is required)
#
# 22/1/2016 - Michele Giugliano (mgiugliano@gmail.com)

WPM=20	# (Character) speed, work per minute
FPM=20  # Effective speed 

#----- DO NOT MODIFY THE CODE BELOW THIS LINE --------------------------------------------------
# All letters, numbers, and (some) symbols are placed into an array
all=( a b c d e f g h i j k l m n o p q r s t u v w x y z 0 1 2 3 4 5 6 7 8 9 ? / )

mkdir -p single			# create, if not already existing, a dir for single chars
mkdir -p pairs			# create, if not already existing, a dir for pairs

for index in ${!all[@]}; do				# For each element of the array
    x=${all[index]}                     # Current letter in a temporary variable
    echo Generating an mp3 for "$x"     # Print information on screen
    echo $x > tmp.txt			        # Create an ASCII file

    ./ebook2cw -w $WPM -e $FPM -f 650 -T 0  -o tmp.mp3 ./tmp.txt 	# Launch eBook2cw <----------------------

     if [ $x = "?" ]; then			        # Let's deal with special cases
        mv tmp.mp30000.mp3 single/_.mp3
     elif [ $x = "/" ]; then
        mv tmp.mp30000.mp3 single/+.mp3
     else 
        mv tmp.mp30000.mp3 single/$x.mp3 	# Otherwise let's rename the mp3 file
     fi
done

for index1 in ${!all[@]}; do             # For each element of the array
 for index2 in ${!all[@]}; do             # For each element of the array
    x1=${all[index1]}                     # Current letter in a temporary variable
    x2=${all[index2]}                     # Current letter in a temporary variable
    echo Generating an mp3 for "$x1$x2"  # Print information on screen
    echo $x1$x2 > tmp.txt                   # Create an ASCII file

    ./ebook2cw -w $WPM -e $FPM -f 650 -T 0  -o tmp.mp3 ./tmp.txt    # Launch eBook2cw <----------------------

     if [ $x1 = "?" ]; then                  # Let's deal with special cases
        x1=_
     elif [ $x1 = "/" ]; then
        x1=+
     fi
     if [ $x2 = "?" ]; then                  # Let's deal with special cases
        x2=_
     elif [ $x2 = "/" ]; then
        x2=+
     fi

    mv tmp.mp30000.mp3 pairs/$x1$x2.mp3    # Otherwise let's rename the mp3 file
 done
done

rm tmp.txt
