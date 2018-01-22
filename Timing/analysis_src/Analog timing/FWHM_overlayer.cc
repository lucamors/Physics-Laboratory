void FWHM_over(string infile_1, string infile_2)
{
  // Retrieving data
   TCanvas* C = new TCanvas("mad skillz","mad skillz");

   TGraphErrors *Graph_1 = new TGraphErrors(infile_1.c_str(),"%lg%lg%lg");
   Graph_1->GetXaxis()->SetTitle("Energy [KeV]");
   Graph_1->GetYaxis()->SetTitle("FWHM [ns]");
   Graph_1->GetXaxis()->SetLimits(0,1400);
   Graph_1->GetYaxis()->SetLimits(0.53,0.67);
   Graph_1->SetTitle("Energy windows");
   Graph_1->SetFillStyle(0);
   Graph_1->SetLineColor(kRed);
   Graph_1->SetMarkerStyle(20);
   Graph_1->SetMarkerColor(kBlue);
   Graph_1->SetLineWidth(2);
   Graph_1->Draw();

   TGraphErrors *Graph_2 = new TGraphErrors(infile_2.c_str(),"%lg%lg%lg");
   Graph_2->GetXaxis()->SetTitle("Energy [KeV]");
   Graph_2->GetYaxis()->SetTitle("FWHM [ns]");
   Graph_2->GetXaxis()->SetLimits(0,1400);
   Graph_2->GetYaxis()->SetLimits(0.53,0.67);
   Graph_2->SetTitle("Energy thresholds");
   Graph_2->SetFillStyle(0);
   Graph_2->SetLineColor(kBlue);
   Graph_2->SetMarkerStyle(20);
   Graph_2->SetMarkerColor(kRed);
   Graph_2->SetLineWidth(2);
   Graph_2->Draw("SAME");

   C->Update();

  return ;
}
