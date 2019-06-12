#define NPTS 5000

int NP=0;
double a=0.0;
double m=0.0;
double cl=0.0;
double rho=0.0;



#include "TMinuit.h"

Float_t vv[NPTS], zz[NPTS],rr[NPTS],tt[NPTS],errorz[NPTS];

//______________________________________________________________________________
Double_t Vfunc(float z, float r,float th,Double_t *par)
{

double C00=par[0];
double C01=par[1];
double C10=par[2];
double C11=par[3];
double C20=par[4];
double C21=par[5];
double C30=par[6];
double C31=par[7];

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

if ( insidepole ) {if (th<TMath::Pi()/4) {v=1;} else {v=-1;} } else
{
 v=C00*pow(r,2.0)*cos(2.*th)
  +C10*ROOT::Math::cyl_bessel_i(0,k*r)*cos(k*z)
  +C01*pow(r,6.0)*cos(6.*th)
  +C11*ROOT::Math::cyl_bessel_i(4,   k*r)*cos(4.*th)*cos(   k*z)
  +C20*ROOT::Math::cyl_bessel_i(2,2.*k*r)*cos(2.*th)*cos(2.*k*z)
  +C21*ROOT::Math::cyl_bessel_i(6,2.*k*r)*cos(6.*th)*cos(2.*k*z)
  +C30*ROOT::Math::cyl_bessel_i(0,3.*k*r)*cos(3.*k*z)
  +C31*ROOT::Math::cyl_bessel_i(4,3.*k*r)*cos(4.*th)*cos(3.*k*z);
}

 return v;
}

//______________________________________________________________________________
void fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{
  Int_t nbins =NP; 
  Int_t i;

//calculate chisquare
   Double_t chisq = 0;
   Double_t delta;
   for (i=0;i<nbins; i++) {
     delta  = fabs(vv[i]-Vfunc(zz[i],rr[i],tt[i],par))/errorz[i];
     chisq += delta*delta;
   }
   f = chisq;
}

//______________________________________________________________________________
void Ifit(double c_m, double c_a, double c_l, double c_rho)
{
gSystem->Load("libMathCore");
gSystem->Load("libMathMore");

m=c_m;
a=c_a;
cl=c_l;
rho=c_rho;

// Set starting values and step sizes for parameters
   Double_t vstart[8]={5.7,   4.4, 0.006, 0.05,  -0.0001,   0.0000,   0.000, 0.000};
   Double_t   step[8]={0.01, 0.01, 0.001, 0.001, 0.001, 0.001,0.001,0.001};
   Double_t vtest[8]={
    5.73007    ,    // c00 /a^2
    4.94852    ,    // c01 /a^6
    0.00606    ,    // c10
    0.05304    ,    // c11
   -0.00003    ,    // c20
    0.0        ,    // c30
    0.00072    ,    // c21
    0.0             // c31
   };


 Float_t error = 0.1;// too small or too large?
 ifstream datafile;
 datafile.open("AllnodevaluesFEM_withcoordinates.txt");

 int kk=0;
 for (int ii=0; ii<NPTS; ii++){
  float xt, yt, zt, vt;
  datafile  >> xt >> yt >> zt >> vt;
  //test
  //float difference=test (xt, yt,zt,vt,vtest);
  
  //select only the good ones
  if (sqrt(xt*xt +yt*yt)<0.39)
  {
     rr[kk]=sqrt(xt*xt +yt*yt);
     tt[kk]=atan2(yt,xt);// safe way of using tan to get theta
     zz[kk]=zt;
     vv[kk]=vt/100;
     errorz[kk]=error;
     kk++;
  } 
 }
  NP=kk;
  cout << "Test for "<<NP<< " points...\n";
  Double_t arglist[10];
  Int_t ierflg = 0;

   TMinuit *gMinuit = new TMinuit(8);  //initialize TMinuit with a maximum of 9 params
            gMinuit->SetFCN(fcn);


   arglist[0] = 1;
   gMinuit->mnexcm("SET ERR", arglist ,1,ierflg);

   gMinuit->mnparm(0, "C00", vstart[0], step[0], 0,10, ierflg);
   gMinuit->mnparm(1, "C01", vstart[1], step[1], 0,10, ierflg);
   gMinuit->mnparm(2, "C10", vstart[2], step[2], 0,00, ierflg);
   gMinuit->mnparm(3, "C11", vstart[3], step[3], 0,00, ierflg);
   gMinuit->mnparm(4, "C20", vstart[4], step[4], 0,00, ierflg);
   gMinuit->mnparm(5, "C21", vstart[5], step[5], 0,00, ierflg);
   gMinuit->mnparm(6, "C30", vstart[6], step[6], 0,00, ierflg);
   gMinuit->mnparm(7, "C31", vstart[7], step[7], 0,00, ierflg);

// Now ready for minimization step
   arglist[0] = NP;
   arglist[1] = 1.;
   gMinuit->mnexcm("MIGRAD", arglist ,2,ierflg);

// Print results
   Double_t amin,edm,errdef;
   Int_t nvpar,nparx,icstat;
   gMinuit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
 //  gMinuit->mnprin(3,amin);

}

double test (float x, float y,float z,float v,Double_t *par)
{

  float xt=x;
  float yt=y;
  float zz= z;
  float vv=v;
  float rr=sqrt(xt*xt +yt*yt);
  float tt=atan2(yt,xt);// safe way of using tan to get theta
  vv=vv/100;

  float vv2=Vfunc( zz, rr, tt, par);

//if (fabs(vv2-vv)>0.1 ) {
//   std::cout <<"potential value in the file "<<vv<<" calculated with func "<<vv2<<"\n";
//   std::cout <<"z: "<<zz<<" rr: "<<rr<<" th: "<<tt<< " Dl: "<<fabs(vv2-vv)<<endl;
//}
  return fabs(vv2-vv);
}

