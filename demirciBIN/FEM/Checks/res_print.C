{
  TTree *MyTree = new TTree("MyTree", "MyTree");
  MyTree->ReadFile("AllnodevaluesFEM_8T_withcoordinates.txt","x:y:z:UFem:U8T");
  TCanvas a;
    a->cd();
  MyTree->Draw("UFem:U8T","x*x+y*y<0.152");
  
  TCanvas b;
    b->cd();
  MyTree->Draw("UFem-U8T","x*x+y*y<0.152");

  
}
