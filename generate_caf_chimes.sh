#!/bin/bash
# generate_caf_chimes.sh (command line utility)
# (a working ebook2cw installation - in the current working directory - is required)
# (afconvert utility is provided with macOS and used here to convert mp3 to caf)
#
# This script generates audio files (caf format) in the 'chimes' folder, required to replace the files at
# blacktree-chime-master/Themes/2 when "sideloading" the app
#
# 1/1/2018 - Michele Giugliano (mgiugliano@gmail.com)

WPM=30	# (Character) speed, work per minute
FPM=20  # Effective speed 

#----- DO NOT MODIFY THE CODE BELOW THIS LINE --------------------------------------------------
hours=(1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24)
frac=(0 1 2 3)

mkdir -p chimes                  # create, if not already existing, a dir for chimes 

for index in ${!hours[@]}; do                            # For each element of the array
 for j in ${!frac[@]}; do                      			 # For each fraction of hour 

    hour=${hours[index]}								 # The actual hour in a variable
	f=${frac[j]}										 # The actual fraction in a variable
    echo Generating an mp3 for "$hour and $f"     		 # Print information on screen

	if [ $f = "0" ]; then                                # Let's handle the desired format 
       echo $hour > tmp.txt
     elif [ $f = "1" ]; then
       echo $hour 15 > tmp.txt 
     elif [ $f = "2" ]; then
       echo $hour 30 > tmp.txt
	 elif [ $f = "3" ]; then
       echo $hour 45 > tmp.txt 
     fi

    head tmp.txt

    ./ebook2cw -w $WPM -e $FPM -f 650 -T 0  -o tmp.mp3 ./tmp.txt 	# Launch eBook2cw <----------------------
    afconvert -v -f 'caff' -d aac -s 1 tmp.mp30000.mp3 speech-$hour-$f.caf 

 done
done
rm tmp.txt
rm tmp.mp30000.mp3
mv *.caf chimes/
