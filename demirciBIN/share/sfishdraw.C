#include <iostream>
#include <fstream>

int read1line(TString option="x=0.,y=0. &", double *xr, double *yr){
 Ssiz_t from = 0 ;
 TString act;
 TString act2;
 double xp, yp, x0, y0;

 int apair=0, cnt=0;
 //cout << "1:"<<option <<endl;
 option.Tokenize(act, from, " ");
 TSubString interest=act.SubString("&po", 0);
 if (interest.IsNull() ) { 
 //     cout<<"reject"<<endl; 
    return 0;
 }
 option.Tokenize(act, from, " ");
 
 option=act; from=0;
 //cout << "2:"<<option <<endl;
 do {
    if (!option.Tokenize(act, from, ",")) {
     // cout  << "finished parsing line params:"<< endl;
      break;
    }
   //cout << "ACT:"<<  act << " from:"<<from<<endl;

   TSubString xstring=act.SubString("x=", 0);
   if  (!xstring.IsNull() )  {                    // x string
             act.Tokenize(act2, 2, "="); xp=act2.Atof();
             if (apair!=2) apair=1;
   //          cout << "Xf is:"<< act2<<"value:" <<act2.Atof() <<endl;
   }

   TSubString ystring=act.SubString("y=", 0);
   if  (!ystring.IsNull() )  {                    // y string
             act.Tokenize(act2, 2, "="); yp=act2.Atof();
   //          cout << "Yf is:"<< act2<<"value:" <<act2.Atof() <<endl;
   }

   TSubString ntstring = act.SubString("nt=", 0);
   if  ( !ntstring.IsNull() )  {                    // arc string
    apair=2; //this should be read from the string
    //cout << "This will be an arc:"<< ntstring <<endl;
   }
   TSubString x0string = act.SubString("x0=", 0);
   if  ( !x0string.IsNull() )  {                    // x0 string
             act.Tokenize(act2, 3, "="); x0=act2.Atof();
    //         cout << "X0 is:"<< act2<<"value:" <<act2.Atof() <<endl;
   }

   TSubString y0string = act.SubString("y0=", 0);
   if  ( !y0string.IsNull() )  {                    // y0 string
             act.Tokenize(act2, 3, "="); y0=act2.Atof();
   //          cout << "Y0 is:"<< act2<<"value:" <<act2.Atof() <<endl;
   }
 } while ( 1 );

   if (apair==1) {
       xr[0]=xp; yr[0]=yp;
  //     cout  <<"Xp:"<<xp<< "  Yp:"<<yp<<endl;
   }
   if (apair==2) {
       xr[0]=x0; yr[0]=y0;
       xr[1]=xp; yr[1]=yp;
       //cout <<" ====>" <<"X0:"<<x0<< " Y0:"<<y0<<" Xp:"<<xp<< "  Yp:"<<yp<<endl;
   }
    return apair;
}// end of read1line

int sfishdraw (TString filename){
TCanvas *c1 = new TCanvas ("c1", "c1", 700, 700);
     gStyle->SetOptStat(0);
     double crossection_dim=25;
TH2F *hpf = new TH2F ("hpf", " RFQ Cross-Section " , 40, -crossection_dim, crossection_dim, 40, -crossection_dim, crossection_dim);
hpf->SetXTitle("x (cm)");
hpf->SetYTitle("y (cm)");
hpf->Draw();
double x[9],y[9], xi=0.0, yi=0.0;
int ip, retval;
char aline[256];
ifstream fin;
         fin.open(filename);
         do {
           fin.getline(aline, 256);
          // cout << "R:"<<aline<<endl;
           retval=read1line(aline, x, y); 
           if (retval==1) {  // we received a line
            if ((xi==0 && yi==0 )  || (x[0]==0 && y[0]==0)) {
             ;
            } else {
             TLine *tl1= new TLine(xi, yi, x[0], y[0]); tl1->Draw(); // top right vane
	       	     TLine *tl2= new TLine(-xi, -yi, -x[0], -y[0]); tl2->Draw(); // bottom left vane
  	     TLine *tl3= new TLine(-xi, yi, -x[0], y[0]); tl3->Draw(); // top left vane
  	     TLine *tl4= new TLine(xi, -yi, x[0], -y[0]); tl4->Draw(); // bottom right vane
            }
  //           cout << "did a Line:"<< xi<<" "<< yi<<" "<< x[0]<<" "<< y[0]<<endl;
             xi=x[0]; yi=y[0];
           }
           if (retval==2) {  // we received an arc
            double radius= sqrt( x[1]*x[1] + y[1]*y[1]);
            double thetai=180*atan(y[1]/x[1])/3.14159;
            double thetaf=180*atan((yi-y[0])/(xi-x[0]))/3.14159;
            if (thetaf==0) {thetaf=180;} 
            if (y[1]<0 && x[1]<=0) { 
                 thetaf-=180;
            } 
            if (y[1]<=0 && x[1]<0) { 
                 thetaf+=260;
                 thetai+=100;
            }
            xi=x[0]+x[1];
            yi=y[0]+y[1];
	    TCrown *ar1= new TCrown (x[0], y[0], radius, radius, thetai, thetaf); ar1->SetLineColor(1); ar1->Draw(); // arcs of top right vane
            TCrown *ar2= new TCrown (-x[0], -y[0], -radius, -radius, thetai, thetaf); ar2->SetLineColor(2); ar2->Draw(); // arcs of bottom left vane
            TCrown *ar3= new TCrown (x[0], -y[0], radius, -radius, -thetai, -thetaf); ar3->SetLineColor(3); ar3->Draw(); // arcs of bottom right vane
            TCrown *ar4= new TCrown (-x[0], y[0], -radius, -radius, -thetai, -thetaf); ar4->SetLineColor(4); ar4->Draw(); // arcs of top left vane

            //TArc *ar= new TArc (x[0], y[0], radius, thetai, thetaf); ar->Draw();
 //    cout << "did an Arc "<< x[0] <<" "<< y[0] <<" "<<x[1] <<" "<< y[1] <<" "<< endl;
           }
           c1->Update();
         } while (!fin.eof()) ;
	 //    double rado=9.1;
   TEllipse *cc1 = new TEllipse(0,0, crossection_dim,crossection_dim, 0, 360);
             cc1->SetFillStyle(0);
             cc1->Draw("same");
}
