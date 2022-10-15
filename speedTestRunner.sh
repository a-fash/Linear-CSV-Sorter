#!/bin/bash

colFlag="-c"
col="director_name"
dirFlag="-d"
dir="dir0"
outFlag="-o"
out="output"
lineBuffer=512
dataBuffer=64

for (( i=1 ; i<=1000 ; i++ ));
do

	./test $colFlag $col $dirFlag $dir $outFlag $out $lineBuffer $dataBuffer
	#./test <<'-c director_name -d dir0 -o output'
	echo $i
done
