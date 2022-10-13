#!/bin/bash

colFlag="-c"
col="director_name"
dirFlag="-d"
dir="dir0"
outFlag="-o"
out="output"

for (( i=8; i<=512; i=i*2));
do
	for (( j=8; j<=128; j=j*2));
	do
		for (( k=1 ; k<=100 ; k++ ));
		do

			./test $colFlag $col $dirFlag $dir $outFlag $out $i $j
			#./test <<'-c director_name -d dir0 -o output'
			echo $k $i $j
		done
	done
done
