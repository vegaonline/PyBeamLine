#!/bin/bash
#simple bicubic element stiffness matrix for test cases
#./e_stif.exe
# creates: residuals.dat element_stiff_2.dat element_stiff.dat


if [ $# -gt 4 ] ; then
 do_cell=-1
 c_ba=$1
 c_rho=$2
 c_m=$3
 c_a=$4
 c_l=$5
# problem is from the driving programme.
else 
#define the problem here, locally
 # c_ba=1.1780972451
 c_ba=1.2053
 c_rho=0.348
 do_cell=100
fi

# of elements
X=4
Y=4
Z=8


case "$do_cell" in 

20) c_m=1.020
    c_a=0.409
    c_l=0.58
    ;;

60) c_m=1.072
    c_a=0.399
    c_l=0.60
    ;;

100) c_m=1.111
     c_a=0.392
     c_l=0.68
     ;;

140) c_m=1.171
     c_a=0.381
     c_l=0.92
     ;;

180) c_m=1.631
     c_a=0.309
     c_l=1.94
     ;;

181) c_m=1.68
     c_a=0.309
     c_l=2.285
     c_ba=1.25
     c_rho=0.376601
     ;;

182) c_m=1.000000
     c_a=1.53
     c_l=0.277882
     c_ba=1.171
     c_rho=1.361700    
     ;;
esac

#build connection matrix using nx *ny *nz brickes, 
./connectionmatrix.exe -x $X -y $Y -z $Z -Ym 100 -Yp -100 # -Ym 50  -Yp 200   #-Xm 100 -Xp -100 # -Ym 50  -Yp 200 #  -Zm 10 -Zp 10
#creates: test_connection.dat known_nodes.mat

# R: tip radius
# L: cell length in Z
# M: modulation
# A: min. bore radius
# T: break of angle, radians
# coordinatemap.dat
echo ./Cell_Mesher.exe -Nx $X  -Ny $Y -Nz $Z  -A $c_a -M $c_m -L $c_l -R $c_rho  -T $c_ba  #Cell 100 
./Cell_Mesher.exe -Nx $X  -Ny $Y -Nz $Z  -A $c_a -M $c_m -L $c_l -R $c_rho  -T $c_ba  #Cell 100 

./Build_solve_matrix.exe -C test_connection.dat -E element_stiff.dat -K known_nodes.mat -M coordinatemap.dat
# Ku = F
# out: All_nodes_value.txt

cd Checks

#./residual.exe
./printnodevalues.exe -Rcut $c_a # prints 2 txt files
./8T.exe -a $c_a -m $c_m -cl $c_l   
##echo "=============" 
##echo  $c_ba " " $c_rho "  " $c_m "  " $c_a "  " $c_l 
##echo $do_cell " " $X " " $Y " " $Z " " 
##### the following should not be changed...
cat 8Tresults.txt >> all_8Terms.txt
#root -x -q res_print.C
#root -x -q Ifit.C\($c_m,$c_a,$c_l,$c_rho\)
cd -


