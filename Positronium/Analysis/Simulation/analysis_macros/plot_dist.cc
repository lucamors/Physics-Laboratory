
TCanvas * plot_dist(string filename)
{
  TFile * infile = new TFile(filename.c_str());

  TH2F * hlcc = (TH2F*) infile->Get("heat_map");

   TCanvas *c3 = new TCanvas("c3","c3",600,400);
   c3->Divide(1,1);
   hlcc->SetTitle("Distribution");
   c3->cd(1); hlcc->Draw("LEGO2 POL");
   // c3->cd(2); TH2F *hlpc = (TH2F*) hlcc->DrawClone("LEGO2 SPH");
   // hlpc->SetTitle("");

   return c3;
}
