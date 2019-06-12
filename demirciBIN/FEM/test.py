
X=6
Y=6
Z=8
c_ba=1.1780972451
c_rho=0.348
c_m=1.171
c_a=0.381
c_l=0.92

import numpy as np
import os
from subprocess import call




# C_A=np.arange(0.381,0.391,0.03).tolist()
# C_M=np.arange(1.171,1.181,0.04).tolist()
# C_L=np.arange(0.82,1.02,0.001).tolist()

# print C_A
# print len(C_A)
# print range(len(C_A))
import time;  # This is required to include time module.
import random
foo = random.SystemRandom() # Uses /dev/urandom or Windows CryptGenRandom

# Print 10 random numbers:
for i in range(40000):
    c_a= foo.uniform(0.1,0.5);
    c_m= foo.uniform(1.0,2.0);
    c_l= foo.uniform(0.1,3.0);

    constart = time.time()
    conmat= ["./connectionmatrix.exe","-x",str(X),"-y",str(Y),"-z",str(Z),"-Ym","100","-Yp","-100"]
    print conmat
    call(conmat ,shell=False)
    cellstart = time.time()
    cllmesh=["./Cell_Mesher.exe","-Nx",str(X),"-Ny",str(Y),"-Nz",str(Z),"-A",str(c_a),"-M",str(c_m),"-L",str(c_l),"-R",str(c_rho),"-T",str(c_ba)]
    print cllmesh
    call(cllmesh,shell=False)
    builtstart = time.time()
    x=call(["./Build_solve_matrix.exe","-C","test_connection.dat","-E","element_stiff.dat","-K","known_nodes.mat","-M","coordinatemap.dat"])
    if x==0 : 
        call(["pwd"])
        os.chdir("Checks")
        call(["pwd"])
        printstart= time.time()
        call(["./printnodevalues.exe"],stdin=None,stdout=None,stderr=None,shell=False)
#       os.rename("AllnodevaluesFEM_withcoordinates.txt","../multiplecells/m"+str(c_m)+"_a"+str(c_a)+"_l"+str(c_l)+".txt")
        call(["./8T.exe","-a",str(c_a),"-m",str(c_m),"-cl",str(c_l)])
        os.chdir("../")
        print "txt file copied to multiplecells"
        call(["pwd"])
        printend= time.time()

        print "--------------------"
        print cellstart-constart
        print builtstart-cellstart
        print printstart-builtstart
        print printend - printstart
        print "--------------------"
    else :
        with open("sorunlular.txt", "a") as myfile:
            myfile.write("Nx "+str(X)+" Ny "+str(Y)+" Nz "+str(Z)+" A "+str(c_a)+" M "+str(c_m)+" L "+str(c_l)+" R "+str(c_rho)+" T "+str(c_ba)+"\n")
print "done"


# for i in range(len(C_A)):
#     print "CA"
#     for j in range(len(C_M)):
#         print "CM"
#         for k in range(len(C_L)):
#             print "CL"




#call(["mv"," AllnodevaluesFEM_withcoordinates.txt ../multiplecells/m_a_l.txt"])



#AllnodevaluesFEM_withcoordinates.txt
#mv 

#./connectionmatrix.exe -x $X -y $Y -z $Z -Ym 100 -Yp -100 # -Ym 50  -Yp 200   #-Xm 100 -Xp -100 # -Ym 50  -Yp 200 #  -Zm 10 -Zp 10


# R: tip radius
# L: cell length in Z
# M: modulation
# A: min. bore radius
# T: break of angle, radians
# coordinatemap.dat
# echo ./Cell_Mesher.exe -Nx $X  -Ny $Y -Nz $Z  -A $c_a -M $c_m -L $c_l -R $c_rho  -T $c_ba  #Cell 100 
# ./Cell_Mesher.exe -Nx $X  -Ny $Y -Nz $Z  -A $c_a -M $c_m -L $c_l -R $c_rho  -T $c_ba  #Cell 100 

# ./Build_solve_matrix.exe -C test_connection.dat -E element_stiff.dat -K known_nodes.mat -M coordinatemap.dat
# # Ku = F
# # out: All_nodes_value.txt

# cd Checks
# #./8T.exe    #hardcoded for Cell 180
# #./residual.exe
#./printnodevalues.exe # prints 2 txt files

# #root -x -q res_print.C
# root -x -q Ifit.C\($c_m,$c_a,$c_l,$c_rho\)
# cd -


