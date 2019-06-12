#!/bin/bash

if [ $# -ge 1 ] ; then
 if [ $1 != "x" ] ; then
  TOUTATISPATH=$1
 fi
else
 TOUTATISPATH=$HOME/Softwares/Toutatis/bin
 if [ $USER == "ngu" ] ; then
  TOUTATISPATH=/Applications/Toutatis.app/Contents/MacOS
 fi
fi

echo Preparing to run Toutatis from path: $TOUTATISPATH

if [ -f dmr.4mc ]; then
 a=1;
 while read myline
 do
  case $a in
 1) epsin=$myline
    ;;
 2) rbeta=$myline
    ;;
 3) rgamma=$myline
    ;;
 4) xpmax=$myline
    ;;
 5) xmax=$myline
    ;;
 6) xpatxmax=$myline
    ;;     
 7) numparts=$myline
    ;;
 *) echo unknown line
    ;;
 esac
 a=$(( $a+1))
 done < dmr.4mc
else 
 epsin=1                         # normalized total emittance in pi mm mrad 
 rbeta=0.006529  		 # relativistic beta @ 20 keV 4 proton
 rgamma=1.000021 		 # relativistic gamma @ 20 keV 4 proton
 xpmax=135.1     		 # mrad
 xmax=1.4        		 # mm
 xpatxmax=-69.4  		 # mrad
 numparts=50000   		 # just number of particles
fi

if [ 0 ] ; then
 echo numparts $numparts
 echo epsin $epsin
 echo add more for debug FIXME
fi
#calculation scale
scl=10

epsig=`echo "scale = $scl; ($epsin)/(($rbeta)*($rgamma))" | bc`
beta=`echo "scale = $scl; (($xmax)*($xmax))/($epsig)" | bc`
alpha=`echo "scale = $scl; (-1)*($xpatxmax)*sqrt($beta/$epsig)" | bc `
epsig=`echo "scale = $scl; $epsig/10000" | bc `
beta=`echo "scale = $scl; $beta*100" | bc `
echo epsig = $epsig  [cm.rad]
echo beta = $beta  [cm/rad]
echo alpha = $alpha [unitless]

cd share
cat ex_start.txt  > myex.inp

if [ -f gap.in ] ; then
 cat gap.in >> myex.inp
else
 echo end >> myex.inp
fi

rv=""
if [ $# -eq 2 ] ; then
 rv=$2
 echo preparing for $rv
fi
cat dmr${rv}.out  >> myex.inp
echo " " >>myex.inp 
echo "input -6 $numparts $alpha $beta $epsig $alpha $beta $epsig 180. 0. " >>myex.inp
echo "scheff 1.0 " >> myex.inp
echo "exitffl 1.882 " >> myex.inp
echo "vfac 1.0 " >> myex.inp
echo "end " >> myex.inp


cd ..
mkdir toutatis_result
cd toutatis_result

echo Preparation of the vane files _will_take long time...
$TOUTATISPATH/Toutatis -i ../share/myex.inp > terminal.out
