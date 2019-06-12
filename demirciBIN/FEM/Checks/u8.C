#include "TMath.h"
#include "TCanvas.h"
#include <Riostream.h>
#include "TLegend.h"
#include "TLegendEntry.h"
#include "Math/IFunction.h"
#include <cmath>
#include "TSystem.h"
#include "TAxis.h"
#include "TPaveLabel.h"
#include "TRandom2.h"
#include "TF3.h"
#include "TError.h"
#include "Fit/BinData.h"
#include "Fit/Fitter.h"
#include "Math/WrappedMultiTF1.h"


/*
// cell 180
double a=0.309;
double m=1.631;
double cl=1.94; // cell length
double C00=5.65552;
double C01=2.36514;
double C10=0.44918;
double C11=-36.14094;
double C20=-0.04355;
double C21=-100.715;
double C30=-0.00002;
double C31=-0.10643;
*/

// cell 100
double a=0.392;
double m=1.111;
double cl=0.68;
double C00=5.74320;
double C01=4.88818;
double C10=0.04307;
double C11=0.45675;
double C20=0.00018;
double C21=0.00087;
double C30=0.0;
double C31=0.0;

/*
// cell 20
double a=0.409;
double m=1.020;
double cl=0.58;
double C00=5.87466;
double C01=4.26021;
double C10=5.66954e-03;
double C11=5.16410e-02;
double C20=-1.94050e-03;
double C21=-3.28459e-03;
double C30=1.15755e-06;
double C31=8.76245e-06;
*/

double rho=0.348;
double V(double r, double th, double z){

double k=TMath::Pi()/cl;
double v;
bool insidepole=false;

double x=r*cos(th);
double y=r*sin(th);
double xp,yp;

xp=x-(a+rho);
yp=y-(a*m+rho);
if (sqrt(y*y + xp*xp)< rho && th<TMath::Pi()/4) insidepole=true;
if (sqrt(yp*yp + x*x)< rho && th>TMath::Pi()/4) insidepole=true;

if ( insidepole ) {v=0;} else
{
 v=C00*pow(r,2.0)*cos(2.*th)
  +C10*ROOT::Math::cyl_bessel_i(0,k*r)*cos(k*z)
  +C01*pow(r,6.0)*cos(6.*th)
  +C11*ROOT::Math::cyl_bessel_i(4,k*r)*cos(4.*th)*cos(k*z)
  +C20*ROOT::Math::cyl_bessel_i(2,2.*k*r)*cos(2.*th)*cos(2.*k*z)
  +C21*ROOT::Math::cyl_bessel_i(6,2.*k*r)*cos(6.*th)*cos(2.*k*z)
  +C30*ROOT::Math::cyl_bessel_i(0,3.*k*r)*cos(3.*k*z)
  +C31*ROOT::Math::cyl_bessel_i(4,3.*k*r)*cos(4.*th)*cos(3.*k*z);
}

return v;
}

int u8() {
gSystem->Load("libMathCore");
gSystem->Load("libMathMore");
gStyle->SetPalette(1);
cout << "at the center:"<< V(0,0,0) <<endl;

TH2D *xy= new TH2D("xy","xy",50,0,1, 50, 0,1);
for (double t_d=0; t_d<=90; t_d+=0.001){
 double vmax=0, rmax, thmax;
 double t=t_d*TMath::Pi()/180.; // t in radians
 for (double r=0.00; r<0.810; r+=0.01) {
   double x=r*cos(t);
   double y=r*sin(t);
   double res=V(r,t,0);
   if (fabs(res)>vmax) {vmax=fabs(res); rmax=r; thmax=t;}
//   cout << res<<endl;
   xy->Fill(x,y, res);
 }
}
// cout << vmax<< " @ r:"<<rmax<<" th:"<<thmax*180/TMath::Pi()<<endl;
// cout << -vmax<< "  "<<rmax<<"    "<<thmax<<endl;

double alpha,h,L,tet,res;
// horizontal electrode
for (double alpha_d=0; alpha_d<=45; alpha_d+=0.1){
 alpha=alpha_d*TMath::Pi()/180.;
 h=rho*sin(alpha);
 L=sqrt(a*a + 2*rho*(1-cos(alpha))*(rho+a));
 tet=asin(h/L);
 res=V(L,tet,0);
 //cout << alpha_d<<" " << h<< "  "<<L<<"    "<<tet<<endl;
 cout << res<<" " <<L<<"    "<<tet <<endl;
}

// veritcal electrode
for (double alpha_d=45; alpha_d>=0; alpha_d-=0.1){
 alpha=alpha_d*TMath::Pi()/180.;
 h=rho*sin(alpha);
 L=sqrt(a*a*m*m + 2*rho*(1-cos(alpha))*(rho+a*m));
 tet=TMath::Pi()/2 - asin(h/L);
 res=V(L,tet,0);
 //cout << alpha_d<<" " << h<< "  "<<L<<"    "<<tet<<endl;
 cout << res<<" " <<L<<"    "<<tet <<endl;
}

// to draw if needed.
xy->Draw("COLZ");
TLine *tl=new TLine  (0,0, 1,1);
tl->Draw();

TCrown *arc1=new TCrown(a+rho, 0, rho, rho, 90, 180); arc1->Draw();
TCrown *arc2=new TCrown(0, m*a+rho, rho,rho, 270, 360 ); 
arc2->SetLineStyle(4);arc2->Draw();
return 0;
}

