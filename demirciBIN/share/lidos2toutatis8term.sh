#!/bin/bash
if [ $# -lt 3 ] ; then
 echo "$0 [horizontalvanefile (in Lidos format)] [verticalvanefile (in Lidos format)] [vane-voltage (kV)]"
 exit -1
fi

NUMOFLINES1=$(wc -l < $1)
nbof1=$(($NUMOFLINES1-32))

NUMOFLINES2=$(wc -l < $2)
nbof2=$(($NUMOFLINES2-32))

rm -f *.out
hv=`wc -l $1| awk '{print $20-'$nbof1'}'`
  tail -n $(( hv ))  $1 > tmp1
vv=`wc -l $2| awk '{print $20-'$nbof2'}'`
  tail -n $(( vv ))  $2 > tmp2

cn=`paste tmp1 | awk 'NR=='$nbof1' {print $4}'`

n=0

while [ $n -le $cn ]
do

paste tmp1 tmp2 | awk '$4=='$n' {a=$6>a||a==""?$6:a;ma=$6<ma||ma==""?$6:ma;l=$5>l||l==""?$5:l;lmi=$5<lmi||lmi==""?$5:lmi} END{ print '$n','$3', 0 , 0 , 0 , 0 , 0 , (-1)*a,ma/a,(((ma/a)+1)*(-1)*a)/2, 0, 0, l-lmi, l, 0, 0 }' >> lidos2toutatis8term.out

n=$(( n+1 ))
done

rm -f tmp1 tmp2
echo "lidos2toutatis8term.out file is created. Format:" 
echo "[n] [V(kV)] [Ws(MeV)] [beta] [E0(MV/m)] [A10] [phi(deg)] [a(cm)] [m] [r0(cm)] [rho(cm)] [B] [cl] [l] [ImT(A)] [ImL(A)]"
exit 0
