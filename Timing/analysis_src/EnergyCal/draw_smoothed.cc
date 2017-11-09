void draw(std::string filename)
{
  TFile * smoothed_input_file = new TFile(filename.c_str());

  // Retrieving smoothed histograms of interest

  TH1F * sigma_1 = (TH1F*) smoothed_input_file->Get("smooth_511_1");
  TH1F * sigma_10 = (TH1F*) smoothed_input_file->Get("smooth_511_10");
  TH1F * sigma_20 = (TH1F*) smoothed_input_file->Get("smooth_511_20");
  TH1F * sigma_30 = (TH1F*) smoothed_input_file->Get("smooth_511_30");
  TH1F * sigma_40 = (TH1F*) smoothed_input_file->Get("smooth_511_40");
  TH1F * sigma_50 = (TH1F*) smoothed_input_file->Get("smooth_511_50");


  gStyle->SetOptStat(false);

  TCanvas * c1 = new TCanvas("Gaussian_Smeared_Spectra", "Gaussian Smeared Spectra", 500);
  sigma_1->SetLineColor(kRed);
  sigma_10->SetLineColor(kRed+4);
  sigma_20->SetLineColor(kBlue);
  sigma_30->SetLineColor(kBlue+4);
  sigma_40->SetLineColor(kGreen);
  sigma_50->SetLineColor(kGreen+4);

  sigma_1->SetTitle("Gaussian Smeared Spectra");
  sigma_1->GetXaxis()->SetTitle("Energy [keV]");
  sigma_1->GetYaxis()->SetTitle("Counts #");

  sigma_1->Draw();
  sigma_10->Draw("SAME");
  sigma_20->Draw("SAME");
  sigma_30->Draw("SAME");
  sigma_40->Draw("SAME");
  sigma_50->Draw("SAME");


  return ;
}
