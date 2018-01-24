TCanvas * display()
{
  TFile * d1_file = new TFile("calibrated_spectra/d1_calibrated.root");
  TFile * d2_file = new TFile("calibrated_spectra/d2_calibrated.root");
  TFile * d3_file = new TFile("calibrated_spectra/d3_calibrated.root");
  TFile * d4_file = new TFile("calibrated_spectra/d4_calibrated.root");


  TH1F * d1_sp = (TH1F * ) d1_file->Get("ch0");
  TH1F * d2_sp = (TH1F * ) d2_file->Get("ch1");
  TH1F * d3_sp = (TH1F * ) d3_file->Get("ch2");
  TH1F * d4_sp = (TH1F * ) d4_file->Get("ch2");


  d1_sp->SetStats(kFALSE);
  d2_sp->SetStats(kFALSE);
  d3_sp->SetStats(kFALSE);
  d4_sp->SetStats(kFALSE);

  d1_sp->SetTitle("Detector #1");
  d2_sp->SetTitle("Detector #2");
  d3_sp->SetTitle("Detector #3");
  d4_sp->SetTitle("Detector #4");

  d1_sp->GetXaxis()->SetTitle("Energy [keV]");
  d2_sp->GetXaxis()->SetTitle("Energy [keV]");
  d3_sp->GetXaxis()->SetTitle("Energy [keV]");
  d4_sp->GetXaxis()->SetTitle("Energy [keV]");
  

  TCanvas * spectra_canvas = new TCanvas("Energy Spectra","Energy Spectra");

  spectra_canvas->Divide(2,2);

  spectra_canvas->cd(1);
  d1_sp->Draw();
  spectra_canvas->cd(2);
  d2_sp->Draw();
  spectra_canvas->cd(3);
  d3_sp->Draw();
  spectra_canvas->cd(4);
  d4_sp->Draw();

  return spectra_canvas;
}
