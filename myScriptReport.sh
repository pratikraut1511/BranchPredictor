#!/bin/bash
clear
make clean
rm -rf report
make all
mkdir report
#validation run for bimodal predictor
m2=7
BTB_SIZE=0
BTB_ASSOC=0 
TYPE=bimodal
#validation  1
for TRACE in gcc_trace perl_trace jpeg_trace
do
	mkdir ./report/${TRACE}_${TYPE}
	for m2 in 7 8 9 10 11 12
	do
		INPUT=./traces/${TRACE}.txt
		FILE="./report/${TRACE}_${TYPE}/output_${TYPE}_${TRACE}_${m2}_${BTB_SIZE}_${BTB_ASSOC}.txt"
		./sim $TYPE $m2 $BTB_SIZE $BTB_ASSOC $INPUT > $FILE
		#echo $FILE
	done
done
#validation run for gshare predictor
M1=14
N=2
BTB_SIZE=0
BTB_ASSOC=0 
TYPE=gshare

for TRACE in gcc_trace perl_trace jpeg_trace
do
	mkdir ./report/${TRACE}_${TYPE}
	for M1 in 7 8 9 10 11 12
	do
		N=2
		while(($N < $M1))
		do
			INPUT=./traces/${TRACE}.txt
			FILE="./report/${TRACE}_${TYPE}/output_${TYPE}_${TRACE}_${M1}_${N}_${BTB_SIZE}_${BTB_ASSOC}.txt"
			./sim $TYPE $M1 $N $BTB_SIZE $BTB_ASSOC $INPUT > $FILE
			#echo $FILE
			N=$(($N+2))
		done
	done
done

echo gshare completed
echo validation run completed
echo reading data from files
for subFolder in ./report/*
do
	echo subfolder = $subFolder
	cd $subFolder
	rm -rf data.txt
	touch data.txt
	for file in *
	do	
		echo $file >> data.txt
		awk '/misprediction rate:/{print $NF >> "./data.txt"}' $file
	done
	cd ../..
done
echo "Check data.txt file"


