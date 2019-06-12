#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>


#define MAXCELL 400

struct rfq_data
{
 float N[MAXCELL];
 float LT[MAXCELL];
 float L0[MAXCELL];
 float BS[MAXCELL];
 float W[MAXCELL];
 float M[MAXCELL];
 float U[MAXCELL];
 float T[MAXCELL];
 float FS[MAXCELL];
 float rmi[MAXCELL];
 float rma[MAXCELL];
 float R0[MAXCELL];
};


class g_item{
 public:
     g_item(TString itemname, int nc){ p_label=itemname; p_nc=nc;}
    ~g_item();
     TGraph * getHP()     {return p_graf;}
     TString getLabel()   {return p_label;}
     void setCell(bool v) { p_vscell=v;} // if not cell then plot as function of Z.
     void setColor(int c) { p_color=c;}  // if not cell then plot as function of Z.
     void setYvals (float *y1, float *y2) { 
                      for (int i=0; i<p_nc; i++) y[i]=y1[i]/y2[i];
                                          }  
     void activate(float *x1 ){ p_graf=new TGraph(p_nc, x1, y);
                                p_graf->SetLineColor(p_color);
                                p_graf->Draw("L");
                              }

 private:
    TString p_label;
    bool p_drawable;
    bool p_vscell;
     int p_nc;
     int p_color;
    TGraph *p_graf;
    float y[200];
};


int readRFQData ( char *filename, rfq_data *a_rfq_data) {
 int nc=0;
 ifstream li_in;
          li_in.open(filename);
 for (nc=0; nc<MAXCELL && !li_in.eof(); nc++) {
  li_in >> a_rfq_data->N[nc]
        >> a_rfq_data->LT[nc]  >> a_rfq_data->L0[nc] >> a_rfq_data->BS[nc] >> a_rfq_data->W[nc] 
        >> a_rfq_data->M[nc]   >> a_rfq_data->U[nc]  >> a_rfq_data->T[nc]  >> a_rfq_data->FS[nc]
        >> a_rfq_data->rmi[nc] >> a_rfq_data->rma[nc] ;
        a_rfq_data->LT[nc]/=10; 
        a_rfq_data->R0[nc]=0.5*(a_rfq_data->M[nc]+1)*a_rfq_data->rmi[nc];
 }
 nc--;
// cout << "# cells read:"<<nc<<endl;
 li_in.close();
 return nc;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void plotSet(rfq_data dat1, rfq_data dat2, int nc, bool useZ, int BCI) {
// the initial setup 
 Double_t xmax, xmin = 0.0;
 if (useZ) {
   xmax = 11.7; // read from the rfq_data FIXME.
 } else {
   xmax = 200; // read from the rfq_data FIXME.
 }
 gStyle->SetOptStat(0);
 gStyle->SetLineWidth(2.5);
 gStyle->SetTitleBorderSize(0);
 gStyle->SetTitleFont(32,""); // histogram title
 TCanvas *c1 = new TCanvas("c1","transparent pad",200,10,700,500);
 TPad *pad1 = new TPad("pad1","",0,0,1,1);
 TPad *pad2 = new TPad("pad2","",0,0,1,1);
 pad2->SetFillStyle(4000); //will be transparent
 pad1->Draw();
 pad1->cd();

 TH2F *hpx = new TH2F("hpx"," SANAEM-RFQ ",40,xmin,xmax, 40,0.8,1.2);
 if (useZ) {
   hpx->GetXaxis()->SetTitle("RFQ length (m)");
 } else {
   hpx->GetXaxis()->SetTitle("RFQ length (cell number)");
 }
 hpx->GetXaxis()->CenterTitle(1);
 hpx->SetTitleFont(32); // x-axis title by default
 hpx->Draw();
 c1->Modified(); 

// the functions
 const int Nprops=11;
 g_item *aplot[Nprops];

 const Char_t *textBut[Nprops]= { "Cell Length(cm)", "Cell Position(cm)", "beta", "K. Energy(MeV)", "Modulation",
                                  "I.Voltage(MV)", "time(ns)","S.Phase (D)",
                                  "Min.Aperture(mm)", "Max.Aperture(mm)", "Bore Radius(mm)",
                                }; //11

 const float *yvalsA[Nprops]= { dat1.LT, dat1.L0, dat1.BS, dat1.W, dat1.M, dat1.U, dat1.T, dat1.FS,
                                dat1.rmi,dat1.rma, dat1.R0 }; // 11
 const float *yvalsB[Nprops]= { dat2.LT, dat2.L0, dat2.BS, dat2.W, dat2.M, dat2.U, dat2.T, dat2.FS,
                                dat2.rmi,dat2.rma,dat2.R0 }; // 11

 const  int    cols[Nprops]= { 6, 3, 3, 1, 4, 5, 5, 2, kCyan+2, 4, 3 }; // 11


 for (int k=0; k<Nprops; k++) {
  bool on=BCI&(long int )pow(2,k);
  if (on && (k!=7)) {
   aplot[k]= new g_item(textBut[k], nc);
   aplot[k]->setCell(useZ           );
   aplot[k]->setColor(cols[k]       );
   aplot[k]->setYvals(yvalsA[k], yvalsB[k]);
   aplot[k]->activate(dat1.N        );
  }
 }

 //bool om=BCI&(long int )pow(2,7); // k=7 is the sync. phase. 
 bool om=0;
 if ( 0 ) {

// the following is for the right axis, i.e. phi distro
//compute the pad range with suitable margins
  Double_t ymin = -90;
  Double_t ymax = -20;
  Double_t dy = (ymax-ymin)/0.8; //10 per cent margins top and bottom
  Double_t dx = (xmax-xmin)/0.8; //10 per cent margins left and right
  pad2->Range(xmin-0.1*dx,ymin-0.1*dy,xmax+0.1*dx,ymax+0.1*dy);
  pad2->Draw();
  pad2->cd();
   aplot[7]= new g_item( textBut[7], nc);
   aplot[7]->setCell(useZ);
   aplot[7]->setColor(cols[7]);
   aplot[k]->setYvals(yvalsA[7], yvalsB[7]);
   aplot[7]->activate(dat.N);
  
  pad2->Update();

TGaxis *axis2 = new TGaxis( xmax,ymin,xmax,ymax,ymin,ymax,510,"+L" );
        axis2->SetLineColor(2);
        axis2->SetLabelColor(2);
        axis2->Draw();
        axis2->SetTitle("Sync. Angle #phi (degree)");
        axis2->SetTitleColor(2);
        axis2->SetTitleFont(32);
        axis2->CenterTitle(1);
}

// the legend box
TLegend *leg = new TLegend(0.65,0.98,0.88,0.84);
for (int k=0; k<Nprops; k++) {
  bool onoff=BCI&(long int )pow(2,k);
  if (onoff) {
    leg->AddEntry(aplot[k]->getHP(), textBut[k], "l");
  }
}
leg->SetFillColor(0);
leg->SetFillStyle(1001);
leg->Draw();

}
//~~~~~~~~~~~~~~~~~~~~~~~
int compare(bool useZ=false, char *fileA="sanaemRFQ_1", char *fileB="dmr4lidos.out", int BCI=25) {
 int ncell1=0;
 int ncell2=0;
 rfq_data rfqA;
 rfq_data rfqB;

 ncell2=readRFQData(fileB, &rfqB);
 ncell1=readRFQData(fileA, &rfqA);

 if (ncell1 != ncell2) {
  std::cout << "number of cells different, can not compare the RFQs\n";
  exit -1;
 }
 plotSet(rfqA, rfqB, ncell1, useZ, BCI);
}
