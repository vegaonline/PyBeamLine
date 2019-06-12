#!/bin/bash

if [ $# -lt 2 ] ; then
 echo "$0 [inputfilename] [outputfilename] [-g]"
 echo "-g is optional to output in gdmr format."
 echo "inputfilename will be Lidos Advisor format"
 echo "outputfilename will be in demirci.exe input format"
 exit -1
fi

ifn=$1
ofn=$2
nc=`wc -l $ifn| awk '{print $1-39}'`


freq=`grep Frequency $ifn| awk '{print $2}'`
Ein=`head -n5 $ifn | tail -n1| awk '{print 1000*$5}'`

if [ $# -gt 2 ] ; then
 echo "output file in gdmr format"


 echo '$HOME/Toutatis.app/Contents/MacOS SANAEM ' > $ofn
 echo "$nc $nc  $Ein  $freq               999.5" >> $ofn
 echo "1.00  135.100  1.40  -69.400 5000 "       >> $ofn
 echo "8388608 1.800000 0.8  1.000000 1.000000 0"       >> $ofn
 echo "-1 -1 -1 -1 "                             >> $ofn
 tail -n $(( nc +35))  $ifn > pippo
 cat pippo | grep  -B1000 'Input Current'| awk '{if ( $1*10 > 0 ) { print $7*1000, $1,  $9, $1,  $6, $1,  $10/10, $1} }' >>$ofn
 rm -f pippo

else
 echo "output file in dmr format"

  echo  "$freq" '      '  "$Ein"  '       1.53	0.89' >$ofn
  echo  'RFfreq (MHz) |  Ein (kV) | KP | rho_over_r0' >>$ofn
  echo  'V(kV)  | phi(deg)  |  m   | a(cm)' >>$ofn
  tail -n $(( nc +35))  $ifn > pippo
  cat pippo | grep  -B1000 'Input Current'| awk '{if ( $1*10 > 0 ) { print $7*1000, $9, $6, $10/10} }' >>$ofn
  rm -f pippo
fi
echo "$ofn is (re)written."
exit 0
