TCanvas * plot_all(string filename)
{
  gStyle->SetTitleSize(0.5);

  TFile * infile = new TFile(filename.c_str());

  TCanvas * c1 = new TCanvas("summary","summary");

  TH1F * gamma1_sp = (TH1F*) infile->Get("d1_en_sp");
  TH1F * gamma2_sp = (TH1F*) infile->Get("d2_en_sp");
  TH1F * gamma3_sp = (TH1F*) infile->Get("d3_en_sp");

  c1->Divide(2,2);

  // Generated Events Energy Spectra
  c1->cd(1);

  gamma1_sp->SetLineColor(kBlue);
  gamma1_sp->SetLineWidth(2);
  gamma2_sp->SetLineColor(kGreen+3);
  gamma2_sp->SetLineWidth(2);
  gamma3_sp->SetLineColor(kRed-3);
  gamma3_sp->SetLineWidth(2);

  gamma1_sp->SetTitle("Energy Spectrum of generated #gamma");
  gamma1_sp->GetXaxis()->SetTitle("Energy [keV]");
  gamma1_sp->GetYaxis()->SetTitle("Count #");
  gamma1_sp->GetYaxis()->SetTitleOffset(1.5);
  gamma1_sp->SetStats(kFALSE);

  gamma1_sp->Draw();
  gamma2_sp->Draw("SAME");
  gamma3_sp->Draw("SAME");

  // Coincidence Energy Spectra

  c1->cd(2);

  TH1F * g1_coinc = (TH1F*) infile->Get("coincidences_sp_1");
  TH1F * g2_coinc = (TH1F*) infile->Get("coincidences_sp_2");
  TH1F * g3_coinc = (TH1F*) infile->Get("coincidences_sp_3");

  g1_coinc->SetTitle("Energy Spectrum of 3-coincidence #gamma");
  g1_coinc->GetXaxis()->SetTitle("Energy [keV]");
  g1_coinc->GetYaxis()->SetTitle("Count #");
  g1_coinc->SetStats(kFALSE);
  g1_coinc->GetYaxis()->SetTitleOffset(1.5);

  g1_coinc->SetLineColor(kBlue);
  g1_coinc->SetLineWidth(2);
  g2_coinc->SetLineColor(kGreen+3);
  g2_coinc->SetLineWidth(2);
  g3_coinc->SetLineColor(kRed-3);
  g3_coinc->SetLineWidth(2);

  g1_coinc->Draw();
  g2_coinc->Draw("SAME");
  g3_coinc->Draw("SAME");

  // Angular Distribution of generated gamma

  c1->cd(3);

  TH2F * angular_distribution = (TH2F*) infile->Get("ang_dist");

  angular_distribution->SetTitle("Angular distribution of generated #gamma");
  angular_distribution->GetXaxis()->SetTitle("#theta_{12}");
  angular_distribution->GetYaxis()->SetTitle("#theta_{23}");
  angular_distribution->SetStats(kFALSE);
  angular_distribution->GetYaxis()->SetTitleOffset(1.5);

  angular_distribution->Draw("COLZ");

  // Angular Distribution of triple-coincident gamma

  c1->cd(4);

  TH2F * angular_distribution_coinc = (TH2F*) infile->Get("ang_dist_coinc");

  angular_distribution_coinc->SetTitle("Angular distribution of 3-coincidence #gamma");
  angular_distribution_coinc->GetXaxis()->SetTitle("#theta_{12}");
  angular_distribution_coinc->GetYaxis()->SetTitle("#theta_{23}");
  angular_distribution_coinc->SetStats(kFALSE);
  angular_distribution_coinc->GetYaxis()->SetTitleOffset(1.5);

  angular_distribution_coinc->Draw("COLZ");



  return c1;
}
