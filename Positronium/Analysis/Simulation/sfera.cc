TCanvas * sfera()
{
   TCanvas *c3 = new TCanvas("c3","c3",600,400);
   c3->Divide(1,1);
   TH2F *hlcc = new TH2F("heat_map","heat_map", 100,-90,90,100,-180,180);
   Float_t px, py;

   hlcc->Fill(0.0,0.0);
   hlcc->Fill(0.0,120.0);
   hlcc->Fill(0.0,-120.0);

   hlcc->SetFillColor(kYellow);
   c3->cd(1); hlcc->Draw("LEGO1 SPH");

   return c3;
}
