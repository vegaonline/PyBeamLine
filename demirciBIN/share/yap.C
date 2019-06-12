#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <fstream>

int yap(){

 double delta_z[4], delta_y[4];
// order is b1 b2 k1 k2
 delta_z[0]=24;
 delta_y[0]=36.62; 
 delta_z[1]=24;
 delta_y[1]=36.62; 
 delta_z[2]=24;
 delta_y[2]=36.62; 
 delta_z[3]=24;
 delta_y[3]=36.62; 


 std::vector <double> zpos, buyuk1y, buyuk2y, kucuk1y, kucuk2y;
 std::vector <double> ezpos, eypos;
 double z,y1,y2,y3,y4;

 char *vane_filename="vanes";
 ifstream li_in;
          li_in.open(vane_filename);

 for (int nc=0; !li_in.eof(); nc++) { 
  li_in >>z >>y1 >>y2 >>y3 >>y4;
  zpos.push_back(z*1000+delta_z[0]); // we read in m but we need in mm // they all have the same z
  buyuk1y.push_back( delta_y[0]-y1*1000 ); // we need to invert and convert to mm
  kucuk1y.push_back( delta_y[1]-y2*1000 ); // we need to invert and convert to mm
  buyuk2y.push_back( delta_y[2]-y3*1000 ); // we need to invert and convert to mm
  kucuk2y.push_back( delta_y[3]-y4*1000 ); // we need to invert and convert to mm
 }

// -------------for buyuk1 vane--------
 char *err_filename="err_b1.txt";
 ifstream err_in;
          err_in.open(err_filename);
 for (int nc=0; !err_in.eof(); nc++) { 
  err_in >>  z >> y1;
  ezpos.push_back(z); // we read in mm
  eypos.push_back( y1/1000); // we need to convert from um to mm
 }

 for (int i=0; i<zpos.size(); i++){
// we study the toutatis z pos values one by one
  for (int j=0; j<ezpos.size(); j++){
   if (ezpos[j]>zpos[i] ){ // should do only once 
    double m,b,err;
    // y=mx+b : I use j and j-1 to find m and b
    m=(eypos[j]-eypos[j-1] )/(ezpos[j]-ezpos[j-1]);
    b= eypos[j]-m*ezpos[j] ;
    err=m*zpos[i]+b;
    buyuk1y[i]=buyuk1y[i]-err;
    break;
   }
  }
 }
 cout << "b1 done\n";
 err_in.close();
 ezpos.clear();
 eypos.clear();

// -------------for kucuk1 vane--------
 err_filename="err_k1.txt";
          err_in.open(err_filename);
 for (int nc=0; !err_in.eof(); nc++) { 
  err_in >>  z >> y2;
  ezpos.push_back(z); // we read in mm
  eypos.push_back( y2/1000); // we need to convert from um to mm
 }

 for (int i=0; i<zpos.size(); i++){
// we study the toutatis z pos values one by one
  for (int j=0; j<ezpos.size(); j++){
   if (ezpos[j]>zpos[i] ){ // should do only once 
    double m,b,err;
    // y=mx+b : I use j and j-1 to find m and b
    m=(eypos[j]-eypos[j-1] )/(ezpos[j]-ezpos[j-1]);
    b= eypos[j]-m*ezpos[j] ;
    err=m*zpos[i]+b;
    kucuk1y[i]=kucuk1y[i]-err;
    break;
   }
  }
 }
 err_in.close();
 ezpos.clear();
 eypos.clear();
 cout << "k1 done\n";


// -------------for buyuk2 vane--------
 err_filename="err_b2.txt";
          err_in.open(err_filename);
 for (int nc=0; !err_in.eof(); nc++) { 
  err_in >>  z >> y3;
  ezpos.push_back(z); // we read in mm
  eypos.push_back( y3/1000); // we need to convert from um to mm
 }

 for (int i=0; i<zpos.size(); i++){
// we study the toutatis z pos values one by one
  for (int j=0; j<ezpos.size(); j++){
   if (ezpos[j]>zpos[i] ){ // should do only once 
    double m,b,err;
    // y=mx+b : I use j and j-1 to find m and b
    m=(eypos[j]-eypos[j-1] )/(ezpos[j]-ezpos[j-1]);
    b= eypos[j]-m*ezpos[j] ;
    err=m*zpos[i]+b;
    buyuk2y[i]=buyuk2y[i]-err;
    break;
   }
  }
 }
 err_in.close();
 ezpos.clear();
 eypos.clear();
 cout << "b2 done\n";

// -------------for buyuk2 vane--------
 err_filename="err_k2.txt";
          err_in.open(err_filename);
 for (int nc=0; !err_in.eof(); nc++) { 
  err_in >>  z >> y4;
  ezpos.push_back(z); // we read in mm
  eypos.push_back( y4/1000); // we need to convert from um to mm
 }

 for (int i=0; i<zpos.size(); i++){
// we study the toutatis z pos values one by one
  for (int j=0; j<ezpos.size(); j++){
   if (ezpos[j]>zpos[i] ){ // should do only once 
    double m,b,err;
    // y=mx+b : I use j and j-1 to find m and b
    m=(eypos[j]-eypos[j-1] )/(ezpos[j]-ezpos[j-1]);
    b= eypos[j]-m*ezpos[j] ;
    err=m*zpos[i]+b;
    kucuk2y[i]=kucuk2y[i]-err;
    break;
   }
  }
 }
 err_in.close();
 ezpos.clear();
 eypos.clear();
 cout << "k2 done\n";


//--------------------------------------------------------
// spit out...
char *out_filename="real_vanes.txt";
 ofstream out_f;
          out_f.open(out_filename);
out_f << fixed << setprecision(8) ;
for (int i=0; i< (zpos.size()-1); i++){
         out_f << (zpos[i]-delta_z[0]   )/1000
      << "   " << (delta_y[0]-buyuk1y[i])/1000 
      << "   " << (delta_y[1]-kucuk1y[i])/1000
      << "   " << (delta_y[2]-buyuk2y[i])/1000
      << "   " << (delta_y[3]-kucuk2y[i])/1000
      <<endl;
}
out_f.close();

} 
