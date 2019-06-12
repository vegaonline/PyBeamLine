#!/bin/bash
if [ $# -lt 4 ] ; then
 echo "$0 [horizontalvanefile (in Lidos format)] [verticalvanefile (in Lidos format)] [vanevoltage (V)] [vanedisplacement (m)]"
 echo "tip: vane voltage = inter-vane voltage/2."
 exit -1
fi

rm -f *.vane
hv=`wc -l $1| awk '{print $1-32}'`
  tail -n $(( hv ))  $1 > tmp1
vv=`wc -l $2| awk '{print $1-32}'`
  tail -n $(( vv ))  $2 > tmp2
vavo=$3 # vane voltage
vdisp=$4 # vane displacement

paste tmp1 tmp2 tmp1 tmp2| awk '{ print $5/100, (-1)*$6/100, $7/100, '$vavo', '$vdisp', (-1)*$14/100, $15/100, (-1)*'$vavo', '$vdisp', (-1)*$22/100, $23/100, '$vavo', '$vdisp', (-1)*$30/100, $31/100, (-1)*'$vavo', '$vdisp'}' >> toutatis.vane

rm -f tmp1 tmp2
echo "toutatis.vane file is created."
exit 0
