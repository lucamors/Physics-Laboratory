void background_noise_viewer(string filename, string outfile_name)
{

  TFile * input_file = new TFile(filename.c_str());

  TCanvas * comparison = new TCanvas("canvas");

  comparison->Divide(2,1);

  TH2F * correlation = (TH2F *) input_file->Get("correlation");
  TH2F * correlation_coinc = (TH2F *) input_file->Get("correlation_coinc");

  correlation->GetXaxis()->SetRangeUser(0, 600);
  correlation_coinc->GetXaxis()->SetRangeUser(0, 600);

  correlation->GetYaxis()->SetRangeUser(0, 600);
  correlation_coinc->GetYaxis()->SetRangeUser(0, 600);

  correlation->SetStats(kFALSE);
  correlation_coinc->SetStats(kFALSE);

  correlation->SetTitle("");
  correlation_coinc->SetTitle("");

  correlation->GetXaxis()->SetTitle("Tagger Energy [keV]");
  correlation->GetYaxis()->SetTitle("Detector Energy [keV]");

  correlation_coinc->GetXaxis()->SetTitle("Tagger Energy [keV]");
  correlation_coinc->GetYaxis()->SetTitle("Detector Energy [keV]");

  comparison->cd(1);
  correlation->Draw("COLZ");
  comparison->cd(2);
  correlation_coinc->Draw("COLZ");

  TFile * outfile = new TFile(outfile_name.c_str(), "RECREATE");

  comparison->Write();

  outfile->Close();

  return ;
}
