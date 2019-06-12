#!/bin/bash

if [ $# -ne 1 ]; then 
  echo "please give an argument as get or set "
  exit 1
fi

ifile="toutatis.vane"
efile="real_vanes.txt"


if [ $1 == "get" ]; then
  echo "getting from toutatis"
  awk '{print $1,$2,$6,$10,$14}' ${ifile} > vanes
fi


if [ $1 == "set" ]; then
  echo "setting into toutatis"
  echo '
        FILENAME == ARGV[1] {   one[FNR]=$1 " " $2 " " }
        FILENAME == ARGV[2] {   two[FNR]=$3 " " $4 " " $5 " " }
        FILENAME == ARGV[3] { three[FNR]=$3 " " }
        FILENAME == ARGV[4] {  four[FNR]=$7 " " $8 " " $9 " " }
        FILENAME == ARGV[5] {  five[FNR]=$4 " " }
        FILENAME == ARGV[6] {   six[FNR]=$11 " " $12 " " $13 " " }
        FILENAME == ARGV[7] { seven[FNR]=$5 " " }
        FILENAME == ARGV[8] { eight[FNR]=$15 " " $16 " " $17 " " }

        END {
            for (i=1; i<=length(one); i++) {
                print one[i],  two[i]  three[i]  four[i]  five[i]  six[i]  seven[i]  eight[i]
            }
        }
  ' > reassemble.awk
  awk -f reassemble.awk ${efile} ${ifile} ${efile} ${ifile} ${efile} ${ifile} ${efile} ${ifile} > outv.txt

fi


#awk '
#    {line[FNR] = line[FNR] $1 OFS}
#    END {for (i=1; i<=FNR; i++) print line[i]}
#' zs_errors_1.txt zs_errors_2.txt > zs_errors.txt
