#!/bin/bash
clear
make clean
rm -rf scriptOutput
make
mkdir scriptOutput
#validation run for bimodal predictor
m2=6
BTB_SIZE=0
BTB_ASSOC=0 
TYPE=bimodal
#validation  1
echo Bimodal validation 1
INPUT="./traces/gcc_trace.txt"
FILE="./scriptOutput/output_${TYPE}_${m2}_${BTB_SIZE}_${BTB_ASSOC}.txt"
./sim $TYPE $m2 $BTB_SIZE $BTB_ASSOC $INPUT > $FILE
echo $FILE
diff -iw $FILE ./validation_runs/val_bimodal_1.txt
#validation 2
echo Bimodal validation 2
m2=9
FILE="./scriptOutput/output_${TYPE}_${m2}_${BTB_SIZE}_${BTB_ASSOC}.txt"
./sim $TYPE $m2 $BTB_SIZE $BTB_ASSOC $INPUT > $FILE
diff -iw $FILE ./validation_runs/val_bimodal_2.txt
echo $FILE
#validation 3
echo Bimodal validation 3
m2=5
INPUT="./traces/perl_trace.txt"
FILE="./scriptOutput/output_${TYPE}_${m2}_${BTB_SIZE}_${BTB_ASSOC}.txt"
./sim $TYPE $m2 $BTB_SIZE $BTB_ASSOC $INPUT > $FILE
echo $FILE
diff -iw $FILE ./validation_runs/val_bimodal_3.txt
#validation 4
echo Bimodal validation 4
m2=10
INPUT="./traces/jpeg_trace.txt"
FILE="./scriptOutput/output_${TYPE}_${m2}_${BTB_SIZE}_${BTB_ASSOC}.txt"
./sim $TYPE $m2 $BTB_SIZE $BTB_ASSOC $INPUT > $FILE
echo $FILE
diff -iw $FILE ./validation_runs/val_bimodal_4.txt
#validation run for ghsare predictor
#validation  1
echo ghsare validation 1
M1=10
N=5
BTB_SIZE=0
BTB_ASSOC=0 
TYPE=gshare
INPUT="./traces/gcc_trace.txt"
FILE="./scriptOutput/output_${TYPE}_${M1}_${N}_${BTB_SIZE}_${BTB_ASSOC}.txt"
./sim $TYPE $M1 $N $BTB_SIZE $BTB_ASSOC $INPUT > $FILE
echo $FILE
diff -iw $FILE ./validation_runs/val_gshare_1.txt
#validation  2
echo ghsare validation 2
M1=11
N=6
BTB_SIZE=0
BTB_ASSOC=0 
TYPE=gshare
INPUT="./traces/gcc_trace.txt"
FILE="./scriptOutput/output_${TYPE}_${M1}_${N}_${BTB_SIZE}_${BTB_ASSOC}.txt"
./sim $TYPE $M1 $N $BTB_SIZE $BTB_ASSOC $INPUT > $FILE
echo $FILE
diff -iw $FILE ./validation_runs/val_gshare_2.txt
#validation 3
echo ghsare validation 3
M1=15
N=10
BTB_SIZE=0
BTB_ASSOC=0 
TYPE=gshare
INPUT="./traces/jpeg_trace.txt"
FILE="./scriptOutput/output_${TYPE}_${M1}_${N}_${BTB_SIZE}_${BTB_ASSOC}.txt"
./sim $TYPE $M1 $N $BTB_SIZE $BTB_ASSOC $INPUT > $FILE
echo $FILE
diff -iw $FILE ./validation_runs/val_gshare_3.txt
#validation 4
echo ghsare validation 4
M1=14
N=4
BTB_SIZE=0
BTB_ASSOC=0 
TYPE=gshare
INPUT="./traces/perl_trace.txt"
FILE="./scriptOutput/output_${TYPE}_${M1}_${N}_${BTB_SIZE}_${BTB_ASSOC}.txt"
./sim $TYPE $M1 $N $BTB_SIZE $BTB_ASSOC $INPUT > $FILE
echo $FILE
diff -iw $FILE ./validation_runs/val_gshare_4.txt
#BTB validation
echo BTB validation 1
m2=5
BTB_SIZE=2048
BTB_ASSOC=4 
TYPE=bimodal
INPUT="./traces/gcc_trace.txt"
FILE="./scriptOutput/output_${TYPE}_${m2}_${BTB_SIZE}_${BTB_ASSOC}.txt"
./sim $TYPE $m2 $BTB_SIZE $BTB_ASSOC $INPUT > $FILE
echo $FILE
diff -iw $FILE ./validation_runs/val_BTB_1.txt
#BTB validation
echo BTB validation 2
m2=5
BTB_SIZE=2048
BTB_ASSOC=4 
TYPE=bimodal
INPUT="./traces/gcc_trace.txt"
FILE="./scriptOutput/output_${TYPE}_${m2}_${BTB_SIZE}_${BTB_ASSOC}.txt"
./sim $TYPE $m2 $BTB_SIZE $BTB_ASSOC $INPUT > $FILE
echo $FILE
diff -iw $FILE ./validation_runs/val_BTB_1.txt
#BTB validation 2
echo BTB validation 2
M1=8
N=6
BTB_SIZE=4096
BTB_ASSOC=4 
TYPE=gshare
INPUT="./traces/perl_trace.txt"
FILE="./scriptOutput/output_${TYPE}_${M1}_${N}_${BTB_SIZE}_${BTB_ASSOC}.txt"
./sim $TYPE $M1 $N $BTB_SIZE $BTB_ASSOC $INPUT > $FILE
echo $FILE
diff -iw $FILE ./validation_runs/val_BTB_2.txt
#hybrid validation 1
#sim hybrid <K> <M1> <N> <M2> <BTB size> <BTB assoc> <tracefile>
echo hybrid validation 1
TYPE=hybrid
K=6
M1=11
N=11
M2=5
BTB_SIZE=0
BTB_ASSOC=0
INPUT="./traces/gcc_trace.txt"
FILE="./scriptOutput/output_${TYPE}_${K}_${M1}_${N}_${M2}_${BTB_SIZE}_${BTB_ASSOC}.txt"
./sim $TYPE $K $M1 $N $M2 $BTB_SIZE $BTB_ASSOC $INPUT > $FILE
echo $FILE
diff -iw $FILE ./validation_runs/val_hybrid_1.txt
#hybrid validation 2
echo hybrid validation 2
TYPE=hybrid
K=5
M1=10
N=8
M2=5
BTB_SIZE=0
BTB_ASSOC=0
INPUT="./traces/perl_trace.txt"
FILE="./scriptOutput/output_${TYPE}_${K}_${M1}_${N}_${M2}_${BTB_SIZE}_${BTB_ASSOC}.txt"
./sim $TYPE $K $M1 $N $M2 $BTB_SIZE $BTB_ASSOC $INPUT > $FILE
echo $FILE
diff -iw $FILE ./validation_runs/val_hybrid_2.txt















