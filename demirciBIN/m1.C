//#include <math.h>
#include <vector>
{

struct p3d{
 Float_t x;
 Float_t y;
 Float_t z;
};
 
struct brick
{
  p3d pnt[20];
};

//------input parameters---------
Float_t x, y, th;
Float_t rho1, rho2, theta_m1, theta_m2, l1, l2;
Float_t t1, t2;
rho1=3.5;
rho2=3.0;
theta_m1=65*3.1416/180.0;
theta_m2=70*3.1416/180.0;
l1=2.5;
l2=3.0;
t1=2.2;
t2=2.4;

TCanvas *c1 = new TCanvas ("c1", "c1", 700, 700);
     gStyle->SetOptStat(0);
     double crossection_dim=9;
TH2F *hpf = new TH2F ("hpf", " RFQ M1 " , 400, 0, crossection_dim, 400, 0, crossection_dim);
hpf->SetXTitle("x (cm)");
hpf->SetYTitle("y (cm)");
hpf->Draw();

TCrown *ar1= new TCrown (0, l1+rho1, rho1, rho1, -90, -90+theta_m1*180/3.1416); ar1->SetLineColor(1); ar1->Draw();
TCrown *ar2= new TCrown (l2+rho2, 0, rho2, rho2, 180, 180-theta_m2*180/3.1416); ar2->SetLineColor(1); ar2->Draw();
Int_t N_v=4;// can be increased.
Int_t N_a=3;// can be increased.
Int_t N_ap=2*(N_a-1)+3; // derived.

Float_t *x1 = new Float_t [N_v+3]; //x
Float_t *y1 = new Float_t [N_v+3]; //y
Float_t *x2 = new Float_t [N_v+3]; //x
Float_t *y2 = new Float_t [N_v+3]; //y
Float_t *xc = new Float_t [N_v+3]; //x
Float_t *yc = new Float_t [N_v+3]; //x
Float_t **xa = new Float_t *[N_ap];
Float_t **ya = new Float_t *[N_ap]; // this is how you allocate double pointer.
 for (int i=0; i<N_ap; i++){
  xa[i]=new Float_t *[N_v+3];
  ya[i]=new Float_t *[N_v+3];
 }
//------------------------------

for (Int_t k=0; k<=N_v; k++)
{
   th=k*theta_m1/N_v;
   x1[k]= rho1*sin(th);
   y1[k]= l1+rho1*(1-cos(th));
   th=k*theta_m2/N_v;
   y2[k]= rho2*sin(th);
   x2[k]= l2+rho2*(1-cos(th));
}

Float_t ma=tan(theta_m1);
Float_t xpa=x1[N_v]+t1/sqrt(1+ma*ma);
Float_t ypa=ma*xpa + y1[N_v]-ma*x1[N_v];
x1[N_v+2] = xpa;
y1[N_v+2] = ypa;
x1[N_v+1] = 0.5*(xpa+x1[N_v]);
y1[N_v+1] = 0.5*(ypa+y1[N_v]);
TLine *tl1= new TLine (x1[N_v], y1[N_v], xpa, ypa);
tl1->Draw();

Float_t mb=tan(3.1416/2-theta_m2);
Float_t xpb=x2[N_v]+t2/sqrt(1+mb*mb);
Float_t ypb=mb*xpb + y2[N_v]-mb*x2[N_v];
x2[N_v+2] = xpb;
y2[N_v+2] = ypb;
x2[N_v+1] = 0.5*(xpb+x2[N_v]);
y2[N_v+1] = 0.5*(ypb+y2[N_v]);
TLine *tl2= new TLine (x2[N_v], y2[N_v], xpb, ypb); 
tl2->Draw();

//45 degree line for circles
Float_t xm=(x2[N_v]+x1[N_v])/2;
Float_t ym=(y1[N_v]+y2[N_v])/2;
xc[0]=0;
yc[0]=0;
for (Int_t k=1; k<N_v; k++)
{
 xc[k]=k*xm/N_v;
 yc[k]=k*ym/N_v;
}
xc[N_v]=xm;
yc[N_v]=ym;

//45 degree line for lines
xc[N_v+2]=(xpa+xpb)/2;
yc[N_v+2]=(ypa+ypb)/2;
xc[N_v+1]=(xc[N_v+2] + xc[N_v])/2;
yc[N_v+1]=(yc[N_v+2] + yc[N_v])/2;

 for (Int_t j=0; j<=N_v+2; j++) {
  xa[0][j]=x2[j];
  ya[0][j]=y2[j];
  xa[N_a][j]=xc[j];
  ya[N_a][j]=yc[j];
  xa[N_ap-1][j]=x1[j];
  ya[N_ap-1][j]=y1[j];
 }

 for (Int_t k=1; k<N_a; k++)
{
 for (Int_t j=0; j<=N_v+2; j++)
 {
  xa[N_a+k][j]=(N_a-k)*(xc[j]-x1[j])/N_a +x1[j] ;
  ya[N_a+k][j]=k*(y1[j]-yc[j])/N_a +yc[j];
  xa[k][j]=(N_a-k)*(x2[j]-xc[j])/N_a +xc[j] ;
  ya[k][j]=k*(yc[j]-y2[j])/N_a +y2[j];
 }
}

//----------drawing --------------
//TGraph *d0 = new TGraph(N_v+3, xc, yc); d0->Draw("*");
//TGraph *d1 = new TGraph(N_v+3, x1, y1); d1->Draw("*");
//TGraph *d2 = new TGraph(N_v+3, x2, y2); d2->Draw("*");
for (int i=0; i<N_ap; i++){
 TGraph *d4 = new TGraph(N_v+3, xa[i], ya[i]); d4->Draw("*C");
}

TLine *lm= new TLine(0, 0, xm, ym); lm->Draw(); // middle line 45
TLine *lt= new TLine(x1[N_v], y1[N_v], x2[N_v], y2[N_v]); lt->Draw(); // middle line 135
TLine *tc2 = new TLine (xpa, ypa, xpb, ypb); tc2->Draw();
TLine *tc3 = new TLine (xc[N_v], yc[N_v], (xpa+xpb)/2, (ypa+ypb)/2); tc3->Draw();

for (Int_t i=0; i<N_v+2; i++){
 TLine *lta=new TLine(xc[i],yc[i], x1[i],y1[i]); lta->Draw();
 TLine *ltb=new TLine(xc[i],yc[i], x2[i],y2[i]); ltb->Draw();
}

// make the brick faces here...
// shoud better go into a function

std::vector <brick> bricks;

for (k=0; k<2*N_a; k++){ // add the middle points later on
 for (j=0; j<N_v+2; j++) {
brick abri; // brick one borders.
      abri.pnt[1].x=xa[k+1][j];
      abri.pnt[3].x=xa[k][j];
      abri.pnt[6].x=xa[k+1][j+1];
      abri.pnt[8].x=xa[k][j+1];
// add y information
     
  bricks.push_back(abrick);

}

}
