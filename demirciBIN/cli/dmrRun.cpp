#include "inc/demirci.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h>
#include <cstdlib>
#include <string.h>

using namespace std;

int main (int argc, char *argv[] ) {

// some switches
bool demirci2lidos=false;
bool demirci4lidos=false;
bool readExtField=false;
bool demirci4vane=false;
bool redefinePhi=false; //redefinition of cells
bool plot=false;
bool runp=false;
bool do_twiss=false;

// needed for the calculations
float targetE=9999999.9; // in MeV
float targetL=9999999.9; // in cm
int nparticles=0;
float Vforced=0;
int rv; //generic return value

cout << "~~~~~~~~~~~~~~~~~~~~~~~~"<< endl;
cout << "Welcome to  Demirci  v"<<DEMIRCI_VERSION << endl;
cout << "~~~~~~~~~~~~~~~~~~~~~~~~"<< endl;

demir dmr;
string input_fname="./share/dmr.in";
string ExtE_fname="./share/SPP_RFQ_ES_3Dfield.txt";

// command line arguments
for(int i=1; i <argc; i++) {
   if        (std::string(argv[i]) == "-h") { dmr.displayHelp();
   } else if (std::string(argv[i]) == "-v") { dmr.Verbose( atoi(argv[i + 1]) );
   } else if (std::string(argv[i]) == "-f") { input_fname=argv[i + 1];
   } else if (std::string(argv[i]) == "-l") { demirci4lidos=true;
   } else if (std::string(argv[i]) == "-L") { demirci2lidos=true;
   } else if (std::string(argv[i]) == "-S") { demirci4vane=true;
   } else if (std::string(argv[i]) == "-r") { runp=true;
   } else if (std::string(argv[i]) == "-c") { dmr.getPhase( ); redefinePhi=true; dmr.savePHI ();
   } else if (std::string(argv[i]) == "-p") { dmr.readPhase( ); redefinePhi=true; dmr.savePHI ();
   } else if (std::string(argv[i]) == "-s") { dmr.readGap( ); dmr.saveGap ();
   } else if (std::string(argv[i]) == "-g") { dmr.getGap( );  dmr.saveGap ();
   } else if (std::string(argv[i]) == "-A") { dmr.accepCalc();
   } else if (std::string(argv[i]) == "-t") { targetE=atof(argv[i + 1]);
   } else if (std::string(argv[i]) == "-z") { targetL=atof(argv[i + 1]);
   } else if (std::string(argv[i]) == "-V") { Vforced=atof(argv[i + 1]);
   } else if (std::string(argv[i]) == "-P") { demirci2lidos=true; plot=true;
   } else if (std::string(argv[i]) == "-E") { readExtField=true; if (strlen(argv[i + 1])>2) ExtE_fname=argv[i + 1];
   } else if (std::string(argv[i]) == "-T") { do_twiss=true; nparticles=atoi(argv[i + 1]);
   }
}

dmr.initialize((char *)input_fname.data());
dmr.printBasics();
dmr.readRFQparams( Vforced,  redefinePhi);
dmr.engageRFQ( targetE, targetL);    // to process the protons in the RFQ -----------
dmr.printResult();         // to print final energy and time spend in the RFQ


if (readExtField){
 dmr.getExtField((char *)ExtE_fname.data()); // read the external field
 dmr.p_ExtE=true;
//TODO: check return value and print some error msgs
} else {dmr.p_ExtE=false;}

if (do_twiss) {
               dmr.twissCalc(2, nparticles);
               cout << "Phase space parameters are calculated and saved in ./share/x_xp.txt"<<endl;
              // system("if [ `uname` == 'Darwin' ] ; then open ./share/xxp.eps; fi "); // open is mac only // put some intelligence here
}

if (runp) {
std::vector<part_data> p_p = dmr.vector_p_p();
cout << "Starting Beam Dynamics on "<< p_p.size()<< " particles.\n";
double timeT= dmr.moveParticles(0, p_p);//now p_f is initialized
std::vector<part_data> p_f = dmr.vector_p_f();

  while (p_f.at(0).z < 10*dmr.getRFQ().z[dmr.getRFQ().nocells-1] ) { // was in cm
//    cout << "~~~~~~~~~~~~~ time (ns):"<<timeT*1E9<<endl;
    timeT=dmr.moveParticles(timeT, p_f);
      p_f=dmr.vector_p_f();
    if (p_f.size()<1) { cout << "no particles left!\n"; break;}
//    char a; cin>>a;
  }
 cout <<"Dynamics over, E:"<<p_f.at(0).E<<" MeV\n";

}
cout << "preparing main output file ... ";
rv=dmr.dump4toutatis ((char *) "dmr.out"); 
if (!rv) {cout << "ok."; } else {cout <<"fails";}; cout << endl;
if (demirci4lidos) { 
   cout << "preparing dmr4lidos.out file... "; 
   rv=dmr.dump4lidos ((char *) "dmr4lidos.out"); 
   if (!rv) {cout << "ok."; } else {cout <<"fails";}; cout << endl;
}
if (demirci2lidos) { 
   cout << "preparing dmr2lidos.out file... "; 
   rv=dmr.dump4lidos ((char *) "dmr2lidos.out"); 
   if (!rv) {cout << "ok."; } else {cout <<"fails";}; cout << endl;
}
if (demirci4vane) { 
   cout << "preparing dmr4vane.out file... "; 
   rv=dmr.dump4vane ((char *) "dmr4vane.out"); 
   if (!rv) {cout << "ok."; } else {cout <<"fails";}; cout << endl;
}
if (plot) {
dmr.plot ((char *) "AutoPlotRFQ.plt");
system("gnuplot AutoPlotRFQ.plt");
cout<< "prepared DemirciRFQ.eps file"<<endl;
system("rm AutoPlotRFQ.plt");}

return 0;
}
