#!/bin/bash

if [ $# -ne 1 ]; then
  echo "please give a1 or a2 "
  exit 1
fi

if [ $1 == "a1" ]; then
 cat a1.csv | awk -F "," '{print $1, $2}' > err_k1.txt
 cat a1.csv | awk -F "," '{print $3, $4}' > err_k2.txt
 cat a1.csv | awk -F "," '{print $5, $6}' > err_b1.txt
 cat a1.csv | awk -F "," '{print $7, $8}' > err_b2.txt
fi

if [ $1 == "a2" ]; then
 cat a2.csv | awk -F "," '{print $1, $2}' > err_k1.txt
 cat a2.csv | awk -F "," '{print $3, $4}' > err_k2.txt
 cat a2.csv | awk -F "," '{print $5, $6}' > err_b1.txt
 cat a2.csv | awk -F "," '{print $7, $8}' > err_b2.txt
fi
